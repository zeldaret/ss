#include "egg/core/eggSystem.h"

#include "egg/audio/eggAudioMgr.h"
#include "egg/core/eggController.h"
#include "egg/core/eggDisplay.h"
#include "egg/core/eggExpHeap.h"
#include "egg/core/eggHeap.h"
#include "egg/core/eggPerf.h"
#include "egg/core/eggSceneManager.h"

#include "rvl/OS.h"

#define MB_TO_BYTES(mb) ((mb) * 1024 * 1024)
#define RETAIL_MEM2_SIZE MB_TO_BYTES(64)

namespace EGG {

ConfigurationData *BaseSystem::mConfigData;

void ConfigurationData::initMemory() {
    void *pMEM1ArenaLo = OSGetMEM1ArenaLo();
    void *pMEM1ArenaHi = OSGetMEM1ArenaHi();
    void *pMEM2ArenaLo = OSGetMEM2ArenaLo();
    void *pMEM2ArenaHi = OSGetMEM2ArenaHi();

    pMEM1ArenaLo = ROUND_UP_PTR(pMEM1ArenaLo, 32);
    pMEM1ArenaHi = ROUND_DOWN_PTR(pMEM1ArenaHi, 32);
    pMEM2ArenaLo = ROUND_UP_PTR(pMEM2ArenaLo, 32);
    pMEM2ArenaHi = ROUND_DOWN_PTR(pMEM2ArenaHi, 32);

    mBootInfo = &OS_BOOT_INFO;
    mSystemHeapStart = pMEM1ArenaLo;
    mRoot1HeapStart = pMEM1ArenaLo;
    mRoot1HeapEnd = pMEM1ArenaHi;
    mRoot2HeapStart = pMEM2ArenaLo;
    mRoot2HeapEnd = pMEM2ArenaHi;
    mMemSize = mBootInfo->physMemSize;

    OSSetMEM1ArenaLo(pMEM1ArenaHi);
    OSSetMEM1ArenaHi(pMEM1ArenaHi);
    OSSetMEM2ArenaLo(pMEM2ArenaHi);
    OSSetMEM2ArenaHi(pMEM2ArenaHi);

    Heap::initialize();

    void *root1HeapPtr = mRoot1HeapStart;
    u32 root1HeapSize = (u32)mRoot1HeapEnd - (u32)mRoot1HeapStart;

    void *root2HeapPtr = mRoot2HeapStart;
    u32 root2HeapSize = (u32)mRoot2HeapEnd - (u32)mRoot2HeapStart;

    u32 debugHeapSize;
    if (root2HeapSize >= RETAIL_MEM2_SIZE) {
        debugHeapSize = RETAIL_MEM2_SIZE;
        root2HeapSize = OSRoundDown32B(root2HeapSize - RETAIL_MEM2_SIZE);
    } else {
        debugHeapSize = 0;
    }
    void *debugHeapPtr = (u8 *)root2HeapPtr + root2HeapSize;

    mRootHeapMem1 = ExpHeap::create(root1HeapPtr, root1HeapSize, 0);
    mRootHeapMem1->setName("EGGRootMEM1");

    mRootHeapMem2 = ExpHeap::create(root2HeapPtr, root2HeapSize, 0);
    mRootHeapMem2->setName("EGGRootMEM2");

    if (debugHeapSize > 0) {
        mRootHeapDebug = ExpHeap::create(debugHeapPtr, debugHeapSize, 0);
        mRootHeapDebug->setName("EGGRootDebug");
    } else {
        mRootHeapDebug = nullptr;
    }

    mSystemHeap = ExpHeap::create(mSystemHeapSize, mRootHeapMem1, 0);
    mSystemHeap->setName("EGGSystem");
    mSystemHeap->becomeCurrentHeap();
}

void ConfigurationData::run() {
    CoreControllerMgr *pControllerMgr = CoreControllerMgr::instance();

    while (true) {
        getDisplay()->beginFrame();

        onBeginFrame();

        getPerfView()->measureBeginFrame();

        pControllerMgr->beginFrame();

        getDisplay()->beginRender();

        getPerfView()->measureBeginRender();

        getSceneMgr()->draw();

        getPerfView()->draw();

        getPerfView()->measureEndRender();

        getDisplay()->endRender();

        getSceneMgr()->calc();

        pControllerMgr->endFrame();

        if (getAudioMgr()) {
            getAudioMgr()->calc();
        }

        getDisplay()->endFrame();

        onEndFrame();

        getPerfView()->measureEndFrame();
    }
}

} // namespace EGG
