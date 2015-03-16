#ifndef __VDRAWING_H__
#define __VDRAWING_H__
// Vanilla Drawing Functions

#define VTS_BOLD 1
#define VTS_ITALIC 2
#define VTS_ALIGN_LEFT 0
#define VTS_ALIGN_CENTER 4
#define VTS_ALIGN_RIGHT 8
#define VTS_VALIGN_TOP 0
#define VTS_VALIGN_MIDDLE 16
#define VTS_VALIGN_BOTTOM 32
#define VTS_SHADOW 64

VAPI(VanillaVoid) VanillaFillRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height);
VAPI(VanillaVoid) VanillaDrawFilledRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth);
VAPI(VanillaVoid) VanillaDrawRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth);
VAPI(VanillaVoid) VanillaDrawString(VanillaGraphics Graphics, VanillaStringFormat StringFormat, VanillaText String, VanillaRect Rect);
VAPI(VanillaVoid) VanillaCreateRoundRectPath(VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round, SkPath* Path);
VAPI(VanillaVoid) VanillaDrawFilledRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round);
VAPI(VanillaVoid) VanillaDrawRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round);
VAPI(VanillaVoid) VanillaFillRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round);
VAPI(VanillaVoid) VanillaDrawImage(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y);
VAPI(VanillaVoid) VanillaDrawImageEx(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha);
VAPI(VanillaVoid) VanillaAlphaBlendEx(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha);
VAPI(VanillaVoid) VanillaAlphaBlend(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaByte Alpha);
VAPI(VanillaVoid) VanillaGraphicsClear(VanillaGraphics Graphics, VanillaColor Color);
VAPI(VanillaStringFormat) VanillaCreateStringFormat(VanillaText FontName, VanillaReal FontSize, VanillaColor TextColor, VanillaColor ShadowColor, VanillaInt StringStyle);
VAPI(VanillaVoid) VanillaDestroyStringFormat(VanillaStringFormat StringFormat);
VAPI(VanillaGraphics) VanillaCreateGraphicsOfWindow(VanillaWindow Window);
VAPI(VanillaVoid) VanillaDestroyGraphicsOfWindow(VanillaGraphics Graphcis);
VAPI(VanillaGraphics) VanillaCreateGraphicsInMemory(VanillaInt Width, VanillaInt Height);
VAPI(VanillaVoid) VanillaDestroyGraphics(VanillaGraphics Graphics);
VAPI(VanillaImage) VanillaLoadImageFromFile(VanillaText FileName);
VAPI(VanillaImage) VanillaLoadImageFromBinary(VanillaBin Binary);
VAPI(VanillaVoid) VanillaDestroyImage(VanillaImage Image);
#endif	//__VDRAWING_H__