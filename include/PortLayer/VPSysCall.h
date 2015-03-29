#ifndef __VANILLA_PORT_SYSCALL_H__
#define __VANILLA_PORT_SYSCALL_H__

typedef int(*VTimerProc)(VanillaInt ID);

/**
* 此函数用作获取系统启动时间.
* @param Returns int.
*/
VanillaInt VanillaPSGetTickCount();
/**
* 此函数用作设置定时器(Timer).
* @param Elapse 事件时间间隔
* @param Callback 事件回调
* @param UserData 用户数据
* @param Returns 成功返回定时器ID
*/
VanillaInt VanillaPSSetTimer(VanillaInt Elapse, VTimerProc Callback, VanillaInt UserData);
/**
* 此函数用作销毁定时器(Timer).
* @param TimerID VanillaPSSetTimer返回的ID
* @param Returns 返回是否成功
*/
VanillaBool VanillaPSKillTimer(VanillaInt TimerID);
#endif	//__VANILLA_PORT_SYSCALL_H__
