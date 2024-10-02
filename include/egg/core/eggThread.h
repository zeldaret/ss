#ifndef EGG_THREAD_H
#define EGG_THREAD_H

#include "egg/core/eggHeap.h"
#include "nw4r/ut.h"
#include "rvl/OS.h"

#include <common.h>

namespace EGG {

class Thread {
public: // vtable: 0x00
    /* 0x08 */ virtual ~Thread();
    /* 0x0C */ virtual void *run() {
        return nullptr;
    }
    /* 0x10 */ virtual void onEnter() {}
    /* 0x14 */ virtual void onExit() {}

public:
    /* 0x04 */ Heap *mContainingHeap;
    /* 0x08 */ OSThread *mOSThread;
    /* 0x0C */ OSMessageQueue mMesgQueue;
    /* 0x2C */ OSMessage *mMesgBuffer;
    /* 0x30 */ int mMesgCount;
    /* 0x34 */ void *mStackMemory;
    /* 0x38 */ u32 mStackSize;
    /* 0x3C */ Heap *mAllocatableHeap;
    // TODO from the usage in eggThread this really looks like
    // it's stashed thread that's restored when switching threads
    /* 0x40 */ Heap *mCurrentHeap;
    /* 0x44 */ nw4r::ut::Node mLink;

public:
    Thread(u32 stackSize, int msgCount, int priority, Heap *heap);
    Thread(OSThread *osThread, int msgCount);
    static Thread *findThread(OSThread *thread);
    static void initialize();
    void setThreadCurrentHeap(Heap *heap);
    static void switchThreadCallback(OSThread *from, OSThread *to);
    void setCommonMesgQueue(int mesgCount, Heap *heap);
    static void *start(void *thread);

public:
    static nw4r::ut::List sThreadList;
    static void (*sOldSwitchThreadCallback)(OSThread *, OSThread *);
};

} // namespace EGG

#endif
