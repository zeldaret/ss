#ifndef NW4R_SND_TASK_MANAGER_H
#define NW4R_SND_TASK_MANAGER_H
#include "snd_Task.h"
#include "types_nw4r.h"
#include "ut_LinkList.h"
#include "ut_lock.h"
#include <rvl/OS/OSThread.h>


namespace nw4r {
namespace snd {
namespace detail {
struct TaskManager {
    enum TaskPriority {
        PRIORITY_0 = 0,
        PRIORITY_1 = 1,
        PRIORITY_2 = 2
        // PRIORITY_MAX = 3
    };

    inline TaskManager() : mTaskStacks(), WORD_0x24(0), BYTE_0x28(0) {
        OSInitThreadQueue(&mQueue_0x2C);
        OSInitThreadQueue(&mQueue_0x34);
    }

    static TaskManager *GetInstance();

    ~TaskManager();
    void AppendTask(Task *, TaskPriority);
    Task *PopTask();
    UNKTYPE GetNextTask();
    UNKTYPE ExecuteTask();
    UNKTYPE CancelTask(Task *);
    UNKTYPE CancelAllTask();
    UNKTYPE WaitTask();
    UNKTYPE CancelWaitTask();

    ut::LinkList<Task, 4> mTaskStacks[3];
    u32 WORD_0x24;
    u8 BYTE_0x28;
    OSThreadQueue mQueue_0x2C;
    OSThreadQueue mQueue_0x34;
};
} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
