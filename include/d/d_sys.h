#ifndef D_SYS_H
#define D_SYS_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "nw4r/ut/ut_Color.h"

class dSys_c {
public:
    static void setFrameRate(u8);
    static u8 getFrameRate();

    static void setClearColor(nw4r::ut::Color clr);

    static EGG::Heap *ms_RootHeapMem1;
    static EGG::Heap *ms_RootHeapMem2;
};

#endif
