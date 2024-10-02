#ifndef EGG_DISPLAY_H
#define EGG_DISPLAY_H

#include "egg/core/eggSystem.h" // for BaseSystem config
#include "egg/core/eggVideo.h"
#include "egg/prim/eggBitFlag.h"
#include "nw4r/ut.h"
#include "rvl/VI.h"

#include <common.h>

namespace EGG {

class Display {
public:
    enum EFlagBits {
        mFlag_SetClear,
        mFlag_WaitForRetrace
    };
    /* 0x00*/ TBitFlag<u8> mFlag;
    enum EScreenStateBits {
        mScreenStateFlag_SetBlack
    };

public:
    // vtable at 0x04
    /* vt 0x08 */ virtual void beginFrame();
    /* vt 0x0C */ virtual void beginRender();
    /* vt 0x10 */ virtual void endRender();
    /* vt 0x14 */ virtual void endFrame();
    /* vt 0x18 */ virtual u32 getTickPerFrame();
    /* vt 0x1C */ virtual void preVRetrace();

public:
    /* 0x08 */ u8 mMaxRetraces;
    /* 0x09 */ TBitFlag<u8> mScreenStateFlag;
    /* 0x0C */ u32 mRetraceCount;
    /* 0x10 */ u32 mFrameCount;
    /* 0x14 */ nw4r::ut::Color mClearColor;
    /* 0x18 */ u32 mClearZ;
    /* 0x1C */ s32 mBeginTick;
    /* 0x20 */ s32 mLastTick; // new
    /* 0x24 */ s32 mDeltaTick;
    /* 0x28 */ f32 mFrequency;

public:
    Display(u8 maxRetrace);
    void copyEFBtoXFB();
    void calcFrequency();
    void setBlack(bool b) {}

public:
    static u32 sTickPeriod;
};

} // namespace EGG

#endif
