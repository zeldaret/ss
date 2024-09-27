#ifndef EGG_SYSTEM_H
#define EGG_SYSTEM_H

#include "egg/core/eggHeap.h"

#include <common.h>

namespace EGG {

class Display;
class XfbManager;
class Video;

class ConfigurationData {
public:
    // vtable at 0x0
    /* vt 0x08 */ virtual Video *getVideo() = 0;
    /* vt 0x0C */ virtual Heap *getSystemHeap() = 0;
    /* vt 0x10 */ virtual Display *getDisplay() = 0;
    /* vt 0x14 */ virtual XfbManager *getXfbMgr() = 0;
    /* vt 0x18 */ virtual void getPerfView() = 0;
    /* vt 0x1C */ virtual void getScnMgr() = 0;
    /* vt 0x20 */ virtual void getAudioMgr() = 0;
    /* vt 0x24 */ virtual void onBeginFrame();
    /* vt 0x28 */ virtual void onEndFrame();
    /* vt 0x2C */ virtual void initRenderMode();
    /* vt 0x30 */ virtual void initMemory();
    /* vt 0x34 */ virtual void run();

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
    /* 0x38 */ u32 mGraphicsFifoSize;
    /* 0x3C */ u32 field_0x3C;
};
class BaseSystem {
public:
    static ConfigurationData *mConfigData;
    static XfbManager *getXfbMgr() { return mConfigData->getXfbMgr(); }
    static Display *getDisplay() { return mConfigData->getDisplay(); }
    static Video *getVideo() { return mConfigData->getVideo(); }
};

template <class TVideo, class TDisplay, class TXfbManager, class TAudioManager, class TSceneManager, class TPerfView>
class TSystem : ConfigurationData {};

} // namespace EGG

#endif
