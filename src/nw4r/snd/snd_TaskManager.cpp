#include "nw4r/snd/TaskManager.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_TaskManager.cpp
 */

/*******************************************************************************
 * headers
 */

#include <decomp.h>
#include <macros.h> // NW4R_RANGE_FOR_NO_AUTO_INC
#include <types.h> // nullptr

#include "nw4r/snd/Task.h"

#include "nw4r/ut/Lock.h" // ut::AutoInterruptLock

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

TaskManager &TaskManager::GetInstance()
{
	static TaskManager instance;

	return instance;
}

TaskManager::TaskManager() :
	mCurrentTask		(nullptr),
	mCancelWaitTaskFlag	(false)
{
	OSInitThreadQueue(&mAppendThreadQueue);
	OSInitThreadQueue(&mDoneThreadQueue);
}

void TaskManager::AppendTask(Task *task, TaskPriority priority)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLValue_Line(67, priority, PRIORITY_LOW,
	                                   PRIORITY_NUM);

	ut::AutoInterruptLock lock;

	task->mBusyFlag = true;
	mTaskList[priority].PushBack(task);

	OSWakeupThread(&mAppendThreadQueue);
}

// TaskManager::FindTask, probably
DECOMP_FORCE_CLASS_METHOD(Task::LinkList::Iterator, operator *());

Task *TaskManager::GetNextTask(TaskPriority priority, bool doRemove)
{
	ut::AutoInterruptLock lock;

	if (mTaskList[priority].IsEmpty())
		return nullptr;

	Task *task = &mTaskList[priority].GetFront();

	if (doRemove)
		mTaskList[priority].PopFront();

	return task;
}

Task *TaskManager::PopTask()
{
	ut::AutoInterruptLock lock;

	Task *task;

	if ((task = GetNextTask(PRIORITY_HIGH, true)))
		return task;

	if ((task = GetNextTask(PRIORITY_MIDDLE, true)))
		return task;

	if ((task = GetNextTask(PRIORITY_LOW, true)))
		return task;

	return nullptr;
}

Task *TaskManager::GetNextTask()
{
	ut::AutoInterruptLock lock;

	Task *task;

	if ((task = GetNextTask(PRIORITY_HIGH, false)))
		return task;

	if ((task = GetNextTask(PRIORITY_MIDDLE, false)))
		return task;

	if ((task = GetNextTask(PRIORITY_LOW, false)))
		return task;

	return nullptr;
}

Task *TaskManager::ExecuteTask()
{
	Task *task = PopTask();
	if (!task)
		return nullptr;

	mCurrentTask = task;

	task->mBusyFlag = false;
	task->Execute();

	mCurrentTask = nullptr;

	OSWakeupThread(&mDoneThreadQueue);

	return task;
}

void TaskManager::CancelTask(Task *task)
{
	ut::AutoInterruptLock lock;

	if (task == mCurrentTask)
	{
		task->OnCancel();

		while (task == mCurrentTask)
			OSSleepThread(&mDoneThreadQueue);
	}
	else
	{
		for (int i = 0; i < PRIORITY_NUM; i++)
		{
			TaskPriority priority = static_cast<TaskPriority>(i);

			NW4R_RANGE_FOR_NO_AUTO_INC(itr, mTaskList[priority])
			{
				decltype(itr) curItr = itr++;

				if (&(*curItr) == task)
				{
					mTaskList[priority].Erase(curItr);

					curItr->mBusyFlag = false;
					curItr->Cancel();

					break;
				}
			}
		}
	}
}

void TaskManager::CancelAllTask()
{
	ut::AutoInterruptLock lock;

	for (int i = 0; i < PRIORITY_NUM; i++)
	{
		TaskPriority priority = static_cast<TaskPriority>(i);

		Task::LinkList &list = mTaskList[priority];
		while (!list.IsEmpty())
		{
			Task &task = list.GetBack();
			list.PopBack();

			task.mBusyFlag = false;
			task.Cancel();
		}
	}

	if (mCurrentTask)
	{
		mCurrentTask->OnCancel();

		while (mCurrentTask)
			OSSleepThread(&mDoneThreadQueue);
	}
}

void TaskManager::WaitTask()
{
	ut::AutoInterruptLock lockIntr;

	mCancelWaitTaskFlag = false;

	while (!GetNextTask() && !mCancelWaitTaskFlag) // TODO: implies volatile?
		OSSleepThread(&mAppendThreadQueue);
}

void TaskManager::CancelWaitTask()
{
	ut::AutoInterruptLock lockIntr;

	mCancelWaitTaskFlag = true;
	OSWakeupThread(&mAppendThreadQueue);
}

}}} // namespace nw4r::snd::detail
