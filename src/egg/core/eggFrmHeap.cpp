#include "egg/core/eggFrmHeap.h"

namespace EGG {

FrmHeap::FrmHeap(MEMiHeapHead *pHead) : Heap(pHead) {}

FrmHeap::~FrmHeap() {
    dispose();
    MEMDestroyFrmHeap(mHeapHandle);
}

FrmHeap *FrmHeap::create(void *pBlock, u32 size, u16 attr) {
    FrmHeap *heap = nullptr;
    void *startAddr = pBlock;
    void *heapEnd = ROUND_DOWN_PTR((u8 *)pBlock + size, 0x04);
    pBlock = ROUND_UP_PTR(pBlock, 0x04);

    if (pBlock > heapEnd || ((u8 *)heapEnd - (u8 *)pBlock) < 0x38u) {
        return nullptr;
    }

    MEMiHeapHead *head =
        MEMCreateFrmHeapEx((u8 *)pBlock + sizeof(FrmHeap), ((u8 *)heapEnd - (u8 *)pBlock) - sizeof(FrmHeap), attr);
    if (head != nullptr) {
        heap = new (pBlock) FrmHeap(head);
        heap->mParentBlock = startAddr;
        if (sCreateCallback != nullptr) {
            (sCreateCallback)(heap);
        }
    }

    return heap;
}

void FrmHeap::destroy() {
    if (sDestroyCallback) {
        sDestroyCallback(this);
    }

    Heap *parent = findParentHeap();
    this->~FrmHeap();
    if (parent) {
        parent->free(this);
    }
}

void *FrmHeap::alloc(u32 size, s32 align) {
    void *pBlock = MEMAllocFromFrmHeapEx(mHeapHandle, size, align);
    if (sAllocCallback) {
        HeapAllocArg arg;
        arg.userArg = sAllocCallbackArg;
        arg.ptr = pBlock;
        arg.size = size;
        arg.align = align;
        arg.heap = this;
        sAllocCallback(&arg);
    }
    return pBlock;
}

void FrmHeap::free(void *pBlock) {
    if (sFreeCallback != nullptr) {
        HeapFreeArg arg;
        arg.userArg = sFreeCallbackArg;
        arg.ptr = pBlock;
        arg.heap = this;
        (sFreeCallback)(&arg);
    }
}

void FrmHeap::free(s32 flags) {
    MEMFreeToFrmHeap(mHeapHandle, flags);
}

u32 FrmHeap::resizeForMBlock(void *pBlock, u32 size) {
    return MEMResizeForMBlockFrmHeap(mHeapHandle, pBlock, size);
}

u32 FrmHeap::getTotalFreeSize() {
    return getAllocatableSize(4);
}

u32 FrmHeap::getAllocatableSize(s32 align) {
    return MEMGetAllocatableSizeForFrmHeapEx(mHeapHandle, align);
}

u32 FrmHeap::adjust() {
    u32 adjustedSize = MEMAdjustFrmHeap(mHeapHandle);
    u32 totalSize = adjustedSize + sizeof(FrmHeap);
    if (totalSize > sizeof(FrmHeap)) {
        Heap *parent = findParentHeap();
        if (parent != nullptr) {
            parent->resizeForMBlock(mParentBlock, totalSize);
            return totalSize;
        }
    }
    return 0;
}

void FrmHeap::recordState(u32 id) {
    MEMRecordStateForFrmHeap(mHeapHandle, id);
}

void FrmHeap::freeByState(u32 id) {
    MEMFreeByStateToFrmHeap(mHeapHandle, id);
}

Heap::eHeapKind FrmHeap::getHeapKind() const {
    return HEAP_KIND_FRAME;
}

} // namespace EGG
