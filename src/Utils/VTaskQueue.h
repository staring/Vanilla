#ifndef __VTSAKQUEUE_H__
#define __VTSAKQUEUE_H__
// Vanilla Task Queue (not used yet)

// 任务优先级
#define VTL_NORMAL	0	// 普通
#define VTL_HIGH	1	// 高
#define VTL_IMMEDIATELY	2	// 立即


// 队列中的刷新任务
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
#endif	//__VTSAKQUEUE_H__
