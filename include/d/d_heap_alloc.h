#ifndef D_HEAP_ALLOC
#define D_HEAP_ALLOC

#include <egg/core/eggHeap.h>

class dHeapAllocatorBase {
public:
    /* vtable at 0x00 */
    dHeapAllocatorBase()
        : mCallbacksInitialized(0), mPreviousAllocCallback(nullptr), mPreviousAllocCallbackArg(nullptr),
          mPreviousFreeCallback(nullptr), mPreviousFreeCallbackArg(nullptr) {}
    inline void doInitCallbacks() {
        if (!mCallbacksInitialized) {
            mCallbacksInitialized = 1;
            EGG::Heap::AllocCallbackBundle prev = EGG::Heap::setAllocCallback(&allocCallback, this);
            mPreviousAllocCallback = prev.AllocCallback;
            mPreviousAllocCallbackArg = prev.AllocCallbackArg;

            EGG::Heap::FreeCallbackBundle prev2 = EGG::Heap::setFreeCallback(&freeCallback, this);
            mPreviousFreeCallback = prev2.FreeCallback;
            mPreviousFreeCallbackArg = prev2.FreeCallbackArg;
        }
    }
    static void allocCallback(EGG::HeapAllocArg *arg) {
        dHeapAllocatorBase *allocator = (dHeapAllocatorBase *)(arg->userArg);
        allocator->onAlloc(arg);
        if (allocator->mPreviousAllocCallback) {
            EGG::HeapAllocArg chainArg = *arg;
            chainArg.userArg = allocator->mPreviousAllocCallbackArg;
            (allocator->mPreviousAllocCallback)(&chainArg);
        }
    };
    static void freeCallback(EGG::HeapFreeArg *arg) {
        dHeapAllocatorBase *allocator = (dHeapAllocatorBase *)(arg->userArg);
        EGG::HeapFreeArg chainArg;
        allocator->onFree(arg);
        if (allocator->mPreviousFreeCallback) {
            chainArg = *arg;
            chainArg.userArg = allocator->mPreviousFreeCallbackArg;
            (allocator->mPreviousFreeCallback)(&chainArg);
        }
    }

    virtual ~dHeapAllocatorBase() {}
    virtual void onAlloc(EGG::HeapAllocArg *arg) {};
    virtual void onFree(EGG::HeapFreeArg *arg) {};

    /* 0x04 */ bool mCallbacksInitialized;
    /* 0x08 */ EGG::HeapAllocCallback mPreviousAllocCallback;
    /* 0x0C */ void *mPreviousAllocCallbackArg;
    /* 0x10 */ EGG::HeapFreeCallback mPreviousFreeCallback;
    /* 0x14 */ void *mPreviousFreeCallbackArg;
};

void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *ptr);
void operator delete[](void *ptr);

class dHeapAllocator : public dHeapAllocatorBase {
public:
    dHeapAllocator() {}
    virtual ~dHeapAllocator() {}
    virtual void onAlloc(EGG::HeapAllocArg *arg);

    static void initCallbacks();

    static dHeapAllocator sAllocator;
};

#endif
