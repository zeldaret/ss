#include <egg/core/eggExpHeap.h>

namespace EGG {

/* 80495ab0 */ ExpHeap::ExpHeap(MEMiHeapHead *heapHandle): Heap(heapHandle) {}

/* 80495af0 */ ExpHeap::~ExpHeap() {
    dispose();
    MEMDestroyExpHeap(mHeapHandle);
}

/* 80495b70 */ ExpHeap* ExpHeap::create(void *heapStart, size_t size, u16 attr) {
    ExpHeap *heap = nullptr;
    void *startAddr = heapStart;
    void *heapEnd = ROUND_DOWN_PTR((u8*)heapStart + size, 0x04);
    heapStart = ROUND_UP_PTR(heapStart, 0x04);


    if (heapStart > heapEnd || ((u8*)heapEnd - (u8*)heapStart) < 0x38u) {
        return nullptr;
    }

    MEMiHeapHead *head = MEMCreateExpHeapEx(((ExpHeap*)heapStart) + 1, ((u8*)heapEnd - (u8*)heapStart) - 0x34, attr);
    if (head != nullptr) {
        heap = new(heapStart) ExpHeap(head);
        heap->mParentBlock = startAddr;
        if (sCreateCallback != nullptr) {
            (sCreateCallback)(heap);
        }
    }

    return heap;
}

/* 80495c30 */ ExpHeap* ExpHeap::create(size_t size, Heap *heap, u16 attr) {
    ExpHeap *newHeap = nullptr;
    if (heap == nullptr) {
        heap = sCurrentHeap;
    }
    
    if (size == -1) {
        size = heap->getAllocatableSize(0x04);
    }

    void *block = heap->alloc(size, 0x04);
    if (block != nullptr) {
        newHeap = ExpHeap::create(block, size, attr);
        if (newHeap == nullptr) {
            heap->free(block);
        }
    }

    return newHeap;
}

/* 80495d00 */ void ExpHeap::destroy() {
    if (sDestroyCallback != nullptr) {
        sDestroyCallback(this);
    }

    Heap *parentHeap = findParentHeap();
    this->~ExpHeap();
    if (parentHeap != nullptr) {
        parentHeap->free(this);
    }
}

/* 80495d90 */ void *ExpHeap::alloc(u32 size, s32 align) {
    if (mFlag.onBit(0)) {
        #line 182
        OSError("DAME DAME\n");
    }

    void *ptr = MEMAllocFromExpHeapEx(mHeapHandle, size, align);
    if (sAllocCallback != nullptr) {
        HeapAllocArg arg;
        arg.userArg = sAllocCallbackArg;
        arg.ptr = ptr;
        arg.size = size;
        arg.align = align;
        arg.heap = this;
        (sAllocCallback)(&arg);
    }

    return ptr;
}

/* 80495e50 */ void ExpHeap::free(void *ptr) {
    if (sFreeCallback != nullptr) {
        HeapFreeArg arg;
        arg.userArg = sFreeCallbackArg;
        arg.ptr = ptr;
        arg.heap = this;
        (sFreeCallback)(&arg);
    }
    MEMFreeToExpHeap(mHeapHandle, ptr);
}

/* 80495ec0 */ u32 ExpHeap::resizeForMBlock(void *block, u32 size) {
    return MEMResizeForMBlockExpHeap(mHeapHandle, block, size);
}

/* 80495ed0 */ u32 ExpHeap::getTotalFreeSize() {
    return MEMGetAllocatableSizeForExpHeap(mHeapHandle);
}

/* 80495ee0 */ u32 ExpHeap::getAllocatableSize(s32 align) {
    return MEMGetAllocatableSizeForExpHeapEx(mHeapHandle, align);
}

/* 80495d00 */ void ExpHeap::setGroupID(u16 groupId) {
    MEMSetGroupIdForExpHeap(mHeapHandle, groupId);
}

/* 80495f00 */ u32 ExpHeap::adjust() {
    u32 adjustedSize = MEMAdjustExpHeap(mHeapHandle);
    u32 totalSize = adjustedSize + 0x34;
    if (totalSize > 0x34) {
        Heap *parent = findParentHeap();
        if (parent != nullptr) {
            parent->resizeForMBlock(mParentBlock, totalSize);
            return totalSize;   
        }
    }
    return 0;
}

// TODO marked as "idk" in Ghidra
extern "C" u32 fn_803CD4B0(const void *block);

/* 80495f80 */ u32 ExpHeap::getSizeForMBlock(const void *block) {
    return fn_803CD4B0(block);
}

/* 80495f90 */ Heap::eHeapKind ExpHeap::getHeapKind() const {
    return HEAP_KIND_EXPANDED;
}


} // namespace EGG
