#include "egg/core/eggHeap.h"

#include "egg/core/eggAllocator.h"
#include "egg/core/eggThread.h"
#include "nw4r/ut/ut_list.h"

namespace EGG {

nw4r::ut::List Heap::sHeapList;
OSMutex Heap::sRootMutex;
Heap *Heap::sCurrentHeap;
int Heap::sIsHeapListInitialized;
Heap *Heap::sAllocatableHeap;
ErrorCallback Heap::sErrorCallback;
HeapAllocCallback Heap::sAllocCallback;
HeapFreeCallback Heap::sFreeCallback;
void *Heap::sErrorCallbackArg;
void *Heap::sAllocCallbackArg;
void *Heap::sFreeCallbackArg;
HeapCreateCallback Heap::sCreateCallback;
HeapDestroyCallback Heap::sDestroyCallback;

void Heap::initialize() {
    nw4r::ut::List_Init(&sHeapList, 0x1c /* todo offsetof() */);
    OSInitMutex(&sRootMutex);
    sIsHeapListInitialized = true;
}

Heap::Heap(MEMiHeapHead *head) : mHeapHandle(head), mParentBlock(nullptr), mName("NoName"), mFlag() {
    mFlag.value = 0;
    nw4r::ut::List_Init(&mChildren, 0x8 /* todo offsetof() */);
    OSLockMutex(&sRootMutex);
    nw4r::ut::List_Append(&sHeapList, this);
    OSUnlockMutex(&sRootMutex);
}

Heap::~Heap() {
    OSLockMutex(&sRootMutex);
    nw4r::ut::List_Remove(&sHeapList, this);
    OSUnlockMutex(&sRootMutex);
}

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

Heap *Heap::findParentHeap() {
    Heap *retHeap = nullptr;
    MEMiHeapHead *heap = MEMFindParentHeap(mHeapHandle);
    if (heap) {
        retHeap = findHeap(heap);
    }

    return retHeap;
}

Heap *Heap::findContainHeap(const void *memBlock) {
    Heap *retHeap = nullptr;
    MEMiHeapHead *heap = MEMFindContainHeap(memBlock);
    if (heap) {
        retHeap = findHeap(heap);
    }

    return retHeap;
}

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

void Heap::dispose() {
    mFlag.setBit(1);
    Heap *heap;
    while ((heap = (Heap *)nw4r::ut::List_GetFirst(&mChildren)) != nullptr) {
        heap->~Heap();
    }
    mFlag.resetBit(1);
}

void Heap::dump() {}

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

Heap *Heap::becomeCurrentHeap() {
    OSLockMutex(&sRootMutex);
    Heap *h = sCurrentHeap;
    sCurrentHeap = this;
    OSUnlockMutex(&sRootMutex);
    return h;
}

Heap *Heap::_becomeCurrentHeapWithoutLock() {
    Heap *h = sCurrentHeap;
    sCurrentHeap = this;
    DCStoreRange(&sCurrentHeap, sizeof(sCurrentHeap));
    return h;
}

// TODO this could be an inline virtual function
void Heap::initAllocator(Allocator *alloc, s32 align) {
    MEMInitAllocatorFor_Heap(alloc, align, this);
}

} // namespace EGG

void *operator new(size_t, void *p) {
    return p;
}
void *operator new(size_t size, EGG::Heap *heap, int align) {
    return EGG::Heap::alloc(size, align, heap);
}
void *operator new(size_t size, EGG::Allocator *alloc) {
    return MEMAllocFromAllocator(alloc->getHandle(), size);
}
void *operator new[](size_t size, int align) {
    return EGG::Heap::alloc(size, align, nullptr);
}
void *operator new[](size_t size, EGG::Heap *heap, int align) {
    return EGG::Heap::alloc(size, align, heap);
}
