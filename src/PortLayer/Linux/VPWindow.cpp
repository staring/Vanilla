#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VDrawing.h"
#include "VGlobalFunction.h"
#include "VPWindow.h"
#include "VWindow.h"


#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>

xcb_connection_t *connection;
const xcb_setup_t *setup;
xcb_screen_iterator_t iter;
xcb_screen_t *screen;

std::map<xcb_window_t, VanillaPWWindow> WindowHashTable;

typedef struct VPWWindow
{
    VanillaWindow Window;
    cairo_t* cairo;
    cairo_surface_t* cairo_surface;
	bool mapped;
	xcb_window_t xwindow;
	xcb_pixmap_t xpixmap;
	xcb_gcontext_t gc;
	xcb_colormap_t xcolormap;
} *VanillaPWWindow, _VPWWindow;

typedef struct VPWGraphics
{
    cairo_surface_t* cairo_surface;
} *VanillaPWGraphics, _VPWGraphics;

VanillaVoid VanillaPWLinuxWindowProc(xcb_generic_event_t *e);

VanillaBool VanillaPWInitialize() {
	connection = xcb_connect(NULL, NULL);
	setup = xcb_get_setup(connection);
	iter = xcb_setup_roots_iterator(setup);
	screen = iter.data;
	return screen != NULL;
}

VanillaPWWindow VanillaPWCreateWindow(VanillaRect Rect, VanillaString Title, VanillaBool ShowInTaskbar, VanillaBool PosMiddle, VanillaWindow Window) {
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

    VanillaPWWindow PortWindow = new VPWWindow;

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
}

VanillaVoid VanillaPWDestroyWindow(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		xcb_destroy_window(connection, PortWindow->xwindow);
		delete PortWindow;
	}
}

VanillaVoid VanillaPWSetWindowTitle(VanillaPWWindow PortWindow, VanillaString Title) {
	if (PortWindow) {
		xcb_change_property(connection,
			XCB_PROP_MODE_REPLACE,
			PortWindow->xwindow,
			XCB_ATOM_WM_NAME,
			XCB_ATOM_STRING,
			8,
			strlen(Title.c_str()),
			Title.c_str());
	}
}

VanillaString VanillaPWGetWindowTitle(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		xcb_generic_error_t *error;
		xcb_get_property_reply_t *reply = xcb_get_property_reply(connection, xcb_get_property(connection, false, PortWindow->xwindow, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, -1), &error);
		VanillaString Result((char*)xcb_get_property_value(reply), xcb_get_property_value_length(reply));
		free(reply);
		return Result;
	}
	return NULL;
}
VanillaVoid VanillaPWSetWindowVisible(VanillaPWWindow PortWindow, VanillaBool Visible) {
	if (PortWindow) {
		if (PortWindow->mapped && !Visible) {
			xcb_unmap_window(connection, PortWindow->xwindow);
		}
		if (!PortWindow->mapped && Visible) {
			xcb_map_window(connection, PortWindow->xwindow);
		}
		xcb_flush(connection);
	}
}

VanillaBool VanillaPWGetWindowVisible(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		return PortWindow->mapped;
	}
	return false;
}

VanillaVoid VanillaPWSetWindowComposite(VanillaPWWindow PortWindow, VanillaBool Composite) {
	if (PortWindow) {
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
	}
	return;
}

VanillaBool VanillaPWGetWindowComposite(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		return true;
	}
	return false;
}

VanillaVoid VanillaPWDragWindow(VanillaPWWindow PortWindow) {
	if (PortWindow) {
		return;
	}
	return;
}

VanillaVoid VanillaPWDestroyGraphicsOfWindowCachedInMemoey(VanillaGraphics Graphics) {
	if (Graphics) {
        cairo_surface_destroy(VanillaGraphicsGetPWGraphics(Graphics)->cairo_surface);
        delete VanillaGraphicsGetPWGraphics(Graphics);
        VanillaDestroyGraphics(Graphics);
	}
	return;
}

VanillaGraphics VanillaPWCreateGraphicsOfWindowCachedInMemoey(VanillaWindow Window) {
	if (Window) {
        VanillaGraphics Graphics = VanillaCreateGraphicsInMemory(Window->Rect.Width, Window->Rect.Height);

        VanillaGraphicsSetPWGraphics(Graphics, new VPWGraphics);
        VanillaGraphicsGetPWGraphics(Graphics)->cairo_surface = cairo_image_surface_create_for_data((unsigned char *)VanillaGraphicsGetPixels(Graphics), CAIRO_FORMAT_ARGB32, Window->Rect.Width, Window->Rect.Height, ((Window->Rect.Width * 32 + 15) / 16) * 2);
        return Graphics;
	}
	return NULL;
}

VanillaVoid VanillaPWUpdateWindow(VanillaWindow Window, VanillaRect UpdateRect) {
	if (Window) {
        VRect _UpdateRect;
        if (NULL == UpdateRect) {
			    _UpdateRect = Window->Rect;
				_UpdateRect.Left = 0;
				_UpdateRect.Top = 0;
				UpdateRect = &_UpdateRect;
        }
        cairo_surface_flush(VanillaGraphicsGetPWGraphics(Window->GraphicsWindow)->cairo_surface);
        cairo_save(Window->PortWindow->cairo);
        cairo_set_source_surface(Window->PortWindow->cairo, VanillaGraphicsGetPWGraphics(Window->GraphicsWindow)->cairo_surface, 0, 0);
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
	}
}

VanillaInt VanillaPWMessageLoop() {
    xcb_generic_event_t *e;
    while ((e = xcb_wait_for_event(connection))) {
        VanillaPWLinuxWindowProc(e);
        free (e);
    }
    //xcb_flush(connection);
    //pause();
	return 0;
}

VanillaInt VanillaPWGetTickCount() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

/**
* 移植层的消息回调 来自操作系统的消息通知由此函数分发处理
*/
VanillaVoid VanillaPWLinuxWindowProc(xcb_generic_event_t *event) {
#define REAL_WINDOW(evt) Window = ((VanillaPWWindow)WindowHashTable [evt])->Window
    VanillaWindow Window;
    switch(event->response_type & ~0x80) {
            case XCB_EXPOSE: {
            xcb_expose_event_t *expose = (xcb_expose_event_t *)event;
            REAL_WINDOW(expose->window);
            VRect UpdateRect(expose->x, expose->y, expose->width, expose->height);
            VanillaPWUpdateWindow(Window, &UpdateRect);
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
                VanillaDispatchMouseMessage(Window, 1, ButtonCode [bp->detail - 1], bp->event_x, bp->event_y);
                break;
            }
            break;
        }
        case XCB_BUTTON_RELEASE: {
            xcb_button_release_event_t *br = (xcb_button_release_event_t *)event;
            REAL_WINDOW(br->event);
            if (br->detail >= 1 && br->detail <= 3) {
                VanillaInt ButtonCode[] = { 0, 2, 1 };
                VanillaDispatchMouseMessage(Window, 2, ButtonCode [br->detail - 1], br->event_x, br->event_y);
            }
            break;
        }
        case XCB_MOTION_NOTIFY: {
            xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
            REAL_WINDOW(motion->event);
            VanillaDispatchMouseMessage(Window, -1, -1, motion->event_x, motion->event_y);
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
