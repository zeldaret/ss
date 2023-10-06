#pragma once

#include "types.h"
#include "rvl/MEM.h"
#include "egg/core/eggHeap.h"

// /* 80495310 */ MEMInitAllocatorFor_Heap(MEMAllocator* alloc, s32 align, void* heap);
// /* 804952f0 */ MEM_AllocFor_Heap(MEMAllocator* alloc, void* block);
// /* 804952d0 */ MEM_AllocFor_Heap(MEMAllocator* alloc, u32 size, s32 align);
namespace EGG {
    class Heap;
    class Allocator : public MEMAllocator {
    public:
        /* 80495330 */ Allocator(Heap* heap, s32 align);
    public:
        /* vt 0x08 | 80495380 */ virtual ~Allocator();
        /* vt 0x0C | 804953c0 */ virtual void* alloc(u32 size);
        /* vt 0x10 | 804953e0 */ virtual void free(void* block);

        inline MEMAllocator* getHandle() { return static_cast<MEMAllocator*>(this); }
        /* 0x14 */ Heap* mHeap;
        /* 0x18 */ s32 align;
    };
};