#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VDrawing.h"
#include "VGlobalFunction.h"
#include "Controls/Include.h"
#include "Utils/VSkinDB.h"


int TestEventProc(VanillaWindow Window, VanillaControl Control, int Event, int Param1, int Param2, int Param3);

VanillaControl Label1, Label2;
VanillaControl Button1, Button2, Button3;
VanillaWindow Window;
VanillaImage BkgImage;

VAPI(void) DLLDebug() {

	if (VanillaInit()) {
		//MessageBox(NULL, L"VanillaUI is running!", NULL, MB_OK);
	} else {
		//MessageBoxW(NULL, L"Failed to load VanillaUI!", NULL, MB_OK);
	}
	//VanillaSetWindowUserProc(Window, &TestWindowUserProc);
	//ARGB();
	// Load a SkinDB
	//VanillaBin Bin = VanillaReadFileToBin("./Bkg.png");
	//VanillaSkinDB SkinDB = VanillaSkinDBCreate("Vanilla Default Skin");
	//VanillaSkinDBSet(SkinDB, "Bkg", Bin);
	//VanillaSkinDBSave(SkinDB, "skindb.vsk");


	// Get the image
	VanillaSkinDB SkinDB2 = VanillaSkinDBLoad("DebugResource/skindb.vsk");
	VanillaBin Bin2 = VanillaSkinDBGet(SkinDB2, "Bkg");

	//BkgImage = VanillaLoadImageFromFile(L"./Bkg.png", 0, 0);
	BkgImage = VanillaLoadImageFromBinary(Bin2);
	VanillaSkinDBDestroy(SkinDB2);

	Window = VanillaCreateWindow(RECT(100, 100, 600, 460), VWS_TITLE | VWS_POSMIDDLE | VWS_CTLBTN_MIN, "VanillaUI Project On Linux", VanillaCreateStringFormat("文泉驿微米黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_VALIGN_TOP), NULL, ARGB(255, 128, 128, 128), NULL, VWFS_RECT);


	VanillaSetWindowBkg(Window, BkgImage, 0, VBT_TENSILE | VBT_NOCOLORFILL);
	VanillaSetWindowShadowColor(Window, RGB(0, 0, 0));
	VanillaSetWindowComposite(Window, false);
	VanillaSetWindowAlpha(Window, 216);



	//VanillaRegisterControlClass(L"TestControlClass", TestCtlProc, TRUE);
	//VanillaControl Control1 = VanillaControlCreate(Window, NULL, L"TestControlClass");
	//VanillaControlInitialize(Control1, &VRect(50, 50, 100, 100), NULL, (int)Control1, (LPVOID)30103);

	Label1 = VanillaLabelCreate(VanillaGetWindowRootControl(Window), RECT(50, 50, 100, 100), "Label", VanillaCreateStringFormat("文泉驿微米黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_ALIGN_CENTER | VTS_VALIGN_MIDDLE | VTS_SHADOW), true, true);
	Label2 = VanillaLabelCreate(Label1, RECT(0, 0, 80, 50), "\r\n\r\nSubLabel", VanillaCreateStringFormat("文泉驿微米黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_ALIGN_CENTER | VTS_VALIGN_MIDDLE | VTS_SHADOW), true, true);

	VanillaColor ButtonColor[] = { ARGB(196, 0, 102, 178), ARGB(196, 0, 153, 153), ARGB(196, 0, 178, 102), ARGB(196, 123, 123, 123), ARGB(196, 255, 255, 255) };
	Button1 = VanillaButtonCreate(VanillaGetWindowRootControl(Window), RECT(50, 210, 90, 35), " I am a Button.\r\n=_=哦哦", VanillaCreateStringFormat("文泉驿微米黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_ALIGN_CENTER | VTS_VALIGN_MIDDLE | VTS_SHADOW), true, true);
	Button2 = VanillaButtonCreate(VanillaGetWindowRootControl(Window), RECT(50, 250, 90, 35), " I am a Button, too.\r\n=_=", VanillaCreateStringFormat("文泉驿微米黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_ALIGN_CENTER | VTS_VALIGN_MIDDLE | VTS_SHADOW), true, true);
	Button3 = VanillaButtonCreate(VanillaGetWindowRootControl(Window), RECT(50, 400, 90, 35), "Close Window", VanillaCreateStringFormat("文泉驿微米黑", 12, ARGB(255, 0, 0, 0), ARGB(255, 255, 255, 255), VTS_ALIGN_CENTER | VTS_VALIGN_MIDDLE | VTS_SHADOW), true, true);
	VanillaButtonSetStyle_Metro(Button1, ButtonColor);
	VanillaButtonSetStyle_Metro(Button2, ButtonColor);
	VanillaButtonSetStyle_Metro(Button3, ButtonColor);


	VanillaControlSetDefaultEventProc(&TestEventProc);
	VanillaRedrawWindow(Window);
	VanillaSetWindowVisible(Window, true);
	//VanillaControlDestroy(Label1);
	VanillaMsgLoop();


	VanillaDestroyWindow(Window);
	VanillaDestroyImage(BkgImage);
}

int TestEventProc(VanillaWindow Window, VanillaControl Control, int Event, int Param1, int Param2, int Param3) {
	if (Control == Label1) {
		switch (Event)
		{
		case VE_LBTNDBCLK:
//			MessageBoxW(Window->PortWindow->hWnd, L"You double clicked Label1", L"VanillaUI Message", MB_ICONINFORMATION);
			break;
		}
	} else if (Control == Label2) {
		switch (Event)
		{
		case VE_LBTNDBCLK:
//			MessageBoxW(Window->PortWindow->hWnd, L"You double clicked Label2", L"VanillaUI Message", MB_ICONINFORMATION);
			break;
		}
	} else if (Control == Button1) {
		switch (Event)
		{
		case VE_LBTNCLK:
//			MessageBoxW(Window->PortWindow->hWnd, L"You clicked Button1", L"VanillaUI Message", MB_ICONINFORMATION);
			break;
		}
	} else if (Control == Button2) {
		switch (Event)
		{
		case VE_LBTNCLK:
//			MessageBoxW(Window->PortWindow->hWnd, L"You clicked Button2", L"VanillaUI Message", MB_ICONINFORMATION);
			break;
		}
	} else if (Control == Button3) {
		switch (Event)
		{
		case VE_LBTNCLK:
//			SendMessageW(Window->PortWindow->hWnd, WM_CLOSE, 0, 0);
//			PostQuitMessage(301);
			break;
		}
	}
	return NULL;
}

/*bool __stdcall TestWindowUserProc(VanillaWindow Window, int CallBackType, int uMsg, WPARAM wParam, LPARAM lParam, LPVOID Result) {
	if (Window == ::Window) {
		switch (uMsg) {
		case WM_DESTROY:
			ExitProcess(301);
			break;
		}
	}
	return FALSE;
}*/