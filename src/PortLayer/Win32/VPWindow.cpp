#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VDrawing.h"
#include "VGlobalFunction.h"
#include "VPWindow.h"

#undef RGB

#include <tchar.h>
#include <windows.h>


typedef struct VPWWindow
{
    VanillaWindow Window;
	HWND hWnd;
	HDC hDC;
	bool Layered;
} *VanillaPWWindow, _VPWWindow;

typedef struct VPWGraphics
{
	HDC MemoryDC;
	HBITMAP OldBitmap;
	HBITMAP CurrentBitmap;
} *VanillaPWGraphics, _VPWGraphics;

#define PROP_ID		(LPCWSTR)101
#define PROP_OLDPROC	(LPCWSTR)102

LRESULT CALLBACK VanillaPWWin32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

VanillaBool VanillaPWInitialize() {
	return true;
}

VanillaPWWindow VanillaPWCreateWindow(VanillaRect Rect, VanillaString Title, VanillaBool ShowInTaskbar, VanillaBool PosMiddle, VanillaWindow Window) {
	static VanillaBool ClassRegistered;//是否注册WNDCLASS
	VANILLA_ICONV;
	if (!ClassRegistered) {
		/*第一次调用该函数向系统注册VanillaUI.Window类*/
		WNDCLASSEXW WindowClass;
		memset(&WindowClass, 0, sizeof(WindowClass));
		WindowClass.cbSize = sizeof(WindowClass);
		WindowClass.lpfnWndProc = (WNDPROC)&VanillaPWWin32WindowProc;
		WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		WindowClass.lpszClassName = L"VanillaUI.Window";
		WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);/*箭头光标*/
		RegisterClassExW(&WindowClass);
		ClassRegistered = true;
	}
	if (PosMiddle) {
		/*窗口居中*/
		Rect->Left = (GetSystemMetrics(SM_CXSCREEN) - Rect->Width) / 2;
		Rect->Top = (GetSystemMetrics(SM_CYSCREEN) - Rect->Height) / 2;
	}
	HWND hWnd = CreateWindowExW(ShowInTaskbar ? NULL : WS_EX_TOOLWINDOW,
		L"VanillaUI.Window",
		VANILLA_U2W(Title).c_str(),
		WS_POPUP | WS_TILED | WS_SYSMENU,
		Rect->Left,
		Rect->Top,
		Rect->Width,
		Rect->Height,
		NULL,
		NULL,
		NULL,
		NULL
		);
	if (!IsWindow(hWnd)) {
		return NULL;
	}
	/*保存相关参数到VPWWindow*/
	VanillaPWWindow PortWindow = new VPWWindow;
	PortWindow->hWnd = hWnd;
	PortWindow->hDC = GetDC(hWnd);
	PortWindow->Window = Window;
	SetPropW(PortWindow->hWnd, PROP_ID, (HANDLE)Window);
	return PortWindow;
}

VanillaVoid VanillaPWDestroyWindow(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		/*释放由GetDc得到的窗口DC*/
		ReleaseDC(PortWindow->hWnd, PortWindow->hDC);
		/*释放窗口*/
		DestroyWindow(PortWindow->hWnd);
		delete PortWindow;
	}
}

VanillaVoid VanillaPWSetWindowTitle(VanillaPWWindow PortWindow, VanillaString Title) {
	if (PortWindow) {
		VANILLA_ICONV;
		SetWindowTextW(PortWindow->hWnd, VANILLA_U2W(Title).c_str());
	}
}

VanillaString VanillaPWGetWindowTitle(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		VANILLA_ICONV;
		int Length = GetWindowTextLengthW(PortWindow->hWnd);
		wchar_t* Buffers = new wchar_t [Length + 1];
		GetWindowTextW(PortWindow->hWnd, Buffers, Length);
		VanillaString Result = VANILLA_W2U((std::wstring)Buffers);
		delete []Buffers;
		return Result;
	}
	return NULL;
}
VanillaVoid VanillaPWSetWindowVisible(VanillaPWWindow PortWindow, VanillaBool Visible) {
	if (PortWindow) {
		ShowWindow(PortWindow->hWnd, Visible ? SW_SHOW : SW_HIDE);
	}
}

VanillaBool VanillaPWGetWindowVisible(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		return IsWindowVisible(PortWindow->hWnd) == TRUE;
	}
	return false;
}

VanillaVoid VanillaPWSetWindowComposite(VanillaPWWindow PortWindow, VanillaBool Composite) {
	if (PortWindow) {
		PortWindow->Layered = Composite;
		if (Composite) {
			SetWindowLongW(PortWindow->hWnd, GWL_EXSTYLE, GetWindowLongW(PortWindow->hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		}
		else {
			SetWindowLongW(PortWindow->hWnd, GWL_EXSTYLE, GetWindowLongW(PortWindow->hWnd, GWL_EXSTYLE) | ~WS_EX_LAYERED);;
		}
	}
	return;
}

VanillaBool VanillaPWGetWindowComposite(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		return PortWindow->Layered;
	}
	return false;
}

VanillaVoid VanillaPWDragWindow(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		SendMessageW(PortWindow->hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
	}
	return;
}

VanillaVoid VanillaPWDestroyGraphicsOfWindowCachedInMemoey(VanillaGraphics Graphics) {
	if (Graphics) {
		DeleteObject((HBITMAP)SelectObject(VanillaGraphicsGetPWGraphics(Graphics)->MemoryDC, (HGDIOBJ)VanillaGraphicsGetPWGraphics(Graphics)->OldBitmap));
		DeleteDC(VanillaGraphicsGetPWGraphics(Graphics)->MemoryDC);
		delete VanillaGraphicsGetPWGraphics(Graphics);
        VanillaDestroyGraphics(Graphics);
	}
	return;
}

VanillaGraphics VanillaPWCreateGraphicsOfWindowCachedInMemoey(VanillaWindow Window) {
	if (Window) {
		HDC MemoryDC = CreateCompatibleDC(NULL);
		if (!MemoryDC) {
			return NULL;
		}
		BITMAPINFO BitmapInfo;
		memset(&BitmapInfo, 0, sizeof(BitmapInfo));
		BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
		BitmapInfo.bmiHeader.biBitCount = 32;
		BitmapInfo.bmiHeader.biWidth = Window->Rect.Width;
		BitmapInfo.bmiHeader.biHeight = - Window->Rect.Height;
		BitmapInfo.bmiHeader.biPlanes = 1;
		VanillaAny Bits;
		HBITMAP HBitmap = CreateDIBSection(0, &BitmapInfo, 0, &Bits, 0, 0);
		if (!HBitmap) {
			DeleteDC(MemoryDC);
			return NULL;
		}

		HBITMAP OldBitmap = (HBITMAP)SelectObject(MemoryDC, (HGDIOBJ)HBitmap);

		VanillaGraphics Graphics = VanillaCreateGraphicsFromPixelAddress(Bits, Window->Rect.Width, Window->Rect.Height);

		VanillaPWGraphics PWGraphics = new VPWGraphics;
		PWGraphics->MemoryDC = MemoryDC;
		PWGraphics->CurrentBitmap = HBitmap;
		PWGraphics->OldBitmap = OldBitmap;
		VanillaGraphicsSetPWGraphics(Graphics, PWGraphics);
		return Graphics;
	}
	return NULL;
}

VanillaVoid VanillaPWUpdateWindow(VanillaWindow Window, VanillaRect UpdateRect) {
	if (Window) {
		if (GetWindowLongW(Window->PortWindow->hWnd, GWL_EXSTYLE) & WS_EX_LAYERED) {
			/*分层窗口*/
			BLENDFUNCTION Blend;
			Blend.AlphaFormat = AC_SRC_ALPHA;
			Blend.BlendFlags = NULL;
			Blend.BlendOp = NULL;
			Blend.SourceConstantAlpha = Window->Alpha;

			POINT pt1 = { Window->Rect.Left, Window->Rect.Top };
			POINT pt2 = { 0, 0 };
			SIZE sz = { Window->Rect.Width, Window->Rect.Height };
			HDC SrcDC = VanillaGraphicsGetPWGraphics(Window->GraphicsWindow)->MemoryDC;
			UpdateLayeredWindow(Window->PortWindow->hWnd,
				Window->PortWindow->hDC,
				&pt1,
				&sz,
				SrcDC,
				&pt2,
				NULL,
				&Blend,
				ULW_ALPHA);
		}
		else {
			/*普通窗口*/
			VRect _UpdateRect;
			if (NULL == UpdateRect) {
			    _UpdateRect = Window->Rect;
				_UpdateRect.Left = 0;
				_UpdateRect.Top = 0;
				UpdateRect = &_UpdateRect;
			}
			BitBlt(Window->PortWindow->hDC, UpdateRect->Left, UpdateRect->Top, UpdateRect->Width, UpdateRect->Height, VanillaGraphicsGetPWGraphics(Window->GraphicsWindow)->MemoryDC, UpdateRect->Left, UpdateRect->Top, SRCCOPY);
		}
	}
}

VanillaInt VanillaPWMessageLoop() {
	MSG Msg;
	while (GetMessage(&Msg, NULL, NULL, NULL))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

VanillaInt VanillaPWGetTickCount() {
	return GetTickCount();
}

/**
* 移植层的消息回调 来自操作系统的消息通知由此函数分发处理
*/
LRESULT CALLBACK VanillaPWWin32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	VanillaWindow Window = (VanillaWindow)GetPropW(hWnd, PROP_ID);//取出窗口对应的VanillaWindow对象
	if (Window == NULL) {
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
	switch (uMsg) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		BitBlt(ps.hdc, 0, 0, Window->Rect.Width, Window->Rect.Height, VanillaGraphicsGetPWGraphics(Window->GraphicsWindow)->MemoryDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;
		break;
	}
	case WM_SIZE: {
		/*窗口大小被改变*/
		Window->Rect.Width = (VanillaInt)(short)LOWORD(lParam);
		Window->Rect.Height = (VanillaInt)(short)HIWORD(lParam);
		VanillaControlMove(Window->Title.Control, Window->Title.Control->Rect.Left, Window->Title.Control->Rect.Top, Window->Rect.Width - Window->Title.Control->Rect.Left, Window->Title.Control->Rect.Height);
		VanillaWindowInitGraphics(Window, true);
		break;
	}
	case WM_MOVE: {
		/*窗口移动 更新VanillaWindow对象窗口位置*/
		Window->Rect.Left = (VanillaInt)(short)LOWORD(lParam);
		Window->Rect.Top = (VanillaInt)(short)HIWORD(lParam);
		break;
	}
	case WM_MOUSEMOVE: {
		/*鼠标移动*/
		VanillaDispatchMouseMessage(Window, -1, -1, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_LBUTTONDOWN: {
		/*鼠标左键被按下*/
		VanillaDispatchMouseMessage(Window, 1, 0, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_LBUTTONUP: {
		/*鼠标左键被弹起*/
		VanillaDispatchMouseMessage(Window, 2, 0, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_LBUTTONDBLCLK: {
		/*鼠标左键被双击*/
		VanillaDispatchMouseMessage(Window, 3, 0, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_RBUTTONDOWN: {
		/*鼠标右键被按下*/
		VanillaDispatchMouseMessage(Window, 1, 1, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_RBUTTONUP: {
		/*鼠标右键被弹起*/
		VanillaDispatchMouseMessage(Window, 2, 1, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_RBUTTONDBLCLK: {
		/*鼠标右键被双击*/
		VanillaDispatchMouseMessage(Window, 3, 1, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_MBUTTONDOWN: {
		VanillaDispatchMouseMessage(Window, 1, 2, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_MBUTTONUP: {
		VanillaDispatchMouseMessage(Window, 2, 2, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_MBUTTONDBLCLK: {
		VanillaDispatchMouseMessage(Window, 3, 2, LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_MOUSEWHEEL: {
		/*鼠标滚动消息*/
		if (Window->FocusControl) {
			VanillaControlSendMessage(Window->FocusControl, VM_MOUSEWHEEL, (VanillaInt)(HIWORD(wParam) / 120), NULL);
		}
		break;
	}
	case WM_KEYDOWN: {
		if (Window->FocusControl) {
			VanillaControlSendMessage(Window->FocusControl, VM_KEYDOWN, (VanillaInt)wParam, NULL);
		}
		break;
	}
	case WM_KEYUP: {
		if (Window->FocusControl) {
			VanillaControlSendMessage(Window->FocusControl, VM_KEYUP, (VanillaInt)wParam, NULL);
		}
		break;
	}
	case WM_CHAR: {
		wchar_t Char = (wchar_t)wParam;
		char Buffers[5];
		VanillaUTF16ToUTF8(Buffers, 5, &Char, 1);
		if (Window->FocusControl) {
			VanillaControlSendMessage(Window->FocusControl, VM_CHAR, (VanillaInt)Buffers, NULL);
		}
		break;
	}
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
