#include <stdafx.h>
#include <VDefine.h>
#include <VPSysCall.h>

VanillaInt VanillaPSGetTickCount() {
	return GetTickCount();
}

VanillaInt VanillaPSSetTimer(VanillaInt Elapse, VTimerProc Callback, VanillaInt UserData);
VanillaBool VanillaPSKillTimer(VanillaInt TimerID);
