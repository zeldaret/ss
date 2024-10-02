#ifndef NW4R_SND_TASK_MANAGER_H
#define NW4R_SND_TASK_MANAGER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Task.h>

#include <nw4r/ut.h>

#include <rvl/OS.h>

namespace nw4r {
namespace snd {
namespace detail {

class TaskManager {
public:
    enum TaskPriority {
        PRIORITY_LOW = 0,
        PRIORITY_MIDDLE = 1,
        PRIORITY_HIGH = 2,

        PRIORITY_MAX
    };

public:
    static TaskManager& GetInstance();

    void AppendTask(Task* pTask, TaskPriority priority = PRIORITY_MIDDLE);

    Task* ExecuteTask();
    void CancelTask(Task* pTask);
    void CancelAllTask();

    void WaitTask();
    void CancelWaitTask();

private:
    TaskManager();

    Task* PopTask();
    Task* GetNextTask();
    Task* GetNextTask(TaskPriority priority, bool remove);

private:
    TaskList mTaskList[PRIORITY_MAX]; // at 0x0
    Task* volatile mCurrentTask;      // at 0x24
    bool mCancelWaitTaskFlag;         // at 0x28
    OSThreadQueue mAppendThreadQueue; // at 0x2C
    OSThreadQueue mDoneThreadQueue;   // at 0x34
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
