#ifndef D_SYS_H
#define D_SYS_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "nw4r/ut/ut_Color.h"

class dSys_c {
public:
    static void setBlack(bool);
    /* Frame rate values: 1 - 60fps, 2 - 30fps */
    static void setFrameRate(u8);
    static u8 getFrameRate();

    static void setClearColor(nw4r::ut::Color clr);

    static void create();
    static void execute();

    static EGG::Heap *ms_RootHeapMem1;
    static EGG::Heap *ms_RootHeapMem2;
};

namespace dSystem {
void fixHeaps();
};

#endif
