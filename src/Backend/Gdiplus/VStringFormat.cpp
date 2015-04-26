// Vanilla Gdiplus StringFormat Backend
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VGlobalFunction.h"
#include "VGraphics.h"
#include "VStringFormat.h"

#undef RGB

#include "windows.h"
#include "gdiplus.h"

typedef struct VStringFormat
{
	Gdiplus::Font* Font;
	Gdiplus::Brush* BrushShadow;
	Gdiplus::Brush* Brush;
	Gdiplus::StringFormat* StringFormat;
	VanillaBool Shadow;
	VanillaInt StringStyle;
} *VanillaStringFormat, _VStringFromat;


VAPI(VanillaStringFormat) VanillaCreateStringFormat(VanillaText FontName, VanillaReal FontSize, VanillaColor TextColor, VanillaColor ShadowColor, VanillaInt StringStyle) {
	VanillaStringFormat StrFormat = new VStringFormat;
	StrFormat->StringFormat = new Gdiplus::StringFormat(Gdiplus::StringFormat::GenericTypographic());

	DWORD Flags = Gdiplus::StringFormatFlagsMeasureTrailingSpaces | Gdiplus::StringFormatFlagsNoFontFallback;

	StrFormat->StringFormat->SetFormatFlags(Flags);
	DWORD Align = 0;
	if (StringStyle & VTS_ALIGN_LEFT) {
		Align = 0;
	}
	else if (StringStyle & VTS_ALIGN_CENTER) {
		Align = 1;
	}
	else if (StringStyle & VTS_ALIGN_RIGHT) {
		Align = 2;
	}
	StrFormat->StringFormat->SetAlignment((Gdiplus::StringAlignment)Align);

	if (StringStyle & VTS_VALIGN_TOP) {
		Align = 0;
	}
	else if (StringStyle & VTS_VALIGN_MIDDLE) {
		Align = 1;
	}
	else if (StringStyle & VTS_VALIGN_BOTTOM) {
		Align = 2;
	}
	StrFormat->StringFormat->SetLineAlignment((Gdiplus::StringAlignment)Align);

	VANILLA_ICONV;

	Gdiplus::FontFamily* Family = new Gdiplus::FontFamily(VANILLA_U2W(FontName).c_str(), NULL);
	DWORD FontStyle = 0;
	if (StringStyle & VTS_BOLD) {
		FontStyle = FontStyle | 1;
	}
	if (StringStyle & VTS_ITALIC) {
		FontStyle = FontStyle | 2;
	}
	StrFormat->Font = new Gdiplus::Font(Family, (Gdiplus::REAL)FontSize, FontStyle, Gdiplus::UnitWorld);
	delete Family;

	if (StringStyle & VTS_SHADOW) {
		StrFormat->Shadow = true;
		StrFormat->BrushShadow = new Gdiplus::SolidBrush(ShadowColor);
	}
	else {
		StrFormat->Shadow = false;
		StrFormat->BrushShadow = NULL;
	}
	StrFormat->Brush = new Gdiplus::SolidBrush(TextColor);
	return StrFormat;
}

VAPI(VanillaVoid) VanillaDestroyStringFormat(VanillaStringFormat StringFormat) {
	if (StringFormat) {
		delete StringFormat->Brush;
		if (StringFormat->BrushShadow) {
			delete StringFormat->BrushShadow;
		}
		delete StringFormat->Font;
		delete StringFormat->StringFormat;
		delete StringFormat;
	}
}
