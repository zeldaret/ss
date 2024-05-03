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

            void *oldAllocCallbackArg = EGG::Heap::sAllocCallbackArg;
            void *oldFreeCallbackArg = EGG::Heap::sFreeCallbackArg;
            EGG::HeapAllocCallback oldAllocCallback = EGG::Heap::sAllocCallback;
            EGG::HeapFreeCallback oldFreeCallback = EGG::Heap::sFreeCallback;

            EGG::HeapFreeCallback freeThunk = &freeCallback;
            EGG::HeapAllocCallback allocThunk = &allocCallback;

            EGG::Heap::sAllocCallback = allocThunk;
            EGG::Heap::sAllocCallbackArg = this;
            mPreviousAllocCallback = oldAllocCallback;
            mPreviousAllocCallbackArg = oldAllocCallbackArg;

            EGG::Heap::sFreeCallback = freeThunk;
            EGG::Heap::sFreeCallbackArg = this;
            mPreviousFreeCallback = oldFreeCallback;
            mPreviousFreeCallbackArg = oldFreeCallbackArg;
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
