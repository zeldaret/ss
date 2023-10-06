#pragma once

// This file was adapted from https://github.com/riidefi/mkw/blob/master/source/egg/core/eggDisposer.hpp 

#include "nw4r/ut/List.h"
#include "types.h"

namespace EGG {

class Heap;
class Disposer : private NonCopyable {
    friend class Heap;
protected:
    /* vt 0x08 | 80496890 */ virtual ~Disposer();
    /* 80496830 */ Disposer();
public:
    enum eLifetime {
        LIFETIME_UNMANAGED,
        LIFETIME_HEAP_GC
    };
    inline eLifetime getLifetime() const {
        return mContainHeap != nullptr ? LIFETIME_HEAP_GC : LIFETIME_UNMANAGED;
    }

private:
    /* 0x04 */ Heap* mContainHeap;
    /* 0x08 */ nw4r::ut::Node mList;
};

// TODO: Add singleton define for the T__Disposer stuff. 
// see https://github.com/open-ead/sead/blob/master/include/heap/seadDisposer.h 
// for reference

};