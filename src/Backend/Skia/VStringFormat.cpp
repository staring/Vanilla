// Vanilla Skia StringFormat Backend
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VGraphics.h"
#include "VStringFormat.h"

#include "Skia.h"

typedef struct VStringFormat
{
	SkTypeface* Typeface;
	SkTextBox::SpacingAlign VAlign;
	SkPaint::Align Align;
	SkTextBox::Mode LineMode;
	VanillaColor Color;
	VanillaColor Shadow;
	VanillaInt Style;
	SkScalar Size;
	static SkMaskFilter* BlurMaskFilter;
}*VanillaStringFormat, _VStringFromat;

SkMaskFilter* VStringFormat::BlurMaskFilter = SkBlurMaskFilter::Create(kOuter_SkBlurStyle, 3, SkBlurMaskFilter::kNone_BlurFlag);

VAPI(VanillaStringFormat) VanillaCreateStringFormat(VanillaText FontName, VanillaReal FontSize, VanillaColor TextColor, VanillaColor ShadowColor, VanillaInt StringStyle) {
	VanillaStringFormat StringFormat = new VStringFormat;
	StringFormat->Color = TextColor;
	StringFormat->Style = StringStyle;
	StringFormat->Shadow = ShadowColor;
	if (StringStyle & VTS_VALIGN_MIDDLE) {
		StringFormat->VAlign = SkTextBox::kCenter_SpacingAlign;
	} else if (StringStyle & VTS_VALIGN_BOTTOM) {
		StringFormat->VAlign = SkTextBox::kEnd_SpacingAlign;
	} else {
		StringFormat->VAlign = SkTextBox::kStart_SpacingAlign;
	}

	if (StringStyle & VTS_ALIGN_CENTER) {
		StringFormat->Align = SkPaint::kCenter_Align;
	} else if (StringStyle & VTS_ALIGN_RIGHT) {
		StringFormat->Align = SkPaint::kRight_Align;
	} else {
		StringFormat->Align = SkPaint::kLeft_Align;
	}
	StringFormat->LineMode = SkTextBox::kLineBreak_Mode;
	StringFormat->Size = FontSize;
	//StringFormat->Typeface = VanillaPortCreateSkTypeface(FontName, (StringStyle & VTS_ITALIC) ? SkTypeface::kItalic : SkTypeface::kNormal + (StringStyle & VTS_BOLD) ? SkTypeface::kBold : SkTypeface::kNormal);
	StringFormat->Typeface = SkTypeface::CreateFromName(FontName, (StringStyle & VTS_ITALIC) ? SkTypeface::kItalic : SkTypeface::kNormal + (StringStyle & VTS_BOLD) ? SkTypeface::kBold : SkTypeface::kNormal);
	// fixme: FontName 在win32下必须转换为Ansi编码
	return StringFormat;
}

VAPI(VanillaVoid) VanillaDestroyStringFormat(VanillaStringFormat StringFormat) {
	delete StringFormat;
}
