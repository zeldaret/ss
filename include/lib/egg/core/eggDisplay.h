#pragma once


#include <types.h>
#include <rvl/VI.h>
#include <nw4r/ut/Color.h>
#include <egg/prim/eggBitFlag.h>
#include <egg/core/eggSystem.h> // for BaseSystem config
#include <egg/core/eggVideo.h>


/* 80497530 */ // void PreRetraceCallback(void);

namespace EGG
{

class Display {
public:
    enum EFlagBits { mFlag_SetClear, mFlag_WaitForRetrace };
    /* 0x00*/ TBitFlag<u8> mFlag;
public:
    // vtable at 0x04 | 8056eac8
    /* vt 0x08 | 80497600 */ virtual void beginFrame();
    /* vt 0x0C | 804976c0 */ virtual void beginRender();
    /* vt 0x10 | 804976d0 */ virtual void endRender();
    /* vt 0x14 | 804976e0 */ virtual void endFrame();
    /* vt 0x18 | 804975f0 */ virtual u32 getTickPerFrame();
    /* vt 0x1C | 804976f0 */ virtual void preVRetrace();
public:
    /* 0x08 */ u8 mMaxRetraces;
    enum EScreenStateBits { mScreenStateFlag_SetBlack };
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
    /* 80497570 */ Display(u8 maxRetrace);
    /* 804977d0 */ void copyEFBtoXFB();
    /* 80497870 */ void calcFrequency();
    /*  inline  */ void setBlack(bool b) {}
public:
    /* 80497900 */ static u32 sTickPeriod;
};

} // namespace EGG
