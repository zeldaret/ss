#include "nw4r/snd/TaskThread.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_TaskThread.cpp
 */

/*******************************************************************************
 * headers
 */

#include <types.h>

#include "nw4r/snd/TaskManager.h"

#if 0
#include <revolution/OS/OSThread.h>
#else
#include <context_rvl.h>
#endif

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

TaskThread::TaskThread() :
	mStackEnd	(nullptr),
	mFinishFlag	(false),
	mCreateFlag	(false)
{
}

TaskThread::~TaskThread()
{
	if (mCreateFlag)
		Destroy();
}

bool TaskThread::Create(s32 priority, void *stack, u32 stackSize)
{
	NW4RAssertPointerNonnull_Line(59, stack);
	NW4RAssertAligned_Line(60, stack, 4);

	if (mCreateFlag)
		Destroy();

	BOOL result = OSCreateThread(&mThread, &ThreadFunc, this,
	                             static_cast<byte_t *>(stack) + stackSize,
	                             stackSize, priority, OS_THREAD_NO_FLAGS);
	if (!result)
		return false;

	mStackEnd	= static_cast<byte4_t *>(stack);
	mFinishFlag	= false;
	mCreateFlag	= true;

	OSResumeThread(&mThread);

	return true;
}

void TaskThread::Destroy()
{
	if (!mCreateFlag)
		return;

	mFinishFlag = true;
	TaskManager::GetInstance().CancelWaitTask();

	BOOL result = OSJoinThread(&mThread, nullptr);
	NW4RAssert_Line(105, result);

	mCreateFlag = false;
}

void *TaskThread::ThreadFunc(void *arg)
{
	TaskThread *taskThread = static_cast<TaskThread *>(arg);

	taskThread->ThreadProc();

	return nullptr;
}

void TaskThread::ThreadProc()
{
	while (!mFinishFlag) // TODO: implies volatile?
	{
		TaskManager::GetInstance().WaitTask();

		if (mFinishFlag)
			break;

		TaskManager::GetInstance().ExecuteTask();

		NW4RAssert_Line(160, *mStackEnd == OS_THREAD_STACK_MAGIC);
	}
}

}}} // namespace nw4r::snd::detail
