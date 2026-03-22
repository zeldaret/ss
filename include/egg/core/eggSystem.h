#ifndef EGG_SYSTEM_H
#define EGG_SYSTEM_H

#include "common.h"
#include "egg/core/eggGraphicsFifo.h"
#include "egg/core/eggHeap.h"

#include "rvl/SC.h"

// Ported from https://github.com/snailspeed3/mkw/blob/master/lib/egg/core/eggSystem.hpp

namespace EGG {

class Display;
class Heap;
class PerformanceView;
class SceneManager;
class SimpleAudioMgr;
class Thread;
class Video;
class XfbManager;

class BaseSystem {
public:
    static BaseSystem *sSystem;

public:
    inline BaseSystem() : mSysHeapSize(0x97000), mGraphicsFifoSize(0x80000), mRenderMode(nullptr) {}

    /* vt 0x08 */ virtual Video *getVideo() = 0;
    /* vt 0x0C */ virtual Heap *getSysHeap() = 0;
    /* vt 0x10 */ virtual Display *getDisplay() = 0;
    /* vt 0x14 */ virtual XfbManager *getXfbMgr() = 0;
    /* vt 0x18 */ virtual PerformanceView *getPerfView() = 0;
    /* vt 0x1C */ virtual SceneManager *getSceneMgr() = 0;
    /* vt 0x20 */ virtual SimpleAudioMgr *getAudioMgr() = 0;
    /* vt 0x24 */ virtual void onBeginFrame();
    /* vt 0x28 */ virtual void onEndFrame();
    /* vt 0x2C */ virtual void initRenderMode();
    /* vt 0x30 */ virtual void initMemory();
    /* vt 0x34 */ virtual void run();
    /* vt 0x38 */ virtual void initialize() = 0;

public:
    /* 0x04 */ void *mMEM1ArenaLo;
    /* 0x08 */ void *mMEM1ArenaHi;
    /* 0x0C */ void *mMEM2ArenaLo;
    /* 0x10 */ void *mMEM2ArenaHi;
    /* 0x14 */ u32 mMemorySize;
    /* 0x18 */ Heap *mRootHeapMem1;
    /* 0x1C */ Heap *mRootHeapMem2;
    /* 0x20 */ Heap *mRootHeapDebug;
    /* 0x24 */ Heap *mSysHeap;
    /* 0x28 */ Thread *mThread;
    /* 0x2C */ void *_2c;
    /* 0x30 */ void *_30;
    /* 0x34 */ u32 mSysHeapSize;
    /* 0x38 */ u32 mGraphicsFifoSize;
    /* 0x3C */ GXRenderModeObj *mRenderMode;
};

template <class tVideo, class tDisplay, class tXfbMgr, class tAudioMgr, class tSceneMgr, class tPerfView>
class TSystem : public BaseSystem {
public:
    inline TSystem() : BaseSystem() {}

    Video *getVideo() override {
        return static_cast<Video *>(mVideo);
    }

    Heap *getSysHeap() override {
        return mSysHeap;
    }

    Display *getDisplay() override {
        return static_cast<Display *>(mDisplay);
    }

    XfbManager *getXfbMgr() override {
        return static_cast<XfbManager *>(mXfbMgr);
    }

    PerformanceView *getPerfView() override {
        return static_cast<PerformanceView *>(mPerfView);
    }

    SceneManager *getSceneMgr() override {
        return static_cast<SceneManager *>(mSceneMgr);
    }

    SimpleAudioMgr *getAudioMgr() override {
        return static_cast<SimpleAudioMgr *>(mAudioMgr);
    }

    void onBeginFrame() override {}
    void onEndFrame() override {}

    void initRenderMode() override {}

    void initialize() override {
        // DVDInit();
        // SCInit();

        // initMemory();
        // initRenderMode();

        // GraphicsFifo::create(mGraphicsFifoSize, nullptr);

        // mVideo = new tVideo(mRenderMode, nullptr);

        // mXfbMgr = new tXfbMgr(nullptr);
        // for (int i = 0; i < 2; ++i) {
        //     mXfbMgr->attach(new Xfb(mRootHeapMem2));
        // }

        // mDisplay = new tDisplay(1);

        // Thread::initialize();
        // mThread = new Thread(OSGetCurrentThread(), 4);

        // mPerfView = new tPerfView(true);

        // DvdFile::initialize();

        // CoreControllerMgr::createStaticInstance();
        // GCControllerMgr::createStaticInstance();

        // Exception_create(0x40, 0x20, 4, nullptr, 1);

        // mSceneMgr = new tSceneMgr(nullptr);

        // mRootHeapMem1->becomeCurrentHeap();

        // mAudioMgr = new tAudioMgr();
    }

public:
    /* 0x40 */ tAudioMgr *mAudioMgr;
    /* 0x44 */ tVideo *mVideo;
    /* 0x48 */ tXfbMgr *mXfbMgr;
    /* 0x4c */ tDisplay *mDisplay;
    /* 0x50 */ tPerfView *mPerfView;
    /* 0x54 */ tSceneMgr *mSceneMgr;
};

} // namespace EGG

#endif
