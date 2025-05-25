#ifndef NW4R_SND_TASK_MANAGER_H
#define NW4R_SND_TASK_MANAGER_H

/*******************************************************************************
 * headers
 */

#include "nw4r/snd/snd_Task.h"

#include <rvl/OS/OSThread.h> // OSThreadQueue

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2fd1de
	class TaskManager
	{
	// enums
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2fd480
		enum TaskPriority
		{
			PRIORITY_LOW,
			PRIORITY_MIDDLE,
			PRIORITY_HIGH,
		};

	// methods
	public:
		// instance accessors
		static TaskManager &GetInstance();

		// methods
		void AppendTask(Task *task, TaskPriority priority);

		Task *ExecuteTask();
		void CancelTask(Task *task);
		void CancelAllTask();

		void WaitTask();
		void CancelWaitTask();

	private:
		// cdtors
		TaskManager();

		// methods
		Task *GetNextTask();
		Task *GetNextTask(TaskPriority priority, bool doRemove);

		Task *PopTask();

	// static members
	public:
		static int const PRIORITY_NUM = 3;

	// members
	private:
		Task::LinkList	mTaskList[PRIORITY_NUM];	// size 0x24, offset 0x00
		Task			* volatile mCurrentTask;	// size 0x04, offset 0x24
		bool			mCancelWaitTaskFlag;		// size 0x01, offset 0x28 // TODO: volatile? (see WaitTask)
		/* 3 bytes padding */
		OSThreadQueue	mAppendThreadQueue;			// size 0x08, offset 0x2c
		OSThreadQueue	mDoneThreadQueue;			// size 0x08, offset 0x34
	}; // size 0x3c
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_TASK_MANAGER_H
