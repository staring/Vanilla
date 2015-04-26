// Vanilla Gdiplus Image Backend
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VGlobalFunction.h"
#include "VGraphics.h"
#include "VImage.h"

#undef RGB

#include "windows.h"
#include "gdiplus.h"

typedef struct VImage {
	Gdiplus::Bitmap* Image;
	VanillaInt Width;
	VanillaInt Height;

	HGLOBAL GMem;
	IStream* Stream;
} *VanillaImage, _VImage;


// ** These code from http://www.cnblogs.com/LinuxHunter/archive/2012/07/15/2592015.html
int GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
{
	UINT num = 0;
	UINT size = 0;

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);

	if (size == 0)
		return FALSE; 

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return FALSE; 

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT i = 0; i < num; ++i)
	{
		if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{
			*pCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return TRUE;
		}
	}

	free(pImageCodecInfo);
	return FALSE;
}
// **

VAPI(VanillaBool) VanillaSaveImageToFile(VanillaImage Image, VanillaText FileName, VanillaImageFormat ImageFormat) {
	wchar_t* MineType;
	switch (ImageFormat) {
	case ImageFormatPNG:
		MineType = L"image/png";
		break;
	case ImageFormatJPEG:
		MineType = L"image/jpeg";
		break;
	default:
		MineType = NULL;
		break;
	}
	if (!MineType) {
		return false;
	}
	CLSID Clsid;
	GetImageCLSID(MineType, &Clsid);
	VANILLA_ICONV;
	return Image->Image->Save(VANILLA_U2W(FileName).c_str(), &Clsid) == Gdiplus::Status::Ok;
}

VAPI(VanillaBinary) VanillaSaveImageToBinary(VanillaImage Image, VanillaImageFormat ImageFormat) {
	wchar_t* MineType;
	switch (ImageFormat) {
	case ImageFormatPNG:
		MineType = L"image/png";
		break;
	case ImageFormatJPEG:
		MineType = L"image/jpeg";
		break;
	default:
		MineType = NULL;
		break;
	}
	if (!MineType) {
		return false;
	}
	CLSID Clsid;
	GetImageCLSID(MineType, &Clsid);
	VANILLA_ICONV;
	IStream* Stream;

	CreateStreamOnHGlobal(NULL, true, &Stream);

	if (Image->Image->Save(Stream, &Clsid) != Gdiplus::Status::Ok) {
		return NULL;
	}

	HGLOBAL HGlobal;
	if (GetHGlobalFromStream(Stream, &HGlobal) != S_OK) {
		Stream->Release();
		return NULL;
	}

	VanillaInt Length = GlobalSize(HGlobal);
	if (Length <= 0) {
		Stream->Release();
		return NULL;
	}

	VanillaBinary Binary = new VBinary;
	Binary->Length = Length;
	Binary->Address = (VanillaByte*)malloc(Length);

	VanillaAny Address = GlobalLock(HGlobal);
	memcpy(Binary->Address, Address, Length);

	GlobalUnlock(HGlobal);
	Stream->Release();
	return Binary;
}

VAPI(VanillaImage) VanillaLoadImageFromFile(VanillaText FileName) {
	VanillaImage Image = new VImage;
	VANILLA_ICONV;
	Image->Image = new Gdiplus::Bitmap(VANILLA_U2W(FileName).c_str());
	Image->Stream = NULL;
	Image->Width = Image->Image->GetWidth();
	Image->Height = Image->Image->GetHeight();
	return Image;
}

VAPI(VanillaImage) VanillaLoadImageFromBinary(VanillaBinary Binary) {
	if (!Binary || Binary->Length <= 0) {
		return NULL;
	}
	CoInitialize(NULL);

	VanillaImage Image = new VImage;

	Image->GMem = GlobalAlloc(GMEM_FIXED, Binary->Length);
	LPVOID Mem = GlobalLock(Image->GMem);
	RtlMoveMemory(Mem, Binary->Address, Binary->Length);
	GlobalUnlock(Image->GMem);
	CreateStreamOnHGlobal(Image->GMem, false, &Image->Stream);	
	Image->Image = new Gdiplus::Bitmap(Image->Stream);
	Image->Width = Image->Image->GetWidth();
	Image->Height = Image->Image->GetHeight();

	CoUninitialize();
	return Image;
}

VAPI(VanillaVoid) VanillaDestroyImage(VanillaImage Image) {
	if (Image) {
		if (Image->Stream) {
			CoInitialize(NULL);
			Image->Stream->Release();
			GlobalFree(Image->GMem);
			CoUninitialize();
		}
		delete Image;
	}
}
