/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004-2017 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "StyleSheetContents.h"

#include "CSSImportRule.h"
#include "CSSParser.h"
#include "CSSStyleSheet.h"
#include "CachedCSSStyleSheet.h"
#include "Document.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "MediaList.h"
#include "Node.h"
#include "Page.h"
#include "PageConsoleClient.h"
#include "ResourceLoadInfo.h"
#include "RuleSet.h"
#include "SecurityOrigin.h"
#include "StyleRule.h"
#include "StyleRuleImport.h"
#include <wtf/Deque.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/Ref.h>

#if ENABLE(CONTENT_EXTENSIONS)
#include "UserContentController.h"
#endif

namespace WebCore {

// Rough size estimate for the memory cache.
unsigned StyleSheetContents::estimatedSizeInBytes() const
{
    // Note that this does not take into account size of the strings hanging from various objects. 
    // The assumption is that nearly all of of them are atomic and would exist anyway.
    unsigned size = sizeof(*this);

    // FIXME: This ignores the children of media and region rules.
    // Most rules are StyleRules.
    size += ruleCount() * StyleRule::averageSizeInBytes();

    for (unsigned i = 0; i < m_importRules.size(); ++i) {
        if (StyleSheetContents* sheet = m_importRules[i]->styleSheet())
            size += sheet->estimatedSizeInBytes();
    }
    return size;
}

StyleSheetContents::StyleSheetContents(StyleRuleImport* ownerRule, const String& originalURL, const CSSParserContext& context)
    : m_ownerRule(ownerRule)
    , m_originalURL(originalURL)
    , m_defaultNamespace(starAtom())
    , m_isUserStyleSheet(ownerRule && ownerRule->parentStyleSheet() && ownerRule->parentStyleSheet()->isUserStyleSheet())
    , m_parserContext(context)
{
}

StyleSheetContents::StyleSheetContents(const StyleSheetContents& o)
    : RefCounted<StyleSheetContents>()
    , m_ownerRule(0)
    , m_originalURL(o.m_originalURL)
    , m_encodingFromCharsetRule(o.m_encodingFromCharsetRule)
    , m_importRules(o.m_importRules.size())
    , m_namespaceRules(o.m_namespaceRules.size())
    , m_childRules(o.m_childRules.size())
    , m_namespaces(o.m_namespaces)
    , m_defaultNamespace(o.m_defaultNamespace)
    , m_isUserStyleSheet(o.m_isUserStyleSheet)
    , m_loadCompleted(true)
    , m_hasSyntacticallyValidCSSHeader(o.m_hasSyntacticallyValidCSSHeader)
    , m_usesStyleBasedEditability(o.m_usesStyleBasedEditability)
    , m_parserContext(o.m_parserContext)
{
    ASSERT(o.isCacheable());

    // FIXME: Copy import rules.
    ASSERT(o.m_importRules.isEmpty());

    for (unsigned i = 0; i < m_childRules.size(); ++i)
        m_childRules[i] = o.m_childRules[i]->copy();
}

StyleSheetContents::~StyleSheetContents()
{
    clearRules();
}

bool StyleSheetContents::isCacheable() const
{
    // FIXME: Support copying import rules.
    if (!m_importRules.isEmpty())
        return false;
    // FIXME: Support cached stylesheets in import rules.
    if (m_ownerRule)
        return false;
    // This would require dealing with multiple clients for load callbacks.
    if (!m_loadCompleted)
        return false;
    if (m_didLoadErrorOccur)
        return false;
    // It is not the original sheet anymore.
    if (m_isMutable)
        return false;
    // If the header is valid we are not going to need to check the SecurityOrigin.
    // FIXME: Valid mime type avoids the check too.
    if (!m_hasSyntacticallyValidCSSHeader)
        return false;
    return true;
}

void StyleSheetContents::parserAppendRule(Ref<StyleRuleBase>&& rule)
{
    ASSERT(!rule->isCharsetRule());

    if (is<StyleRuleImport>(rule)) {
        // Parser enforces that @import rules come before anything else except @charset.
        ASSERT(m_childRules.isEmpty());
        m_importRules.append(downcast<StyleRuleImport>(rule.ptr()));
        m_importRules.last()->setParentStyleSheet(this);
        m_importRules.last()->requestStyleSheet();
        return;
    }

    if (is<StyleRuleNamespace>(rule)) {
        // Parser enforces that @namespace rules come before all rules other than
        // import/charset rules
        ASSERT(m_childRules.isEmpty());
        StyleRuleNamespace& namespaceRule = downcast<StyleRuleNamespace>(rule.get());
        parserAddNamespace(namespaceRule.prefix(), namespaceRule.uri());
        m_namespaceRules.append(downcast<StyleRuleNamespace>(rule.ptr()));
        return;
    }

    if (is<StyleRuleMedia>(rule))
        reportMediaQueryWarningIfNeeded(singleOwnerDocument(), downcast<StyleRuleMedia>(rule.get()).mediaQueries());

    // NOTE: The selector list has to fit into RuleData. <http://webkit.org/b/118369>
    // If we're adding a rule with a huge number of selectors, split it up into multiple rules
    if (is<StyleRule>(rule) && downcast<StyleRule>(rule.get()).selectorList().componentCount() > RuleData::maximumSelectorComponentCount) {
        m_childRules.appendVector(downcast<StyleRule>(rule.get()).splitIntoMultipleRulesWithMaximumSelectorComponentCount(RuleData::maximumSelectorComponentCount));
        return;
    }

    m_childRules.append(WTFMove(rule));
}

StyleRuleBase* StyleSheetContents::ruleAt(unsigned index) const
{
    ASSERT_WITH_SECURITY_IMPLICATION(index < ruleCount());
    
    unsigned childVectorIndex = index;
    if (childVectorIndex < m_importRules.size())
        return m_importRules[childVectorIndex].get();

    childVectorIndex -= m_importRules.size();
    
    if (childVectorIndex < m_namespaceRules.size())
        return m_namespaceRules[childVectorIndex].get();
    
    childVectorIndex -= m_namespaceRules.size();
    
    return m_childRules[childVectorIndex].get();
}

unsigned StyleSheetContents::ruleCount() const
{
    unsigned result = 0;
    result += m_importRules.size();
    result += m_namespaceRules.size();
    result += m_childRules.size();
    return result;
}

void StyleSheetContents::clearCharsetRule()
{
    m_encodingFromCharsetRule = String();
}

void StyleSheetContents::clearRules()
{
    for (unsigned i = 0; i < m_importRules.size(); ++i) {
        ASSERT(m_importRules.at(i)->parentStyleSheet() == this);
        m_importRules[i]->clearParentStyleSheet();
    }
    m_importRules.clear();
    m_namespaceRules.clear();
    m_childRules.clear();
    clearCharsetRule();
}

void StyleSheetContents::parserSetEncodingFromCharsetRule(const String& encoding)
{
    // Parser enforces that there is ever only one @charset.
    ASSERT(m_encodingFromCharsetRule.isNull());
    m_encodingFromCharsetRule = encoding; 
}

bool StyleSheetContents::wrapperInsertRule(Ref<StyleRuleBase>&& rule, unsigned index)
{
    ASSERT(m_isMutable);
    ASSERT_WITH_SECURITY_IMPLICATION(index <= ruleCount());
    // Parser::parseRule doesn't currently allow @charset so we don't need to deal with it.
    ASSERT(!rule->isCharsetRule());
    
    unsigned childVectorIndex = index;
    if (childVectorIndex < m_importRules.size() || (childVectorIndex == m_importRules.size() && rule->isImportRule())) {
        // Inserting non-import rule before @import is not allowed.
        if (!is<StyleRuleImport>(rule))
            return false;
        m_importRules.insert(childVectorIndex, downcast<StyleRuleImport>(rule.ptr()));
        m_importRules[childVectorIndex]->setParentStyleSheet(this);
        m_importRules[childVectorIndex]->requestStyleSheet();
        // FIXME: Stylesheet doesn't actually change meaningfully before the imported sheets are loaded.
        return true;
    }
    // Inserting @import rule after a non-import rule is not allowed.
    if (is<StyleRuleImport>(rule))
        return false;
    childVectorIndex -= m_importRules.size();

    
    if (childVectorIndex < m_namespaceRules.size() || (childVectorIndex == m_namespaceRules.size() && rule->isNamespaceRule())) {
        // Inserting non-namespace rules other than import rule before @namespace is
        // not allowed.
        if (!is<StyleRuleNamespace>(rule))
            return false;
        // Inserting @namespace rule when rules other than import/namespace/charset
        // are present is not allowed.
        if (!m_childRules.isEmpty())
            return false;
        
        StyleRuleNamespace& namespaceRule = downcast<StyleRuleNamespace>(rule.get());
        m_namespaceRules.insert(index, downcast<StyleRuleNamespace>(rule.ptr()));
        
        // For now to be compatible with IE and Firefox if a namespace rule with the same
        // prefix is added, it overwrites previous ones.
        // FIXME: The eventual correct behavior would be to ensure that the last value in
        // the list wins.
        parserAddNamespace(namespaceRule.prefix(), namespaceRule.uri());
        return true;
    }
    if (is<StyleRuleNamespace>(rule))
        return false;
    childVectorIndex -= m_namespaceRules.size();

    // If the number of selectors would overflow RuleData, we drop the operation.
    if (is<StyleRule>(rule) && downcast<StyleRule>(rule.get()).selectorList().componentCount() > RuleData::maximumSelectorComponentCount)
        return false;

    m_childRules.insert(childVectorIndex, WTFMove(rule));
    return true;
}

void StyleSheetContents::wrapperDeleteRule(unsigned index)
{
    ASSERT(m_isMutable);
    ASSERT_WITH_SECURITY_IMPLICATION(index < ruleCount());

    unsigned childVectorIndex = index;
    if (childVectorIndex < m_importRules.size()) {
        m_importRules[childVectorIndex]->clearParentStyleSheet();
        m_importRules.remove(childVectorIndex);
        return;
    }
    childVectorIndex -= m_importRules.size();

    if (childVectorIndex < m_namespaceRules.size()) {
        if (!m_childRules.isEmpty())
            return;
        m_namespaceRules.remove(childVectorIndex);
        return;
    }
    childVectorIndex -= m_namespaceRules.size();

    m_childRules.remove(childVectorIndex);
}

void StyleSheetContents::parserAddNamespace(const AtomicString& prefix, const AtomicString& uri)
{
    ASSERT(!uri.isNull());
    if (prefix.isNull()) {
        m_defaultNamespace = uri;
        return;
    }
    PrefixNamespaceURIMap::AddResult result = m_namespaces.add(prefix, uri);
    if (result.isNewEntry)
        return;
    result.iterator->value = uri;
}

const AtomicString& StyleSheetContents::namespaceURIFromPrefix(const AtomicString& prefix)
{
    PrefixNamespaceURIMap::const_iterator it = m_namespaces.find(prefix);
    if (it == m_namespaces.end())
        return nullAtom();
    return it->value;
}

void StyleSheetContents::parseAuthorStyleSheet(const CachedCSSStyleSheet* cachedStyleSheet, const SecurityOrigin* securityOrigin)
{
    bool isSameOriginRequest = securityOrigin && securityOrigin->canRequest(baseURL());
    CachedCSSStyleSheet::MIMETypeCheckHint mimeTypeCheckHint = isStrictParserMode(m_parserContext.mode) || !isSameOriginRequest ? CachedCSSStyleSheet::MIMETypeCheckHint::Strict : CachedCSSStyleSheet::MIMETypeCheckHint::Lax;
    bool hasValidMIMEType = true;
    String sheetText = cachedStyleSheet->sheetText(mimeTypeCheckHint, &hasValidMIMEType);

    if (!hasValidMIMEType) {
        ASSERT(sheetText.isNull());
        if (auto* document = singleOwnerDocument()) {
            if (auto* page = document->page()) {
                if (isStrictParserMode(m_parserContext.mode))
                    page->console().addMessage(MessageSource::Security, MessageLevel::Error, makeString("Did not parse stylesheet at '", cachedStyleSheet->url().stringCenterEllipsizedToLength(), "' because non CSS MIME types are not allowed in strict mode."));
                else if (!cachedStyleSheet->mimeTypeAllowedByNosniff())
                    page->console().addMessage(MessageSource::Security, MessageLevel::Error, makeString("Did not parse stylesheet at '", cachedStyleSheet->url().stringCenterEllipsizedToLength(), "' because non CSS MIME types are not allowed when 'X-Content-Type: nosniff' is given."));
                else
                    page->console().addMessage(MessageSource::Security, MessageLevel::Error, makeString("Did not parse stylesheet at '", cachedStyleSheet->url().stringCenterEllipsizedToLength(), "' because non CSS MIME types are not allowed for cross-origin stylesheets."));
            }
        }
        return;
    }

    CSSParser p(parserContext());
    p.parseSheet(this, sheetText, CSSParser::RuleParsing::Deferred);

    if (m_parserContext.needsSiteSpecificQuirks && isStrictParserMode(m_parserContext.mode)) {
        // Work around <https://bugs.webkit.org/show_bug.cgi?id=28350>.
        static NeverDestroyed<const String> mediaWikiKHTMLFixesStyleSheet(MAKE_STATIC_STRING_IMPL("/* KHTML fix stylesheet */\n/* work around the horizontal scrollbars */\n#column-content { margin-left: 0; }\n\n"));
        // There are two variants of KHTMLFixes.css. One is equal to mediaWikiKHTMLFixesStyleSheet,
        // while the other lacks the second trailing newline.
        if (baseURL().string().endsWith("/KHTMLFixes.css") && !sheetText.isNull() && mediaWikiKHTMLFixesStyleSheet.get().startsWith(sheetText)
            && sheetText.length() >= mediaWikiKHTMLFixesStyleSheet.get().length() - 1)
            clearRules();
    }
}

bool StyleSheetContents::parseString(const String& sheetText)
{
    CSSParser p(parserContext());
    p.parseSheet(this, sheetText, parserContext().mode != UASheetMode ? CSSParser::RuleParsing::Deferred : CSSParser::RuleParsing::Normal);
    return true;
}

bool StyleSheetContents::isLoading() const
{
    for (unsigned i = 0; i < m_importRules.size(); ++i) {
        if (m_importRules[i]->isLoading())
            return true;
    }
    return false;
}

void StyleSheetContents::checkLoaded()
{
    if (isLoading())
        return;

    Ref<StyleSheetContents> protectedThis(*this);
    StyleSheetContents* parentSheet = parentStyleSheet();
    if (parentSheet) {
        parentSheet->checkLoaded();
        m_loadCompleted = true;
        return;
    }
    RefPtr<Node> ownerNode = singleOwnerNode();
    if (!ownerNode) {
        m_loadCompleted = true;
        return;
    }
    m_loadCompleted = ownerNode->sheetLoaded();
    if (m_loadCompleted)
        ownerNode->notifyLoadedSheetAndAllCriticalSubresources(m_didLoadErrorOccur);
}

void StyleSheetContents::notifyLoadedSheet(const CachedCSSStyleSheet* sheet)
{
    ASSERT(sheet);
    m_didLoadErrorOccur |= sheet->errorOccurred();
    m_didLoadErrorOccur |= !sheet->mimeTypeAllowedByNosniff();
}

void StyleSheetContents::startLoadingDynamicSheet()
{
    if (Node* owner = singleOwnerNode())
        owner->startLoadingDynamicSheet();
}

StyleSheetContents* StyleSheetContents::rootStyleSheet() const
{
    const StyleSheetContents* root = this;
    while (root->parentStyleSheet())
        root = root->parentStyleSheet();
    return const_cast<StyleSheetContents*>(root);
}

Node* StyleSheetContents::singleOwnerNode() const
{
    StyleSheetContents* root = rootStyleSheet();
    if (root->m_clients.isEmpty())
        return 0;
    ASSERT(root->m_clients.size() == 1);
    return root->m_clients[0]->ownerNode();
}

Document* StyleSheetContents::singleOwnerDocument() const
{
    Node* ownerNode = singleOwnerNode();
    return ownerNode ? &ownerNode->document() : 0;
}

URL StyleSheetContents::completeURL(const String& url) const
{
    return m_parserContext.completeURL(url);
}

static bool traverseSubresourcesInRules(const Vector<RefPtr<StyleRuleBase>>& rules, const WTF::Function<bool (const CachedResource&)>& handler)
{
    for (auto& rule : rules) {
        switch (rule->type()) {
        case StyleRuleBase::Style: {
            auto* properties = downcast<StyleRule>(*rule).propertiesWithoutDeferredParsing();
            if (properties && properties->traverseSubresources(handler))
                return true;
            break;
        }
        case StyleRuleBase::FontFace:
            if (downcast<StyleRuleFontFace>(*rule).properties().traverseSubresources(handler))
                return true;
            break;
        case StyleRuleBase::Media: {
            auto* childRules = downcast<StyleRuleMedia>(*rule).childRulesWithoutDeferredParsing();
            if (childRules && traverseSubresourcesInRules(*childRules, handler))
                return true;
            break;
        }
        case StyleRuleBase::Import:
            ASSERT_NOT_REACHED();
#if ASSERT_DISABLED
            FALLTHROUGH;
#endif
        case StyleRuleBase::Page:
        case StyleRuleBase::Keyframes:
        case StyleRuleBase::Namespace:
        case StyleRuleBase::Unknown:
        case StyleRuleBase::Charset:
        case StyleRuleBase::Keyframe:
        case StyleRuleBase::Supports:
#if ENABLE(CSS_DEVICE_ADAPTATION)
        case StyleRuleBase::Viewport:
#endif
            break;
        }
    }
    return false;
}

bool StyleSheetContents::traverseSubresources(const WTF::Function<bool (const CachedResource&)>& handler) const
{
    for (auto& importRule : m_importRules) {
        if (auto* cachedResource = importRule->cachedCSSStyleSheet()) {
            if (handler(*cachedResource))
                return true;
        }
        auto* importedStyleSheet = importRule->styleSheet();
        if (importedStyleSheet && importedStyleSheet->traverseSubresources(handler))
            return true;
    }
    return traverseSubresourcesInRules(m_childRules, handler);
}

bool StyleSheetContents::subresourcesAllowReuse(CachePolicy cachePolicy, FrameLoader& loader) const
{
    bool hasFailedOrExpiredResources = traverseSubresources([cachePolicy, &loader](const CachedResource& resource) {
        if (resource.loadFailedOrCanceled())
            return true;
        // We can't revalidate subresources individually so don't use reuse the parsed sheet if they need revalidation.
        if (resource.makeRevalidationDecision(cachePolicy) != CachedResource::RevalidationDecision::No)
            return true;

#if ENABLE(CONTENT_EXTENSIONS)
        // If a cached subresource is blocked or made HTTPS by a content blocker, we cannot reuse the cached stylesheet.
        auto* page = loader.frame().page();
        auto* documentLoader = loader.documentLoader();
        if (page && documentLoader) {
            const auto& request = resource.resourceRequest();
            auto blockedStatus = page->userContentProvider().processContentExtensionRulesForLoad(request.url(), toResourceType(resource.type()), *documentLoader);
            if (blockedStatus.blockedLoad || blockedStatus.madeHTTPS)
                return true;
        }
#else
        UNUSED_PARAM(loader);
#endif

        return false;
    });
    return !hasFailedOrExpiredResources;
}

bool StyleSheetContents::isLoadingSubresources() const
{
    return traverseSubresources([](const CachedResource& resource) {
        return resource.isLoading();
    });
}

StyleSheetContents* StyleSheetContents::parentStyleSheet() const
{
    return m_ownerRule ? m_ownerRule->parentStyleSheet() : 0;
}

void StyleSheetContents::registerClient(CSSStyleSheet* sheet)
{
    ASSERT(!m_clients.contains(sheet));
    m_clients.append(sheet);
}

void StyleSheetContents::unregisterClient(CSSStyleSheet* sheet)
{
    bool removed = m_clients.removeFirst(sheet);
    ASSERT_UNUSED(removed, removed);
}

void StyleSheetContents::addedToMemoryCache()
{
    ASSERT(isCacheable());
    ++m_inMemoryCacheCount;
}

void StyleSheetContents::removedFromMemoryCache()
{
    ASSERT(m_inMemoryCacheCount);
    ASSERT(isCacheable());
    --m_inMemoryCacheCount;
}

void StyleSheetContents::shrinkToFit()
{
    m_importRules.shrinkToFit();
    m_childRules.shrinkToFit();
}

}
