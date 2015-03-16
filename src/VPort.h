#ifndef __VPORT_H__
#define __VPORT_H__

#define VanillaPort_ICONV std::string _VanillaPort_lpu;char* _VanillaPort_lpu_;int _VanillaPort_lpu_len;std::wstring _VanillaPort_lpw;wchar_t* _VanillaPort_lpw_;int _VanillaPort_lpw_len;

#define VanillaPort_U2W(VanillaPort_lpu) ((_VanillaPort_lpu = (VanillaPort_lpu),_VanillaPort_lpu_=(char*)(_VanillaPort_lpu.c_str()),_VanillaPort_lpu_len = strlen(_VanillaPort_lpu_),_VanillaPort_lpw_len = VanillaPortUTF8ToUTF16(NULL, NULL, _VanillaPort_lpu_, _VanillaPort_lpu_len) + 1,_VanillaPort_lpw_ = (wchar_t*)malloc(_VanillaPort_lpw_len * 2),VanillaPortUTF8ToUTF16(_VanillaPort_lpw_, _VanillaPort_lpw_len, _VanillaPort_lpu_, _VanillaPort_lpu_len),_VanillaPort_lpw=_VanillaPort_lpw_,free(_VanillaPort_lpw_),_VanillaPort_lpw))
#define VanillaPort_W2U(VanillaPort_lpw) ((_VanillaPort_lpw = (VanillaPort_lpw),_VanillaPort_lpw_=(wchar_t*)(_VanillaPort_lpw.c_str()),_VanillaPort_lpw_len = wcslen(_VanillaPort_lpw_),_VanillaPort_lpu_len = VanillaPortUTF16ToUTF8(NULL, NULL, _VanillaPort_lpw_, _VanillaPort_lpw_len) + 1,_VanillaPort_lpu_ = (char*)malloc(_VanillaPort_lpu_len),VanillaPortUTF16ToUTF8(_VanillaPort_lpu_, _VanillaPort_lpu_len, _VanillaPort_lpw_, _VanillaPort_lpw_len),_VanillaPort_lpu=_VanillaPort_lpu_,free(_VanillaPort_lpu_),_VanillaPort_lpu))

VAPI(int) VanillaPortUTF16ToUTF8(char* Output, int OutputSize, wchar_t* Input, int InputLength);
VAPI(int) VanillaPortUTF8ToUTF16(wchar_t* Output, int OutputSize, char* Input, int Inputsize);
VanillaBool VanillaPortInitializeX();
VanillaPortWindow VanillaPortCreateWindow(VanillaRect Rect, VanillaString Title, VanillaBool ShowInTaskbar, VanillaBool PosMiddle, VanillaWindow Window);
VanillaVoid VanillaPortDestroyWindow(VanillaPortWindow PortWindow);
VanillaVoid VanillaPortSetWindowTitle(VanillaPortWindow PortWindow, VanillaString Title);
VanillaString VanillaPortGetWindowTitle(VanillaPortWindow PortWindow);
VanillaVoid VanillaPortSetWindowVisible(VanillaPortWindow PortWindow, VanillaBool Visible);
VanillaBool VanillaPortGetWindowVisible(VanillaPortWindow PortWindow);
VanillaVoid VanillaPortSetWindowComposite(VanillaPortWindow PortWindow, VanillaBool Composite);
VanillaBool VanillaPortGetWindowComposite(VanillaPortWindow PortWindow);
VanillaVoid VanillaPortDragWindow(VanillaPortWindow PortWindow);

VanillaVoid VanillaPortDestroyGraphicsOfWindowCachedInMemoey(VanillaGraphics Graphics);
VanillaGraphics VanillaPortCreateGraphicsOfWindowCachedInMemoey(VanillaWindow Window);
VanillaVoid VanillaPortUpdateWindow(VanillaWindow Window, VanillaRect UpdateRect);
SkTypeface* VanillaPortCreateSkTypeface(VanillaText FontName, SkTypeface::Style Style);
VanillaInt VanillaPortMessageLoop();
VanillaInt VanillaPortGetTickCount();

VanillaControl VanillaPortDispatchMouseMessage(VanillaWindow Window, VanillaInt Action, VanillaInt Button, VPoint pt);
#endif	//__VPORT_H__