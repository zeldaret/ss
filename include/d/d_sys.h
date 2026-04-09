#ifndef D_SYS_H
#define D_SYS_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/core/eggPerformanceView.h"
#include "egg/core/eggSystem.h"
#include "nw4r/ut/ut_Color.h"

#define ROUND_UP_4KB(x) (((u32)(x) + 4096 - 1) & ~(4096 - 1))

typedef EGG::TSystem<
    EGG::Video, EGG::AsyncDisplay, EGG::XfbManager, EGG::SimpleAudioMgr, EGG::SceneManager, EGG::ProcessMeter>
    System;

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

extern void *s_OrgMEM1ArenaLo;
extern void *s_NewMEM1ArenaLo;
extern void *s_OrgMEM1ArenaHi;
extern void *s_NewMEM1ArenaHi;

void fixHeaps();

}; // namespace dSystem

#endif
