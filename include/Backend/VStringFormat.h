#ifndef __VANILLA_BACKEND_STRINGFORMAT_H__
#define __VANILLA_BACKEND_STRINGFORMAT_H__


#define VTS_BOLD 1				//粗体
#define VTS_ITALIC 2			//斜体
#define VTS_ALIGN_LEFT 0		//水平居左
#define VTS_ALIGN_CENTER 4		//水平居中
#define VTS_ALIGN_RIGHT 8		//水平居右
#define VTS_VALIGN_TOP 0		//垂直顶部对齐
#define VTS_VALIGN_MIDDLE 16	//垂直居中对齐
#define VTS_VALIGN_BOTTOM 32	//垂直底部对齐
#define VTS_SHADOW 64

/**
* 此函数用作创建VanillaStringFormat(文本格式)对象.
* @param FontName 字体名称
* @param FontSize 字体大小
* @param TextColor 字体颜色
* @param ShadowColor 阴影颜色
* @param StringStyle 字体风格 VTS_开头的常量
* @param Returns 成功返回VanillaStringFormat对象.
*/
VAPI(VanillaStringFormat) VanillaCreateStringFormat(VanillaText FontName, VanillaReal FontSize, VanillaColor TextColor, VanillaColor ShadowColor, VanillaInt StringStyle);
/**
* 此函数用作销毁VanillaStringFormat对象.
* @param StringFormat VanillaStringFormat对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDestroyStringFormat(VanillaStringFormat StringFormat);
#endif
