#ifndef __VANILLA_PORT_WINDOW_H__
#define __VANILLA_PORT_WINDOW_H__

/**
* 此函数用作初始化Window移植层.
* @param Returns 成功返回true.
*/
VanillaBool VanillaPWInitialize();
/**
* 此函数用作创建VanillaPWWindow对象.
* @param Rect 窗口矩形
* @param Title 窗口标题
* @param ShowInTaskbar 是否在任务栏显示
* @param PosMiddle 是否居中
* @param Window 框架层VanillaWindow指针
* @param Returns 成功返回VanillaPWWindow对象,不成功返回NULL.
*/
VanillaPWWindow VanillaPWCreateWindow(VanillaRect Rect, VanillaString Title, VanillaBool ShowInTaskbar, VanillaBool PosMiddle, VanillaWindow Window);
/**
* 此函数用作销毁由VanillaPWCreateWindow创建的对象.
* @param PortWindow VanillaPWWindow对象
* @此函数没有返回值.
*/
VanillaVoid VanillaPWDestroyWindow(VanillaPWWindow PortWindow);
/**
* 此函数用作设置窗口标题.
* @param PortWindow VanillaPWWindow对象
* @param Title 标题
* @此函数没有返回值.
*/
VanillaVoid VanillaPWSetWindowTitle(VanillaPWWindow PortWindow, VanillaString Title);
/**
* 此函数用作获取窗口标题.
* @param PortWindow VanillaPWWindow对象
* @param Returns 当前窗口标题.
*/
VanillaString VanillaPWGetWindowTitle(VanillaPWWindow PortWindow);
/**
* 此函数用作设置窗口可视状态.
* @param PortWindow VanillaPWWindow对象
* @param Visible 可视状态
* @此函数没有返回值.
*/
VanillaVoid VanillaPWSetWindowVisible(VanillaPWWindow PortWindow, VanillaBool Visible);
/**
* 此函数用作获取窗口当前可视状态.
* @param PortWindow VanillaPWWindow对象
* @param Returns 当前窗口可视状态.
*/
VanillaBool VanillaPWGetWindowVisible(VanillaPWWindow PortWindow);
/**
* 此函数用作设置是否开启混合效果.
* @param PortWindow VanillaPWWindow对象
* @param Composite true or false
* @此函数没有返回值.
*/
VanillaVoid VanillaPWSetWindowComposite(VanillaPWWindow PortWindow, VanillaBool Composite);
/**
* 此函数用作获取是否开启混合效果.
* @param PortWindow VanillaPWWindow对象
* @param Returns 当前混合状态.
*/
VanillaBool VanillaPWGetWindowComposite(VanillaPWWindow PortWindow);
/**
* 此函数用作拖动窗口.
* @param PortWindow VanillaPWWindow对象
* @此函数没有返回值.
*/
VanillaVoid VanillaPWDragWindow(VanillaPWWindow PortWindow);
VanillaVoid VanillaPWDestroyGraphicsOfWindowCachedInMemoey(VanillaGraphics Graphics);
VanillaGraphics VanillaPWCreateGraphicsOfWindowCachedInMemoey(VanillaWindow Window);
/**
* 此函数用作刷新窗口显示.
* @param PortWindow VanillaPWWindow对象
* @param UpdateRect 刷新区域
* @此函数没有返回值.
*/
VanillaVoid VanillaPWUpdateWindow(VanillaWindow Window, VanillaRect UpdateRect);
/**
* 此函数用作移植层的消息循环.
* @param Returns int.
*/
VanillaInt VanillaPWMessageLoop();
/**
* 此函数用作处理鼠标消息.
* @param Window 消息对应的Window
* @param Action 消息对应的行为
* @param Button 消息对应的按钮
* @param x
* @param y
* @param Returns int.
*/
VanillaControl VanillaPWDispatchMouseMessage(VanillaWindow Window, VanillaInt Action, VanillaInt Button, VanillaInt x, VanillaInt y);
#endif	//__VANILLA_PORT_WINDOW_H__
