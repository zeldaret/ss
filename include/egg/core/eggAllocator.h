#ifndef EGG_ALLOCATOR_H
#define EGG_ALLOCATOR_H
#include "rvl/MEM.h"

#include <common.h>

void MEMInitAllocatorFor_Heap(MEMAllocator *alloc, s32 align, void *heap);

namespace EGG {
class Heap;
class Allocator : public MEMAllocator {
public:
    Allocator(Heap *heap, s32 align);

public:
    /* vt 0x08 */ virtual ~Allocator();
    /* vt 0x0C */ virtual void *alloc(u32 size);
    /* vt 0x10 */ virtual void free(void *block);

    inline MEMAllocator *getHandle() {
        return static_cast<MEMAllocator *>(this);
    }
    /* 0x14 */ Heap *mHeap;
    /* 0x18 */ s32 align;
};
}; // namespace EGG

#endif
