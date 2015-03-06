#pragma once

#define VWS_POSMIDDLE		1   //居中
#define VWS_CTLBTN_LEFT		256
#define VWS_CTLBTN_RIGHT	512
#define VWS_CTLBTN_CLOSE	2   //控制按钮_关闭
#define VWS_CATBTN_MAX		4   //控制按钮_最大化
#define VWS_CTLBTN_MIN		8   //控制按钮_最小化
#define VWS_NOTASKBAR		16  //不在任务栏显示
#define VWS_TITLE		32  //标题栏
#define VWS_DRAG_TITLE		0   //拖动标题栏(默认)
#define VWS_DRAG_ANY		128 //拖动任意
#define VWS_DRAG_NO		64  //拖动禁止


#define VBT_LEFTTOP		1  //居左上
#define VBT_TENSILE		2  //拉伸
#define VBT_NOCOLORFILL		4  //无填充颜色

#define VWFS_RECT	0
#define VWFS_ROUNDRECT	1
#define VWFS_CUSTOM	2




typedef struct VWindowDefaultControl
{
	VanillaControl Control;
	VanillaWindow Window;
	VanillaInt ID;
} *VanillaWindowDefaultControl, _VWindowDefaultControl;

#define VWDC_ROOT  0
#define VWDC_TITLE 1
#define VWDC_ICON  2


typedef struct VWindow
{
	VWindow() {}

	VanillaPortWindow PortWindow;
	VRect Rect;
	VanillaByte Alpha;
	VanillaTaskQueue TaskQueue;
	VanillaInt Shape;
	VanillaInt ShadowColor;
	VanillaInt DragType;

	VanillaGraphics GraphicsBackground;
	VanillaGraphics GraphicsWindow;

	VanillaImage BackgroundImage;
	VanillaColor BackgroundColor;
	VanillaInt BackgroundType;

	VWindowDefaultControl RootControl;
	VWindowDefaultControl Title;

	VanillaControl ButtonDownControl [3];
	VanillaControl MouseInControl;
	VanillaControl FocusControl;
	VanillaControl CaptureControl;


	VanillaBool FirstShow;
} *VanillaWindow, _VWindow;

VAPI(VanillaWindow) VanillaCreateWindow(VanillaRect Rect,
	VanillaInt WndStyle,
	VanillaText Title,
	VanillaStringFormat StringFormat,
	VanillaImage BackgroundImage,
	VanillaColor BackgroundColor,
	VanillaInt BackgroundType,
	VanillaInt Shape
	);
VAPI(VanillaVoid) VanillaDestroyWindow(VanillaWindow Window);
VAPI(VanillaVoid) VanillaSetWindowUserProc(VanillaWindow Window, VCtlEventProc UserProc);
VAPI(VanillaVoid) VanillaSetWindowVisible(VanillaWindow Window, VanillaBool Visible);
VAPI(VanillaBool) VanillaGetWindowVisible(VanillaWindow Window);
VAPI(VanillaControl) VanillaGetWindowRootControl(VanillaWindow Window);
VAPI(VanillaVoid) VanillaSetWindowBkg(VanillaWindow Window, VanillaImage BackkgroundImage, VanillaColor BackkgroundColor, VanillaInt BackkgroundType);
VAPI(VanillaVoid) VanillaSetWindowShape(VanillaWindow Window, VanillaInt Shape);
VAPI(VanillaInt) VanillaGetWindowShape(VanillaWindow Window);
VAPI(VanillaVoid) VanillaSetWindowShadowColor(VanillaWindow Window, VanillaInt ShadowColor);
VAPI(VanillaInt) VanillaGetWindowShadowColor(VanillaWindow Window);
VAPI(VanillaVoid) VanillaRedrawWindowMDC(VanillaWindow Window);
VAPI(VanillaVoid) VanillaRedrawWindow(VanillaWindow Window);
VAPI(VanillaVoid) VanillaFlashWindow(VanillaWindow Window);
VAPI(VanillaVoid) VanillaSetWindowComposite(VanillaWindow Window, VanillaBool Composite);
VAPI(VanillaBool) VanillaGetWindowComposite(VanillaWindow Window);
VAPI(VanillaVoid) VanillaSetWindowAlpha(VanillaWindow Window, VanillaByte Alpha);
VAPI(VanillaByte) VanillaGetWindowAlpha(VanillaWindow Window);



VanillaVoid VanillaWindowInitGraphics(VanillaWindow Window, VanillaBool ForceRecreate);
VanillaVoid VanillaWindowDrawWindow(VanillaWindow Window, VanillaGraphics Graphics);
VanillaVoid VanillaWindowDrawBackground(VanillaWindow Window, VanillaGraphics Graphics);
VanillaVoid VanillaWindowDrawBackgroundImage(VanillaWindow Window, VanillaGraphics Graphics);
VanillaVoid VanillaWindowDrawControl(VanillaWindow Window);
VanillaVoid VanillaWindowUpdate(VanillaWindow Window, VanillaRect UpdateRect);
VanillaVoid VanillaWindowUpdateGraphicsRect(VanillaWindow Window, VanillaRect UpdateRect, VanillaBool ForceRedraw, VanillaBool Flash);

VanillaInt VanillaWindowDefaultControlsProc(VanillaInt ID, VanillaInt Message, VanillaInt Param1, VanillaInt Param2);
