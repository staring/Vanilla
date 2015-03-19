#ifndef __VWINDOW_H__
#define __VWINDOW_H__

#define VWS_POSMIDDLE		1   //居中
#define VWS_CTLBTN_LEFT		256	//
#define VWS_CTLBTN_RIGHT	512	//
#define VWS_CTLBTN_CLOSE	2   //控制按钮_关闭
#define VWS_CATBTN_MAX		4   //控制按钮_最大化
#define VWS_CTLBTN_MIN		8   //控制按钮_最小化
#define VWS_NOTASKBAR		16  //不在任务栏显示
#define VWS_TITLE			32  //标题栏
#define VWS_DRAG_TITLE		0   //拖动标题栏(默认)
#define VWS_DRAG_ANY		128 //拖动任意
#define VWS_DRAG_NO			64  //拖动禁止


#define VBT_LEFTTOP			1	//居左上
#define VBT_TENSILE			2	//拉伸
#define VBT_NOCOLORFILL		4	//无填充颜色

#define VWFS_RECT			0	//矩形
#define VWFS_ROUNDRECT		1	//圆角矩形
#define VWFS_CUSTOM			2	//




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

	VanillaPortWindow PortWindow;			//移植层窗口
	VRect Rect;								//窗口矩形
	VanillaByte Alpha;						//透明度
	VanillaTaskQueue TaskQueue;				//任务列队
	VanillaInt Shape;						//形状
	VanillaInt ShadowColor;					//
	VanillaInt DragType;					//

	VanillaGraphics GraphicsBackground;		//背景图形
	VanillaGraphics GraphicsWindow;			//窗口图形

	VanillaImage BackgroundImage;			//背景图片
	VanillaColor BackgroundColor;			//背景颜色
	VanillaInt BackgroundType;				//背景类型

	VWindowDefaultControl RootControl;
	VWindowDefaultControl Title;			//标题控件

	VanillaControl ButtonDownControl [3];	//
	VanillaControl MouseInControl;			//
	VanillaControl FocusControl;			//焦点控件
	VanillaControl CaptureControl;			


	VanillaBool FirstShow;					//可视
} *VanillaWindow, _VWindow;
/*
* 此函数用作创建VanillaWindow对象.
* @param WndStyle 窗口风格
* @param Title 窗口标题
* @param StringFormat 文本格式
* @param BackgroundImage 背景图像
* @param BackgroundColor 背景颜色
* @param BackgroundType 背景类型
* @param Shape 形状 VWFS_开头的常量
* @param Returns 成功返回VanillaWindow对象,不成功返回NULL.
*/
VAPI(VanillaWindow) VanillaCreateWindow(VanillaRect Rect,
	VanillaInt WndStyle,
	VanillaText Title,
	VanillaStringFormat StringFormat,
	VanillaImage BackgroundImage,
	VanillaColor BackgroundColor,
	VanillaInt BackgroundType,
	VanillaInt Shape
	);
/*
* 此函数用作销毁VanillaWindow对象.
* @param Window VanillaWindow对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDestroyWindow(VanillaWindow Window);
/*
* 此函数用作设置用户回调.
* @param Window VanillaWindow对象
* @param UserProc 用户回调函数
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowUserProc(VanillaWindow Window, VCtlEventProc UserProc);
/*
* 此函数用作设置窗口可视状态.
* @param Window VanillaWindow对象
* @param Visible 是否可视
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowVisible(VanillaWindow Window, VanillaBool Visible);
/*
* 此函数用作获取窗口当前的可视状态.
* @param Window VanillaWindow对象
* @param Returns 当前的可视状态.
*/
VAPI(VanillaBool) VanillaGetWindowVisible(VanillaWindow Window);
VAPI(VanillaControl) VanillaGetWindowRootControl(VanillaWindow Window);
/*
* 此函数用作设置窗口背景.
* @param Window VanillaWindow对象
* @param BackkgroundImage 背景图片
* @param BackkgroundColor 背景颜色
* @param BackkgroundType 背景类型
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowBkg(VanillaWindow Window, VanillaImage BackkgroundImage, VanillaColor BackkgroundColor, VanillaInt BackkgroundType);
/*
* 此函数用作设置窗口形状.
* @param Window VanillaWindow对象
* @param Shape 形状 VWFS_开头的常量
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaSetWindowShape(VanillaWindow Window, VanillaInt Shape);
/*
* 此函数用作获取窗口当前形状.
* @param Window VanillaWindow对象
* @param Returns 返回当前形状.
*/
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
#endif	//__VWINDOW_H__
