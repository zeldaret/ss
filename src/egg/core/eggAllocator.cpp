#include "egg/core/eggAllocator.h"

#include "egg/core/eggHeap.h"

/* 804952d0 */ static void *AllocatorAllocForHeap_(MEMAllocator *alloc, u32 size) {
    return static_cast<EGG::Heap *>(static_cast<EGG::Allocator *>(alloc)->heap)->alloc(size, alloc->heapParam1);
}
/* 804952f0 */ static void AllocatorFreeForHeap_(MEMAllocator *alloc, void *block) {
    return static_cast<EGG::Heap *>(static_cast<EGG::Allocator *>(alloc)->heap)->free(block);
}

// TODO this is used from eggHeap for some reason. Figure out
// the correct privacy boundary later.
/* 80495310 */ void MEMInitAllocatorFor_Heap(MEMAllocator *alloc, s32 align, void *heap) {
    static const MEMAllocatorFuncs sAllocatorFunc = {
        &AllocatorAllocForHeap_,
        &AllocatorFreeForHeap_,
    };

    alloc->funcs = &sAllocatorFunc;
    alloc->heap = heap;
    alloc->heapParam1 = align;
    alloc->heapParam2 = 0;
}

namespace EGG {

/* 80495330 */ Allocator::Allocator(Heap *heap, s32 align) : mHeap(heap), align(align) {
    MEMInitAllocatorFor_Heap(this, align, heap);
}

/* 80495380 */ Allocator::~Allocator() {}

/* 804953c0 */ void *Allocator::alloc(u32 size) {
    return Heap::alloc(size, align, mHeap);
}

/* 804953e0 */ void Allocator::free(void *block) {
    Heap::free(block, mHeap);
}

} // namespace EGG
