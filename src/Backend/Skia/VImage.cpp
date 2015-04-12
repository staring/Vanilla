// Vanilla Skia Image Backend
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VGraphics.h"

#include "Skia.h"

typedef struct VImage {
	SkBitmap Bitmap;
	VanillaInt Width;
	VanillaInt Height;
}*VanillaImage, _VImage;

VAPI(VanillaImage) VanillaLoadImageFromFile(VanillaText FileName) {
	VanillaImage Image = (VanillaImage)malloc(sizeof(VImage));
	memset(Image, 0, sizeof(VImage));
	if (!SkImageDecoder::DecodeFile(FileName, &Image->Bitmap)) {
		free(Image);
		return NULL;
	}
	Image->Width = Image->Bitmap.width();
	Image->Height = Image->Bitmap.height();
	return Image;
}

VAPI(VanillaImage) VanillaLoadImageFromBinary(VanillaBin Binary) {
	if (!Binary) {
		return NULL;
	}
	VanillaImage Image = (VanillaImage)malloc(sizeof(VImage));
	memset(Image, 0, sizeof(VImage));
	if (!SkImageDecoder::DecodeMemory(Binary->Address, Binary->Length, &Image->Bitmap)) {
		free(Image);
		return NULL;
	}
	Image->Width = Image->Bitmap.width();
	Image->Height = Image->Bitmap.height();
	return Image;
}

VAPI(VanillaVoid) VanillaDestroyImage(VanillaImage Image) {
	if (Image) {
		free(Image);
	}
}
