#include "c/c_counter.h"
#include "c/c_math.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_dvd.h"
#include "d/d_dvd_unk.h"
#include "d/d_gfx.h"
#include "d/d_hbm.h"
#include "d/d_heap.h"
#include "d/d_heap_alloc.h"
#include "d/d_pad.h"
#include "d/d_reset.h"
#include "d/d_scene.h"
#include "d/d_state.h"
#include "d/d_sys.h"
#include "d/d_sys_init.h"
#include "d/flag/flag_managers.h"
#include "d/lyt/d_lyt_battery.h"
#include "d/lyt/d_lyt_system_window.h"
#include "d/snd/d_snd_mgr.h"
#include "egg/core/eggAssertHeap.h"
#include "egg/core/eggDisplay.h"
#include "egg/core/eggExpHeap.h"
#include "egg/core/eggSystem.h"
#include "egg/core/eggVideo.h"
#include "egg/gfx/eggStateGX.h"
#include "f/f_manager.h"
#include "m/m_dvd.h"
#include "m/m_heap.h"
#include "m/m_pad.h"
#include "m/m_video.h"
#include "m/m3d/m3d.h"
#include "nw4r/ut/ut_Color.h"
#include "toBeSorted/arc_callback_handler.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/nand_request_thread.h"
#include "toBeSorted/save_manager.h"
#include "toBeSorted/unk_save_time.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/arc_managers/oarc_manager.h"

#include "rvl/GX.h"
#include "rvl/OS.h"

namespace dSystem {

void *s_OrgMEM1ArenaLo;
void *s_NewMEM1ArenaLo;
void *s_OrgMEM1ArenaHi;
void *s_NewMEM1ArenaHi;

}

System SysConfig;

System *dSys_c::ms_configuration_p;
EGG::Heap *dSys_c::ms_RootHeapMem1;
EGG::Heap *dSys_c::ms_RootHeapMem2;

// TODO Replace with proper import + names
extern u32 lbl_80574FA0;
extern u32 lbl_80574FA4;

dSndMgr_c *dSys_c::initAudioMgr(EGG::Heap *heap) {
    dSndMgr_c *audioMgr;

    EGG::FrmHeap *frmHeap = mHeap::createFrmHeap(
        0x69400,
        heap,
        "オーディオヒープ",  // "Audio heap"
        32,
        mHeap::OPT_NONE
    );

    {
        mHeap temp(frmHeap);

        audioMgr = new dSndMgr_c();
        ms_configuration_p->mAudioMgr = audioMgr;

        frmHeap->adjust();
        frmHeap->mFlag.setBit(0);
    }

    return audioMgr;
}

void dSys_c::beginRender() {
    m3d::calcMaterial();
    ms_configuration_p->mDisplay->beginRender();
}

void dSys_c::endRender() {
    ms_configuration_p->mDisplay->endRender();
}

void dSys_c::beginFrame() {
    EGG::Display *pDisplay = ms_configuration_p->mDisplay;

    pDisplay->beginFrame();
    GXSetCopyClear(pDisplay->getClearColor(), pDisplay->getClearZ());
    ms_configuration_p->onBeginFrame();
}

void dSys_c::endFrame() {
    ms_configuration_p->mDisplay->endFrame();
    ms_configuration_p->onEndFrame();
}

bool dSys_c::setBlack(bool on) {
    EGG::Display *pDisplay = ms_configuration_p->mDisplay;

    EGG::Video *pVideo = EGG::BaseSystem::getVideo();

    if (pVideo->mFlag.onBit(0) != on && pDisplay->mScreenStateFlag.offBit(0)) {
        pDisplay->mScreenStateFlag.setBit(0);
        return true;
    }

    return false;
}

void dSys_c::setFrameRate(u8 maxRetraces) {
    ms_configuration_p->mDisplay->mMaxRetraces = maxRetraces;
}

u8 dSys_c::getFrameRate() {
    return ms_configuration_p->mDisplay->mMaxRetraces;
}

void dSys_c::setClearColor(nw4r::ut::Color clr) {
    EGG::Display *display = ms_configuration_p->mDisplay;
    display->setClearColor(clr);
    EGG::StateGX::s_clearEfb = clr;
}

nw4r::ut::Color dSys_c::getClearColor() {
    return ms_configuration_p->mDisplay->getClearColor();
}

void dSys_c::create() {
    EGG::Heap *pRootHeapMem1;
    EGG::Heap *pRootHeapMem2;
    EGG::Heap *pCmdHeap;
    EGG::Heap *pExpHeap;

    dHeapAllocator::initCallbacks();

    ms_configuration_p = &SysConfig;
    EGG::BaseSystem::mConfigData = &SysConfig;

    ms_configuration_p->initialize();

    pRootHeapMem1 = EGG::BaseSystem::mConfigData->mRootHeapMem1;
    ms_RootHeapMem1 = mHeap::createExpHeap(-1, pRootHeapMem1, "dSys_c::RootHeapMEM1", 32, mHeap::OPT_4);
    pRootHeapMem1->mFlag.setBit(0);

    pRootHeapMem2 = EGG::BaseSystem::mConfigData->mRootHeapMem2;
    ms_RootHeapMem2 = mHeap::createExpHeap(-1, pRootHeapMem2, "dSys_c::RootHeapMEM2", 32, mHeap::OPT_4);
    pRootHeapMem2->mFlag.setBit(0);

    EGG::Heap *heapMem1 = ms_RootHeapMem1;
    EGG::Heap *heapMem2 = ms_RootHeapMem2;

    mVideo::create();

    mHeap::createDylinkHeap(0x1c8000, heapMem1);
    mHeap::createGameHeap1(0x250000, heapMem1);
    dHeap::createLayoutHeap(0x1e0000, heapMem1);
    mHeap::createCommandHeap(0x8000, heapMem1);
    dHeap::createWork1Heap(0x4c8000, heapMem1);
    dHeap::createWorkExHeap(0x480000, heapMem1);
    dHeap::createLayoutExHeap(0xc8000, heapMem2);
    dHeap::createLayoutEx2Heap(0x38000, heapMem1);
    mHeap::createArchiveHeap(0xb85000, heapMem2);
    dHeap::createLayoutResHeap(0x4e7000, heapMem2);
    dHeap::createWork2Heap(0xb08000, heapMem2);

    pExpHeap = EGG::ExpHeap::create(0x40000, mHeap::g_gameHeaps[0], 0);
    pExpHeap->setName("システムcreateヒープ"); // "system create heap"

    mHeap::setCurrentHeap(mHeap::g_assertHeap);

    {
        mHeap heap(pExpHeap);
        dPad::create();
    }

    dPad::fn_80059620();

    mPad::beginPad();
    mPad::g_currentCore->setPosParam(0.1f, 1.0f);
    mPad::endPad();

    {
        mHeap heap(pExpHeap);
        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            mPad::g_padMg->getNthController(i)->createRumbleMgr(4);
        }
    }

    cM::initRnd(OSGetTick());

    dGfx_c::create(pExpHeap);

    {
        mHeap heap(pExpHeap);
        dScene_c::getFader()->create();
    }

    dDvdUnk::FontUnk::create(pExpHeap);
    FileManager::create(pExpHeap);
    SaveMgr::create(pExpHeap);
    setupFlagManagers(pExpHeap);
    dCsMgr_c::create(pExpHeap);

    lbl_80574FA0 = 0;
    lbl_80574FA4 = 0;

    cCounter_c::clear();

    dDvd::create(OSGetThreadPriority(OSGetCurrentThread()) - 1, mHeap::g_commandHeap, mHeap::g_archiveHeap, pExpHeap);
    mDvd::unk_initDecompressors();
    mDvd::setAutoStreamDecomp(true);

    void *p_buf = mHeap::g_gameHeaps[0]->alloc(0x4000, 32);
    pCmdHeap = mHeap::g_commandHeap;
    NandRequestThread::create(
        OSGetThreadPriority(OSGetCurrentThread()) - 2,
        pCmdHeap,
        p_buf,
        0x4000,
        ArcCallbackHandlerCreate(2),
        pExpHeap);

    dReset::Manage_c::CreateInstance(heapMem2);

    dHeap::createHBMHeap(0x180000, heapMem2);
    dHbm::Manage_c::CreateInstance(dHeap::HBMHeap.heap);

    initAudioMgr(heapMem1);

    dSndMgr_c::GetInstance()->setup(heapMem2, 0x1000000);

    dHeap::createFontHeap(0xfa000, heapMem2);

    heapMem1->mFlag.setBit(0);
    heapMem2->mFlag.setBit(0);

    dScene_c::staticCreate();

    CurrentStageArcManager::create(pExpHeap);
    OarcManager::create(pExpHeap);
    LayoutArcManager::create(pExpHeap);

    setFrameRate(1);
    setClearColor(nw4r::ut::Color::BLACK);

    SaveTimeRelated::fn_80190780(pExpHeap);

    pExpHeap->mFlag.setBit(0);
    pExpHeap->adjust();

    mHeap::setCurrentHeap(mHeap::g_gameHeaps[0]);
}

void dSys_c::execute() {
    beginFrame();

    if (dState::fn_80062EC0()) {
        dPad::beginPad_BR();
        dPad::fn_80059630();
    }

    beginRender();
    d3d::fn_80016860();

    dDvdUnk::FontUnk::GetInstance()->preExecute();

    bool isAnyError = dDvdUnk::FontUnk::GetInstance()->isAnyError();
    if (isAnyError) {
        if (dState::fn_80062EC0()) {
            dHbm::Manage_c::GetInstance()->Calculate();
            dReset::Manage_c::GetInstance()->Calculate();
            dDvdUnk::FontUnk::GetInstance()->execute();
        }

        dDvdUnk::FontUnk::GetInstance()->drawNonDriveError();

        if (dLytSystemWindow_c::GetInstance()) {
            dLytSystemWindow_c::GetInstance()->fn_80125EF0();
        }

        SaveMgr::GetInstance()->draw();
        dDvdUnk::FontUnk::GetInstance()->drawDriveError();
        dReset::Manage_c::GetInstance()->Draw();
        dHbm::Manage_c::GetInstance()->DrawMenu(0);
    }
    else {
        dHbm::Manage_c::GetInstance()->DrawMenu(1);

        if (dHbm::Manage_c::GetInstance()->getState() == dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
            if (dState::fn_80062EC0()) {
                dHbm::Manage_c::GetInstance()->Calculate();
            }
        }
        else {
            dGfx_c::GetInstance()->drawBefore();
            m3d::drawDone(0);
            dScene_c::getFader()->draw();
            dGfx_c::GetInstance()->drawLetterbox();
            dLytBattery_c::draw();
            dDvdUnk::FontUnk::GetInstance()->execute();

            if (dLytSystemWindow_c::GetInstance()) {
                dLytSystemWindow_c::GetInstance()->fn_80125EF0();
            }

            SaveMgr::GetInstance()->draw();

            if (dState::fn_80062EC0()) {
                dHbm::Manage_c::GetInstance()->Calculate();
            }
        }

        if (dState::fn_80062EC0()) {
            dReset::Manage_c::GetInstance()->Calculate();
        }
        dReset::Manage_c::GetInstance()->Draw();
    }

    dHbm::Manage_c::GetInstance()->DrawIcon();

    endRender();

    dState::fn_80062EB0();
    dState::fn_80062E40();
    dState::fn_80062E50();

    if (!isAnyError && 
        dHbm::Manage_c::GetInstance()->getState() != dHbm::Manage_c::HBM_MANAGE_ACTIVE)
    {
        if (dSystem::myDylinkInitPhase.callMethod(nullptr) == 2) {
            if (dState::fn_80062EC0()) {
                dScene_c::staticExecute();
            }
            fManager_c::mainLoop();
        }
        
        if (dState::fn_80062EC0()) {
            dScene_c::getFader()->calc();
        }
    }

    SaveMgr::GetInstance()->execute();

    if (dLytSystemWindow_c::GetInstance()) {
        dLytSystemWindow_c::GetInstance()->fn_80125E20();
    }

    dSndMgr_c::GetInstance()->calc();
    SaveTimeRelated::GetInstance()->fn_801907C0();

    if (dState::fn_80062EC0()) {
        cCounter_c::IncrementGameFrame();
        cCounter_c::IncrementUnkCounter();
    }

    m3d::calcWorld(0);
    d3d::fn_80017500();
    d3d::clearList();

    if (dState::fn_80062EC0()) {
        dPad::endPad_BR();
    }

    endFrame();
}

void dSystem::fixHeaps() {
    s_OrgMEM1ArenaLo = OSGetMEM1ArenaLo();

    void *stack_addr = (void *)OSRoundUp32B(_stack_addr);
    void *arena_lo = (void *)OSRoundUp32B(__ArenaLo);
    
    if (s_OrgMEM1ArenaLo < (void *)0x80700000) {
        s_NewMEM1ArenaLo = (void *)0x80700000;
    }
    else {
        s_NewMEM1ArenaLo = (void *)ROUND_UP_4KB(arena_lo);

        if (s_OrgMEM1ArenaLo != stack_addr && s_OrgMEM1ArenaLo != arena_lo) {
            OSPanic("d_system.cpp", 1883, "Arena Error!\n");
        }
    }

    OSSetMEM1ArenaLo(s_NewMEM1ArenaLo);

    s_OrgMEM1ArenaHi = OSGetMEM1ArenaHi();
    s_NewMEM1ArenaHi = (void *)0x817E0000;

    if (s_OrgMEM1ArenaHi > s_NewMEM1ArenaHi) {
        OSSetMEM1ArenaHi(s_NewMEM1ArenaHi);
    }
}
