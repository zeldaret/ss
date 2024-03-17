#ifndef EGG_THREAD_H
#define EGG_THREAD_H

#include "egg/core/eggHeap.h"
#include "nw4r/ut.h"
#include "rvl/OS.h"
#include <common.h>

namespace EGG {

class Thread {
public: // vtable: 0x00 | 8056ea40
    /* 0x08 | 80496a60 */ virtual ~Thread();
    /* 0x0C | 80496dd0 */ virtual void *run();
    /* 0x10 | 800bd600 */ virtual void onEnter();
    /* 0x14 | 800bd5f0 */ virtual void onExit();

public:
    /* 0x04 */ Heap *mContainingHeap;
    /* 0x08 */ OSThread *mOSThread;
    /* 0x0C */ OSMessageQueue mMesgQueue;
    /* 0x2C */ OSMessage *mMesgBuffer;
    /* 0x30 */ int mMesgCount;
    /* 0x34 */ void *mStackMemory;
    /* 0x38 */ u32 mStackSize;
    /* 0x3C */ Heap *mAllocatableHeap;
    /* 0x40 */ Heap *mCurrentHeap;
    /* 0x44 */ nw4r::ut::Node mLink;

public:
    /* 80496910 */ Thread(u32 stackSize, int msgCount, int priority, Heap *heap);
    /* 804969e0 */ Thread(OSThread *osThread, int msgCount);
    /* 80496b20 */ static Thread *findThread(OSThread *thread);
    /* 80496b90 */ static void initialize();
    /* 80496bd0 */ void setThreadCurrentHeap(Heap *heap);
    /* 80496c70 */ static void switchThreadCallback(OSThread *from, OSThread *to);
    /* 80496d60 */ void setCommonMesgQueue(int mesgCount, Heap *heap);
    /* 80496dc0 */ static void *start(void *thread);

public:
    /* 80673b10 */ static nw4r::ut::List sThreadList;
    /* 80576770 */ static void (*sOldSwitchThreadCallback)(OSThread *, OSThread *);
};

} // namespace EGG

#endif
