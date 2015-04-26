// Vanilla Skia Image Backend
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VGraphics.h"
#include "VImage.h"

#include "Skia.h"

typedef struct VImage {
	SkBitmap Bitmap;
	VanillaInt Width;
	VanillaInt Height;
}*VanillaImage, _VImage;

VAPI(VanillaBool) VanillaSaveImageToFile(VanillaImage Image, VanillaText FileName, VanillaImageFormat ImageFormat) {
	SkImageEncoder::Type SkType;
	switch (ImageFormat) {
	case ImageFormatPNG:
		SkType = SkImageEncoder::Type::kPNG_Type;
		break;
	case ImageFormatJPEG:
		SkType = SkImageEncoder::Type::kJPEG_Type;
		break;
	default:
		SkType = (SkImageEncoder::Type)NULL;
		break;
	}
	if (SkType = (SkImageEncoder::Type)NULL) {
		return false;
	}

	return SkImageEncoder::EncodeFile(FileName, Image->Bitmap, SkType, 100);
}

VAPI(VanillaBinary) VanillaSaveImageToBinary(VanillaImage Image, VanillaImageFormat ImageFormat) {
	SkImageEncoder::Type SkType;
	switch (ImageFormat) {
	case ImageFormatPNG:
		SkType = SkImageEncoder::Type::kPNG_Type;
		break;
	case ImageFormatJPEG:
		SkType = SkImageEncoder::Type::kJPEG_Type;
		break;
	default:
		SkType = (SkImageEncoder::Type)NULL;
		break;
	}
	if (SkType = (SkImageEncoder::Type)NULL) {
		return false;
	}

	SkData* Data = SkImageEncoder::EncodeData(Image->Bitmap, SkType, 100);

	if (!Data) {
		return NULL;
	}

	VanillaInt Length = Data->size();
	if (Length <= 0) {
		Data->unref();
		return NULL;
	}

	VanillaBinary Binary = new VBinary;
	Binary->Length = Length;
	Binary->Address = (VanillaByte*)malloc(Length);
	memcpy(Binary->Address, Data->data(), Length);

	return Binary;
}

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

VAPI(VanillaImage) VanillaLoadImageFromBinary(VanillaBinary Binary) {
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
