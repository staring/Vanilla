#include <stdafx.h>
#include <VDefine.h>
#include <VPSysCall.h>

VanillaInt VanillaPSGetTickCount() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

VanillaInt VanillaPSSetTimer(VanillaInt Elapse, VTimerProc Callback, VanillaInt UserData);
VanillaBool VanillaPSKillTimer(VanillaInt TimerID);
