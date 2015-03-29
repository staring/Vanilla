#ifndef __VANILLA_BACKEND_IMAGE_H__
#define __VANILLA_BACKEND_IMAGE_H__
/**
* 此函数用作从文件载入图像.
* @param Graphics VanillaGraphics对象
* @param FileName 路径
* @param Returns 成功返回VanillaImage对象.
*/
VAPI(VanillaImage) VanillaLoadImageFromFile(VanillaText FileName);
/**
* 此函数用作从字节集载入图像.
* @param Graphics VanillaGraphics对象
* @param Binary 字节集
* @param Returns 成功返回VanillaImage对象.
*/
VAPI(VanillaImage) VanillaLoadImageFromBinary(VanillaBin Binary);
/**
* 此函数用作销毁VanillaImage对象.
* @param Image VanillaImage对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDestroyImage(VanillaImage Image);
#endif	//__VANILLA_BACKEND_IMAGE_H__
