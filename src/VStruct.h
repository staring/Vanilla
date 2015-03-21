#ifndef __VANILLA_CORE_STRUCT_H__
#define __VANILLA_CORE_STRUCT_H__
// Vanilla Structs' Define

typedef struct VRect
{
	VRect(VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height) {
		this->Left = Left;
		this->Top = Top;
		this->Width = Width;
		this->Height = Height;
	}
	VRect(){}
	VanillaInt Left;
	VanillaInt Top;
	VanillaInt Width;
	VanillaInt Height;
} *VanillaRect, _VRect;

typedef struct VPoint
{
	VPoint(VanillaInt x, VanillaInt y) {
		this->x = x;
		this->y = y;
	}
	VPoint(){}
	VanillaInt x;
	VanillaInt y;
} *VanillaPoint, _VPoint;

typedef struct VSize
{
	VSize(VanillaInt Width, VanillaInt Height) {
		this->Width = Width;
		this->Height = Height;
	}
	VSize(){}
	VanillaInt Width;
	VanillaInt Height;
} *VanillaSize, _VSize;

typedef struct VRectR
{
	VRectR(VanillaReal Left, VanillaReal Top, VanillaReal Width, VanillaReal Height) {
		this->Left = Left;
		this->Top = Top;
		this->Width = Width;
		this->Height = Height;
	}
	VRectR(){}
	VanillaReal Left;
	VanillaReal Top;
	VanillaReal Width;
	VanillaReal Height;
} *VanillaRectR, _VRectR;

typedef struct VPointR
{
	VPointR(VanillaReal x, VanillaReal y) {
		this->x = x;
		this->y = y;
	}
	VPointR(){}
	VanillaReal x;
	VanillaReal y;
} *VanillaPointR, _VPointR;

typedef struct VSizeR
{
	VSizeR(VanillaReal Width, VanillaReal Height) {
		this->Width = Width;
		this->Height = Height;
	}
	VSizeR(){}
	VanillaReal Width;
	VanillaReal Height;
} *VanillaSizeR, _VSizeR;

typedef struct VImage {
	SkBitmap Bitmap;
	VanillaInt Width;
	VanillaInt Height;
}*VanillaImage, _VImage;

/*
typedef struct VColor
{
	VColor(){}
	VColor(VanillaByte Alpha, VanillaByte Red, VanillaByte Green, VanillaByte Blue) {
		this->Color = SkColorSetARGB(Alpha, Red, Green, Blue);
	}
	SkColor Color;
}*VanillaColor, _VColor;
*/

typedef struct VGraphics {
	SkCanvas Canvas;
	char Buffers[8]; // fixme : Skia's memory-overflow bug.
	SkBitmap Bitmap;
	SkPaint Paint;
	VanillaInt Width;
	VanillaInt Height;
	VanillaPortGraphics PortGraphics;
	~VGraphics() {
        //delete this->Canvas;
        //delete this->Paint;
        //delete this->Bitmap;
	}
	VGraphics(){
        //memset(this->Buffers, 0, sizeof(this->Buffers));
	}
}*VanillaGraphics, _VGraphics;

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

typedef struct VBin
{
	VBin() {
		this->Address = NULL;
		this->Length = 0;
	}

	VBin(VanillaByte* Address, VanillaInt Length) {
		this->Address = Address;
		this->Length = Length;
	}
	~VBin() {
		if (this->Length != 0) {
			delete [] this->Address;
		}
	}
	VanillaByte* Address;
	VanillaInt Length;
} *VanillaBin, _VBin;
#endif	//__VANILLA_CORE_STRUCT_H__
