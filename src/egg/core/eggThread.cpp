#include "egg/core/eggThread.h"

namespace EGG {

nw4r::ut::List Thread::sThreadList;
void (*Thread::sOldSwitchThreadCallback)(OSThread *, OSThread *);

Thread::Thread(u32 stackSize, int msgCount, int priority, Heap *heap) {
    if (heap == nullptr) {
        heap = Heap::sCurrentHeap;
    }

    mContainingHeap = heap;
    mStackSize = ROUND_DOWN(stackSize, 0x20);
    mStackMemory = Heap::alloc(ROUND_DOWN(stackSize, 0x20), 0x20, heap);
    mOSThread = Heap::alloc<OSThread>(mContainingHeap, 0x20);
    OSCreateThread(mOSThread, start, this, (char *)mStackMemory + mStackSize, mStackSize, priority, 1);
    mAllocatableHeap = nullptr;
    mCurrentHeap = nullptr;
    setCommonMesgQueue(msgCount, mContainingHeap);
}

Thread::Thread(OSThread *osThread, int msgCount) {
    mContainingHeap = nullptr;
    mOSThread = osThread;
    mStackSize = (u8 *)osThread->stackBegin - (u8 *)osThread->stackEnd;
    mStackMemory = osThread->stackEnd;
    mAllocatableHeap = nullptr;
    mCurrentHeap = nullptr;
    setCommonMesgQueue(msgCount, Heap::sCurrentHeap);
}

Thread::~Thread() {
    nw4r::ut::List_Remove(&sThreadList, this);
    if (mContainingHeap != nullptr) {
        if (!OSIsThreadTerminated(mOSThread)) {
            OSDetachThread(mOSThread);
            OSCancelThread(mOSThread);
        }
        Heap::free(mStackMemory, mContainingHeap);
        Heap::free(mOSThread, mContainingHeap);
    }
    Heap::free(mMesgBuffer, nullptr);
}

Thread *Thread::findThread(OSThread *thread) {
    Thread *ptr = nullptr;
    while ((ptr = (Thread *)nw4r::ut::List_GetNext(&sThreadList, ptr)) != nullptr) {
        if (ptr->mOSThread == thread) {
            return ptr;
        }
    }
    return nullptr;
}

void Thread::initialize() {
    // TODO offsetof
    nw4r::ut::List_Init(&sThreadList, 0x44);
    sOldSwitchThreadCallback = OSSetSwitchThreadCallback(switchThreadCallback);
}

void Thread::setThreadCurrentHeap(Heap *heap) {
    OSDisableScheduler();
    OSThread *myThread = mOSThread;
    OSThread *currentThread = OSGetCurrentThread();
    if (currentThread != myThread) {
        mCurrentHeap = heap;
    } else {
        if (heap != nullptr) {
            if (mCurrentHeap == nullptr) {
                mCurrentHeap = Heap::sCurrentHeap;
            }
            heap->_becomeCurrentHeapWithoutLock();
        } else {
            if (mCurrentHeap != nullptr) {
                mCurrentHeap->_becomeCurrentHeapWithoutLock();
                mCurrentHeap = nullptr;
            }
        }
    }

    OSEnableScheduler();
}

void Thread::switchThreadCallback(OSThread *from, OSThread *to) {
    Thread *fromThread = from != nullptr ? findThread(from) : nullptr;
    Thread *toThread = to != nullptr ? findThread(to) : nullptr;

    if (fromThread != nullptr) {
        fromThread->onExit();
        if (fromThread->mCurrentHeap != nullptr) {
            Heap *curr = Heap::sCurrentHeap;
            fromThread->mCurrentHeap->_becomeCurrentHeapWithoutLock();
            fromThread->mCurrentHeap = curr;
        }
    }

    if (toThread != nullptr) {
        if (toThread->mCurrentHeap != nullptr) {
            Heap *curr = Heap::sCurrentHeap;
            toThread->mCurrentHeap->_becomeCurrentHeapWithoutLock();
            toThread->mCurrentHeap = curr;
        }
        toThread->onEnter();
    }

    if (sOldSwitchThreadCallback != nullptr) {
        (sOldSwitchThreadCallback)(from, to);
    }
}

void Thread::setCommonMesgQueue(int mesgCount, Heap *heap) {
    mMesgCount = mesgCount;
    mMesgBuffer = Heap::alloc<OSMessage>(mesgCount, heap);
    OSInitMessageQueue(&mMesgQueue, mMesgBuffer, mMesgCount);
    nw4r::ut::List_Append(&sThreadList, this);
}

void *Thread::start(void *arg) {
    Thread *thread = static_cast<Thread *>(arg);
    return thread->run();
}

} // namespace EGG
