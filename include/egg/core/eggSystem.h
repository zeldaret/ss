#ifndef EGG_SYSTEM_H
#define EGG_SYSTEM_H

#include "common.h"
#include "egg/core/eggVideo.h"

namespace EGG {

// TODO Proper definition of PerformanceView
class PerformanceView {
public:
    u8 placeholder[0x4c];
    /* 0x4c */ u32 _4c;
};

class Display;
class Heap;
class PerformanceView;
class SceneManager;
class SimpleAudioMgr;
class Thread;
class Video;
class XfbManager;

class ConfigurationData {
public:
    inline ConfigurationData() : mSystemHeapSize(0x89000) {}

    // vtable at 0x0
    /* vt 0x08 */ virtual Video *getVideo() = 0;
    /* vt 0x0C */ virtual Heap *getSystemHeap() = 0;
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
    /* 0x04 */ u32 mRoot1HeapStart;
    /* 0x08 */ u32 mRoot1HeapEnd;
    /* 0x0C */ u32 mRoot2HeapStart;
    /* 0x10 */ u32 mRoot2HeapEnd;
    /* 0x14 */ u32 mMemSize;
    /* 0x18 */ Heap *mRootHeapMem1;
    /* 0x1C */ Heap *mRootHeapMem2;
    /* 0x20 */ Heap *mRootHeapDebug;
    /* 0x24 */ Heap *mSystemHeap;
    /* 0x28 */ Thread *mSystemThread;
    /* 0x2C */ u32 field_0x2C;
    /* 0x30 */ u32 mSystemHeapStart;
    /* 0x34 */ u32 mSystemHeapSize;
};
class BaseSystem {
public:
    static ConfigurationData *mConfigData;

    static XfbManager *getXfbMgr() {
        return mConfigData->getXfbMgr();
    }
    static Display *getDisplay() {
        return mConfigData->getDisplay();
    }
    static Video *getVideo() {
        return mConfigData->getVideo();
    }
};

template <class TVideo, class TDisplay, class TXfbManager, class TAudioManager, class TSceneManager, class TPerfView>
class TSystem : public ConfigurationData {
public:
    inline TSystem() : mGraphicsFifoSize(0x80000), mRenderMode() {}

    void onBeginFrame() override {}
    void onEndFrame() override {}

    void initRenderMode() override {}
    void initialize() override {}

    Video *getVideo() override {
        return static_cast<Video *>(mVideo);
    }

    Heap *getSystemHeap() override {
        return static_cast<Heap *>(mSystemHeap);
    }

    Display *getDisplay() override {
        return static_cast<Display *>(mDisplay);
    }

    XfbManager *getXfbMgr() override {
        return static_cast<XfbManager *>(mXfbMgr);
    }

    PerformanceView *getPerfView() override {
        if (!mPerfView) {
            return mPerfView;
        }
        // TODO remove cast hack if/when more is known about PerformanceView
        return static_cast<PerformanceView *>((void *)&mPerfView->_4c);
    }

    SceneManager *getSceneMgr() override {
        return static_cast<SceneManager *>(mSceneMgr);
    }

    SimpleAudioMgr *getAudioMgr() override {
        return static_cast<SimpleAudioMgr *>(mAudioMgr);
    }

public:
    /* 0x38 */ u32 mGraphicsFifoSize;
    /* 0x3C */ GXRenderModeObj *mRenderMode;
    /* 0x40 */ TAudioManager *mAudioMgr;
    /* 0x44 */ TVideo *mVideo;
    /* 0x48 */ TXfbManager *mXfbMgr;
    /* 0x4c */ TDisplay *mDisplay;
    /* 0x50 */ TPerfView *mPerfView;
    /* 0x54 */ TSceneManager *mSceneMgr;
};

} // namespace EGG

#endif
