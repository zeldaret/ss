#include "d/d_s_boot.h"

#include "common.h"
#include "d/d_base.h"
#include "d/d_dylink.h"
#include "d/d_font_manager.h"
#include "d/d_heap.h"
#include "d/d_message.h"
#include "d/d_scene.h"
#include "d/d_sys.h"
#include "d/lyt/d_lyt_battery.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "m/m2d.h"
#include "m/m_dvd.h"
#include "m/m_fader_base.h"
#include "m/m_heap.h"
#include "m/m_pad.h"
#include "nw4r/ut/ut_ResFont.h"
#include "rvl/TPL/TPL.h"
#include "s/s_FPhase.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/hbm.h"
#include "toBeSorted/some_gxf_thing.h"
#include "toBeSorted/reload_color_fader.h"

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
extern "C" const char *getEventFlowFileNameByIndex(int, int);
extern "C" u8 fn_80054F30();
extern "C" nw4r::ut::ResFont *lbl_805750D8;

sFPhaseBase::sFPhaseState dScBoot_c::cb1() {
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("cursor", mHeap::g_archiveHeap);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("CursorStick", mHeap::g_archiveHeap);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("System2D", nullptr);
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk("saveBannerU", nullptr);

    for (int i = 0; i < 6; i++) {
        SizedString<128> str;
        str.sprintf("%s/%s", dMessage_c::getLanguageIdentifier(), getEventFlowFileNameByIndex(i, 1));
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
    if (!fn_801967D0(getHBM())) {
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

    // TODO we think this is a ResFont but
    // the offset is larger than ResFont...
    ((u8 *)lbl_805750D8)[0x2C] = 1;
    field_0x5DF = 1;
    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScBoot_c::cb7() {
    if (!dDyl::destroy()) {
        return sFPhaseBase::PHASE_RETRY;
    }

    static u16 profilesToLoad[] = {
        // ??? not sure what these correspond to
        0x278,
        0x279,
        0x1B0,
        0x1A9,
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

    // TODO JParticle monkaS
    OarcManager::GetInstance()->getData("System", "dat/navi_table.dat");
    OarcManager::GetInstance()->getData("JpaCommon", "dat/Common.jpc");
    OarcManager::GetInstance()->getData("JpaCommon", "dat/Common.jpn");

    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScBoot_c::cb8() {
    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dScBoot_c::cb9() {
    dBase_c::createRoot(fProfile::MESSAGE, 0, dBase_c::OTHER);
    dBase_c::createRoot(fProfile::C_BASE, 0, dBase_c::OTHER);

    dLytBattery_c::create();
    return sFPhaseBase::PHASE_NEXT;
}

SPECIAL_BASE_PROFILE(BOOT, dScBoot_c, fProfile::BOOT, 0x2A7, 0);

STATE_DEFINE(dScBoot_c, Init);
STATE_DEFINE(dScBoot_c, Strap);
STATE_DEFINE(dScBoot_c, Connect);
STATE_DEFINE(dScBoot_c, Save);

dScBoot_c::strap_c::strap_c() {
    SizedString<8> str;
    u8 langNum = getUsedLanguageNTSCNum();
    mArcName.mChars[0] = '\0';
    mStr2.mChars[0] = '\0';
    mStr3.mChars[0] = '\0';
    if (langNum == 3) {
        str = "F";
    } else if (langNum == 4) {
        str = "S";
    } else {
        str = "U";
    }
    // UB: Cannot pass object of non-POD type 'SizedString<8>' through variadic method
    mArcName.sprintf("strap%s", str);
    mStr2.sprintf("strap_00_%s.brlyt", str);
    mStr3.sprintf("strap_00_%s_loop.brlan", str);
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
        mLyt.build(mStr2, nullptr);
        mAnm.doSomething(mStr3, &mResAcc);
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
    fn_80197560(getHBM(), 0);
    sInstance = nullptr;
}

int dScBoot_c::create() {
    if (!mStrapScreen.create()) {
        return NOT_READY;
    };

    mFader.create();
    mFader.setFadeInFrame(1);
    // TODO
    dSys::setFrameRate(1);
    mStateMgr.changeState(StateID_Init);

    return SUCCEEDED;
}

int dScBoot_c::doDelete() {
    int removed = mStrapScreen.remove();
    sFPhaseBase::sFPhaseState phaseState = mPhases.step();
    if (!removed || phaseState != sFPhaseBase::PHASE_ALL_DONE) {
        return NOT_READY;
    }
    // TODO
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
    field_0x5D4 = 0;
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
    field_0x5D4 = 0;
    field_0x5E1 = 1;
    mFader.fadeIn();
    mFader.resetFrames();
}

void dScBoot_c::executeState_Strap() {
    if (dScene_c::sFader.isStatus(mFaderBase_c::FADED_IN)) {
        switch (field_0x5D4) {
            case 0:
                if (!checkDone() && mFader.isNotStatus(mFaderBase_c::FADING_IN)) {
                    field_0x5D4 = 1;
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
                            field_0x5D4 = 2;
                        }
                    }
                }
                break;
            case 2:
                if (!checkDone() && mFader.isNotStatus(mFaderBase_c::FADING_OUT)) {
                    if (mFader.fadeIn() == true) {
                        field_0x5D4 = 3;
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
                        field_0x5D4 = 0;
                        mStrapScreen.init();
                    }
                    fn_80197560(getHBM(), 0);
                }
                break;
        }
    }
}

void dScBoot_c::finalizeState_Strap() {
    // TODO
    SomeGfxThing::setField0x09(1);
}

void dScBoot_c::initializeState_Connect() {
    field_0x5D4 = 0;
    dSys::setFrameRate(2);
}

void dScBoot_c::executeState_Connect() {
    // TODO
}

void dScBoot_c::finalizeState_Connect() {}

void dScBoot_c::initializeState_Save() {
    field_0x5D4 = 0;
    field_0x5D8 = 0;
    // TODO
}

void dScBoot_c::executeState_Save() {
    // TODO
}

void dScBoot_c::finalizeState_Save() {}

bool dScBoot_c::checkDone() {
    if (field_0x5E3 == 1) {
        field_0x5E3 = 0;
        field_0x5D4 = 4;
        return 1;
    }
    return 0;
}
