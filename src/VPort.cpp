#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VDrawing.h"
#include "VGlobalFunction.h"
#include "VPort.h"


#ifdef WIN32

#include <tchar.h>
#include <windows.h>

#elif defined LINUX

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <cairo/cairo-xcb.h>

xcb_connection_t *connection;
const xcb_setup_t *setup;
xcb_screen_iterator_t iter;
xcb_screen_t *screen;

std::map<xcb_window_t, VanillaPortWindow> WindowHashTable;

__SK_FORCE_IMAGE_DECODER_LINKING;
#endif

typedef struct VPortWindow
{
    VanillaWindow Window;
#ifdef WIN32
	HWND hWnd;
	HDC hDC;
	bool Layered;
#elif defined LINUX
    cairo_t* cairo;
    cairo_surface_t* cairo_surface;
	bool mapped;
	xcb_window_t xwindow;
	xcb_pixmap_t xpixmap;
	xcb_gcontext_t gc;
	xcb_colormap_t xcolormap;
#endif
} *VanillaPortWindow, _VPortWindow;

typedef struct VPortGraphics
{
#ifdef WIN32
	HDC MemoryDC;
	HBITMAP OldBitmap;
	HBITMAP CurrentBitmap;
#elif defined LINUX
    cairo_surface_t* cairo_surface;
#endif
} *VanillaPortGraphics, _VPortGraphics;

#ifdef WIN32
#define PROP_ID		(LPCWSTR)101
#define PROP_OLDPROC	(LPCWSTR)102

LRESULT CALLBACK VanillaPortWin32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VanillaControl VanillaPortWin32DispatchMessage_Mouse(VanillaWindow Window, LPARAM lParam, VanillaInt Action, VanillaInt Code);
#elif defined LINUX
VanillaVoid VanillaPortLinuxWindowProc(xcb_generic_event_t *e);
#endif

VAPI(VanillaInt) VanillaPortUTF16ToUTF8(char* Output, VanillaInt OutputLength, wchar_t* Input, VanillaInt InputLength) {
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

VAPI(VanillaInt) VanillaPortUTF8ToUTF16(wchar_t* Output, VanillaInt OutputSize, char* Input, VanillaInt Inputsize) {
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

VanillaBool VanillaPortInitializeX() {
#ifdef WIN32
	return true;
#elif defined LINUX
	connection = xcb_connect(NULL, NULL);
	setup = xcb_get_setup(connection);
	iter = xcb_setup_roots_iterator(setup);
	screen = iter.data;
	return screen != NULL;
#endif
}

VanillaPortWindow VanillaPortCreateWindow(VanillaRect Rect, VanillaString Title, VanillaBool ShowInTaskbar, VanillaBool PosMiddle, VanillaWindow Window) {
#ifdef WIN32
	static VanillaBool ClassRegistered;//是否注册WNDCLASS
	VanillaPort_ICONV;
	if (!ClassRegistered) {
		/*第一次调用该函数向系统注册VanillaUI.Window类*/
		WNDCLASSEXW WindowClass;
		memset(&WindowClass, 0, sizeof(WindowClass));
		WindowClass.cbSize = sizeof(WindowClass);
		WindowClass.lpfnWndProc = (WNDPROC)&VanillaPortWin32WindowProc;
		WindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		WindowClass.lpszClassName = L"VanillaUI.Window";
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
		VanillaPort_U2W(Title).c_str(),
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
	/*保存相关参数到VPortWindow*/
	VanillaPortWindow PortWindow = new VPortWindow;
	PortWindow->hWnd = hWnd;
	PortWindow->hDC = GetDC(hWnd);
	PortWindow->Window = Window;
	SetPropW(PortWindow->hWnd, PROP_ID, (HANDLE)Window);
	return PortWindow;
#elif defined LINUX

	if (PosMiddle) {
		Rect->Left = (screen->width_in_pixels - Rect->Width) / 2;
		Rect->Top = (screen->height_in_pixels - Rect->Height) / 2;
	}


    xcb_visualtype_t* visualtype = NULL;
    xcb_depth_iterator_t iter;
    uint8_t depth;

    for (iter = xcb_screen_allowed_depths_iterator(screen); iter.rem; xcb_depth_next(&iter)) {
        xcb_visualtype_t *visualtype2 = xcb_depth_visuals(iter.data);
        if (iter.data->depth == 32) {
            visualtype = visualtype2;
            depth = iter.data->depth;
            break;
        }
    }

    if (visualtype == NULL) {
        for (iter = xcb_screen_allowed_depths_iterator (screen); iter.rem; xcb_depth_next (&iter)) {
            for (xcb_visualtype_iterator_t visualtype_iterator = xcb_depth_visuals_iterator (iter.data); visualtype_iterator.rem; xcb_visualtype_next (&visualtype_iterator)) {
                if (visualtype_iterator.data->visual_id == screen->root_visual) {
                    visualtype = visualtype_iterator.data;
                    depth = iter.data->depth;
                    break;
                }
            }
            if (visualtype != NULL) {
                break;
            }
        }
    }

	xcb_colormap_t colormap = xcb_generate_id(connection);
	xcb_create_colormap(connection, XCB_COLORMAP_ALLOC_NONE, colormap, screen->root, visualtype->visual_id);

    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL | XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
    uint32_t values[5] = {  0,
                            0,
                            0,
                            XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
                            XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
                            XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
                            XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE,
                            colormap
                        };
	xcb_window_t window = xcb_generate_id(connection);
	xcb_create_window_checked(
		connection,
		depth,
		window,
		screen->root,
		Rect->Left, Rect->Top,
		Rect->Width, Rect->Height,
		0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		visualtype->visual_id,
		mask, values);
	xcb_change_property(connection,
		XCB_PROP_MODE_REPLACE,
		window,
		XCB_ATOM_WM_NAME,
		XCB_ATOM_STRING,
		8,
		strlen(Title.c_str()),
		Title.c_str());


	xcb_pixmap_t pixmap = xcb_generate_id(connection);
    xcb_create_pixmap(connection, depth, pixmap, window, Rect->Width, Rect->Height);

    VanillaPortWindow PortWindow = new VPortWindow;

    PortWindow->xpixmap = pixmap;


    PortWindow->cairo_surface = cairo_xcb_surface_create(connection, pixmap, visualtype, Rect->Width, Rect->Height);
    //PortWindow->cairo_surface = cairo_xcb_surface_create_for_bitmap(connection, screen, pixmap, Rect->Width, Rect->Height);
    if (!PortWindow->cairo_surface) {
        delete PortWindow;
        return NULL;
    }
    PortWindow->cairo = cairo_create(PortWindow->cairo_surface);
	PortWindow->xwindow = window;
	PortWindow->mapped = false;
   	PortWindow->Window = Window;


    PortWindow->gc = xcb_generate_id(connection);
    xcb_create_gc(connection, PortWindow->gc, window, 0, NULL);

    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 0, 12, "_NET_WM_STATE");
    xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(connection, cookie, 0);

    unsigned long MwmHints [5] = { 1L << 1, 0, 0, 0, 0};
    xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, reply->atom, reply->atom, 32, 5, MwmHints);


	WindowHashTable [window] = PortWindow;
	return PortWindow;
#endif
}

VanillaVoid VanillaPortDestroyWindow(VanillaPortWindow PortWindow) {
	if (PortWindow) {
#ifdef WIN32
		/*释放由GetDc得到的窗口DC*/
		ReleaseDC(PortWindow->hWnd, PortWindow->hDC);
		/*释放窗口*/
		DestroyWindow(PortWindow->hWnd);
		delete PortWindow;
#elif defined LINUX
		xcb_destroy_window(connection, PortWindow->xwindow);
		delete PortWindow;
#endif
	}
}

VanillaVoid VanillaPortSetWindowTitle(VanillaPortWindow PortWindow, VanillaString Title) {
	if (PortWindow) {
#ifdef WIN32
		VanillaPort_ICONV;
		SetWindowTextW(PortWindow->hWnd, VanillaPort_U2W(Title).c_str());
#elif defined LINUX
		xcb_change_property(connection,
			XCB_PROP_MODE_REPLACE,
			PortWindow->xwindow,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8,
			strlen(Title.c_str()),
			Title.c_str());
#endif
	}
}

VanillaString VanillaPortGetWindowTitle(VanillaPortWindow PortWindow) {
	if (PortWindow) {
#ifdef WIN32
		VanillaPort_ICONV;
		int Length = GetWindowTextLengthW(PortWindow->hWnd);
		wchar_t* Buffers = new wchar_t [Length + 1];
		GetWindowTextW(PortWindow->hWnd, Buffers, Length);
		VanillaString Result = VanillaPort_W2U((std::wstring)Buffers);
		delete []Buffers;
		return Result;
#elif defined LINUX
		xcb_generic_error_t *error;
		xcb_get_property_reply_t *reply = xcb_get_property_reply(connection, xcb_get_property(connection, false, PortWindow->xwindow, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, -1), &error);
		VanillaString Result((char*)xcb_get_property_value(reply), xcb_get_property_value_length(reply));
		free(reply);
		return Result;
#endif
	}
	return NULL;
}
VanillaVoid VanillaPortSetWindowVisible(VanillaPortWindow PortWindow, VanillaBool Visible) {
	if (PortWindow) {
#ifdef WIN32
		ShowWindow(PortWindow->hWnd, Visible ? SW_SHOW : SW_HIDE);
#elif defined LINUX
		if (PortWindow->mapped && !Visible) {
			xcb_unmap_window(connection, PortWindow->xwindow);
		}
		if (!PortWindow->mapped && Visible) {
			xcb_map_window(connection, PortWindow->xwindow);
		}
		xcb_flush(connection);
#endif
	}
}

VanillaBool VanillaPortGetWindowVisible(VanillaPortWindow PortWindow) {
	if (PortWindow) {
#ifdef WIN32
		return IsWindowVisible(PortWindow->hWnd) == TRUE;
#elif defined LINUX
		return PortWindow->mapped;
#endif
	}
	return false;
}

VanillaVoid VanillaPortSetWindowComposite(VanillaPortWindow PortWindow, VanillaBool Composite) {
	if (PortWindow) {
#ifdef WIN32
		PortWindow->Layered = Composite;
		if (Composite) {
			SetWindowLongW(PortWindow->hWnd, GWL_EXSTYLE, GetWindowLongW(PortWindow->hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		}
		else {
			SetWindowLongW(PortWindow->hWnd, GWL_EXSTYLE, GetWindowLongW(PortWindow->hWnd, GWL_EXSTYLE) | ~WS_EX_LAYERED);;
		}
#elif defined LINUX
        /*
        xcb_composite_query_version_reply_t *reply = xcb_composite_query_version_reply(connection, xcb_composite_query_version(connection, 0, 4), NULL);
        if (reply == NULL || reply->minor_version < 2) {
            free(reply);
            return;
        }
        if (Composite) {
            xcb_composite_redirect_window(connection, PortWindow->xwindow, XCB_COMPOSITE_REDIRECT_AUTOMATIC);
        } else {
            xcb_composite_unredirect_window(connection, PortWindow->xwindow, XCB_COMPOSITE_REDIRECT_AUTOMATIC);
        }
        */
#endif
	}
	return;
}

VanillaBool VanillaPortGetWindowComposite(VanillaPortWindow PortWindow) {
	if (PortWindow) {
#ifdef WIN32
		return PortWindow->Layered;
#elif defined LINUX
		return true;
#endif
	}
	return false;
}

VanillaVoid VanillaPortDragWindow(VanillaPortWindow PortWindow) {
	if (PortWindow) {
#ifdef WIN32
		SendMessageW(PortWindow->hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
#elif defined LINUX
		return;
#endif
	}
	return;
}

VanillaVoid VanillaPortDestroyGraphicsOfWindowCachedInMemoey(VanillaGraphics Graphics) {
	if (Graphics) {
#ifdef WIN32
	Graphics->Bitmap.reset();
	DeleteObject((HBITMAP)SelectObject(Graphics->PortGraphics->MemoryDC, (HGDIOBJ)Graphics->PortGraphics->OldBitmap));
	DeleteDC(Graphics->PortGraphics->MemoryDC);
	delete Graphics->PortGraphics;
        VanillaDestroyGraphics(Graphics);
#elif defined LINUX
        cairo_surface_destroy(Graphics->PortGraphics->cairo_surface);
        delete Graphics->PortGraphics;
        VanillaDestroyGraphics(Graphics);
	//xcb_free_pixmap(connection, Graphics->bitmap);
#endif
	}
	return;
}

VanillaGraphics VanillaPortCreateGraphicsOfWindowCachedInMemoey(VanillaWindow Window) {
	if (Window) {
#ifdef WIN32
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
		
		//Bitmap.setConfig(SkBitmap::kARGB_8888_Config, Window->Rect.Width, Window->Rect.Height, ((Window->Rect.Width * 32 + 15) / 16) * 2);
		//Bitmap.setPixels(Bits);
		VanillaGraphics Graphics = new VGraphics;
		Graphics->Bitmap.installPixels(SkImageInfo::MakeN32Premul(Window->Rect.Width, Window->Rect.Height), Bits, ((Window->Rect.Width * 32 + 15) / 16) * 2);
		new (&Graphics->Canvas) SkCanvas(Graphics->Bitmap);
		Graphics->PortGraphics = new VPortGraphics;
		Graphics->PortGraphics->MemoryDC = MemoryDC;
		Graphics->PortGraphics->CurrentBitmap = HBitmap;
		Graphics->Width = Window->Rect.Width;
		Graphics->Height = Window->Rect.Height;
		Graphics->PortGraphics->OldBitmap = OldBitmap;
		return Graphics;
#elif defined LINUX
       	//SkBitmap Bitmap;
        //Bitmap.allocPixels(SkImageInfo::Make(Window->Rect.Width, Window->Rect.Height, kBGRA_8888_SkColorType, kPremul_SkAlphaType));
        VanillaGraphics Graphics = VanillaCreateGraphicsInMemory(Window->Rect.Width, Window->Rect.Height);
        //Graphics->Width = Window->Rect.Width;
        //Graphics->Height = Window->Rect.Height;
        Graphics->PortGraphics = new VPortGraphics;
        Graphics->PortGraphics->cairo_surface = cairo_image_surface_create_for_data((unsigned char *)Graphics->Bitmap.getPixels(), CAIRO_FORMAT_ARGB32, Window->Rect.Width, Window->Rect.Height, ((Window->Rect.Width * 32 + 15) / 16) * 2);
        //Graphics->image = xcb_image_create_from_bitmap_data((uint8_t*)Bitmap->getPixels(), Window->Rect.Width, Window->Rect.Height);
        return Graphics;
#endif
	}
	return NULL;
}

VanillaVoid VanillaPortUpdateWindow(VanillaWindow Window, VanillaRect UpdateRect) {
	if (Window) {
#ifdef WIN32
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
			HDC SrcDC = Window->GraphicsWindow->PortGraphics->MemoryDC;
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
			BitBlt(Window->PortWindow->hDC, UpdateRect->Left, UpdateRect->Top, UpdateRect->Width, UpdateRect->Height, Window->GraphicsWindow->PortGraphics->MemoryDC, UpdateRect->Left, UpdateRect->Top, SRCCOPY);
		}
#elif defined LINUX
        VRect _UpdateRect;
        if (NULL == UpdateRect) {
			    _UpdateRect = Window->Rect;
				_UpdateRect.Left = 0;
				_UpdateRect.Top = 0;
				UpdateRect = &_UpdateRect;
        }
        cairo_surface_flush(Window->GraphicsWindow->PortGraphics->cairo_surface);
        cairo_save(Window->PortWindow->cairo);
        cairo_set_source_surface(Window->PortWindow->cairo, Window->GraphicsWindow->PortGraphics->cairo_surface, 0, 0);
        cairo_rectangle(Window->PortWindow->cairo, UpdateRect->Left, UpdateRect->Top, UpdateRect->Width, UpdateRect->Height);
        cairo_clip(Window->PortWindow->cairo);
        cairo_set_operator(Window->PortWindow->cairo, CAIRO_OPERATOR_CLEAR);
        cairo_paint(Window->PortWindow->cairo);
        cairo_set_operator(Window->PortWindow->cairo, CAIRO_OPERATOR_SOURCE);
        cairo_paint_with_alpha(Window->PortWindow->cairo, (double)Window->Alpha / 255);
        cairo_surface_flush(Window->PortWindow->cairo_surface);
        cairo_restore(Window->PortWindow->cairo);

        xcb_copy_area(connection, Window->PortWindow->xpixmap, Window->PortWindow->xwindow, Window->PortWindow->gc, UpdateRect->Left, UpdateRect->Top, UpdateRect->Left, UpdateRect->Top, UpdateRect->Width, UpdateRect->Height);
        xcb_flush(connection);
#endif
	}
}

SkTypeface* VanillaPortCreateSkTypeface(VanillaText FontName, SkTypeface::Style Style) {
#ifdef WIN32
	VanillaPort_ICONV;
	const wchar_t* Unicode;
	Unicode = VanillaPort_U2W(FontName).c_str();
	int LengthOld = wcslen(Unicode);
	int LengthNew = LengthOld * 2 + 1;
	char* Ansi = (char*)alloca(LengthNew);
	LengthNew = WideCharToMultiByte(CP_ACP, NULL, Unicode, LengthOld, Ansi, LengthNew, NULL, NULL);
	Ansi [LengthNew] = 0;
	return SkTypeface::CreateFromName(Ansi, Style);
#elif defined LINUX
    return SkTypeface::CreateFromName(FontName, Style);
#endif
    return NULL;
}

VanillaInt VanillaPortMessageLoop() {
#ifdef WIN32
	MSG Msg;
	while (GetMessage(&Msg, NULL, NULL, NULL))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
#elif defined LINUX
    xcb_generic_event_t *e;
    while ((e = xcb_wait_for_event(connection))) {
        VanillaPortLinuxWindowProc(e);
        free (e);
    }
    //xcb_flush(connection);
    //pause();
#endif
	return 0;
}

VanillaInt VanillaPortGetTickCount() {
#ifdef WIN32
	return GetTickCount();
#elif defined LINUX
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

VanillaControl VanillaPortDispatchMouseMessage(VanillaWindow Window, VanillaInt Action, VanillaInt Button, VPoint pt) {
    VanillaControl Control;
	VPoint pt2;
	Control = VanillaFindControlInWindow(Window, &pt, &pt2);
	if (Action == -1) {
		if (Control != Window->MouseInControl) {
			VanillaControl OldControl = Window->MouseInControl;
			Window->MouseInControl = Control;
			VanillaControlSendMessage(OldControl, VM_MOUSEOUT, (VanillaInt)Control, NULL);
			VanillaControlSendMessage(Control, VM_MOUSEIN, (VanillaInt)OldControl, (VanillaInt)&pt2);
		}
		VanillaControlSendMessage(Control, VM_MOUSEMOVE, NULL, (VanillaInt)&pt2);
	}
	else {
		if (Action == 1) {
			Window->ButtonDownControl[Button] = Control;
			if (Control && Control->Class->Focusable && Window->FocusControl != Control) {
				VanillaControl OldControl = Window->FocusControl;
				Window->FocusControl = Control;
				VanillaControlSendMessage(OldControl, VM_KILLFOCUS, NULL, (VanillaInt)Control);
				VanillaControlSendMessage(Control, VM_SETFOCUS, NULL, (VanillaInt)OldControl);
			}
		}
		if (Control != NULL) {
            VanillaInt MsgList[2] [3] = { { VM_LBUTTONDOWN, VM_RBUTTONDOWN, VM_MBUTTONDOWN },
                                     { VM_LBUTTONUP, VM_RBUTTONUP, VM_MBUTTONUP } };
			VanillaControlSendMessage(Control, MsgList [Action - 1] [Button], 0, (VanillaInt)&pt2);
		}
		if (Action == 2) {
			if (Window->ButtonDownControl[Button] == Control) {
				VanillaInt MsgList[] = { VM_LBUTTONCLK, VM_RBUTTONCLK, VM_MBUTTONCLK };
				VanillaControlSendMessage(Control, MsgList[Button], 0, (VanillaInt)&pt2);
			}
			Window->ButtonDownControl[Button] = NULL;
		}
	}
	return Control;
}
/**
* 移植层的消息回调 来自操作系统的消息通知由此函数分发处理
*/
#ifdef WIN32
LRESULT CALLBACK VanillaPortWin32WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	VanillaWindow Window = (VanillaWindow)GetPropW(hWnd, PROP_ID);//取出窗口对应的VanillaWindow对象
	if (Window == NULL) {
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
	switch (uMsg) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		BitBlt(ps.hdc, 0, 0, Window->Rect.Width, Window->Rect.Height, Window->GraphicsWindow->PortGraphics->MemoryDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;
		break;
	}
	case WM_SIZE: {
		RECT rc;
		GetWindowRect(hWnd, &rc);
		Window->Rect.Width = rc.right - rc.left;
		Window->Rect.Height = rc.bottom - rc.top;
		VanillaControlMove(Window->Title.Control, Window->Title.Control->Rect.Left, Window->Title.Control->Rect.Top, Window->Rect.Width - Window->Title.Control->Rect.Left, Window->Title.Control->Rect.Height);
		VanillaWindowInitGraphics(Window, true);
		break;
	}
	case WM_MOVE: {
		RECT rc;
		GetWindowRect(hWnd, &rc);
		Window->Rect.Left = rc.left;
		Window->Rect.Top = rc.top;
		break;
	}
	case WM_MOUSEMOVE: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, -1, -1);
		break;
	}
	case WM_LBUTTONDOWN: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, 1, 0);
		break;
	}
	case WM_LBUTTONUP: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, 2, 0);
		break;
	}
	case WM_LBUTTONDBLCLK: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, 3, 0);
		break;
	}
	case WM_RBUTTONDOWN: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, 1, 1);
		break;
	}
	case WM_RBUTTONUP: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, 2, 1);
		break;
	}
	case WM_RBUTTONDBLCLK: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, 3, 1);
		break;
	}
	case WM_MBUTTONDOWN: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, 1, 2);
		break;
	}
	case WM_MBUTTONUP: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, 2, 2);
		break;
	}
	case WM_MBUTTONDBLCLK: {
		VanillaPortWin32DispatchMessage_Mouse(Window, lParam, 3, 2);
		break;
	}
	case WM_MOUSEWHEEL: {
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
		VanillaPortUTF16ToUTF8(Buffers, 5, &Char, 1);
		if (Window->FocusControl) {
			VanillaControlSendMessage(Window->FocusControl, VM_CHAR, (VanillaInt)Buffers, NULL);
		}
		break;
	}
	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

VanillaControl VanillaPortWin32DispatchMessage_Mouse(VanillaWindow Window, LPARAM lParam, VanillaInt Action, VanillaInt Code) {
	return VanillaPortDispatchMouseMessage(Window, Action, Code, VPoint(LOWORD(lParam), HIWORD(lParam)));
}

#elif defined LINUX
VanillaVoid VanillaPortLinuxWindowProc(xcb_generic_event_t *event) {
#define REAL_WINDOW(evt) Window = ((VanillaPortWindow)WindowHashTable [evt])->Window
    VanillaWindow Window;
    switch(event->response_type & ~0x80) {
            case XCB_EXPOSE: {
            xcb_expose_event_t *expose = (xcb_expose_event_t *)event;
            REAL_WINDOW(expose->window);
            VRect UpdateRect(expose->x, expose->y, expose->width, expose->height);
            VanillaPortUpdateWindow(Window, &UpdateRect);
            break;
        }
        case XCB_BUTTON_PRESS: {
            xcb_button_press_event_t *bp = (xcb_button_press_event_t *)event;
            REAL_WINDOW(bp->event);
            switch (bp->detail) {
            case 4:
                if (Window->FocusControl) {
                    VanillaControlSendMessage(Window->FocusControl, VM_MOUSEWHEEL, 1, NULL);
                }
                break;
            case 5:
                if (Window->FocusControl) {
                    VanillaControlSendMessage(Window->FocusControl, VM_MOUSEWHEEL, -1, NULL);
                }
                break;
            case 3:
            case 2:
            case 1:
                VanillaInt ButtonCode[] = { 0, 2, 1 };
                VanillaPortDispatchMouseMessage(Window, 1, ButtonCode [bp->detail - 1], VPoint(bp->event_x, bp->event_y));
                break;
            }
            break;
        }
        case XCB_BUTTON_RELEASE: {
            xcb_button_release_event_t *br = (xcb_button_release_event_t *)event;
            REAL_WINDOW(br->event);
            if (br->detail >= 1 && br->detail <= 3) {
                VanillaInt ButtonCode[] = { 0, 2, 1 };
                VanillaPortDispatchMouseMessage(Window, 2, ButtonCode [br->detail - 1], VPoint(br->event_x, br->event_y));
            }
            break;
        }
        case XCB_MOTION_NOTIFY: {
            xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
            REAL_WINDOW(motion->event);
            VanillaPortDispatchMouseMessage(Window, -1, -1, VPoint(motion->event_x, motion->event_y));
            break;
        }
        case XCB_ENTER_NOTIFY: {
            xcb_enter_notify_event_t *enter = (xcb_enter_notify_event_t *)event;
            REAL_WINDOW(enter->event);
            break;
        }
        case XCB_LEAVE_NOTIFY: {
            xcb_leave_notify_event_t *leave = (xcb_leave_notify_event_t *)event;
            REAL_WINDOW(leave->event);
            if (Window->MouseInControl) {
                VanillaControlSendMessage(Window->MouseInControl, VM_MOUSEOUT, NULL, NULL);
                Window->MouseInControl = NULL;
            }
            break;
        }
        case XCB_KEY_PRESS: {
            xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
            REAL_WINDOW(kp->event);
            break;
        }
        case XCB_KEY_RELEASE: {
            xcb_key_release_event_t *kr = (xcb_key_release_event_t *)event;
            REAL_WINDOW(kr->event);
            break;
        }
    }
#undef REAL_WINDOW
}
#endif
