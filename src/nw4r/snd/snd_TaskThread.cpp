#include "nw4r/snd/snd_TaskThread.h"

#include "nw4r/snd/snd_TaskManager.h"

namespace nw4r {
namespace snd {
namespace detail {

TaskThread::TaskThread() : mStackEnd(NULL), mFinishFlag(false), mCreateFlag(false) {}

TaskThread::~TaskThread() {
    if (mCreateFlag) {
        Destroy();
    }
}

bool TaskThread::Create(s32 priority, void *pStack, u32 stackSize) {
    if (mCreateFlag) {
        Destroy();
    }

    if (!OSCreateThread(
            &mThread, ThreadFunc, &mThread, static_cast<u8 *>(pStack) + stackSize, stackSize, priority, 0
        )) {
        return false;
    }

    mStackEnd = static_cast<u32 *>(pStack);
    mFinishFlag = false;
    mCreateFlag = true;

    OSResumeThread(&mThread);
    return true;
}

void TaskThread::Destroy() {
    if (!mCreateFlag) {
        return;
    }

    mFinishFlag = true;
    TaskManager::GetInstance().CancelWaitTask();
    OSJoinThread(&mThread, NULL);
    mCreateFlag = false;
}

void *TaskThread::ThreadFunc(void *pArg) {
    TaskThread *p = static_cast<TaskThread *>(pArg);
    p->ThreadProc();

    return NULL;
}

void TaskThread::ThreadProc() {
    while (!mFinishFlag) {
        TaskManager::GetInstance().WaitTask();

        if (mFinishFlag) {
            break;
        }

        TaskManager::GetInstance().ExecuteTask();
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r
