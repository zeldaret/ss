#include "egg/core/eggHeap.h"

#include "egg/core/eggAllocator.h"
#include "egg/core/eggThread.h"
#include "nw4r/ut/ut_list.h"

namespace EGG {

/* 80673ae8 */ nw4r::ut::List Heap::sHeapList;
/* 80673af8 */ OSMutex Heap::sRootMutex;
/* 80576740 */ Heap *Heap::sCurrentHeap;
/* 80576744 */ int Heap::sIsHeapListInitialized;
/* 80576748 */ Heap *Heap::sAllocatableHeap;
/* 8057674c */ ErrorCallback Heap::sErrorCallback;
/* 80576750 */ HeapAllocCallback Heap::sAllocCallback;
/* 80576754 */ HeapFreeCallback Heap::sFreeCallback;
/* 80576758 */ void *Heap::sErrorCallbackArg;
/* 8057675c */ void *Heap::sAllocCallbackArg;
/* 80576760 */ void *Heap::sFreeCallbackArg;
/* 80576764 */ HeapCreateCallback Heap::sCreateCallback;
/* 80576764 */ HeapDestroyCallback Heap::sDestroyCallback;

/* 804953f0 */
void Heap::initialize() {
    nw4r::ut::List_Init(&sHeapList, 0x1c /* todo offsetof() */);
    OSInitMutex(&sRootMutex);
    sIsHeapListInitialized = true;
}

/* 80495430 */
Heap::Heap(MEMiHeapHead *head) : mHeapHandle(head), mParentBlock(nullptr), mName("NoName"), mFlag() {
    mFlag.value = 0;
    nw4r::ut::List_Init(&mChildren, 0x8 /* todo offsetof() */);
    OSLockMutex(&sRootMutex);
    nw4r::ut::List_Append(&sHeapList, this);
    OSUnlockMutex(&sRootMutex);
}

/* 804954c0 */
Heap::~Heap() {
    OSLockMutex(&sRootMutex);
    nw4r::ut::List_Remove(&sHeapList, this);
    OSUnlockMutex(&sRootMutex);
}

/* 80495560 */
void *Heap::alloc(size_t size, int align, Heap *heap) {
    Heap *currentHeap = sCurrentHeap;
    Thread *thread = Thread::findThread(OSGetCurrentThread());
    Heap *threadHeap = nullptr;

    if (thread != nullptr) {
        threadHeap = thread->mAllocatableHeap;
        if (threadHeap != nullptr) {
            heap = threadHeap;
        }
    }
    if (sAllocatableHeap != nullptr) {
        if (heap == nullptr) {
            heap = currentHeap;
        }
        if (heap != sAllocatableHeap) {
            OSReport(
                "cannot allocate from heap %x(%s) : allocatable heap is %x(%s)\n", heap, heap->getName(),
                sAllocatableHeap, sAllocatableHeap->getName()
            );
            OSReport("\tthread heap=%x(%s)\n", threadHeap, threadHeap != nullptr ? threadHeap->getName() : "none");
            if (sErrorCallback != nullptr) {
                HeapErrorArg arg;
                arg.msg = "disable_but";
                arg.userdata = sErrorCallbackArg;
                sErrorCallback(&arg);
            }
            dumpAll();
            return nullptr;
        }
    }

    if (heap == nullptr) {
        heap = currentHeap;
    }
    void *ptr = nullptr;
    if (heap) {
        ptr = heap->alloc(size, align);
    }
    return ptr;
}

/* 80495690 */
Heap *Heap::findHeap(MEMiHeapHead *head) {
    Heap *heap = nullptr;
    OSLockMutex(&sRootMutex);
    if (sIsHeapListInitialized) {
        Heap *heap2 = nullptr;
        while ((heap2 = (Heap *)nw4r::ut::List_GetNext(&sHeapList, heap2))) {
            if (heap2->mHeapHandle == head) {
                heap = heap2;
                break;
            }
        }
    }
    OSUnlockMutex(&sRootMutex);
    return heap;
}

/* 80495730 */
Heap *Heap::findParentHeap() {
    Heap *retHeap = nullptr;
    MEMiHeapHead *heap = MEMFindParentHeap(mHeapHandle);
    if (heap) {
        retHeap = findHeap(heap);
    }

    return retHeap;
}

/* 80495780 */
Heap *Heap::findContainHeap(const void *memBlock) {
    Heap *retHeap = nullptr;
    MEMiHeapHead *heap = MEMFindContainHeap(memBlock);
    if (heap) {
        retHeap = findHeap(heap);
    }

    return retHeap;
}

/* 804957c0 */
void Heap::free(void *ptr, Heap *heap) {
    if (heap == nullptr) {
        MEMiHeapHead *iheap = MEMFindContainHeap(ptr);
        if (iheap == nullptr) {
            return;
        }
        heap = findHeap(iheap);
        if (heap == nullptr) {
            return;
        }
    }

    heap->free(ptr);
}

/* 80495830 */
void Heap::dispose() {
    mFlag.setBit(1);
    Heap *heap;
    while ((heap = (Heap *)nw4r::ut::List_GetFirst(&mChildren)) != nullptr) {
        heap->~Heap();
    }
    mFlag.resetBit(1);
}

/* 804958a0 */
void Heap::dump() {}

/* 804958b0 */
void Heap::dumpAll() {
    Heap *heap = nullptr;
    u32 mem[2] = {0, 0};

    OSLockMutex(&sRootMutex);
    while ((heap = (Heap *)nw4r::ut::List_GetNext(&sHeapList, heap)) != nullptr) {
        Heap *childHeap = nullptr;
        Heap *parentHeap = heap->findParentHeap();
        if ((u32)heap < 0x90000000) {
            mem[0] += heap->getAllocatableSize(4);
        } else {
            mem[1] += heap->getAllocatableSize(4);
        }

        while ((childHeap = (Heap *)nw4r::ut::List_GetNext(&sHeapList, childHeap)) != nullptr) {
            if (parentHeap == childHeap) {
                break;
            }
        }
    }
    OSUnlockMutex(&sRootMutex);
}

/* 804959a0 */
Heap *Heap::becomeCurrentHeap() {
    OSLockMutex(&sRootMutex);
    Heap *h = sCurrentHeap;
    sCurrentHeap = this;
    OSUnlockMutex(&sRootMutex);
    return h;
}

/* 80495a00 */
Heap *Heap::_becomeCurrentHeapWithoutLock() {
    Heap *h = sCurrentHeap;
    sCurrentHeap = this;
    DCStoreRange(&sCurrentHeap, sizeof(sCurrentHeap));
    return h;
}

// TODO this could be an inline virtual function
/* 80495a40 */
void Heap::initAllocator(Allocator *alloc, s32 align) {
    MEMInitAllocatorFor_Heap(alloc, align, this);
}

} // namespace EGG

/* 80495a60 */
void *operator new(size_t, void *p) {
    return p;
}
/* 80495a70 */
void *operator new(size_t size, EGG::Heap *heap, int align) {
    return EGG::Heap::alloc(size, align, heap);
}

/* 80495a80 */
void *operator new(size_t size, EGG::Allocator *alloc) {
    return MEMAllocFromAllocator(alloc->getHandle(), size);
}
/* 80495a90 */
void *operator new[](size_t size, int align) {
    return EGG::Heap::alloc(size, align, nullptr);
}
/* 80495aa0 */
void *operator new[](size_t size, EGG::Heap *heap, int align) {
    return EGG::Heap::alloc(size, align, heap);
}
