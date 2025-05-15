#include "d/d_s_boot.h"

#include "common.h"
#include "d/d_base.h"
#include "d/d_d2d.h"
#include "d/d_dvd_unk.h"
#include "d/d_dylink.h"
#include "d/d_font_manager.h"
#include "d/d_gfx.h"
#include "d/d_heap.h"
#include "d/d_message.h"
#include "d/d_scene.h"
#include "d/d_sys.h"
#include "d/lyt/d_lyt_battery.h"
#include "d/lyt/d_lyt_system_window.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "m/m2d.h"
#include "m/m_dvd.h"
#include "m/m_fader_base.h"
#include "m/m_heap.h"
#include "m/m_pad.h"
#include "rvl/TPL/TPL.h"
#include "s/s_FPhase.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_hbm.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_particle.h"
#include "toBeSorted/fi_context.h"
#include "toBeSorted/reload_color_fader.h"
#include "toBeSorted/save_manager.h"
#include "toBeSorted/save_related.h"
#include "toBeSorted/special_item_drop_mgr.h"

sFPhase<dScBoot_c>::phaseCallback dScBoot_c::sCallbacks[] = {&dScBoot_c::cb1, &dScBoot_c::cb2, &dScBoot_c::cb3,
                                                             &dScBoot_c::cb4, &dScBoot_c::cb5, &dScBoot_c::cb9,
                                                             &dScBoot_c::cb6, &dScBoot_c::cb7, &dScBoot_c::cb8,
                                                             nullptr};

// 40 seconds
#define SC_BOOT_STRAP_MAX_DURATION 1200
// 2 seconds
#define SC_BOOT_STRAP_MIN_DURATION 60

sFPhaseBase::sFPhaseState dScBoot_c::executeLoadPhase() {
    return mPhases.step();
}

extern "C" u8 getUsedLanguageNTSCNum();
extern "C" u8 fn_80054F30();

sFPhaseBase::sFPhaseState dScBoot_c::cb1() {
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("cursor", mHeap::g_archiveHeap);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("CursorStick", mHeap::g_archiveHeap);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("System2D", nullptr);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("saveBannerU", nullptr);

    for (int i = 0; i < 6; i++) {
        SizedString<128> str;
        str.sprintf("%s/%s", dMessage_c::getLanguageIdentifier(), dMessage_c::getArcNameByIndex(i, true));
        OarcManager::GetInstance()->loadObjectArcFromDisk(str, mHeap::g_archiveHeap);
    }
    OarcManager::GetInstance()->loadObjectArcFromDisk("System", mHeap::g_archiveHeap);
    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScBoot_c::cb2() {
    if (OarcManager::GetInstance()->ensureAllEntriesLoaded()) {
        return sFPhaseBase::PHASE_RETRY;
    }
    if (LayoutArcManager::GetInstance()->ensureAllEntriesLoaded()) {
        return sFPhaseBase::PHASE_RETRY;
    }

    TPLPalette *tpl =
        static_cast<TPLPalette *>(LayoutArcManager::GetInstance()->getData("saveBannerU", "tmp/saveBanner.tpl"));
    TPLBind(tpl);

    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScBoot_c::cb3() {
    return !dFontMng_c::create() ? sFPhaseBase::PHASE_RETRY : sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScBoot_c::cb4() {
    if (!dHbm_c::GetInstance()->fn_801967D0()) {
        return sFPhaseBase::PHASE_RETRY;
    }
    dHeap::HBMHeap.heap->disableAllocation();
    return sFPhaseBase::PHASE_NEXT;
}

void *dScBoot_c::dvdCallback(void *data) {}

sFPhaseBase::sFPhaseState dScBoot_c::cb5() {
    if (mpDvdCallback == nullptr) {
        mpDvdCallback = mDvd_callback_c::createOrDie(&dvdCallback, nullptr);
        if (mpDvdCallback == nullptr) {
            return sFPhaseBase::PHASE_RETRY;
        }
    }
    if (mpDvdCallback->mStatus == 0) {
        return sFPhaseBase::PHASE_RETRY;
    } else {
        mpDvdCallback->do_delete();
        return sFPhaseBase::PHASE_NEXT;
    }
}

sFPhaseBase::sFPhaseState dScBoot_c::cb6() {
    dDyl::initRelsArc();
    OarcManager::GetInstance()->loadObjectArcFromDisk("JpaCommon", dHeap::workExHeap.heap);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("Main2D", nullptr);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("DoButton", nullptr);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("MenuHelp", nullptr);
    u8 result = fn_80054F30();
    s32 gameOverType;
    if (result == 3) {
        gameOverType = 1;
    } else {
        gameOverType = result == 4 ? 2 : 0;
    }

    if (gameOverType == 0) {
        LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("gameOver_01", nullptr);
    } else if (gameOverType == 1) {
        LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("gameOver_02", nullptr);
    } else {
        LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("gameOver_03", nullptr);
    }

    if (OarcManager::GetInstance()->checkIfObjectArcExistsOnDisk("ObjectPack")) {
        OarcManager::GetInstance()->loadObjectArcFromDisk("ObjectPack", mHeap::g_archiveHeap);
    } else {
        static const char *OBJECT_PACK_OARCS[] = {
            "Camera",
            "Common",
            "Alink",
            "PLCommonAnime",
            "PLCommonAnmSnd",
            "PLCompItem",
            "SwordGirl",
            "GrassA",
            "GrassB",
            "GrassC",
            "GrassD",
            "GrassE",
            "FlowerA00",
            "FlowerB00",
            "FlowerB01",
            "UI_Data",
            "GetBomb",
            "Zelda",
            "Zelda_face",
            "Zelda_handL",
            "Zelda_handR",
            "Zelda_hair",
            "Rival",
            "Bird_Link",
            "FX_WaterColumn",
            "GetBottleWater",
            "PutRupee",
            "GetRupee",
            "PutHeart",
            "GetHeart",
            "PutArrow",
            "GetArrow",
            "PutBombSet",
            "GetBombSet",
            "PutSeed",
            "GetSeed",
            "GetSozaiE",
            "GetSozaiI",
            "GetSozaiJ",
            "GetSozaiM",
            "PutFairy",
            "GetFairy",
            "GetBottleFairy",
            "TBoxNormalT",
            "TBoxSmallT",
            "TBoxBossT",
            "GoddessTBox",
            "FX_TBoxOpen",
            "InsectButterfly",
            "GetInsectButterfly",
            "FlowerHeart",
            "Tubo",
            "GetGenki",
            "PutHeartKakera",
            "GetHeartKakera",
            "GrassOcta",
            "Kanban",
            "KanbanTree",
            "SaveObjectA",
            "SaveObjectB",
            "SaveObjectC",
            "KibakoHang",
            "ShotMark",
            "GetBottleEmpty",
            "GoddessCube",
            "GetGuts",
            "PutGuts",
            "RockCarryMiddle",
            "RockCarrySmall",
            "RockRollB",
            "BRockWall00",
            "BRockWall",
            "BRockHole00",
            "Bee",
            "GetInsectBee",
            "MogumaMud",
            "FlowerBomb",
            "FenceBoko",
            "ShutterFencedAnim",
            "ShutterFenced00",
            "SwitchHit",
            "KanbanStone",
            "GetMap",
            "GetKeySmall",
            "PutKeySmall",
            "MoundShovel",
            "MudShovel",
            "HoleShovel",
            "WiiRemocon",
            "ShutterChain",
            "LockSmall",
            "GetBottleKinokoA",
            "GetBottleKinokoB",
            "DoorBoss",
            "SwitchWall",
            "Candle00",
            "SkyCmn",
            "Bc",
        };
        for (int i = 0; i < ARRAY_LENGTH(OBJECT_PACK_OARCS); i++) {
            OarcManager::GetInstance()->loadObjectArcFromDisk(OBJECT_PACK_OARCS[i], mHeap::g_archiveHeap);
        }
    }

    dDvdUnk::FontUnk::GetInstance()->setField_0x2C(1);
    field_0x5DF = 1;
    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScBoot_c::cb7() {
    if (!dDyl::destroy()) {
        return sFPhaseBase::PHASE_RETRY;
    }

    static u16 profilesToLoad[] = {
        // TODO: Why these in particular?
        fProfile::PL_RESTART,
        fProfile::SW_AREA_TAG,
        fProfile::OBJ_SWHIT,
        fProfile::BOMBF,
    };

    bool allOk = true;
    for (int i = 0; i < ARRAY_LENGTH(profilesToLoad); i++) {
        if (!dDyl::isLinked(profilesToLoad[i]) && dDyl::tryLink(profilesToLoad[i]) != 1) {
            allOk = false;
        }
    }

    if (!allOk) {
        return sFPhaseBase::PHASE_RETRY;
    }

    if (OarcManager::GetInstance()->ensureAllEntriesLoaded()) {
        return sFPhaseBase::PHASE_RETRY;
    }

    if (LayoutArcManager::GetInstance()->ensureAllEntriesLoaded()) {
        return sFPhaseBase::PHASE_RETRY;
    }

    void *naviDat = OarcManager::GetInstance()->getData("System", "dat/navi_table.dat");
    FiContext::initialize(naviDat);
    dParticle::mgr_c::create(dHeap::work1Heap.heap, 0xBB8, 0xFA, 0xF);
    dJEffManager_c::createEffManagers();
    void *jpc = OarcManager::GetInstance()->getData("JpaCommon", "dat/Common.jpc");
    void *jpn = OarcManager::GetInstance()->getData("JpaCommon", "dat/Common.jpn");
    dParticle::mgr_c::GetInstance()->createResource(dHeap::workExHeap.heap, 0, jpc, jpn);

    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScBoot_c::cb8() {
    return sFPhaseBase::PHASE_NEXT;
}
extern "C" void fn_80059E90();
sFPhaseBase::sFPhaseState dScBoot_c::cb9() {
    dBase_c::createRoot(fProfile::MESSAGE, 0, dBase_c::OTHER);
    dBase_c::createRoot(fProfile::C_BASE, 0, dBase_c::OTHER);
    dLytSystemWindow_c::create();
    SaveMgr::GetInstance()->createSaveMsgWindow();
    fn_80059E90();
    dLytBattery_c::create();
    return sFPhaseBase::PHASE_NEXT;
}

SPECIAL_BASE_PROFILE(BOOT, dScBoot_c, fProfile::BOOT, 0x2A7, 0);

STATE_DEFINE(dScBoot_c, Init);
STATE_DEFINE(dScBoot_c, Strap);
STATE_DEFINE(dScBoot_c, Connect);
STATE_DEFINE(dScBoot_c, Save);

void dScBoot_c::drawCallback() {
    d2d::draw();
    if (sInstance != nullptr) {
        sInstance->mFader.draw();
    }
}

dScBoot_c::strap_c::strap_c() {
    SizedString<8> str;
    u8 langNum = getUsedLanguageNTSCNum();
    mArcName.mChars[0] = '\0';
    mLytFileName.mChars[0] = '\0';
    mAnimFileName.mChars[0] = '\0';
    if (langNum == 3) {
        str = "F";
    } else if (langNum == 4) {
        str = "S";
    } else {
        str = "U";
    }
    // UB: Cannot pass object of non-POD type 'SizedString<8>' through variadic method
    mArcName.sprintf("strap%s", str);
    mLytFileName.sprintf("strap_00_%s.brlyt", str);
    mAnimFileName.sprintf("strap_00_%s_loop.brlan", str);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk(mArcName, dHeap::work2Heap.heap);
    field_0x4EC = 0;
}

bool dScBoot_c::strap_c::create() {
    if (!field_0x4EC) {
        if (LayoutArcManager::GetInstance()->ensureLoaded1(mArcName)) {
            return false;
        }
        void *data = LayoutArcManager::GetInstance()->getLoadedData(mArcName);
        // HACK: Why does this use the m2d attach function?
        // Inlines break instruction scheduling, and I don't
        // want to undo shadowing...
        ((m2d::ResAccIf_c *)&mResAcc)->attach(data, "");
        mLyt.setResAcc(&mResAcc);
        mLyt.build(mLytFileName, nullptr);
        mAnm.doSomething(mAnimFileName, &mResAcc);
        mLyt.bind(&mAnm);
        field_0x4EC = 1;
        field_0x4EE = 0;
        field_0x4F0 = 0;
    }

    return true;
}

bool dScBoot_c::strap_c::remove() {
    mLyt.unbind(&mAnm);
    mAnm.destroySomething();
    // HACK: See above
    ((m2d::ResAccIf_c *)&mResAcc)->detach();
    LayoutArcManager::GetInstance()->decrement(mArcName);
    return true;
}

bool dScBoot_c::strap_c::execute() {
    // TODO Button mask
    if (field_0x4F0 == 0 && mPad::g_currentCore->downTrigger(0xF0F)) {
        field_0x4F0 = 1;
    }
    mAnm.play();
    mLyt.calc();
    if (field_0x4EE <= SC_BOOT_STRAP_MAX_DURATION) {
        field_0x4EE++;
    }
    if (!(field_0x4EE <= SC_BOOT_STRAP_MAX_DURATION) ||
        !(field_0x4EE <= SC_BOOT_STRAP_MIN_DURATION || field_0x4F0 == 0)) {
        return true;
    } else {
        return false;
    }
}

void dScBoot_c::strap_c::draw() {
    mLyt.addToDrawList();
}

void dScBoot_c::strap_c::init() {
    mAnm.setFrame(0.0f);
    mLyt.calc();
    field_0x4EE = 0;
    field_0x4F0 = 0;
}

dScBoot_c::dScBoot_c() : mStateMgr(*this, sStateID::null), mPhases(this, sCallbacks), mpDvdCallback(nullptr) {
    field_0x5DF = 0;
    sInstance = this;
    field_0x5E1 = 0;
    field_0x5E2 = 0;
    field_0x5E3 = 0;
    field_0x5E4 = 0;
}

dScBoot_c::~dScBoot_c() {
    dHbm_c::GetInstance()->fn_80197560(0);
    sInstance = nullptr;
}

int dScBoot_c::create() {
    if (!mStrapScreen.create()) {
        return NOT_READY;
    };

    mFader.create();
    mFader.setFadeInFrame(1);
    dGfx_c::GetInstance()->setDrawCallback(&drawCallback);
    dSys::setFrameRate(1);
    mStateMgr.changeState(StateID_Init);
    SaveRelated::create();
    return SUCCEEDED;
}

int dScBoot_c::doDelete() {
    int removed = mStrapScreen.remove();
    sFPhaseBase::sFPhaseState phaseState = mPhases.step();
    if (!removed || phaseState != sFPhaseBase::PHASE_ALL_DONE) {
        return NOT_READY;
    }
    SaveRelated::remove();
    dBase_c::createRoot(fProfile::LAST, 0, 0);
    d3d::createLightTextures();
    SpecialItemDropMgr::create();
    dHbm_c::GetInstance()->offFlags(8);
    dGfx_c::GetInstance()->setDrawCallback(nullptr);
    ReloadColorFader::GetInstance()->fn_80067DD0(true);
    return SUCCEEDED;
}

int dScBoot_c::execute() {
    mFader.calc();
    mStateMgr.executeState();
    sFPhaseBase::sFPhaseState state = executeLoadPhase();
    if (field_0x5DE == 0 && state == sFPhaseBase::PHASE_ALL_DONE) {
        field_0x5DE = 1;
    }

    return SUCCEEDED;
}

int dScBoot_c::draw() {
    if (field_0x5E1 == 1) {
        mStrapScreen.draw();
    }

    return SUCCEEDED;
}

void dScBoot_c::deleteReady() {
    // TODO
}

void dScBoot_c::initializeState_Init() {
    mProgressStage = 0;
    field_0x5D8 = 0;
    field_0x5DC = 0;
    field_0x5DD = 0;
    field_0x5DE = 0;
    field_0x5E0 = 0;
}

void dScBoot_c::executeState_Init() {
    mStateMgr.changeState(StateID_Strap);
}

void dScBoot_c::finalizeState_Init() {}

void dScBoot_c::initializeState_Strap() {
    mProgressStage = 0;
    field_0x5E1 = 1;
    mFader.fadeIn();
    mFader.resetFrames();
}

void dScBoot_c::executeState_Strap() {
    if (dScene_c::sFader.isStatus(mFaderBase_c::FADED_IN)) {
        switch (mProgressStage) {
            case 0:
                if (!checkDone() && mFader.isNotStatus(mFaderBase_c::FADING_IN)) {
                    mProgressStage = 1;
                }
                break;
            case 1:
                if (!checkDone()) {
                    bool ok = mStrapScreen.execute();
                    if (ReloadColorFader::GetInstance()->field_0x0C == 1) {
                        ok = false;
                    }

                    if (ok) {
                        field_0x5DC = 1;
                        if (mFader.fadeOut() == true) {
                            mProgressStage = 2;
                        }
                    }
                }
                break;
            case 2:
                if (!checkDone() && mFader.isNotStatus(mFaderBase_c::FADING_OUT)) {
                    if (mFader.fadeIn() == true) {
                        mProgressStage = 3;
                        field_0x5E1 = 0;
                    }
                }
                break;
            case 3:
                if (!checkDone() && mFader.isNotStatus(mFaderBase_c::FADING_IN)) {
                    mStateMgr.changeState(StateID_Connect);
                }
                break;
            case 4:
                mFader.setStatus(mFaderBase_c::FADED_OUT);
                if (mFader.fadeIn() == true) {
                    field_0x5E4 = 1;
                    if (field_0x5DC != 0) {
                        field_0x5E1 = 0;
                        mStateMgr.changeState(StateID_Connect);
                    } else {
                        mProgressStage = 0;
                        mStrapScreen.init();
                    }
                    dHbm_c::GetInstance()->fn_80197560(0);
                }
                break;
        }
    }
}

void dScBoot_c::finalizeState_Strap() {
    // TODO
    dGfx_c::GetInstance()->setField0x09(1);
}

void dScBoot_c::initializeState_Connect() {
    mProgressStage = 0;
    dSys::setFrameRate(2);
}

void dScBoot_c::executeState_Connect() {
    // TODO
}

void dScBoot_c::finalizeState_Connect() {}

void dScBoot_c::initializeState_Save() {
    mProgressStage = 0;
    field_0x5D8 = 0;
    SaveRelated::GetInstance()->fn_80015F40();
}

void dScBoot_c::executeState_Save() {
    // TODO
}

void dScBoot_c::finalizeState_Save() {}

bool dScBoot_c::checkDone() {
    if (field_0x5E3 == 1) {
        field_0x5E3 = 0;
        mProgressStage = 4;
        return 1;
    }
    return 0;
}
