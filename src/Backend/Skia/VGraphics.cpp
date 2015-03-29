// Vanilla Skia Graphics Backend
#include "stdafx.h"
#include "Skia.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VStringFormat.h"
#include "VImage.h"
#include "VGraphics.h"

__SK_FORCE_IMAGE_DECODER_LINKING;

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

typedef struct VGraphics {
	SkCanvas Canvas;
	char Buffers[8]; // fixme : Skia's memory-overflow bug.
	SkBitmap Bitmap;
	SkPaint Paint;
	VanillaInt Width;
	VanillaInt Height;
	VanillaPWGraphics PortGraphics;
	~VGraphics() {
        //delete this->Canvas;
        //delete this->Paint;
        //delete this->Bitmap;
	}
	VGraphics(){
        //memset(this->Buffers, 0, sizeof(this->Buffers));
	}
}*VanillaGraphics, _VGraphics;

typedef struct VImage {
	SkBitmap Bitmap;
	VanillaInt Width;
	VanillaInt Height;
}*VanillaImage, _VImage;

VAPI(VanillaVoid) VanillaGraphicsSetPWGraphics(VanillaGraphics Graphics, VanillaPWGraphics PWGraphics) {
    Graphics->PortGraphics = PWGraphics;
}

VAPI(VanillaPWGraphics) VanillaGraphicsGetPWGraphics(VanillaGraphics Graphics) {
    return Graphics->PortGraphics;
}

VAPI(VanillaAny) VanillaGraphicsGetPixels(VanillaGraphics Graphics) {
    return Graphics->Bitmap.getPixels();
}

VAPI(VanillaVoid) VanillaFillRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height) {
	SkIRect Rect = SkIRect::MakeXYWH(Top, Left, Width, Height);
	Graphics->Paint.reset();
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kFill_Style);
	Graphics->Canvas.drawIRect(Rect, Graphics->Paint);
}

VAPI(VanillaVoid) VanillaDrawFilledRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth) {
	SkIRect Rect = SkIRect::MakeXYWH(Top, Left, Width, Height);
	Graphics->Paint.reset();
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kFill_Style);
	Graphics->Canvas.drawIRect(Rect, Graphics->Paint);

	Rect = SkIRect::MakeXYWH(Top, Left, Width, Height);
	Graphics->Paint.setStrokeWidth((VanillaReal)LineWidth);
	Graphics->Paint.setColor(BorderColor);
	Graphics->Paint.setStyle(SkPaint::kStroke_Style);
	Graphics->Canvas.drawIRect(Rect, Graphics->Paint);
}

VAPI(VanillaVoid) VanillaDrawRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth) {
	SkIRect Rect = SkIRect::MakeXYWH(Top, Left, Width, Height);
	Graphics->Paint.reset();
	Graphics->Paint.setStrokeWidth((VanillaReal)LineWidth);
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kStroke_Style);
	Graphics->Canvas.drawIRect(Rect, Graphics->Paint);
}

VAPI(SkTextBox*) VanillaCreateTextBox(VanillaGraphics Graphics, VanillaStringFormat StringFormat, VanillaText String, VanillaRectR Rect) {
	SkTextBox* TextBox = new SkTextBox;
	TextBox->setText(String, strlen(String), Graphics->Paint);
	TextBox->setSpacingAlign(StringFormat->VAlign);
	TextBox->setBox(Rect->Left, Rect->Top, Rect->Width, Rect->Height);
	TextBox->setMode(StringFormat->LineMode);
	return TextBox;
}

VAPI(VanillaVoid) VanillaDrawString(VanillaGraphics Graphics, VanillaStringFormat StringFormat, VanillaText String, VanillaRect Rect) {
	VRectR RectD;
	if (Rect == NULL) {
		RectD = VRectR(0, 0, (VanillaReal)Graphics->Width, (VanillaReal)Graphics->Height);
	} else {
		RectD = VRectR((VanillaReal)Rect->Left, (VanillaReal)Rect->Top, (VanillaReal)Rect->Width - 1, (VanillaReal)Rect->Height);
	}
	Graphics->Paint.reset();
	Graphics->Paint.setTypeface(StringFormat->Typeface);
	Graphics->Paint.setTextSize(StringFormat->Size);
	Graphics->Paint.setAntiAlias(true);
	Graphics->Paint.setLCDRenderText(false);
	Graphics->Paint.setColor(StringFormat->Color);
	Graphics->Paint.setTextAlign(StringFormat->Align);
	SkTextBox* TextBox = VanillaCreateTextBox(Graphics, StringFormat, String, &RectD);
	TextBox->draw(&Graphics->Canvas);
	if (StringFormat->Style & VTS_SHADOW) {
        Graphics->Paint.setMaskFilter(StringFormat->BlurMaskFilter);
        Graphics->Paint.setColor(StringFormat->Shadow);
        TextBox->draw(&Graphics->Canvas);
	}
	delete TextBox;
}

VanillaVoid VanillaCreateRoundRectPath(VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round, SkPath* Path) {
	Path->addArc(SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)y, Round, Round), 180, 90);
	Path->addArc(SkRect::MakeXYWH((VanillaReal)(x + Width - Round), (VanillaReal)y, Round, Round), 270, 90);
	Path->addArc(SkRect::MakeXYWH((VanillaReal)(x + Width - Round), (VanillaReal)(y + Height - Round), Round, Round), 0, 90);
	Path->addArc(SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)(y + Height - Round), Round, Round), 90, 90);
	Path->close();
}

VAPI(VanillaVoid) VanillaDrawFilledRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round) {
	if (Round <= 0){
		VanillaDrawFilledRect(Graphics, Color, BorderColor, x, y, Width, Height, LineWidth);
		return;
	}

	SkRect Rect = SkRect::MakeXYWH((VanillaReal)(x), (VanillaReal)(y), (VanillaReal)(Width), (VanillaReal)(Height));
	Graphics->Paint.reset();
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kFill_Style);
	Graphics->Canvas.drawRoundRect(Rect, Round, Round, Graphics->Paint);

	Graphics->Paint.setStrokeWidth((VanillaReal)LineWidth);
	Graphics->Paint.setColor(BorderColor);
	Graphics->Paint.setStyle(SkPaint::kStroke_Style);
	Graphics->Canvas.drawRoundRect(Rect, Round, Round, Graphics->Paint);
}

VAPI(VanillaVoid) VanillaDrawRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round) {
	if (Round <= 0){
		VanillaDrawRect(Graphics, Color, x, y, Width, Height, LineWidth);
		return;
	}
	SkRect Rect = SkRect::MakeXYWH((VanillaReal)(x), (VanillaReal)(y), (VanillaReal)(Width), (VanillaReal)(Height));
	Graphics->Paint.reset();
	Graphics->Paint.setStrokeWidth((VanillaReal)LineWidth);
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kStroke_Style);
	Graphics->Canvas.drawRoundRect(Rect, Round, Round, Graphics->Paint);
}

VAPI(VanillaVoid) VanillaFillRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round) {
	if (Round <= 0){
		VanillaFillRect(Graphics, Color, x, y, Width, Height);
		return;
	}
	SkRect Rect = SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)y, (VanillaReal)Width, (VanillaReal)Height);
	Graphics->Paint.reset();
	Graphics->Paint.setColor(Color);
	Graphics->Paint.setStyle(SkPaint::kFill_Style);
	Graphics->Canvas.drawRoundRect(Rect, Round, Round, Graphics->Paint);
}

VAPI(VanillaVoid) VanillaDrawImage(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y) {
	if (!(Graphics && Image)) {
		return;
	}
	Graphics->Canvas.drawBitmap(Image->Bitmap, (VanillaReal)(x), (VanillaReal)(y), NULL);
}

VAPI(VanillaVoid) VanillaDrawImageEx(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha) {
	if (!(Graphics && Image)) {
		return;
	}

	if (Width <= 0) {
		Width = (VanillaInt)(Image->Width - xSrc);
	}
	if (Height <= 0) {
		Height = (VanillaInt)(Image->Height - ySrc);
	}


	if (WidthSrc <= 0 || WidthSrc + xSrc > Image->Width) {
		WidthSrc = (VanillaInt)(Image->Width - xSrc);
	}
	if (HeightSrc <= 0 || HeightSrc + ySrc > Image->Height) {
		HeightSrc = (VanillaInt)(Image->Height - ySrc);
	}
	Graphics->Paint.reset();
	Graphics->Paint.setAlpha(Alpha);
	SkRect Src = SkRect::MakeXYWH((VanillaReal)xSrc, (VanillaReal)ySrc, (VanillaReal)WidthSrc, (VanillaReal)HeightSrc);
	SkRect Dest = SkRect::MakeXYWH((VanillaReal)(x), (VanillaReal)(y), (VanillaReal)(Width), (VanillaReal)(Height));
	Graphics->Canvas.drawBitmapRectToRect(Image->Bitmap, &Src, Dest, &Graphics->Paint);
}

VAPI(VanillaVoid) VanillaAlphaBlendEx(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha) {
	Dest->Paint.reset();
	Dest->Paint.setAlpha(Alpha);
	SkIRect Rect = SkIRect::MakeXYWH(xSrc, ySrc, WidthSrc, HeightSrc);
	Dest->Canvas.drawBitmapRect(Src->Bitmap, &Rect, SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)y, (VanillaReal)Width, (VanillaReal)Height), &Dest->Paint);
}

VAPI(VanillaVoid) VanillaAlphaBlend(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaByte Alpha) {
	Dest->Paint.reset();
	Dest->Paint.setAlpha(Alpha);
	SkIRect Rect = SkIRect::MakeXYWH(xSrc, ySrc, Width, Height);
	Dest->Canvas.drawBitmapRect(Src->Bitmap, &Rect, SkRect::MakeXYWH((VanillaReal)x, (VanillaReal)y, (VanillaReal)Width, (VanillaReal)Height), NULL);
}

VAPI(VanillaVoid) VanillaGraphicsClear(VanillaGraphics Graphics, VanillaColor Color) {
	Graphics->Bitmap.eraseColor(Color);
}

VAPI(VanillaGraphics) VanillaCreateGraphicsInMemory(VanillaInt Width, VanillaInt Height) {
	VanillaGraphics Graphics = new VGraphics;
    Graphics->Bitmap.allocPixels(SkImageInfo::Make(Width, Height, kBGRA_8888_SkColorType, kPremul_SkAlphaType));
    new (&Graphics->Canvas) SkCanvas(Graphics->Bitmap);
	Graphics->Width = Width;
	Graphics->Height = Height;
	return Graphics;
}

VAPI(VanillaVoid) VanillaDestroyGraphics(VanillaGraphics Graphics) {
	if (Graphics) {
		delete Graphics;
	}
}
