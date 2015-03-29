// Vanilla Task Queue (not used yet)
#include "stdafx.h"
#include "VDefine.h"
#include "VStruct.h"
#include "VWindow.h"
#include "VPWindow.h"
#include "VPSysCall.h"

#include "VHashTable.h"
#include "VTaskQueue.h"


VTaskQueue::VTaskQueue(VanillaWindow Window) {
	this->Init(Window);
}

VTaskQueue::~VTaskQueue() {
	this->TaskQueue.clear();
}

VanillaVoid VTaskQueue::Init(VanillaWindow Window) {
	this->Window = Window;
}

VanillaVoid VTaskQueue::AddTask(VRect UpdateRect, VanillaInt Level) {
	switch (Level) {
	case VTL_NORMAL:
		this->TaskQueue.push_back(new VTask(UpdateRect, Level));
		break;
	case VTL_HIGH:
		this->TaskQueue.insert(this->TaskQueue.begin(), 1, new VTask(UpdateRect, Level));
		break;
	case VTL_IMMEDIATELY:
		VanillaWindowUpdateGraphicsRect(this->Window, &UpdateRect, false, true);
		break;
	}
	this->DoWork();
}

VanillaInt VTaskQueue::DoWork() {
	if (this->Processing) {
		return NULL;
	}
	int StartTime = VanillaPSGetTickCount();
	this->Processing = true;
	VanillaTask Task;
	VanillaBool UpdateType = !VanillaPWGetWindowComposite(this->Window->PortWindow);
	while (this->TaskQueue.size() != 0) {
		Task = *this->TaskQueue.begin();
		VanillaWindowUpdateGraphicsRect(this->Window, &Task->UpdateRect, false, UpdateType);
		this->TaskQueue.erase(this->TaskQueue.begin());
	}
	if (!UpdateType) {
		VanillaWindowUpdate(Window, NULL);
	}
	return VanillaPSGetTickCount() - StartTime;
}

VanillaInt VTaskQueue::IsDone() {
	return !this->Processing;
}
