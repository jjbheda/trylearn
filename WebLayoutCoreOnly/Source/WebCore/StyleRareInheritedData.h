/*
 * Copyright (C) 2000 Lars Knoll (knoll@kde.org)
 *           (C) 2000 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Graham Dennis (graham.dennis@gmail.com)
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

#include "Color.h"
#include "DataRef.h"
#include "Length.h"
#include "StyleCustomPropertyData.h"
#include <wtf/RefCounted.h>
#include <wtf/text/AtomicString.h>

#if ENABLE(TEXT_AUTOSIZING)
#include "TextSizeAdjustment.h"
#endif

namespace WebCore {

class QuotesData;

// This struct is for rarely used inherited CSS3, CSS2, and WebKit-specific properties.
// By grouping them together, we save space, and only allocate this object when someone
// actually uses one of these properties.
class StyleRareInheritedData : public RefCounted<StyleRareInheritedData> {
public:
    static Ref<StyleRareInheritedData> create() { return adoptRef(*new StyleRareInheritedData); }
    Ref<StyleRareInheritedData> copy() const;
    ~StyleRareInheritedData();

    bool operator==(const StyleRareInheritedData& o) const;
    bool operator!=(const StyleRareInheritedData& o) const
    {
        return !(*this == o);
    }


    Color textStrokeColor;
    float textStrokeWidth;
    Color textFillColor;
    Color textEmphasisColor;
    
    Color visitedLinkTextStrokeColor;
    Color visitedLinkTextFillColor;
    Color visitedLinkTextEmphasisColor;

    Color caretColor;
    Color visitedLinkCaretColor;

    Length indent;
    float effectiveZoom;
    
    Length wordSpacing;

    DataRef<StyleCustomPropertyData> customProperties;

    // Paged media properties.
    short widows;
    short orphans;
    unsigned hasAutoWidows : 1;
    unsigned hasAutoOrphans : 1;
    
    unsigned textSecurity : 2; // ETextSecurity
    unsigned userModify : 2; // EUserModify (editing)
    unsigned wordBreak : 2; // EWordBreak
    unsigned overflowWrap : 1; // EOverflowWrap
    unsigned nbspMode : 1; // ENBSPMode
    unsigned lineBreak : 3; // LineBreak
    unsigned userSelect : 2; // EUserSelect
    unsigned colorSpace : 1; // ColorSpace
    unsigned speak : 3; // ESpeak
    unsigned hyphens : 2; // Hyphens
    unsigned textEmphasisFill : 1; // TextEmphasisFill
    unsigned textEmphasisMark : 3; // TextEmphasisMark
    unsigned textEmphasisPosition : 4; // TextEmphasisPosition
    unsigned textOrientation : 2; // TextOrientation
#if ENABLE(CSS3_TEXT)
    unsigned textIndentLine : 1; // TextIndentLine
    unsigned textIndentType : 1; // TextIndentType
#endif
    // CSS Image Values Level 3
#if ENABLE(CSS_IMAGE_ORIENTATION)
    unsigned imageOrientation : 4; // ImageOrientationEnum
#endif
    unsigned imageRendering : 3; // EImageRendering
    unsigned lineSnap : 2; // LineSnap
    unsigned lineAlign : 1; // LineAlign
#if ENABLE(ACCELERATED_OVERFLOW_SCROLLING)
    unsigned useTouchOverflowScrolling: 1;
#endif
#if ENABLE(CSS_IMAGE_RESOLUTION)
    unsigned imageResolutionSource : 1; // ImageResolutionSource
    unsigned imageResolutionSnap : 1; // ImageResolutionSnap
#endif
#if ENABLE(CSS3_TEXT)
    unsigned textAlignLast : 3; // TextAlignLast
    unsigned textJustify : 2; // TextJustify
#endif
    unsigned textDecorationSkip : 5; // TextDecorationSkip
    unsigned textUnderlinePosition : 3; // TextUnderlinePosition
    unsigned rubyPosition : 2; // RubyPosition
    unsigned textZoom: 1; // TextZoom

#if PLATFORM(IOS)
    unsigned touchCalloutEnabled : 1;
#endif

#if ENABLE(CSS_TRAILING_WORD)
    unsigned trailingWord : 1;
#endif

    unsigned hangingPunctuation : 4;

    unsigned paintOrder : 3; // PaintOrder
    unsigned hasSetStrokeWidth : 1;
    unsigned hasSetStrokeColor : 1;
    Length strokeWidth;
    Color strokeColor;
    Color visitedLinkStrokeColor;
    float miterLimit;

    AtomicString hyphenationString;
    short hyphenationLimitBefore;
    short hyphenationLimitAfter;
    short hyphenationLimitLines;

    AtomicString textEmphasisCustomMark;
    RefPtr<QuotesData> quotes;

    AtomicString lineGrid;
    unsigned tabSize;

#if ENABLE(TEXT_AUTOSIZING)
    TextSizeAdjustment textSizeAdjust;
#endif

#if ENABLE(CSS_IMAGE_RESOLUTION)
    float imageResolution;
#endif

#if ENABLE(TOUCH_EVENTS)
    Color tapHighlightColor;
#endif

private:
    StyleRareInheritedData();
    StyleRareInheritedData(const StyleRareInheritedData&);
};

} // namespace WebCore
