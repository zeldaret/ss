#include "egg/core/eggAssertHeap.h"

namespace EGG {

AssertHeap::AssertHeap(MEMiHeapHead *pHead) : Heap(pHead) {}

AssertHeap::~AssertHeap() {
    dispose();
    MEMDestroyFrmHeap(mHeapHandle);
}

AssertHeap *AssertHeap::create(void *pBlock, u32 size) {
    AssertHeap *heap = nullptr;
    void *startAddr = pBlock;
    void *heapEnd = ROUND_DOWN_PTR((u8 *)pBlock + size, 0x04);
    pBlock = ROUND_UP_PTR(pBlock, 0x04);

    if (pBlock > heapEnd || ((u8 *)heapEnd - (u8 *)pBlock) < 0x38u) {
        return nullptr;
    }

    MEMiHeapHead *head =
        MEMCreateFrmHeapEx((u8 *)pBlock + sizeof(AssertHeap), ((u8 *)heapEnd - (u8 *)pBlock) - sizeof(AssertHeap), 0);
    if (head != nullptr) {
        heap = new (pBlock) AssertHeap(head);
        heap->mParentBlock = startAddr;
        if (sCreateCallback != nullptr) {
            (sCreateCallback)(heap);
        }
    }

    return heap;
}

AssertHeap *AssertHeap::create(u32 size, Heap *pHeap) {
    AssertHeap *assertHeap = nullptr;

    if (!pHeap) {
        pHeap = sCurrentHeap;
    }
    if (size == -1) {
        size = pHeap->getAllocatableSize(4);
    } else if (size == 0) {
        size = getSize();
    }

    void *pBlock = pHeap->alloc(size, 4);
    if (pBlock) {
        assertHeap = create(pBlock, size);
        if (!assertHeap) {
            pHeap->free(pBlock);
        }
    }

    return assertHeap;
}

void AssertHeap::destroy() {
    if (sDestroyCallback) {
        sDestroyCallback(this);
    }

    Heap *parent = findParentHeap();
    this->~AssertHeap();
    if (parent) {
        parent->free(this);
    }
}

void *AssertHeap::alloc(u32 size, s32 align) {
    if (sAllocCallback) {
        HeapAllocArg arg;
        arg.userArg = sAllocCallbackArg;
        arg.size = size;
        arg.align = align;
        arg.heap = this;
        arg.ptr = nullptr;
        sAllocCallback(&arg);
    }
    return nullptr;
}

void AssertHeap::free(void *pBlock) {
    if (sFreeCallback != nullptr) {
        HeapFreeArg arg;
        arg.userArg = sFreeCallbackArg;
        arg.ptr = pBlock;
        arg.heap = this;
        (sFreeCallback)(&arg);
    }
}

u32 AssertHeap::resizeForMBlock(void *pBlock, u32 size) {
    return 0;
}

u32 AssertHeap::getTotalFreeSize() {
    return 0;
}

u32 AssertHeap::getAllocatableSize(s32 align) {
    return 0;
}

u32 AssertHeap::adjust() {
    u32 adjustedSize = MEMAdjustFrmHeap(mHeapHandle);
    u32 totalSize = adjustedSize + sizeof(AssertHeap);
    if (totalSize > sizeof(AssertHeap)) {
        Heap *parent = findParentHeap();
        if (parent != nullptr) {
            parent->resizeForMBlock(mParentBlock, totalSize);
            return totalSize;
        }
    }
    return 0;
}

size_t AssertHeap::getSize() {
    return 0x7C;
}

Heap::eHeapKind AssertHeap::getHeapKind() const {
    return HEAP_KIND_ASSERT;
}

} // namespace EGG
