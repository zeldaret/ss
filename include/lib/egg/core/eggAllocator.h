#pragma once

#include <rvl/MEM.h>
#include <egg/core/eggHeap.h>

// Ghidra: Allocator
//   size: 0x1C
//   .text: [0x804952d0, 0x804953f0]
//   .data: [0x8056e8d0, 0x8056e8e8] # Allocator vtable
//  sdata2: [0x8057f2f0, 0x8057f2f8] # MEMAllocator func table

namespace EGG {
    class Heap;
    class Allocator : public MEMAllocator {
    public:
        Allocator(Heap* heap, s32 align);
        // vtable at 0x10
        /* 0x08 */ virtual ~Allocator();
        /* 0x0C */ virtual void* alloc(u32 size);
        /* 0x10 */ virtual void free(void* block);

        inline MEMAllocator* getHandle() { return static_cast<MEMAllocator*>(this); }
    private:
        /* 0x14 */ Heap* mHeap;
        /* 0x18 */ s32 align;
    };
};