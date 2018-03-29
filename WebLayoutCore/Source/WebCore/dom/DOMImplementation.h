/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2016 Apple Inc. All rights reserved.
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
 *
 */

#pragma once

#include "ExceptionOr.h"
#include "XMLDocument.h"

namespace WebCore {

class DOMImplementation : public ScriptWrappable {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit DOMImplementation(Document&);

    void ref() { m_document.ref(); }
    void deref() { m_document.deref(); }
    Document& document() { return m_document; }

     ExceptionOr<Ref<DocumentType>> createDocumentType(const String& qualifiedName, const String& publicId, const String& systemId);
     ExceptionOr<Ref<XMLDocument>> createDocument(const String& namespaceURI, const String& qualifiedName, DocumentType*);
     Ref<HTMLDocument> createHTMLDocument(const String& title);
    static bool hasFeature() { return true; }
     static Ref<CSSStyleSheet> createCSSStyleSheet(const String& title, const String& media);

    static Ref<Document> createDocument(const String& MIMEType, Frame*, const URL&);

private:
    Document& m_document;
};

} // namespace WebCore
