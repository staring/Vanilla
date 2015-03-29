#ifndef __VANILLA_CORE_GLOBALFUNCTION_H__
#define __VANILLA_CORE_GLOBALFUNCTION_H__
// Normal Global Functions
VanillaBool VInit ();

VanillaBool VInit_WndMsg ();
VanillaBool VInit_WindowClass();
VanillaBool VInit_ControlClass();

/**
* 此函数用作Vanilla消息循环.
* @param Returns int.
*/
VAPI(VanillaInt) VanillaMsgLoop();
/**
* 此函数用作Vanilla初始化.
* @param Returns 返回true初始化成功.
*/
VAPI(VanillaBool) VanillaInit();
/**
* 此函数用作读取一个二进制文件的数据到VanillaBin结构中.
* @param FileName 文件名
* @param Returns 成功返回VanillaBin对象.
*/
VAPI(VanillaBin) VanillaReadFileToBin(VanillaText FileName);
/**
* 此函数用作检测某点是否存在与一个矩形之内.
* @param x 欲检测的点
* @param y
* @param Rect 用作检测的矩形
* @param Returns 在或不在.
*/
VAPI(VanillaBool) VanillaIsPointInRect(VanillaInt x, VanillaInt y, VanillaRect Rect);
/**
* 此函数用作计算两个矩形的交.
* @param Rect1 源矩形
* @param Rect2 源矩形
* @param RectResult 两个源矩形相交的矩形
* @param Returns 如果交不为空，则返回true；否则，如果交为空则返回false.
*/
VAPI(VanillaBool) VanillaIntersectRect(VanillaRect Rect1, VanillaRect Rect2, VanillaRect RectResult);

#define VANILLA_ICONV std::string _VanillaPort_lpu;char* _VanillaPort_lpu_;int _VanillaPort_lpu_len;std::wstring _VanillaPort_lpw;wchar_t* _VanillaPort_lpw_;int _VanillaPort_lpw_len;

#define VANILLA_U2W(VanillaPort_lpu) ((_VanillaPort_lpu = (VanillaPort_lpu),_VanillaPort_lpu_=(char*)(_VanillaPort_lpu.c_str()),_VanillaPort_lpu_len = strlen(_VanillaPort_lpu_),_VanillaPort_lpw_len = VanillaPortUTF8ToUTF16(NULL, NULL, _VanillaPort_lpu_, _VanillaPort_lpu_len) + 1,_VanillaPort_lpw_ = (wchar_t*)malloc(_VanillaPort_lpw_len * 2),VanillaPortUTF8ToUTF16(_VanillaPort_lpw_, _VanillaPort_lpw_len, _VanillaPort_lpu_, _VanillaPort_lpu_len),_VanillaPort_lpw=_VanillaPort_lpw_,free(_VanillaPort_lpw_),_VanillaPort_lpw))
#define VANILLA_W2U(VanillaPort_lpw) ((_VanillaPort_lpw = (VanillaPort_lpw),_VanillaPort_lpw_=(wchar_t*)(_VanillaPort_lpw.c_str()),_VanillaPort_lpw_len = wcslen(_VanillaPort_lpw_),_VanillaPort_lpu_len = VanillaPortUTF16ToUTF8(NULL, NULL, _VanillaPort_lpw_, _VanillaPort_lpw_len) + 1,_VanillaPort_lpu_ = (char*)malloc(_VanillaPort_lpu_len),VanillaPortUTF16ToUTF8(_VanillaPort_lpu_, _VanillaPort_lpu_len, _VanillaPort_lpw_, _VanillaPort_lpw_len),_VanillaPort_lpu=_VanillaPort_lpu_,free(_VanillaPort_lpu_),_VanillaPort_lpu))

VAPI(int) VanillaUTF16ToUTF8(char* Output, int OutputSize, wchar_t* Input, int InputLength);
VAPI(int) VanillaUTF8ToUTF16(wchar_t* Output, int OutputSize, char* Input, int Inputsize);
#endif	//__VANILLA_CORE_GLOBALFUNCTION_H__
