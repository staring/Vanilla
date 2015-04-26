// Vanilla Gdiplus Graphics Backend
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VGlobalFunction.h"
#include "VStringFormat.h"
#include "VImage.h"
#include "VGraphics.h"

#undef RGB

#include "windows.h"
#include "gdiplus.h"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "msimg32.lib")

typedef struct VStringFormat
{
	Gdiplus::Font* Font;
	Gdiplus::Brush* BrushShadow;
	Gdiplus::Brush* Brush;
	Gdiplus::StringFormat* StringFormat;
	VanillaBool Shadow;
	VanillaInt StringStyle;
}*VanillaStringFormat, _VStringFromat;

typedef struct VGraphics {
	VanillaPWGraphics PortGraphics;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	HDC hDC;
	LPVOID Pixels;
	Gdiplus::Bitmap* Bitmap;
	Gdiplus::Graphics* Graphics;
	VanillaInt Width;
	VanillaInt Height;
}*VanillaGraphics, _VGraphics;

typedef struct VImage {
	Gdiplus::Bitmap* Image;
	VanillaInt Width;
	VanillaInt Height;

	HGLOBAL GMem;
	IStream* Stream;
}*VanillaImage, _VImage;

ULONG_PTR GdiplusOpenToken;

VAPI(VanillaBool) VanillaInitializeBackend() {
	Gdiplus::GdiplusStartupInput GdiplusStartupInput;
	GdiplusStartupInput.DebugEventCallback = NULL;
	GdiplusStartupInput.SuppressBackgroundThread = NULL;
	GdiplusStartupInput.SuppressExternalCodecs = NULL;
	GdiplusStartupInput.GdiplusVersion = 1;
	Gdiplus::GdiplusStartup(&GdiplusOpenToken, &GdiplusStartupInput, NULL);
	return GdiplusOpenToken != 0;
}

VAPI(VanillaAny) VanillaGraphicsGetPixels(VanillaGraphics Graphics) {
	return Graphics->Pixels;
}

VAPI(VanillaVoid) VanillaFillRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height) {
	Gdiplus::SolidBrush Brush(Color);
	Graphics->Graphics->FillRectangle((Gdiplus::Brush*)&Brush, Left, Top, Width, Height);
}

VAPI(VanillaVoid) VanillaDrawFilledRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth) {
	Gdiplus::SolidBrush Brush(Color);
	Graphics->Graphics->FillRectangle((Gdiplus::Brush*)&Brush, Left, Top, Width, Height);

	Gdiplus::Pen Pen(BorderColor, (Gdiplus::REAL)LineWidth);
	Graphics->Graphics->DrawRectangle(&Pen, Left, Top, Width, Height);
}

VAPI(VanillaVoid) VanillaDrawRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth) {
	Gdiplus::Pen Pen(Color, (Gdiplus::REAL)LineWidth);
	Graphics->Graphics->DrawRectangle(&Pen, Left, Top, Width, Height);
}

VAPI(VanillaVoid) VanillaDrawString(VanillaGraphics Graphics, VanillaStringFormat StringFormat, VanillaText String, VanillaRect Rect) {
	VANILLA_ICONV;
	std::wstring WString = VANILLA_U2W(String);
	Gdiplus::RectF StrRect;
	if (NULL == Rect) {
		Graphics->Graphics->MeasureString(WString.c_str(), WString.length(), StringFormat->Font, Gdiplus::RectF(), StringFormat->StringFormat, &StrRect);
	}
	else {
		StrRect.X = (Gdiplus::REAL)Rect->Left;
		StrRect.Y = (Gdiplus::REAL)Rect->Top;
		StrRect.Width = (Gdiplus::REAL)Rect->Width;
		StrRect.Height = (Gdiplus::REAL)Rect->Height;
	}

	if (StringFormat->Shadow) {
		Gdiplus::RectF Rect2;
		Rect2.Width = StrRect.Width;
		Rect2.Height = StrRect.Height;
		Gdiplus::Bitmap Bitmap1((INT)Rect2.Width, (INT)Rect2.Height, PixelFormat32bppARGB);
		Gdiplus::Graphics Graphics1(&Bitmap1);
		Graphics1.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
		Graphics1.DrawString(WString.c_str(), WString.length(), StringFormat->Font, Rect2, StringFormat->StringFormat, StringFormat->BrushShadow);

		Gdiplus::Bitmap Bitmap2((int)(Rect2.Width / 3), (int)(Rect2.Height / 3), PixelFormat32bppARGB);
		Gdiplus::Graphics Graphics2(&Bitmap2);
		Graphics2.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		Graphics2.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
		Graphics2.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
		Graphics2.DrawImage(&Bitmap1, 0.0, 0.0, Rect2.Width / 3, Rect2.Height / 3);
		Graphics->Graphics->DrawImage(&Bitmap2, StrRect.X, StrRect.Y, StrRect.Width, StrRect.Height);
	}
	Graphics->Graphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
	Graphics->Graphics->DrawString(WString.c_str(), WString.length(), StringFormat->Font, StrRect, StringFormat->StringFormat, StringFormat->Brush);
}

VanillaVoid VanillaCreateRoundRectPath(VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round, Gdiplus::GraphicsPath* Path) {
	int a = x + Width - (VanillaInt)Round - 1;
	int b = y + Height - (VanillaInt)Round - 1;
	Path->AddArc((INT)x, (INT)y, (INT)Round, (INT)Round, 180, 90);
	Path->AddArc((INT)a, (INT)y, (INT)Round, (INT)Round, 270, 90);
	Path->AddArc((INT)a, (INT)b, (INT)Round, (INT)Round, 0, 90);
	Path->AddArc((INT)x, (INT)b, (INT)Round, (INT)Round, 90, 90);
	Path->CloseFigure();
}

VAPI(VanillaVoid) VanillaDrawFilledRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round) {
	if (Round <= 0){
		VanillaDrawFilledRect(Graphics, Color, BorderColor, x, y, Width, Height, LineWidth);
		return;
	}
	Gdiplus::Pen Pen(Color, (Gdiplus::REAL)LineWidth);
	Gdiplus::SolidBrush Brush(Color);
	Gdiplus::GraphicsPath Path(Gdiplus::FillModeAlternate);
	VanillaCreateRoundRectPath(x, y, Width, Height, Round, &Path);
	Graphics->Graphics->FillPath(&Brush, &Path);
	Graphics->Graphics->DrawPath(&Pen, &Path);
}

VAPI(VanillaVoid) VanillaDrawRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round) {
	if (Round <= 0){
		VanillaDrawRect(Graphics, Color, x, y, Width, Height, LineWidth);
		return;
	}
	Gdiplus::Pen Pen(Color, (Gdiplus::REAL)LineWidth);
	Gdiplus::GraphicsPath Path(Gdiplus::FillModeAlternate);
	VanillaCreateRoundRectPath(x, y, Width, Height, Round, &Path);
	Graphics->Graphics->DrawPath(&Pen, &Path);
}

VAPI(VanillaVoid) VanillaFillRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round) {
	if (Round <= 0){
		VanillaFillRect(Graphics, Color, x, y, Width, Height);
		return;
	}
	Gdiplus::SolidBrush Brush(Color);
	Gdiplus::GraphicsPath Path(Gdiplus::FillModeAlternate);
	VanillaCreateRoundRectPath(x, y, Width, Height, Round, &Path);
	Graphics->Graphics->FillPath(&Brush, &Path);
}

VAPI(VanillaVoid) VanillaDrawImage(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y) {
	if (!(Graphics && Image)) {
		return;
	}
	Graphics->Graphics->DrawImage(Image->Image, x, y);
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
	Gdiplus::ImageAttributes IA;
	Gdiplus::ColorMatrix CM = Gdiplus::ColorMatrix();
	CM.m[3][3] = Alpha / 255.0f;
	IA.SetColorMatrix(&CM);
	Gdiplus::Rect TempRect(x, y, Width-10, Height-10);
	Graphics->Graphics->DrawImage(Image->Image, TempRect, xSrc, ySrc, WidthSrc-10, HeightSrc-10, Gdiplus::UnitPixel, &IA, NULL, NULL);
}

VAPI(VanillaVoid) VanillaAlphaBlendEx(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha) {
	BLENDFUNCTION BlendFunc;
	BlendFunc.SourceConstantAlpha = Alpha;
	BlendFunc.BlendOp = AC_SRC_OVER;
	BlendFunc.BlendFlags = NULL;
	BlendFunc.AlphaFormat = AC_SRC_ALPHA;
	HDC hDCDest = (Dest->hDC == 0) ? (Dest->Graphics->GetHDC()) : (Dest->hDC);
	HDC hDCSrc = (Src->hDC == 0) ? (Src->Graphics->GetHDC()) : (Src->hDC);
	AlphaBlend(hDCDest, x, y, Width, Height, hDCSrc, xSrc, ySrc, WidthSrc, HeightSrc, BlendFunc);
	if (Dest->hDC == 0) {
		Dest->Graphics->ReleaseHDC(hDCDest);
	}
	if (Src->hDC == 0) {
		Src->Graphics->ReleaseHDC(hDCSrc);
	}
}

VAPI(VanillaVoid) VanillaAlphaBlend(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaByte Alpha) {
	BLENDFUNCTION BlendFunc;
	BlendFunc.SourceConstantAlpha = Alpha;
	BlendFunc.BlendOp = AC_SRC_OVER;
	BlendFunc.BlendFlags = NULL;
	BlendFunc.AlphaFormat = AC_SRC_ALPHA;
	HDC hDCDest = (Dest->hDC == 0) ? (Dest->Graphics->GetHDC()) : (Dest->hDC);
	HDC hDCSrc = (Src->hDC == 0) ? (Src->Graphics->GetHDC()) : (Src->hDC);
	AlphaBlend(hDCDest, x, y, Width, Height, hDCSrc, xSrc, ySrc, Width, Height, BlendFunc);
	if (Dest->hDC == 0) {
		Dest->Graphics->ReleaseHDC(hDCDest);
	}
	if (Src->hDC == 0) {
		Src->Graphics->ReleaseHDC(hDCSrc);
	}
}

VAPI(VanillaVoid) VanillaGraphicsClear(VanillaGraphics Graphics, VanillaColor Color) {
	Graphics->Graphics->Clear(Color);
}

VAPI(VanillaGraphics) VanillaCreateGraphicsInMemory(VanillaInt Width, VanillaInt Height) {
	BITMAPINFO BitmapInfo;
	memset(&BitmapInfo, 0, sizeof(BitmapInfo));
	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biWidth = Width;
	BitmapInfo.bmiHeader.biHeight = Height;
	BitmapInfo.bmiHeader.biPlanes = 1;
	VanillaAny Pixels;
	HBITMAP hBitmap = CreateDIBSection(0, &BitmapInfo, 0, &Pixels, 0, 0);
	if (!hBitmap) {
		return NULL;
	}

	VanillaGraphics Graphics = new VGraphics;
	Graphics->hDC = CreateCompatibleDC(0);
	if (!Graphics->hDC) {
		delete Graphics;
		return NULL;
	}
	Graphics->hBitmap = hBitmap;
	Graphics->hOldBitmap = (HBITMAP)SelectObject(Graphics->hDC, (HGDIOBJ)Graphics->hBitmap);
	Graphics->Width = Width;
	Graphics->Height = Height;
	Graphics->Graphics = new Gdiplus::Graphics(Graphics->hDC);
	Graphics->Pixels = Pixels;
	return Graphics;
}

VAPI(VanillaGraphics) VanillaCreateGraphicsFromPixelAddress(VanillaAny Pixels, VanillaInt Width, VanillaInt Height) {
	VanillaGraphics Graphics = new VGraphics;
	Graphics->Bitmap = new Gdiplus::Bitmap(Width, Height, ((Width * 32 + 15) / 16) * 2, PixelFormat32bppARGB, (BYTE*)Pixels);
	Graphics->Graphics = new Gdiplus::Graphics(Graphics->Bitmap);
	Graphics->Pixels = Pixels;
	Graphics->Width = Width;
	Graphics->Height = Height;
	Graphics->hDC = NULL;
	return Graphics;
}

VAPI(VanillaVoid) VanillaDestroyGraphics(VanillaGraphics Graphics) {
	if (Graphics) {
		delete Graphics->Graphics;
		if (Graphics->Bitmap) {
			delete Graphics->Bitmap;
		} else if (Graphics->hDC) {
			SelectObject(Graphics->hDC, (HGDIOBJ)Graphics->hBitmap);
			DeleteObject(Graphics->hBitmap);
			DeleteDC(Graphics->hDC);
		}
		delete Graphics;
	}
}
