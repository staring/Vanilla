// Vanilla Controls
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VControl.h"
#include "VDrawing.h"
#include "VGlobalFunction.h"

std::map<VanillaString, VanillaControlClass> ControlClasses;
VCtlEventProc DefEventProc = NULL;

VAPI(VanillaControlClass) VanillaRegisterControlClass(VanillaText ClassName, VCtlProc CtlProc, VanillaBool Focusable, VanillaBool Virtual) {
	VanillaControlClass Class;
	if ((Class = ControlClasses[ClassName]) != NULL) {
		return Class;
	}
	Class = new VControlClass;
	Class->ClassName = ClassName;
	Class->CtlProc = CtlProc;
	Class->Focusable = Focusable;
	Class->Virtual = Virtual;
	ControlClasses [ClassName] = Class;
	return Class;
}

VAPI(VanillaControl) VanillaControlCreate(VanillaControl ParentControl, VanillaText ClassName, VanillaRect Rect, VanillaAny ControlData, VanillaInt CustomID, VanillaBool Visible, VanillaBool Enabled, VanillaAny CreateStruct) {
	VanillaControlClass Class = ControlClasses [ClassName];
	if (ControlClasses [ClassName] == NULL) {
		return NULL;
	}
	if (ParentControl == NULL) {
		return NULL;
	}
	bool RootControl;
	if ((VanillaInt)ParentControl < 0) {
		RootControl = true;
	} else {
		RootControl = false;
	}

	VanillaControl Control = new VControl;
	Control->Class = Class;
	Control->Alpha = 255;
	Control->BindOwner = NULL;
	Control->Window = RootControl ? ((VanillaWindow)(- (VanillaInt)ParentControl)) : ParentControl->Window;
	Control->CtlProc = NULL;
	Control->Enabled = Enabled;
	Control->EventProc = NULL;
	Control->GradientAlpha = 0;
	Control->Gradienting = false;
	Control->GradientType = false;
	Control->GradientUserData = NULL;
	Control->ControlData = ControlData;
	Control->ID = (CustomID == 0) ? (VanillaInt)Control : CustomID;
	Control->LastControl = NULL;
	Control->NextControl = NULL;
	Control->ChildControlFirst = NULL;
	Control->ChildControlEnd = NULL;
	Control->Graphics = NULL;
	Control->Graphics_Gradient1 = NULL;
	Control->Graphics_Gradient2 = NULL;
	Control->Visible = Visible;
	Control->MousePenetration = false;
	Control->Rect = *Rect;
	Control->ParentControl = RootControl ? NULL : ParentControl;
	if (!RootControl) {
		if (Control->ParentControl->ChildControlEnd != NULL) {
			Control->ParentControl->ChildControlEnd->NextControl = Control;
			Control->LastControl = Control->ParentControl->ChildControlEnd;
		}
		Control->ParentControl->ChildControlEnd = Control;
		if (Control->ParentControl->ChildControlFirst == NULL) {
			Control->ParentControl->ChildControlFirst = Control;
		}
	}
	Control->DisabledCount = Control->ParentControl ? (Control->ParentControl->DisabledCount + ((Control->ParentControl->Enabled) ? 0 : 1)) : 0;
	Control->InvisibleCount = Control->ParentControl ? (Control->ParentControl->InvisibleCount + ((Control->ParentControl->Visible) ? 0 : 1)) : 0;
	if (!(RootControl || Class->Virtual)) {
		Control->Graphics = VanillaCreateGraphicsInMemory(Rect->Width, Rect->Height);
	} else {
		Control->Graphics = NULL;
	}
	VanillaControlSendMessage(Control, VM_CREATE, NULL, (VanillaInt)CreateStruct);
	return Control;
}

VAPI(VanillaInt) VanillaControlDestroy(VanillaControl Control) {
	if (Control == NULL) {
		return 0;
	}
	VanillaControl ThisControl;
	VanillaControl NextControl = Control->ChildControlFirst;
	VanillaInt i = 0;
	while (NextControl != NULL) {
		ThisControl = NextControl;
		NextControl = NextControl->NextControl;
		i += VanillaControlDestroy(ThisControl);
	}
	VanillaControlSendMessage(Control, VM_DESTROY, 0, 0);
	VanillaDestroyGraphics(Control->Graphics);
	VanillaDestroyGraphics(Control->Graphics_Gradient1);
	VanillaDestroyGraphics(Control->Graphics_Gradient2);
	if (Control->ParentControl) {
		if (Control->ParentControl->ChildControlFirst == Control) {
			Control->ParentControl->ChildControlFirst = Control->NextControl;
		}
		if (Control->ParentControl->ChildControlEnd == Control) {
			Control->ParentControl->ChildControlEnd = Control->LastControl;
		}
		if (Control->LastControl != NULL) {
			Control->LastControl->NextControl = Control->NextControl;
		}
		if (Control->NextControl != NULL) {
			Control->NextControl->LastControl = Control->LastControl;
		}
	}
	if (Control->Window->ButtonDownControl[0] == Control) {
		Control->Window->ButtonDownControl[0] = NULL;
	}
	if (Control->Window->ButtonDownControl[1] == Control) {
		Control->Window->ButtonDownControl[1] = NULL;
	}
	if (Control->Window->ButtonDownControl[2] == Control) {
		Control->Window->ButtonDownControl[2] = NULL;
	}
	if (Control->Window->CaptureControl == Control) {
		Control->Window->CaptureControl = NULL;
	}
	if (Control->Window->FocusControl == Control) {
		Control->Window->FocusControl = NULL;
	}
	if (Control->Window->MouseInControl == Control) {
		Control->Window->MouseInControl = NULL;
	}
	delete Control;
	return i + 1;
}

VAPI(VanillaInt) VanillaControlRedraw(VanillaControl Control, VanillaBool Update) {
	return VanillaControlSendMessage(Control, VM_REDRAW, (VanillaInt)Update, NULL);
}

VAPI(VanillaVoid) VanillaControlSetEnable(VanillaControl Control, VanillaBool Enabled) {
	if (Control->Enabled != Enabled) {
		VanillaControlSendMessageToChild(Control, VM_SETENABLED, NULL, Enabled ? -1 : 1);
		Control->Enabled = Enabled;
	}
}

VAPI(VanillaBool) VanillaControlIsEnable(VanillaControl Control) {
	return Control->DisabledCount == 0 && Control->Enabled;
}

VAPI(VanillaVoid) VanillaControlSetVisible(VanillaControl Control, VanillaBool Visible) {
	if (Control->Visible != Visible) {
		VanillaControlSendMessageToChild(Control, VM_SETVISIBLE, NULL, Visible ? -1 : 1);
		Control->Visible = Visible;
	}
}

VAPI(VanillaBool) VanillaControlIsVisible(VanillaControl Control) {
	return Control->InvisibleCount == 0 && Control->Visible;
}

VAPI(VanillaVoid) VanillaControlSetAlpha(VanillaControl Control, VanillaByte Alpha) {
	Control->Alpha = Alpha;
}

VAPI(VanillaByte) VanillaControlGetAlpha(VanillaControl Control) {
	return Control->Alpha;
}

VAPI(VanillaVoid) VanillaControlSetDefaultEventProc(VCtlEventProc EventProc) {
	DefEventProc = EventProc;
}

VAPI(VanillaVoid) VanillaControlSetCtlProc(VanillaControl Control, VCtlProc CtlProc) {
	Control->CtlProc = CtlProc;
}

VAPI(VCtlProc) VanillaControlGetCtlProc(VanillaControl Control) {
	return Control->CtlProc;
}

VAPI(VanillaVoid) VanillaControlSetEventProc(VanillaControl Control, VCtlEventProc EventProc) {
	Control->EventProc = EventProc;
}

VAPI(VCtlEventProc) VanillaControlGetEventProc(VanillaControl Control) {
	return Control->EventProc;
}

VAPI(VanillaInt) VanillaControlTriggerEvent(VanillaControl Control, VanillaInt Event, VanillaInt Param1, VanillaInt Param2, VanillaInt Param3) {
	if (Control == NULL) {
		return NULL;
	}
	if (Control->EventProc != NULL) {
		return Control->EventProc(Control->Window, Control, Event, Param1, Param2, Param3);
	} else if (DefEventProc != NULL) {
		return DefEventProc(Control->Window, Control, Event, Param1, Param2, Param3);
	}
	return NULL;
}

VAPI(VanillaInt) VanillaControlSendMessage(VanillaControl Control, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	if (Control == NULL) {
		return NULL;
	}
	return VanillaDefaultControlProc(Control, Message, Param1, Param2);
}

VAPI(VanillaVoid) VanillaControlSendMessageToChild(VanillaControl ParentControl, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	VanillaControl Control = ParentControl->ChildControlEnd;
	while (Control != NULL) {
		VanillaControlSendMessage(Control, Message, Param1, Param2);
		VanillaControlSendMessageToChild(Control, Message, Param1, Param2);
		Control = Control->LastControl;
	}
}

VAPI(VanillaVoid) VanillaControlSendMessageToChildOfWindow(VanillaWindow Window, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	VanillaControl Control = VanillaGetWindowRootControl(Window)->ChildControlEnd;
	while (Control != NULL) {
		VanillaControlSendMessage(Control, Message, Param1, Param2);
		VanillaControlSendMessageToChild(Control, Message, Param1, Param2);
		Control = Control->LastControl;
	}
}

VAPI(VanillaVoid) VanillaControlMove(VanillaControl Control, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height) {
	VanillaBool Moved = false, Sized = false;
	if (Control->Rect.Left != Left || Control->Rect.Top != Top) {
		Moved = true;
	}
	if (Control->Rect.Width != Width || Control->Rect.Height != Height) {
		Sized = true;
	}
	VRect OldRect = Control->Rect;
	VRect OldRectOfWindow;
	VanillaControlGetRectOfWindow(Control, &OldRectOfWindow);
	new(&Control->Rect) VRect(Left, Top, Width, Height);
	if (Moved) {
		VanillaControlSendMessage(Control, VM_MOVE, NULL, (VanillaInt)&OldRect.Left);
	}
	if (Sized) {
		if (!Control->Class->Virtual) {
			VanillaDestroyGraphics(Control->Graphics);
			Control->Graphics = VanillaCreateGraphicsInMemory(Width, Height);
			VanillaControlSendMessage(Control, VM_REDRAW, NULL, NULL);
			VanillaWindowUpdateGraphicsRect(Control->Window, &OldRectOfWindow, false, false);
			VRect NewRectOfWindow;
			VanillaControlGetRectOfWindow(Control, &OldRectOfWindow);
			VanillaWindowUpdateGraphicsRect(Control->Window, &NewRectOfWindow, false, false);
		}
		VanillaControlSendMessage(Control, VM_SIZE, NULL, (VanillaInt)OldRect.Width);
	}
}

VAPI(VanillaRect) VanillaControlGetRect(VanillaControl Control) {
	return &Control->Rect;
}

VAPI(VanillaRect) VanillaControlGetRectOfWindow(VanillaControl Control, VanillaRect Rect) {
	*Rect = Control->Rect;
	VanillaControl ParentControl = Control->ParentControl;
	while (ParentControl) {
		Rect->Left += ParentControl->Rect.Left;
		Rect->Top += ParentControl->Rect.Top;
		ParentControl = ParentControl->ParentControl;
	}
	return Rect;
}

VAPI(VanillaControl) VanillaFindControlInWindow(VanillaWindow Window, VanillaPoint pt, VanillaPoint pt2) {
	VanillaControl Control = VanillaFindControlInControl(VanillaGetWindowRootControl(Window), pt, pt2);
	if (!Control) {
		*pt2 = *pt;
		return VanillaGetWindowRootControl(Window);
	}
	return Control;
}

VAPI(VanillaControl) VanillaFindControlInControl(VanillaControl ParentControl, VanillaPoint pt, VanillaPoint pt2) {
	VanillaControl Control = ParentControl->ChildControlEnd;
	while (Control != NULL) {
		if (!Control->Visible || !VanillaControlIsEnable(Control) || Control->MousePenetration) {
			Control = Control->LastControl;
			continue;
		}
		if (VanillaIsPointInRect(pt, &Control->Rect)) {
			pt2->x = pt->x - Control->Rect.Left;
			pt2->y = pt->y - Control->Rect.Top;
			VanillaControl SubControl = VanillaFindControlInControl(Control, pt2, pt2);
			if (SubControl == NULL) {
				return Control;
			} else {
				return SubControl;
			}
		}
		Control = Control->LastControl;
	}
	return NULL;
}

VanillaInt VanillaDefaultControlProc(VanillaControl Control, VanillaInt Message, VanillaInt Param1, VanillaInt Param2) {
	switch (Message) {
		case VM_SIZE: {
			if (Control->Class->Virtual) {
				return NULL;
			}
			VanillaDestroyGraphics(Control->Graphics_Gradient1);
			VanillaDestroyGraphics(Control->Graphics_Gradient2);
			VanillaDestroyGraphics(Control->Graphics);
			Control->Graphics = VanillaCreateGraphicsInMemory(Control->Rect.Width, Control->Rect.Height);
			VanillaControlSendMessage(Control, VM_REDRAW, 0, 0);
			break;
		}

		case VM_UPDATE: {
			if (Control->Class->Virtual) {
				return NULL;
			}
			VRect RectOfWindow;
			VanillaControlGetRectOfWindow(Control, &RectOfWindow);
			VanillaWindowUpdateGraphicsRect(Control->Window, &RectOfWindow, false, true);
			break;
		}

		case VM_REDRAW: {
			if (Control->Class->Virtual) {
				return NULL;
			}
			VanillaGraphicsClear(Control->Graphics, 0);
			VanillaInt Result = VanillaControlSendMessage(Control, VM_PAINT, 0, (VanillaInt)Control->Graphics);
#if (defined DEBUG_BORDER) && (DEBUG_BORDER == 1)
				VanillaDrawRect(Control->Graphics, ARGB(255, 0, 0, 0), 0, 0, Control->Rect.Width - 1, Control->Rect.Height - 1, 1);
#endif
			if (Param1) {
				VanillaControlSendMessage(Control, VM_UPDATE, 0, 0);
			}
			return Result;
			break;
		}

		case VM_SETENABLED: {
			if (Param2 != 0) {
				Control->DisabledCount = Control->DisabledCount - Param2;
				return NULL;
			}
			break;
		}

		case VM_SETVISIBLE: {
			if (Param2 != 0) {
				Control->InvisibleCount = Control->InvisibleCount - Param2;
				return NULL;
			}
			break;
		}
	}
	VanillaInt Result;
	if (Control->CtlProc == NULL) {
		Result = Control->Class->CtlProc(Control->ID, Message, Param1, Param2);
	} else {
		Result = Control->CtlProc(Control->ID, Message, Param1, Param2);
	}
	switch (Message) {
		case VM_LBUTTONDOWN: {
			VanillaControlTriggerEvent(Control, VE_LBTNDOWN, Param2, NULL, NULL);
			break;
		}

		case VM_MBUTTONDOWN: {
			VanillaControlTriggerEvent(Control, VE_MBTNDOWN, Param2, NULL, NULL);
			break;
		}

		case VM_RBUTTONDOWN: {
			VanillaControlTriggerEvent(Control, VE_RBTNDOWN, Param2, NULL, NULL);
			break;
		}

		case VM_LBUTTONUP: {
			VanillaControlTriggerEvent(Control, VE_LBTNUP, Param2, NULL, NULL);
			break;
		}

		case VM_MBUTTONUP: {
			VanillaControlTriggerEvent(Control, VE_MBTNUP, Param2, NULL, NULL);
			break;
		}

		case VM_RBUTTONUP: {
			VanillaControlTriggerEvent(Control, VE_RBTNUP, Param2, NULL, NULL);
			break;
		}

		case VM_LBUTTONDBCLK: {
			VanillaControlTriggerEvent(Control, VE_LBTNDBCLK, Param2, NULL, NULL);
			break;
		}

		case VM_MBUTTONDBCLK: {
			VanillaControlTriggerEvent(Control, VE_MBTNDBCLK, Param2, NULL, NULL);
			break;
		}

		case VM_RBUTTONDBCLK: {
			VanillaControlTriggerEvent(Control, VE_RBTNDBCLK, Param2, NULL, NULL);
			break;
		}

		case VM_LBUTTONCLK: {
			VanillaControlTriggerEvent(Control, VE_LBTNCLK, Param2, NULL, NULL);
			break;
		}

		case VM_MBUTTONCLK: {
			VanillaControlTriggerEvent(Control, VE_MBTNCLK, Param2, NULL, NULL);
			break;
		}

		case VM_RBUTTONCLK: {
			VanillaControlTriggerEvent(Control, VE_RBTNCLK, Param2, NULL, NULL);
			break;
		}
	}
	return Result;
}
