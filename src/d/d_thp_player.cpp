
#include "common.h"
#include "d/d_sys.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "m/m_color.h"
#include "m/m_video.h"
#include "rvl/OS/OSError.h"
#include "nw4r/ut/ut_LockedCache.h"
#include "m/m_heap.h"
#include "m/m3d/m3d.h"
#include "m/m_dvd.h"
#include "d/d_heap.h"
#include "THPPlayer/THPPlayer.h"
#include "d/d_thp_player.h"
#include "d/d_gfx.h"

SPECIAL_BASE_PROFILE(THPPLAYER, dThpPlayer_c, fProfile::THPPLAYER, 0x3, 0x400);

int dThpPlayer_c::create() {
    reallyCreate(mVideo::m_video->pRenderMode->fbWidth, mVideo::m_video->pRenderMode->efbHeight);
    return SUCCEEDED;
}

int dThpPlayer_c::doDelete() {
  if (destroy() == 0) {
    return NOT_READY;
} else {
    THPPlayerQuit();
    return SUCCEEDED;
  }
}
int dThpPlayer_c::execute() {
    reallyExecute();
    return SUCCEEDED;
}
int dThpPlayer_c::draw() {
    dSys_c::setClearColor(mColor(0xFF));
    mScreen.addToDrawList();
    return SUCCEEDED;
}

dThpPlayer_c::dThpPlayer_c(): mDvdCallback(), mFilename(), mLoadAllAtOnce(0), mFilenameSet(0), mSetupDone(0), isPlaying(0), mBuffer(nullptr), mScreen(), mSpecialType(3) {}

dThpPlayer_c::~dThpPlayer_c() {
    if (mBuffer != nullptr) {
        dHeap::work2Heap.heap->free(mBuffer);
        mBuffer = nullptr;
    }
}

void dThpPlayer_c::reallyCreate(u16 efbWidth, u16 efbHeight) {
    THPPlayerInit(1);
    isPlaying = 0;
    mBuffer = nullptr;
    mScreen.setEfbWidth(efbWidth);
    mScreen.setEfbHeight(efbHeight);
    m3d::resetMaterial();
}

extern "C" {
    s32 THPVideoDecode(s32 p1, s32 p2, s32 p3, s32 p4, s32 p5);
}

void drawTHPFrame(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6) {
    THPPlayerDrawCurrentFrame(p1, p2, p3, p4, p5, p6);
    THPPlayerPostDrawDone();
}

static s32 callback(void* player) {
    ((dThpPlayer_c*) player)->open();
    return 1;
}

void dThpPlayer_c::open() {
    if (!THPPlayerOpen(mFilename.mChars, mLoadAllAtOnce)) {
        mFilenameSet = 0;
    }
}

void dThpPlayer_c::setupFile(char* filename, s32 loadAllAtOnce) {
    if (destroy()) {
        mFilenameSet = 1;
        mFilename = filename;
        mLoadAllAtOnce = loadAllAtOnce;
        mDvdCallback = mDvd_callback_c::createOrDie((dvdReadCallback)callback, this);
    }
}

static const u8 SPECIAL_TYPES[] = {0x7A, 0x78, 0x78};

void dThpPlayer_c::reallyExecute() {
    if (mDvdCallback == nullptr || mDvdCallback->mStatus == 0 || !mFilenameSet || mSetupDone) {
        return;
    }
    if (mFilename == "THP/Demo01_01.thp") {
        mSpecialType = 0;
    } else if (mFilename == "THP/Demo01_02.thp") {
        mSpecialType = 1;
    } else if (mFilename == "THP/Demo78_01.thp") {
        mSpecialType = 2;
    }
    if (mSpecialType != 3) {
        THPPlayerSetVolume(SPECIAL_TYPES[mSpecialType], 0);
    }
    THPPlayerGetVideoInfo(&mVideoComponent);
    THPPlayerGetAudioInfo(&mAudioComponent);
    mScreen.setVideoDimensions(
        mVideoComponent.xSize,
        (mVideoComponent.ySize * (dGfx_c::getEFBHeightF() / dGfx_c::getCurrentScreenHeightF()))
    );
    mBuffer = (u8*)dHeap::work2Heap.heap->alloc(THPPlayerCalcNeedMemory(), 0x20);
    if (mBuffer == nullptr) {
        #line 279
        OSError("Can\'t allocate memory.");
    } else {
        THPPlayerSetBuffer(mBuffer);
        if (THPPlayerPrepare(0, 0, 0) == 0) {
            #line 288
            OSError("Fail to prepare.");
        } else {
            mSetupDone = 1;
        }
    }
}

s32 dThpPlayer_c::play() {
    if (!mFilenameSet) {
        return true;
    } else if (!mSetupDone) {
        return false;
    } else {
        THPPlayerPlay();
        isPlaying = 1;
        return true;
    }
}

s32 dThpPlayer_c::destroy() {
    if (mDvdCallback != nullptr) {
        if (mDvdCallback->mStatus == 0) {
            return NOT_READY;
        }
        THPPlayerStop();
        THPPlayerClose();
        if (mBuffer != nullptr) {
            dHeap::work2Heap.heap->free(mBuffer);
            mBuffer = nullptr;
        }
        mDvdCallback->do_delete();
        mDvdCallback = nullptr;
        mFilenameSet = 0;
        mSetupDone = 0;
        isPlaying = 0;
    }
    return SUCCEEDED;
}

s32 dThpPlayer_c::isDone() {
  return THPPlayerGetState() == 3;
}

void ThpVideoScreen::draw() {
    drawTHPFrame(mEfbWidth, mEfbHeight, (mEfbWidth - mVideoWidth) / 2, (mEfbHeight - mVideoHeight) / 2, mVideoWidth, mVideoHeight);
}

extern "C" s32 fn_80273B50(s32 p1, s32 p2, s32 p3, s32 p4, s32 p5) {
    nw4r::ut::LC::Lock();
    s32 result = THPVideoDecode(p1, p2, p3, p4, p5);
    nw4r::ut::LC::Unlock();
    return result;
}
