#ifndef __VANILLA_BACKEND_GRAPHICS_H__
#define __VANILLA_BACKEND_GRAPHICS_H__

inline VanillaVoid VanillaGraphicsSetPWGraphics(VanillaGraphics Graphics, VanillaPWGraphics PWGraphics) {
	*(VanillaPWGraphics*)Graphics = PWGraphics;
}

inline VanillaPWGraphics VanillaGraphicsGetPWGraphics(VanillaGraphics Graphics) {
	return *(VanillaPWGraphics*)Graphics;
}

VAPI(VanillaAny) VanillaGraphicsGetPixels(VanillaGraphics Graphics);
/**
* 此函数用作填充一个矩形区域.
* @param Graphics VanillaGraphics对象
* @param Color 用作填充的颜色
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaFillRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height);
/**
* 此函数用作绘制并填充一个矩形.
* @param Graphics VanillaGraphics对象
* @param Color 用作填充的颜色
* @param BorderColor 用作描边的颜色
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @param LineWidth 线宽
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawFilledRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth);
/**
* 此函数用作绘制一个矩形.
* @param Graphics VanillaGraphics对象
* @param Color 用作填充的颜色
* @param Left 左边
* @param Top 顶边
* @param Width 宽度
* @param Height 高度
* @param LineWidth 线宽
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt Left, VanillaInt Top, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth);
/**
* 此函数用作绘制文本.
* @param Graphics VanillaGraphics对象
* @param StringFormat 文本格式
* @param String 文本
* @param Rect 矩形区域
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawString(VanillaGraphics Graphics, VanillaStringFormat StringFormat, VanillaText String, VanillaRect Rect);
/**
* 此函数用作绘制并填充圆角矩形.
* @param Graphics VanillaGraphics对象
* @param Color 用作填充的颜色
* @param BorderColor 用作描边的颜色
* @param x
* @param y
* @param Width
* @param Height
* @param LineWidth
* @param Round
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawFilledRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaColor BorderColor, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round);
/**
* 此函数用作绘制圆角矩形.
* @param Graphics VanillaGraphics对象
* @param Color 颜色
* @param x
* @param y
* @param Width
* @param Height
* @param LineWidth
* @param Round
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt LineWidth, VanillaReal Round);
/**
* 此函数用作填充圆角矩形.
* @param Graphics VanillaGraphics对象
* @param Color 颜色
* @param x
* @param y
* @param Width
* @param Height
* @param Round
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaFillRoundRect(VanillaGraphics Graphics, VanillaColor Color, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaReal Round);
/**
* 此函数用作绘制图像.
* @param Graphics VanillaGraphics对象
* @param Image VanillaImage对象
* @param x
* @param y
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawImage(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y);
/**
* 此函数用作绘制图像(扩展).
* @param Graphics VanillaGraphics对象
* @param Image VanillaImage对象
* @param x 左边
* @param y 顶边
* @param Width 宽度
* @param Height 高度
* @param xSrc 源坐标
* @param ySrc 源顶边
* @param WidthSrc 源宽度
* @param HeightSrc 源高度
* @param Alpha 透明度
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDrawImageEx(VanillaGraphics Graphics, VanillaImage Image, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha);
VAPI(VanillaVoid) VanillaAlphaBlendEx(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaInt WidthSrc, VanillaInt HeightSrc, VanillaByte Alpha);
VAPI(VanillaVoid) VanillaAlphaBlend(VanillaGraphics Dest, VanillaInt x, VanillaInt y, VanillaInt Width, VanillaInt Height, VanillaGraphics Src, VanillaInt xSrc, VanillaInt ySrc, VanillaByte Alpha);
/**
* 此函数用作使用指定颜色清空VanillaGraphics对象.
* @param Graphics VanillaGraphics对象
* @param Color 颜色
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaGraphicsClear(VanillaGraphics Graphics, VanillaColor Color);
/**
* 此函数用作在内存中创建VanillaGraphics对象.
* @param Width 宽度
* @param Height 高度
* @param Returns 成功返回创建的VanillaGraphics对象.
*/
VAPI(VanillaGraphics) VanillaCreateGraphicsInMemory(VanillaInt Width, VanillaInt Height);
/**
* 此函数用作使用一个已有的像素数据地址来创建VanillaGraphics对象
* @param Pixels 像素地址
* @param Width 宽度
* @param Height 高度
* @param Returns 成功返回创建的VanillaGraphics对象.
*/
VAPI(VanillaGraphics) VanillaCreateGraphicsFromPixelAddress(VanillaAny Pixels, VanillaInt Width, VanillaInt Height);
/**
* 此函数用作销毁VanillaGraphics对象.
* @param Graphics VanillaGraphics对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDestroyGraphics(VanillaGraphics Graphics);
#endif
