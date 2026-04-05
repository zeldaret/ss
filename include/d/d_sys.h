#ifndef D_SYS_H
#define D_SYS_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/core/eggSystem.h"
#include "nw4r/ut/ut_Color.h"

typedef EGG::TSystem<EGG::Video, EGG::Display, EGG::XfbManager, EGG::SimpleAudioMgr, EGG::SceneManager, EGG::PerformanceView> System;

class dSndMgr_c;

class dSys_c {
public:
    static dSndMgr_c *initAudioMgr(EGG::Heap *heap);

    static void beginRender();
    static void endRender();
    static void beginFrame();
    static void endFrame();

    static bool setBlack(bool);
    /* Frame rate values: 1 - 60fps, 2 - 30fps */
    static void setFrameRate(u8);
    static u8 getFrameRate();

    static void setClearColor(nw4r::ut::Color clr);
    static nw4r::ut::Color getClearColor();

    static void create();
    static void execute();

public:
    static System *ms_configuration_p;
    static EGG::Heap *ms_RootHeapMem1;
    static EGG::Heap *ms_RootHeapMem2;
};

namespace dSystem {

void fixHeaps();

};

#endif
