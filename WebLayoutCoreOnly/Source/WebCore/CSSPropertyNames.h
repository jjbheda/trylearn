/* This file is automatically generated from CSSProperties.json by makeprop, do not edit */

#pragma once

#include <string.h>
#include <wtf/HashFunctions.h>
#include <wtf/HashTraits.h>

namespace WTF {
class AtomicString;
class String;
}

namespace WebCore {

enum CSSPropertyID : uint16_t {
    CSSPropertyInvalid = 0,
    CSSPropertyCustom = 1,
    CSSPropertyColor = 2,
    CSSPropertyDirection = 3,
    CSSPropertyDisplay = 4,
    CSSPropertyFontFamily = 5,
    CSSPropertyFontFeatureSettings = 6,
    CSSPropertyFontOpticalSizing = 7,
    CSSPropertyFontSize = 8,
    CSSPropertyFontStretch = 9,
    CSSPropertyFontStyle = 10,
    CSSPropertyFontSynthesis = 11,
    CSSPropertyFontVariantAlternates = 12,
    CSSPropertyFontVariantCaps = 13,
    CSSPropertyFontVariantEastAsian = 14,
    CSSPropertyFontVariantLigatures = 15,
    CSSPropertyFontVariantNumeric = 16,
    CSSPropertyFontVariantPosition = 17,
    CSSPropertyFontVariationSettings = 18,
    CSSPropertyFontWeight = 19,
    CSSPropertyTextRendering = 20,
    CSSPropertyWritingMode = 21,
    CSSPropertyZoom = 22,
    CSSPropertyWebkitFontKerning = 23,
    CSSPropertyWebkitFontSmoothing = 24,
    CSSPropertyWebkitLocale = 25,
    CSSPropertyWebkitTextOrientation = 26,
    CSSPropertyWebkitTextSizeAdjust = 27,
    CSSPropertyWebkitTextZoom = 28,
    CSSPropertyAlignContent = 29,
    CSSPropertyAlignItems = 30,
    CSSPropertyAlignSelf = 31,
    CSSPropertyAlignmentBaseline = 32,
    CSSPropertyAll = 33,
    CSSPropertyAlt = 34,
    CSSPropertyAnimation = 35,
    CSSPropertyAnimationDelay = 36,
    CSSPropertyAnimationDirection = 37,
    CSSPropertyAnimationDuration = 38,
    CSSPropertyAnimationFillMode = 39,
    CSSPropertyAnimationIterationCount = 40,
    CSSPropertyAnimationName = 41,
    CSSPropertyAnimationPlayState = 42,
    CSSPropertyAnimationTimingFunction = 43,
    CSSPropertyBackground = 44,
    CSSPropertyBackgroundAttachment = 45,
    CSSPropertyBackgroundBlendMode = 46,
    CSSPropertyBackgroundClip = 47,
    CSSPropertyBackgroundColor = 48,
    CSSPropertyBackgroundImage = 49,
    CSSPropertyBackgroundOrigin = 50,
    CSSPropertyBackgroundPosition = 51,
    CSSPropertyBackgroundPositionX = 52,
    CSSPropertyBackgroundPositionY = 53,
    CSSPropertyBackgroundRepeat = 54,
    CSSPropertyBackgroundRepeatX = 55,
    CSSPropertyBackgroundRepeatY = 56,
    CSSPropertyBackgroundSize = 57,
    CSSPropertyBaselineShift = 58,
    CSSPropertyBorder = 59,
    CSSPropertyBorderBottom = 60,
    CSSPropertyBorderBottomColor = 61,
    CSSPropertyBorderBottomLeftRadius = 62,
    CSSPropertyBorderBottomRightRadius = 63,
    CSSPropertyBorderBottomStyle = 64,
    CSSPropertyBorderBottomWidth = 65,
    CSSPropertyBorderCollapse = 66,
    CSSPropertyBorderColor = 67,
    CSSPropertyBorderImage = 68,
    CSSPropertyBorderImageOutset = 69,
    CSSPropertyBorderImageRepeat = 70,
    CSSPropertyBorderImageSlice = 71,
    CSSPropertyBorderImageSource = 72,
    CSSPropertyBorderImageWidth = 73,
    CSSPropertyBorderLeft = 74,
    CSSPropertyBorderLeftColor = 75,
    CSSPropertyBorderLeftStyle = 76,
    CSSPropertyBorderLeftWidth = 77,
    CSSPropertyBorderRadius = 78,
    CSSPropertyBorderRight = 79,
    CSSPropertyBorderRightColor = 80,
    CSSPropertyBorderRightStyle = 81,
    CSSPropertyBorderRightWidth = 82,
    CSSPropertyBorderSpacing = 83,
    CSSPropertyBorderStyle = 84,
    CSSPropertyBorderTop = 85,
    CSSPropertyBorderTopColor = 86,
    CSSPropertyBorderTopLeftRadius = 87,
    CSSPropertyBorderTopRightRadius = 88,
    CSSPropertyBorderTopStyle = 89,
    CSSPropertyBorderTopWidth = 90,
    CSSPropertyBorderWidth = 91,
    CSSPropertyBottom = 92,
    CSSPropertyBoxShadow = 93,
    CSSPropertyBoxSizing = 94,
    CSSPropertyBreakAfter = 95,
    CSSPropertyBreakBefore = 96,
    CSSPropertyBreakInside = 97,
    CSSPropertyBufferedRendering = 98,
    CSSPropertyCaptionSide = 99,
    CSSPropertyCaretColor = 100,
    CSSPropertyClear = 101,
    CSSPropertyClip = 102,
    CSSPropertyClipPath = 103,
    CSSPropertyClipRule = 104,
    CSSPropertyColorInterpolation = 105,
    CSSPropertyColorInterpolationFilters = 106,
    CSSPropertyColorProfile = 107,
    CSSPropertyColorRendering = 108,
    CSSPropertyColumnCount = 109,
    CSSPropertyColumnFill = 110,
    CSSPropertyColumnGap = 111,
    CSSPropertyColumnRule = 112,
    CSSPropertyColumnRuleColor = 113,
    CSSPropertyColumnRuleStyle = 114,
    CSSPropertyColumnRuleWidth = 115,
    CSSPropertyColumnSpan = 116,
    CSSPropertyColumnWidth = 117,
    CSSPropertyColumns = 118,
    CSSPropertyContent = 119,
    CSSPropertyCounterIncrement = 120,
    CSSPropertyCounterReset = 121,
    CSSPropertyCursor = 122,
    CSSPropertyCx = 123,
    CSSPropertyCy = 124,
    CSSPropertyDominantBaseline = 125,
    CSSPropertyEmptyCells = 126,
    CSSPropertyEnableBackground = 127,
    CSSPropertyFill = 128,
    CSSPropertyFillOpacity = 129,
    CSSPropertyFillRule = 130,
    CSSPropertyFilter = 131,
    CSSPropertyFlex = 132,
    CSSPropertyFlexBasis = 133,
    CSSPropertyFlexDirection = 134,
    CSSPropertyFlexFlow = 135,
    CSSPropertyFlexGrow = 136,
    CSSPropertyFlexShrink = 137,
    CSSPropertyFlexWrap = 138,
    CSSPropertyFloat = 139,
    CSSPropertyFloodColor = 140,
    CSSPropertyFloodOpacity = 141,
    CSSPropertyFont = 142,
    CSSPropertyFontDisplay = 143,
    CSSPropertyFontVariant = 144,
    CSSPropertyGlyphOrientationHorizontal = 145,
    CSSPropertyGlyphOrientationVertical = 146,
    CSSPropertyGrid = 147,
    CSSPropertyGridArea = 148,
    CSSPropertyGridAutoColumns = 149,
    CSSPropertyGridAutoFlow = 150,
    CSSPropertyGridAutoRows = 151,
    CSSPropertyGridColumn = 152,
    CSSPropertyGridColumnEnd = 153,
    CSSPropertyGridColumnGap = 154,
    CSSPropertyGridColumnStart = 155,
    CSSPropertyGridGap = 156,
    CSSPropertyGridRow = 157,
    CSSPropertyGridRowEnd = 158,
    CSSPropertyGridRowGap = 159,
    CSSPropertyGridRowStart = 160,
    CSSPropertyGridTemplate = 161,
    CSSPropertyGridTemplateAreas = 162,
    CSSPropertyGridTemplateColumns = 163,
    CSSPropertyGridTemplateRows = 164,
    CSSPropertyHangingPunctuation = 165,
    CSSPropertyHeight = 166,
    CSSPropertyImageRendering = 167,
    CSSPropertyIsolation = 168,
    CSSPropertyJustifyContent = 169,
    CSSPropertyJustifyItems = 170,
    CSSPropertyJustifySelf = 171,
    CSSPropertyKerning = 172,
    CSSPropertyLeft = 173,
    CSSPropertyLetterSpacing = 174,
    CSSPropertyLightingColor = 175,
    CSSPropertyLineBreak = 176,
    CSSPropertyLineHeight = 177,
    CSSPropertyListStyle = 178,
    CSSPropertyListStyleImage = 179,
    CSSPropertyListStylePosition = 180,
    CSSPropertyListStyleType = 181,
    CSSPropertyMargin = 182,
    CSSPropertyMarginBottom = 183,
    CSSPropertyMarginLeft = 184,
    CSSPropertyMarginRight = 185,
    CSSPropertyMarginTop = 186,
    CSSPropertyMarker = 187,
    CSSPropertyMarkerEnd = 188,
    CSSPropertyMarkerMid = 189,
    CSSPropertyMarkerStart = 190,
    CSSPropertyMask = 191,
    CSSPropertyMaskType = 192,
    CSSPropertyMaxHeight = 193,
    CSSPropertyMaxWidth = 194,
    CSSPropertyMinHeight = 195,
    CSSPropertyMinWidth = 196,
    CSSPropertyMixBlendMode = 197,
    CSSPropertyObjectFit = 198,
    CSSPropertyObjectPosition = 199,
    CSSPropertyOpacity = 200,
    CSSPropertyOrder = 201,
    CSSPropertyOrphans = 202,
    CSSPropertyOutline = 203,
    CSSPropertyOutlineColor = 204,
    CSSPropertyOutlineOffset = 205,
    CSSPropertyOutlineStyle = 206,
    CSSPropertyOutlineWidth = 207,
    CSSPropertyOverflow = 208,
    CSSPropertyOverflowWrap = 209,
    CSSPropertyOverflowX = 210,
    CSSPropertyOverflowY = 211,
    CSSPropertyPadding = 212,
    CSSPropertyPaddingBottom = 213,
    CSSPropertyPaddingLeft = 214,
    CSSPropertyPaddingRight = 215,
    CSSPropertyPaddingTop = 216,
    CSSPropertyPage = 217,
    CSSPropertyPageBreakAfter = 218,
    CSSPropertyPageBreakBefore = 219,
    CSSPropertyPageBreakInside = 220,
    CSSPropertyPaintOrder = 221,
    CSSPropertyPerspective = 222,
    CSSPropertyPerspectiveOrigin = 223,
    CSSPropertyPerspectiveOriginX = 224,
    CSSPropertyPerspectiveOriginY = 225,
    CSSPropertyPlaceContent = 226,
    CSSPropertyPlaceItems = 227,
    CSSPropertyPlaceSelf = 228,
    CSSPropertyPointerEvents = 229,
    CSSPropertyPosition = 230,
    CSSPropertyQuotes = 231,
    CSSPropertyR = 232,
    CSSPropertyResize = 233,
    CSSPropertyRight = 234,
    CSSPropertyRx = 235,
    CSSPropertyRy = 236,
    CSSPropertyScrollPadding = 237,
    CSSPropertyScrollPaddingBottom = 238,
    CSSPropertyScrollPaddingLeft = 239,
    CSSPropertyScrollPaddingRight = 240,
    CSSPropertyScrollPaddingTop = 241,
    CSSPropertyScrollSnapAlign = 242,
    CSSPropertyScrollSnapMargin = 243,
    CSSPropertyScrollSnapMarginBottom = 244,
    CSSPropertyScrollSnapMarginLeft = 245,
    CSSPropertyScrollSnapMarginRight = 246,
    CSSPropertyScrollSnapMarginTop = 247,
    CSSPropertyScrollSnapType = 248,
    CSSPropertyShapeImageThreshold = 249,
    CSSPropertyShapeMargin = 250,
    CSSPropertyShapeOutside = 251,
    CSSPropertyShapeRendering = 252,
    CSSPropertySize = 253,
    CSSPropertySpeak = 254,
    CSSPropertySrc = 255,
    CSSPropertyStopColor = 256,
    CSSPropertyStopOpacity = 257,
    CSSPropertyStroke = 258,
    CSSPropertyStrokeColor = 259,
    CSSPropertyStrokeDasharray = 260,
    CSSPropertyStrokeDashoffset = 261,
    CSSPropertyStrokeLinecap = 262,
    CSSPropertyStrokeLinejoin = 263,
    CSSPropertyStrokeMiterlimit = 264,
    CSSPropertyStrokeOpacity = 265,
    CSSPropertyStrokeWidth = 266,
    CSSPropertyTabSize = 267,
    CSSPropertyTableLayout = 268,
    CSSPropertyTextAlign = 269,
    CSSPropertyTextAnchor = 270,
    CSSPropertyTextDecoration = 271,
    CSSPropertyTextIndent = 272,
    CSSPropertyTextOverflow = 273,
    CSSPropertyTextShadow = 274,
    CSSPropertyTextTransform = 275,
    CSSPropertyTop = 276,
    CSSPropertyTransform = 277,
    CSSPropertyTransformBox = 278,
    CSSPropertyTransformOrigin = 279,
    CSSPropertyTransformOriginX = 280,
    CSSPropertyTransformOriginY = 281,
    CSSPropertyTransformOriginZ = 282,
    CSSPropertyTransformStyle = 283,
    CSSPropertyTransition = 284,
    CSSPropertyTransitionDelay = 285,
    CSSPropertyTransitionDuration = 286,
    CSSPropertyTransitionProperty = 287,
    CSSPropertyTransitionTimingFunction = 288,
    CSSPropertyUnicodeBidi = 289,
    CSSPropertyUnicodeRange = 290,
    CSSPropertyVectorEffect = 291,
    CSSPropertyVerticalAlign = 292,
    CSSPropertyVisibility = 293,
    CSSPropertyWhiteSpace = 294,
    CSSPropertyWidows = 295,
    CSSPropertyWidth = 296,
    CSSPropertyWillChange = 297,
    CSSPropertyWordBreak = 298,
    CSSPropertyWordSpacing = 299,
    CSSPropertyWordWrap = 300,
    CSSPropertyX = 301,
    CSSPropertyY = 302,
    CSSPropertyZIndex = 303,
    CSSPropertyApplePayButtonStyle = 304,
    CSSPropertyApplePayButtonType = 305,
    CSSPropertyAppleTrailingWord = 306,
    CSSPropertyWebkitAnimationTrigger = 307,
    CSSPropertyWebkitAppearance = 308,
    CSSPropertyWebkitAspectRatio = 309,
    CSSPropertyWebkitBackdropFilter = 310,
    CSSPropertyWebkitBackfaceVisibility = 311,
    CSSPropertyWebkitBackgroundClip = 312,
    CSSPropertyWebkitBackgroundComposite = 313,
    CSSPropertyWebkitBackgroundOrigin = 314,
    CSSPropertyWebkitBackgroundSize = 315,
    CSSPropertyWebkitBorderAfter = 316,
    CSSPropertyWebkitBorderAfterColor = 317,
    CSSPropertyWebkitBorderAfterStyle = 318,
    CSSPropertyWebkitBorderAfterWidth = 319,
    CSSPropertyWebkitBorderBefore = 320,
    CSSPropertyWebkitBorderBeforeColor = 321,
    CSSPropertyWebkitBorderBeforeStyle = 322,
    CSSPropertyWebkitBorderBeforeWidth = 323,
    CSSPropertyWebkitBorderEnd = 324,
    CSSPropertyWebkitBorderEndColor = 325,
    CSSPropertyWebkitBorderEndStyle = 326,
    CSSPropertyWebkitBorderEndWidth = 327,
    CSSPropertyWebkitBorderFit = 328,
    CSSPropertyWebkitBorderHorizontalSpacing = 329,
    CSSPropertyWebkitBorderImage = 330,
    CSSPropertyWebkitBorderRadius = 331,
    CSSPropertyWebkitBorderStart = 332,
    CSSPropertyWebkitBorderStartColor = 333,
    CSSPropertyWebkitBorderStartStyle = 334,
    CSSPropertyWebkitBorderStartWidth = 335,
    CSSPropertyWebkitBorderVerticalSpacing = 336,
    CSSPropertyWebkitBoxAlign = 337,
    CSSPropertyWebkitBoxDecorationBreak = 338,
    CSSPropertyWebkitBoxDirection = 339,
    CSSPropertyWebkitBoxFlex = 340,
    CSSPropertyWebkitBoxFlexGroup = 341,
    CSSPropertyWebkitBoxLines = 342,
    CSSPropertyWebkitBoxOrdinalGroup = 343,
    CSSPropertyWebkitBoxOrient = 344,
    CSSPropertyWebkitBoxPack = 345,
    CSSPropertyWebkitBoxReflect = 346,
    CSSPropertyWebkitBoxShadow = 347,
    CSSPropertyWebkitClipPath = 348,
    CSSPropertyWebkitColumnAxis = 349,
    CSSPropertyWebkitColumnBreakAfter = 350,
    CSSPropertyWebkitColumnBreakBefore = 351,
    CSSPropertyWebkitColumnBreakInside = 352,
    CSSPropertyWebkitColumnProgression = 353,
    CSSPropertyWebkitCursorVisibility = 354,
    CSSPropertyWebkitDashboardRegion = 355,
    CSSPropertyWebkitFontSizeDelta = 356,
    CSSPropertyWebkitHyphenateCharacter = 357,
    CSSPropertyWebkitHyphenateLimitAfter = 358,
    CSSPropertyWebkitHyphenateLimitBefore = 359,
    CSSPropertyWebkitHyphenateLimitLines = 360,
    CSSPropertyWebkitHyphens = 361,
    CSSPropertyWebkitInitialLetter = 362,
    CSSPropertyWebkitLineAlign = 363,
    CSSPropertyWebkitLineBoxContain = 364,
    CSSPropertyWebkitLineClamp = 365,
    CSSPropertyWebkitLineGrid = 366,
    CSSPropertyWebkitLineSnap = 367,
    CSSPropertyWebkitLogicalHeight = 368,
    CSSPropertyWebkitLogicalWidth = 369,
    CSSPropertyWebkitMarginAfter = 370,
    CSSPropertyWebkitMarginAfterCollapse = 371,
    CSSPropertyWebkitMarginBefore = 372,
    CSSPropertyWebkitMarginBeforeCollapse = 373,
    CSSPropertyWebkitMarginBottomCollapse = 374,
    CSSPropertyWebkitMarginCollapse = 375,
    CSSPropertyWebkitMarginEnd = 376,
    CSSPropertyWebkitMarginStart = 377,
    CSSPropertyWebkitMarginTopCollapse = 378,
    CSSPropertyWebkitMarquee = 379,
    CSSPropertyWebkitMarqueeDirection = 380,
    CSSPropertyWebkitMarqueeIncrement = 381,
    CSSPropertyWebkitMarqueeRepetition = 382,
    CSSPropertyWebkitMarqueeSpeed = 383,
    CSSPropertyWebkitMarqueeStyle = 384,
    CSSPropertyWebkitMask = 385,
    CSSPropertyWebkitMaskBoxImage = 386,
    CSSPropertyWebkitMaskBoxImageOutset = 387,
    CSSPropertyWebkitMaskBoxImageRepeat = 388,
    CSSPropertyWebkitMaskBoxImageSlice = 389,
    CSSPropertyWebkitMaskBoxImageSource = 390,
    CSSPropertyWebkitMaskBoxImageWidth = 391,
    CSSPropertyWebkitMaskClip = 392,
    CSSPropertyWebkitMaskComposite = 393,
    CSSPropertyWebkitMaskImage = 394,
    CSSPropertyWebkitMaskOrigin = 395,
    CSSPropertyWebkitMaskPosition = 396,
    CSSPropertyWebkitMaskPositionX = 397,
    CSSPropertyWebkitMaskPositionY = 398,
    CSSPropertyWebkitMaskRepeat = 399,
    CSSPropertyWebkitMaskRepeatX = 400,
    CSSPropertyWebkitMaskRepeatY = 401,
    CSSPropertyWebkitMaskSize = 402,
    CSSPropertyWebkitMaskSourceType = 403,
    CSSPropertyWebkitMaxLogicalHeight = 404,
    CSSPropertyWebkitMaxLogicalWidth = 405,
    CSSPropertyWebkitMinLogicalHeight = 406,
    CSSPropertyWebkitMinLogicalWidth = 407,
    CSSPropertyWebkitNbspMode = 408,
    CSSPropertyWebkitPaddingAfter = 409,
    CSSPropertyWebkitPaddingBefore = 410,
    CSSPropertyWebkitPaddingEnd = 411,
    CSSPropertyWebkitPaddingStart = 412,
    CSSPropertyWebkitPrintColorAdjust = 413,
    CSSPropertyWebkitRtlOrdering = 414,
    CSSPropertyWebkitRubyPosition = 415,
    CSSPropertyWebkitSvgShadow = 416,
    CSSPropertyWebkitTextCombine = 417,
    CSSPropertyWebkitTextDecoration = 418,
    CSSPropertyWebkitTextDecorationColor = 419,
    CSSPropertyWebkitTextDecorationLine = 420,
    CSSPropertyWebkitTextDecorationSkip = 421,
    CSSPropertyWebkitTextDecorationStyle = 422,
    CSSPropertyWebkitTextDecorationsInEffect = 423,
    CSSPropertyWebkitTextEmphasis = 424,
    CSSPropertyWebkitTextEmphasisColor = 425,
    CSSPropertyWebkitTextEmphasisPosition = 426,
    CSSPropertyWebkitTextEmphasisStyle = 427,
    CSSPropertyWebkitTextFillColor = 428,
    CSSPropertyWebkitTextSecurity = 429,
    CSSPropertyWebkitTextStroke = 430,
    CSSPropertyWebkitTextStrokeColor = 431,
    CSSPropertyWebkitTextStrokeWidth = 432,
    CSSPropertyWebkitTextUnderlinePosition = 433,
    CSSPropertyWebkitTransformStyle = 434,
    CSSPropertyWebkitUserDrag = 435,
    CSSPropertyWebkitUserModify = 436,
    CSSPropertyWebkitUserSelect = 437,
};

const int firstCSSProperty = 2;
const int numCSSProperties = 436;
const int lastCSSProperty = 437;
const size_t maxCSSPropertyNameLength = 34;
const CSSPropertyID lastHighPriorityProperty = CSSPropertyWebkitTextZoom;

bool isInternalCSSProperty(const CSSPropertyID);
const char* getPropertyName(CSSPropertyID);
const WTF::AtomicString& getPropertyNameAtomicString(CSSPropertyID id);
WTF::String getPropertyNameString(CSSPropertyID id);
WTF::String getJSPropertyName(CSSPropertyID);

inline CSSPropertyID convertToCSSPropertyID(int value)
{
    ASSERT((value >= firstCSSProperty && value <= lastCSSProperty) || value == CSSPropertyInvalid || value == CSSPropertyCustom);
    return static_cast<CSSPropertyID>(value);
}

} // namespace WebCore

namespace WTF {
template<> struct DefaultHash<WebCore::CSSPropertyID> { typedef IntHash<unsigned> Hash; };
template<> struct HashTraits<WebCore::CSSPropertyID> : GenericHashTraits<WebCore::CSSPropertyID> {
    static const bool emptyValueIsZero = true;
    static const bool needsDestruction = false;
    static void constructDeletedValue(WebCore::CSSPropertyID& slot) { slot = static_cast<WebCore::CSSPropertyID>(WebCore::lastCSSProperty + 1); }
    static bool isDeletedValue(WebCore::CSSPropertyID value) { return value == (WebCore::lastCSSProperty + 1); }
};
} // namespace WTF

