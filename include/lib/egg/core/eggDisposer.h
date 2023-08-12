#pragma once

// This file was adapted from https://github.com/riidefi/mkw/blob/master/source/egg/core/eggDisposer.hpp 

#include <nw4r/ut/utList.h>
#include <types.h>

namespace EGG {

    class Heap;

    // Ghidra: EggBase
    //   size: 0x8
    //  .text: [0x80496830, 0x80496910]
    class Disposer : private NonCopyable {
        friend class Heap;
    protected:
        virtual ~Disposer();
        Disposer();
    public:
      enum eLifetime {
            LIFETIME_UNMANAGED,
            LIFETIME_HEAP_GC
        };
        inline eLifetime getLifetime() const {
            return mContainHeap != nullptr ? LIFETIME_HEAP_GC : LIFETIME_UNMANAGED;
        }

    private:
        Heap* mContainHeap;
        nw4r::ut::Node mList;
    };
};