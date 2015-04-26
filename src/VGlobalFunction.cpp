// Vanilla Normal Global Functions
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VPWindow.h"
#include "VControl.h"
#include "VGlobalFunction.h"
#include "VGraphics.h"
#include "VControlRegister.h"

VanillaBool VInit() {
	if (!VInit_ControlClass()) {
		return false;
	}
	if (!VanillaPWInitialize()) {
		return false;
	}
	if (!VanillaInitializeBackend()) {
		return false;
	}
	return true;
}

VanillaBool VInit_ControlClass()
{
    VanillaRegisterControls();
    VanillaRegisterControlClass("VanillaUI.WindowRootControl", &VanillaWindowDefaultControlsProc, false, true);
	return true;
}

VAPI(VanillaInt) VanillaMsgLoop() {
	return VanillaPWMessageLoop();
}

VAPI(VanillaBool) VanillaInit() {
	return VInit();
}

VAPI(VanillaBinary) VanillaReadFileToBin(VanillaText FileName) {
	FILE* File = fopen(FileName, "rb");
	fseek(File, 0, SEEK_END);
	int Size = ftell(File);
	fseek(File, 0, SEEK_SET);
	VanillaByte* Buffer = new VanillaByte[Size];
	fread(Buffer, 1, Size, File);
	fclose(File);
	return new VBinary(Buffer, Size);
}

VAPI(VanillaBool) VanillaIsPointInRect(VanillaInt x, VanillaInt y, VanillaRect Rect) {
	if (x <= Rect->Left) {
		return false;
	}
	if (x >= Rect->Width + Rect->Left) {
		return false;
	}

	if (y <= Rect->Top) {
		return false;
	}
	if (y >= Rect->Height + Rect->Top) {
		return false;
	}

	return true;
}

VAPI(VanillaBool) VanillaIntersectRect(VanillaRect Rect1, VanillaRect Rect2, VanillaRect RectResult) {
	RectResult->Left = std::max(Rect1->Left, Rect2->Left);
	RectResult->Top = std::max(Rect1->Top, Rect2->Top);
	RectResult->Width = std::min(Rect1->Left + Rect1->Width, Rect2->Left + Rect2->Width) - RectResult->Left;
	RectResult->Height = std::min(Rect1->Top + Rect1->Height, Rect2->Top + Rect2->Height) - RectResult->Top;
	return (RectResult->Width > 0) && (RectResult->Height > 0);
}

VAPI(VanillaInt) VanillaUTF16ToUTF8(char* Output, VanillaInt OutputLength, wchar_t* Input, VanillaInt InputLength) {
	if (Input == NULL || InputLength<0)
	{
		return -1;
	}
	int totalNum = 0;
	for (VanillaInt i = 0; i < InputLength; i++)
	{
		wchar_t unicode = Input[i];
		if (unicode >= 0x0000 && unicode <= 0x007f)
		{
			totalNum += 1;
		}
		else if (unicode >= 0x0080 && unicode <= 0x07ff)
		{
			totalNum += 2;
		}
		else if (unicode >= 0x0800 && unicode <= 0xffff)
		{
			totalNum += 3;
		}
	}
	if (Output == NULL) {
		return totalNum;
	}

	if (OutputLength < totalNum)
	{
		return -1;
	}

	int OutputSize = 0;
	char *tmp = Output;
	int i = 0;
	for (i = 0; i < InputLength; i++)
	{
		if (OutputSize>OutputLength)
		{
			return -1;
		}
		wchar_t unicode = Input[i];

		if (unicode >= 0x0000 && unicode <= 0x007f)
		{
			*tmp = (char)unicode;
			tmp += 1;
			OutputSize += 1;
		}
		else if (unicode >= 0x0080 && unicode <= 0x07ff)
		{
			*tmp = 0xc0 | (unicode >> 6);
			tmp += 1;
			*tmp = 0x80 | (unicode & (0xff >> 2));
			tmp += 1;
			OutputSize += 2;
		}
		else if (unicode >= 0x0800 && unicode <= 0xffff)
		{
			*tmp = 0xe0 | (unicode >> 12);
			tmp += 1;
			*tmp = 0x80 | (unicode >> 6 & 0x00ff);
			tmp += 1;
			*tmp = 0x80 | (unicode & (0xff >> 2));
			tmp += 1;
			OutputSize += 3;
		}
	}
	if (OutputLength >= OutputSize + 1) {
		tmp++;
		*tmp = '\0';
	}
	return OutputSize;
}

VAPI(VanillaInt) VanillaUTF8ToUTF16(wchar_t* Output, VanillaInt OutputSize, char* Input, VanillaInt Inputsize) {
	if (Input == NULL || Inputsize<0)
	{
		return -1;
	}

	int totalNum = 0;
	char *p = Input;
	for (VanillaInt i = 0; i<Inputsize; i++)
	{
		if (*p >= 0x00 && *p <= 0x7f)
		{
			p++;
			totalNum += 1;
		}
		else if ((*p & (0xe0)) == 0xc0)
		{
			p++;
			p++;
			totalNum += 1;
		}
		else if ((*p & (0xf0)) == 0xe0)
		{
			p++;
			p++;
			p++;
			totalNum += 1;
		}
	}

	if (Output == NULL) {
		return totalNum;
	}

	if (OutputSize < totalNum)
	{
		return -1;
	}

	int resultsize = 0;

	p = Input;
	char* tmp = (char *)Output;
	//tmp++;
	while (*p)
	{
		if (*p >= 0x00 && *p <= 0x7f)
		{
			*tmp = *p;
			tmp++;
			*tmp = 0;
			tmp++;
			resultsize += 1;
		}
		else if ((*p & 0xe0) == 0xc0)
		{
			char t1 = 0;
			char t2 = 0;

			t1 = *p & (0x1f);
			p++;
			t2 = *p & (0x3f);

			*tmp = t2 | ((t1 & (0x03)) << 6);
			tmp++;
			*tmp = t1 >> 2;
			tmp++;
			resultsize += 1;
		}
		else if ((*p & (0xf0)) == 0xe0)
		{
			wchar_t t1 = 0;
			wchar_t t2 = 0;
			wchar_t t3 = 0;
			t1 = *p & (0x1f);
			p++;
			t2 = *p & (0x3f);
			p++;
			t3 = *p & (0x3f);

			*tmp = ((t2 & (0x03)) << 6) | t3;
			tmp++;
			*tmp = (t1 << 4) | (t2 >> 2);
			tmp++;
			resultsize += 1;
		}
		p++;
	}
	if (OutputSize >= totalNum + 1) {
		*tmp = '\0';
		tmp++;
		*tmp = '\0';
		resultsize += 2;
	}
	return resultsize;
}
