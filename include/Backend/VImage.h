#ifndef __VANILLA_BACKEND_IMAGE_H__
#define __VANILLA_BACKEND_IMAGE_H__

enum VanillaImageFormat {
	ImageFormatPNG,
	ImageFormatJPEG
};

/**
* 此函数用作保存图像到文件.
* @param Graphics VanillaImage对象
* @param FileName 文件名
* @param VanillaImageFormat 图像格式
* @param Returns 成功返回true.
*/
VAPI(VanillaBool) VanillaSaveImageToFile(VanillaImage Image, VanillaText FileName, VanillaImageFormat ImageFormat);

/**
* 此函数用作保存图像到字节集.
* @param Graphics VanillaImage对象
* @param VanillaImageFormat 图像格式
* @param Returns 成功返回VanillaBinary对象.
*/
VAPI(VanillaBinary) VanillaSaveImageToBinary(VanillaImage Image, VanillaImageFormat ImageFormat);

/**
* 此函数用作从文件载入图像.
* @param FileName 文件名
* @param Returns 成功返回VanillaImage对象.
*/
VAPI(VanillaImage) VanillaLoadImageFromFile(VanillaText FileName);

/**
* 此函数用作从字节集载入图像.
* @param Binary 字节集
* @param Returns 成功返回VanillaImage对象.
*/
VAPI(VanillaImage) VanillaLoadImageFromBinary(VanillaBinary Binary);

/**
* 此函数用作销毁VanillaImage对象.
* @param Image VanillaImage对象
* @此函数没有返回值.
*/
VAPI(VanillaVoid) VanillaDestroyImage(VanillaImage Image);

#endif	//__VANILLA_BACKEND_IMAGE_H__
