#ifndef NW4R_SND_TASK_THREAD_H
#define NW4R_SND_TASK_THREAD_H
#include <nw4r/types_nw4r.h>

#include <rvl/OS.h>

namespace nw4r {
namespace snd {
namespace detail {

class TaskThread {
public:
    TaskThread();
    ~TaskThread();

    bool Create(s32 priority, void* pStack, u32 stackSize);
    void Destroy();

private:
    static void* ThreadFunc(void* pArg);
    void ThreadProc();

private:
    OSThread mThread; // at 0x0
    u32* mStackEnd;   // at 0x318

    volatile bool mFinishFlag; // at 0x31C
    bool mCreateFlag;          // at 0x31D
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
