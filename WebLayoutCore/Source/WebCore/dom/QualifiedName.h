/*
 * Copyright (C) 2005-2017 Apple Inc. All rights reserved.
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

#include <wtf/HashTraits.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/text/AtomicString.h>

namespace WebCore {

struct QualifiedNameComponents {
    StringImpl* m_prefix;
    StringImpl* m_localName;
    StringImpl* m_namespace;
};

class QualifiedName {
    WTF_MAKE_FAST_ALLOCATED;
public:
    class QualifiedNameImpl : public RefCounted<QualifiedNameImpl> {
    public:
        static Ref<QualifiedNameImpl> create(const AtomicString& prefix, const AtomicString& localName, const AtomicString& namespaceURI)
        {
            return adoptRef(*new QualifiedNameImpl(prefix, localName, namespaceURI));
        }

         ~QualifiedNameImpl();

        unsigned computeHash() const;

        mutable unsigned m_existingHash { 0 };
        const AtomicString m_prefix;
        const AtomicString m_localName;
        const AtomicString m_namespace;
        mutable AtomicString m_localNameUpper;

#if ENABLE(JIT)
        static ptrdiff_t localNameMemoryOffset() { return OBJECT_OFFSETOF(QualifiedNameImpl, m_localName); }
        static ptrdiff_t namespaceMemoryOffset() { return OBJECT_OFFSETOF(QualifiedNameImpl, m_namespace); }
#endif

    private:
        QualifiedNameImpl(const AtomicString& prefix, const AtomicString& localName, const AtomicString& namespaceURI)
            : m_prefix(prefix)
            , m_localName(localName)
            , m_namespace(namespaceURI)
        {
            ASSERT(!namespaceURI.isEmpty() || namespaceURI.isNull());
        }        
    };

     QualifiedName(const AtomicString& prefix, const AtomicString& localName, const AtomicString& namespaceURI);
    explicit QualifiedName(WTF::HashTableDeletedValueType) : m_impl(WTF::HashTableDeletedValue) { }
    bool isHashTableDeletedValue() const { return m_impl.isHashTableDeletedValue(); }
#ifdef QNAME_DEFAULT_CONSTRUCTOR
    QualifiedName() { }
#endif

    bool operator==(const QualifiedName& other) const { return m_impl == other.m_impl; }
    bool operator!=(const QualifiedName& other) const { return !(*this == other); }

    bool matches(const QualifiedName& other) const { return m_impl == other.m_impl || (localName() == other.localName() && namespaceURI() == other.namespaceURI()); }

    bool hasPrefix() const { return !m_impl->m_prefix.isNull(); }
    void setPrefix(const AtomicString& prefix) { *this = QualifiedName(prefix, localName(), namespaceURI()); }

    const AtomicString& prefix() const { return m_impl->m_prefix; }
    const AtomicString& localName() const { return m_impl->m_localName; }
    const AtomicString& namespaceURI() const { return m_impl->m_namespace; }

    // Uppercased localName, cached for efficiency
    const AtomicString& localNameUpper() const;

    String toString() const;

    QualifiedNameImpl* impl() const { return m_impl.get(); }
#if ENABLE(JIT)
    static ptrdiff_t implMemoryOffset() { return OBJECT_OFFSETOF(QualifiedName, m_impl); }
#endif
    
    // Init routine for globals
     static void init();

private:
    static QualifiedNameImpl* hashTableDeletedValue() { return RefPtr<QualifiedNameImpl>::hashTableDeletedValue(); }
    
    RefPtr<QualifiedNameImpl> m_impl;
};

extern LazyNeverDestroyed<const QualifiedName> anyName;
inline const QualifiedName& anyQName() { return anyName; }

const QualifiedName& nullQName();

inline bool operator==(const AtomicString& a, const QualifiedName& q) { return a == q.localName(); }
inline bool operator!=(const AtomicString& a, const QualifiedName& q) { return a != q.localName(); }
inline bool operator==(const QualifiedName& q, const AtomicString& a) { return a == q.localName(); }
inline bool operator!=(const QualifiedName& q, const AtomicString& a) { return a != q.localName(); }

inline unsigned hashComponents(const QualifiedNameComponents& buf)
{
    return StringHasher::hashMemory<sizeof(QualifiedNameComponents)>(&buf);
}

struct QualifiedNameHash {
    static unsigned hash(const QualifiedName& name) { return hash(name.impl()); }

    static unsigned hash(const QualifiedName::QualifiedNameImpl* name) 
    {
        if (!name->m_existingHash)
            name->m_existingHash = name->computeHash();
        return name->m_existingHash;
    }

    static bool equal(const QualifiedName& a, const QualifiedName& b) { return a == b; }
    static bool equal(const QualifiedName::QualifiedNameImpl* a, const QualifiedName::QualifiedNameImpl* b) { return a == b; }

    static const bool safeToCompareToEmptyOrDeleted = false;
};

inline String QualifiedName::toString() const
{
    if (!hasPrefix())
        return localName();

    return prefix().string() + ':' + localName().string();
}

} // namespace WebCore

namespace WTF {
    
    template<typename T> struct DefaultHash;

    template<> struct DefaultHash<WebCore::QualifiedName> {
        typedef WebCore::QualifiedNameHash Hash;
    };
    
    template<> struct HashTraits<WebCore::QualifiedName> : SimpleClassHashTraits<WebCore::QualifiedName> {
        static const bool emptyValueIsZero = false;
        static WebCore::QualifiedName emptyValue() { return WebCore::nullQName(); }
    };

} // namespace WTF
