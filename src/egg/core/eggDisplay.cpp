#include "egg/core/eggDisplay.h"

#include "egg/core/eggXfbManager.h"
#include "nw4r/db/db_directPrint.h"
#include "rvl/GX.h" // IWYU pragma: export
#include "rvl/OS.h" // IWYU pragma: export

extern "C" {
void PreRetraceCallback(u32 retraceCount) {
    EGG::BaseSystem::getDisplay()->preVRetrace();
}
}
namespace EGG {

u32 Display::sTickPeriod = ((OS_BUS_CLOCK_SPEED >> 2) / 125000) * 300 >> 3;

/* 80497570 */
Display::Display(u8 maxRetrace)
    : mMaxRetraces(maxRetrace), mScreenStateFlag(0), mRetraceCount(0), mFrameCount(0), mClearColor(0x808080ff),
      mClearZ(0xFFFFFF), mBeginTick(0), mFlag(0) {
    mFlag.setBit(mFlag_SetClear);
    mFlag.setBit(mFlag_WaitForRetrace);
    nw4r::db::DirectPrint_Init();
    VISetPreRetraceCallback(PreRetraceCallback);
}

/* vt 0x18 | 804975f0 */
u32 Display::getTickPerFrame() {
    return Video::getTickPerVRetrace();
}
/* vt 0x08 | 80497600 */
void Display::beginFrame() {
    GXDrawDone();
    if (mFlag.onBit(mFlag_WaitForRetrace) ||
        (BaseSystem::getXfbMgr()->mNumXfbs == 1 && OSGetTick() - mLastTick > sTickPeriod)) {
        VIWaitForRetrace();
    }
    // Holds while the next XFB gets to its copy state
    while (BaseSystem::getXfbMgr()->mToCopyXfb == 0) {
        VIWaitForRetrace();
    }
    copyEFBtoXFB();
    calcFrequency();
    mFrameCount++;
}

/* vt 0x0C | 804976c0 */
void Display::beginRender() {
    return;
}

/* vt 0x10 | 804976d0 */
void Display::endRender() {
    return;
}

/* vt 0x14 | 804976e0 */
void Display::endFrame() {
    return;
}

/* vt 0x1C | 804976f0 */
void Display::preVRetrace() {
    mLastTick = OSGetTick();
    if (VIGetRetraceCount() - mRetraceCount < mMaxRetraces) {
        return;
    }
    if (mScreenStateFlag.onBit(mScreenStateFlag_SetBlack)) {
        BaseSystem::getVideo()->setBlack(1);
        mScreenStateFlag.resetBit(mScreenStateFlag_SetBlack);
    }
    BaseSystem::getXfbMgr()->setNextFrameBuffer();
    mRetraceCount = VIGetRetraceCount();
}

/* 804977d0 */
void Display::copyEFBtoXFB() {
    if (mFlag.onBit(mFlag_SetClear)) {
        GXSetCopyClear(mClearColor, mClearZ);
    }
    GXRenderModeObj const *renderObj = BaseSystem::getVideo()->pRenderMode;
    GXSetCopyFilter(renderObj->aa, renderObj->sample_pattern, renderObj->aa == 0, renderObj->vfilter);
    BaseSystem::getXfbMgr()->copyEFB(mFlag.onBit(mFlag_SetClear));
}

/* 80497870 */
void Display::calcFrequency() {
    s32 endTick = OSGetTick();
    mDeltaTick = endTick - mBeginTick;
    mFrequency = 1000000.0f / ((mDeltaTick * 8) / ((OS_BUS_CLOCK_SPEED >> 2) / 125000));
    mBeginTick = endTick;
}

} // namespace EGG
