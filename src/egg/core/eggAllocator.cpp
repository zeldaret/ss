#include "egg/core/eggAllocator.h"

#include "egg/core/eggHeap.h"

static void *AllocatorAllocForHeap_(MEMAllocator *alloc, u32 size) {
    return static_cast<EGG::Heap *>(static_cast<EGG::Allocator *>(alloc)->heap)->alloc(size, alloc->heapParam1);
}
static void AllocatorFreeForHeap_(MEMAllocator *alloc, void *block) {
    return static_cast<EGG::Heap *>(static_cast<EGG::Allocator *>(alloc)->heap)->free(block);
}

// TODO this is used from eggHeap for some reason. Figure out
// the correct privacy boundary later.
void MEMInitAllocatorFor_Heap(MEMAllocator *alloc, s32 align, void *heap) {
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

Allocator::Allocator(Heap *heap, s32 align) : mHeap(heap), align(align) {
    MEMInitAllocatorFor_Heap(this, align, heap);
}

Allocator::~Allocator() {}

void *Allocator::alloc(u32 size) {
    return Heap::alloc(size, align, mHeap);
}

void Allocator::free(void *block) {
    Heap::free(block, mHeap);
}

} // namespace EGG
