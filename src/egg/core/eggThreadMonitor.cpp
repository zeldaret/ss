#include "egg/core/eggThreadMonitor.h"

#include "rvl/OS.h"

namespace EGG {

ThreadMonitor *ThreadMonitor::sInstance; // Never initialized

s32 ThreadMonitor::getThreadIndex(OSThread *thread) {
    for (s32 i = 0; i < mThreadCount; i++) {
        if (mThreadList[i].mThread == thread) {
            return i;
        }
    }
    return -1;
}

s32 ThreadMonitor::doRegisterThread(OSThread *thread) {
    if (mThreadCount != mMaxThreads) {
        s32 i = mThreadCount;

        mThreadList[i].mThread = thread;
        mThreadList[i]._04 = _EC;
        mThreadList[i]._08 = true;
        mThreadList[i]._09[0] = '\0';
        mThreadList[i]._2C = _F4;

        mThreadCount++;

        if (mSortEnabled) {
            sortByPriority();
        }

        return getThreadIndex(thread);
    }

    return -1;
}

void ThreadMonitor::sortByPriority() {
    // Selection sort on thread priority
    for (s32 i = 0; i < mThreadCount - 1; i++) {
        OSPriority minPrio = 31;
        s32 minPrioIndex = -1;

        for (s32 j = i; j < mThreadCount; j++) {
            OSPriority prio = OSGetThreadPriority(mThreadList[j].mThread);
            if (prio <= minPrio) {
                minPrio = prio;
                minPrioIndex = j;
            }
        }

        if (i != minPrioIndex) {
            ThreadInfo temp = mThreadList[i];
            mThreadList[i] = mThreadList[minPrioIndex];
            mThreadList[minPrioIndex] = temp;
        }
    }
}

void ThreadMonitor::registerThread(OSThread *thread, UnknownStruct arg) {
    s32 i = getThreadIndex(thread);
    if (i == -1) {
        i = doRegisterThread(thread);
    }

    if (i != -1) {
        mThreadList[i]._04 = arg;
    }
}

} // namespace EGG
