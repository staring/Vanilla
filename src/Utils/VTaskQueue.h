// Vanilla Task Queue (not used yet)
#pragma once

// �������ȼ�
#define VTL_NORMAL	0	// ��ͨ
#define VTL_HIGH	1	// ��
#define VTL_IMMEDIATELY	2	// ����


// �����е�ˢ������
typedef struct VTask
{
	VTask(){}
	VTask(VRect UpdateRect, VanillaInt Level) {
		this->UpdateRect = UpdateRect;
		this->Level = Level;
	}
	VRect UpdateRect;
	VanillaInt Level;
} *VanillaTask, _VTask;


typedef struct VTaskQueue
{
	std::vector<VanillaTask> TaskQueue;
	VanillaWindow Window;
	VanillaBool Processing;

	VTaskQueue(VanillaWindow Window);
	~VTaskQueue();
	VanillaVoid Init(VanillaWindow Window);
	VanillaVoid AddTask(VRect UpdateRect, VanillaInt Level);
	VanillaInt DoWork();
	VanillaInt IsDone();
} *VanillaTaskQueue, _VTaskQueue;