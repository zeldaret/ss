#pragma once

// this file was ported from https://github.com/riidefi/mkw/blob/master/source/egg/core/eggAllocator.hpp

#include <lib/rvl/MEM.h>
#include <lib/egg/core/eggHeap.h>

namespace EGG {
    class Heap;
    // Ghidra: Allocator
    //   size: 0x1C
    //   .text: [0x804952d0, 0x804953f0]
    //   .data: [0x8056e8d0, 0x8056e8e8] # Allocator vtable
    //  sdata2: [0x8057f2f0, 0x8057f2f8] # MEMAllocator func table
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