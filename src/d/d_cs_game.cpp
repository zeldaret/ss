#include "d/d_cs_game.h"

#include "d/d_cs_base.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_structd.h"
#include "f/f_base.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/small_sound_mgr.h"

SPECIAL_BASE_PROFILE(C_GAME, dCsGame_c, fProfile::C_GAME, 0x2BF, 0x06F9);

CsGameConfigMaybe sConfig;

CsGameConfigMaybe::CsGameConfigMaybe() {
    field_0x04 = 0.0f;
    field_0x08 = 0.0f;
    field_0x0C = 0.0f;
    field_0x10 = 0.0f;
    field_0x14 = 3.0f;
    field_0x20 = 10.0f;
    field_0x24 = 1.0f;
    field_0x28 = 6.0f;
    field_0x18 = 1.3f;
    field_0x1C = 0.5f;
    field_0x42 = 0;
    field_0x2C = 3.5f;
    field_0x30 = 40;
    field_0x34 = 17.0f;
    field_0x38 = 3.0f;
    field_0x41 = 0;
    field_0x3C = 1.0f;
    field_0x40 = 5;
}
// CsGameConfigMaybe::~CsGameConfigMaybe() {}

STATE_DEFINE(dCsGame_c::lytItemCursor_c, Invisible);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, Bow);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, Dowsing);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, Vacuum);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, Pachinko);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, HookShot);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, PlayerCam);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, NotFind);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, ToFind);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, Find);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, FindMinimum);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, ToNotFind);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Invisible);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Select);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Ready);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, ToAimStart);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, AimStart);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Draw);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Charge);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Invisible);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Select);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, ToDraw);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Draw);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Out);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Charge);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, ChargeFull);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c, Normal);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c, ToLock);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c, Lock);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c, ToNormal);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytVacuumCsr_c, Normal);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytVacuumCsr_c, ToLock);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytVacuumCsr_c, Lock);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytVacuumCsr_c, ToNormal);

#define MAIN_ANIM_CURSOR 0
#define MAIN_ANIM_DOWSE_IN_OUT 1
#define MAIN_ANIM_LOOP 2
#define MAIN_ANIM_HOOK_FIX 3
#define MAIN_ANIM_VACUUM_LOCK 4
#define MAIN_ANIM_LOCK_LOOP 5
#define MAIN_ANIM_DOWSE_LOOP_1 6
#define MAIN_ANIM_DOWSE_LOOP_2 7
#define MAIN_ANIM_DOWSE_LOOP_3 8
#define MAIN_ANIM_DOWSE_ZOOM 9
#define MAIN_ANIM_ARROW_ON 10
#define MAIN_ANIM_ARROW_CURSOR 11
#define MAIN_ANIM_ARROW_EFFECT 12
#define MAIN_ANIM_ARROW_KEEP 13
#define MAIN_ANIM_ARROW_PEAK 14
#define MAIN_ANIM_SLING_ON 15
#define MAIN_ANIM_SLING_KEEP 16
#define MAIN_ANIM_SLING_PEAK 17
#define MAIN_ANIM_SLING_PEAK_RING 18
#define MAIN_ANIM_SLING_OUT 19
#define MAIN_ANIM_ARROW_LOOP 20
#define MAIN_ANIM_LOOP_END 21
#define MAIN_ANIM_LOOP_END_2 22

static const d2d::LytBrlanMapping lytItemCursorMap[] = {
    {       "itemCursor_00_cursor.brlan",      "G_cursor_00"},
    {   "itemCursor_00_dowseInOut.brlan",    "G_dowseAll_00"},
    {         "itemCursor_00_loop.brlan",        "G_loop_00"},
    {      "itemCursor_00_hookFix.brlan",     "G_hookFix_00"},
    {   "itemCursor_00_vacuumLock.brlan",  "G_vacuumLock_00"},
    {     "itemCursor_00_lockLoop.brlan",    "G_lockLoop_00"},
    {   "itemCursor_00_dowseLoop1.brlan",   "G_dowseLoop_00"},
    {   "itemCursor_00_dowseLoop2.brlan",   "G_dowseLoop_01"},
    {   "itemCursor_00_dowseLoop3.brlan",    "G_auraLoop_00"},
    {    "itemCursor_00_dowseZoom.brlan",    "G_dowseAll_00"},
    {      "itemCursor_00_arrowOn.brlan",     "G_arrowOn_00"},
    {  "itemCursor_00_arrowCursor.brlan", "G_arrowCursor_00"},
    {  "itemCursor_00_arrowEffect.brlan", "G_arrowEffect_00"},
    {    "itemCursor_00_arrowKeep.brlan", "G_arrowCursor_00"},
    {    "itemCursor_00_arrowPeak.brlan", "G_arrowCursor_00"},
    {      "itemCursor_00_slingOn.brlan", "G_slingEffect_00"},
    {    "itemCursor_00_slingKeep.brlan",  "G_slingGauge_00"},
    {    "itemCursor_00_slingPeak.brlan",    "G_slingAll_00"},
    {"itemCursor_00_slingPeakRing.brlan",   "G_slingRing_00"},
    {     "itemCursor_00_slingOut.brlan", "G_slingEffect_00"},
    {        "itemCursor_00_loop2.brlan",      "G_arrowLoop"},
    {      "itemCursor_00_loopEnd.brlan",     "G_loopEnd_00"},
    {     "itemCursor_00_loopEnd2.brlan",     "G_loopEnd_01"},
};

dCsGame_c::dCsGame_c() : mCursorType(0) {
    sInstance = this;
}

dCsGame_c::~dCsGame_c() {
    sInstance = nullptr;
}

int dCsGame_c::create() {
    void *csData = LayoutArcManager::GetInstance()->getLoadedData("cursor");
    mCursorResAcc.attach(csData, "arc");
    void *mainData = LayoutArcManager::GetInstance()->getLoadedData("Main2D");
    mMain2DResAcc.attach(mainData, "");

    mLyt1.setResAcc(&mCursorResAcc);
    mLyt1.dCsGameLytBase_0x10();
    mLyt2.setResAcc(&mCursorResAcc);
    mLyt2.dCsGameLytBase_0x10();
    mCursor.setResAcc(&mMain2DResAcc);
    mCursor.init();

    setSomething(0);
    mCursorType = 0;

    mCursor.setField0x9A0(0);
    mStructC.field_0x10 = 2;
    d2d::dLytStructDList::GetInstance()->appendToList1(&mStructC);
    dCsBase_c::GetInstance()->setField703(false);
    return SUCCEEDED;
}

int dCsGame_c::doDelete() {
    mCursor.remove();
    mLyt2.dCsGameLytBase_0x14();
    mLyt1.dCsGameLytBase_0x14();
    d2d::dLytStructDList::GetInstance()->removeFromList1(&mStructC);
    mCursorResAcc.detach();
    mMain2DResAcc.detach();
    return SUCCEEDED;
}

void dCsGame_c::setNextCursorType(lytItemCursor_c::CursorType_e cs) {
    mCursor.setNextCursorType(cs);
}

void dCsGame_c::lytItemCursor_c::initializeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::executeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::finalizeState_Invisible() {}

void dCsGame_c::lytItemCursor_c::initializeState_Bow() {
    mBow.enter();
    mBow.select();
}
void dCsGame_c::lytItemCursor_c::executeState_Bow() {
    field_0x990 = 1;
    mBow.execute();
}
void dCsGame_c::lytItemCursor_c::finalizeState_Bow() {}

void dCsGame_c::lytItemCursor_c::initializeState_Dowsing() {
    mDowsing.enter();
}
void dCsGame_c::lytItemCursor_c::executeState_Dowsing() {
    mDowsing.execute();
}
void dCsGame_c::lytItemCursor_c::finalizeState_Dowsing() {}

void dCsGame_c::lytItemCursor_c::initializeState_Vacuum() {
    mVacuum.enter();
}
void dCsGame_c::lytItemCursor_c::executeState_Vacuum() {
    field_0x99C = 1;
    mVacuum.execute();
}
void dCsGame_c::lytItemCursor_c::finalizeState_Vacuum() {}

void dCsGame_c::lytItemCursor_c::initializeState_Pachinko() {}
void dCsGame_c::lytItemCursor_c::executeState_Pachinko() {
    field_0x99C = 1;
    mPachinko.execute();
}
void dCsGame_c::lytItemCursor_c::finalizeState_Pachinko() {}

void dCsGame_c::lytItemCursor_c::initializeState_HookShot() {
    mCrawShot.enter();
}
void dCsGame_c::lytItemCursor_c::executeState_HookShot() {
    field_0x99C = 1;
    mCrawShot.execute();
}
void dCsGame_c::lytItemCursor_c::finalizeState_HookShot() {}

void dCsGame_c::lytItemCursor_c::initializeState_PlayerCam() {}
void dCsGame_c::lytItemCursor_c::executeState_PlayerCam() {
    d2d::AnmGroup_c &g = mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_CURSOR];
    g.setFrame(9.0f);
}
void dCsGame_c::lytItemCursor_c::finalizeState_PlayerCam() {}

void dCsGame_c::lytItemCursor_c::changeState(const sFStateID_c<lytItemCursor_c> &newState) {
    mStateMgr.changeState(newState);
}

bool dCsGame_c::lytItemCursor_c::doInit() {
    mEffects.init(dCsGame_c::sInstance);
    preInit();
    mLyt.setPriority(0x88);
    mLyt.calc();
    mAnm.init(mAnmGroups.tmp.mAnmGroups, lytItemCursorMap, 0x17, mpResAcc, mLyt.getLayout());

    for (int i = 0; i < 0x17; i++) {
        mAnmGroups.tmp.mAnmGroups[i].setDirection(false);
        mAnmGroups.tmp.mAnmGroups[i].setAnimEnable(false);
    }

    mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_CURSOR].setAnimEnable(true);
    mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_LOOP].setFrame(0.0f);
    mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_LOOP].setAnimEnable(true);

    mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_ARROW_LOOP].setAnimEnable(true);
    mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_ARROW_LOOP].setFrame(0.0f);

    mBow.init();
    mDowsing.init();
    mPachinko.init();
    mCrawShot.init();
    mVacuum.init();

    mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_VACUUM_LOCK].setAnimEnable(false);
    mEffectRelated.doSomething(&mEffects);
    mLyt.setPriority(0x88);

    mLyt.calc();
    mAnmGroups.tmp.mAnmGroups[0x14].setAnimEnable(false);
    mStateMgr.changeState(StateID_Invisible);
    return true;
}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_NotFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_NotFind() {
    if (field_0xC0 < 0.999f) {
        mAnm[MAIN_ANIM_DOWSE_IN_OUT].setAnimEnable(true);
        mAnm[MAIN_ANIM_DOWSE_IN_OUT].setForward();
        mStateMgr.changeState(StateID_Find);
    }
}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_NotFind() {}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_ToFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_ToFind() {
    if (mAnm[MAIN_ANIM_DOWSE_IN_OUT].isEndReached()) {
        mAnm[MAIN_ANIM_DOWSE_IN_OUT].setAnimEnable(false);
        mAnm[MAIN_ANIM_DOWSE_ZOOM].setAnimEnable(true);
        // TODO
        mAnm[MAIN_ANIM_DOWSE_ZOOM].setRatio(1.0f - field_0xC0);
        mpLyt->calc();
        mStateMgr.changeState(StateID_Find);
    } else {
        mAnm[MAIN_ANIM_DOWSE_IN_OUT].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_ToFind() {}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_Find() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_Find() {
    mAnm[MAIN_ANIM_DOWSE_LOOP_2].setAnimEnable(true);
    mAnm[MAIN_ANIM_DOWSE_ZOOM].setAnimEnable(true);
    // TODO
    mAnm[MAIN_ANIM_DOWSE_ZOOM].setRatio(1.0f - field_0xC0);

    if (field_0xC0 > 0.9999f) {
        mAnm[MAIN_ANIM_DOWSE_ZOOM].setToEnd();
        mpLyt->calc();
        mAnm[MAIN_ANIM_DOWSE_IN_OUT].setAnimEnable(true);
        mAnm[MAIN_ANIM_DOWSE_ZOOM].setAnimEnable(false);
        mAnm[MAIN_ANIM_DOWSE_IN_OUT].setBackward();
        mStateMgr.changeState(StateID_ToNotFind);
    } else if (field_0xC0 < 0.0001f) {
        mAnm[MAIN_ANIM_DOWSE_LOOP_2].setFrame(0.0f);
        mAnm[MAIN_ANIM_DOWSE_LOOP_3].setAnimEnable(true);
        mAnm[MAIN_ANIM_DOWSE_LOOP_3].setFrame(0.0f);
        mpLyt->calc();
        mAnm[MAIN_ANIM_DOWSE_LOOP_2].setAnimEnable(false);
        mStateMgr.changeState(StateID_FindMinimum);
    }
}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_Find() {}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_FindMinimum() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_FindMinimum() {
    if (field_0xC0 > 0.0001f) {
        mStateMgr.changeState(StateID_Find);
    }
}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_FindMinimum() {}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_ToNotFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_ToNotFind() {
    if (mAnm[MAIN_ANIM_DOWSE_IN_OUT].isStop2()) {
        mpLyt->calc();
        mStateMgr.changeState(StateID_NotFind);
        mAnm[MAIN_ANIM_DOWSE_IN_OUT].setAnimEnable(false);
    } else {
        mAnm[MAIN_ANIM_DOWSE_IN_OUT].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_ToNotFind() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Invisible() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Select() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Select() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Select() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Ready() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Ready() {
    mAnm[MAIN_ANIM_LOOP].setAnimEnable(true);
}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Ready() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_ToAimStart() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_ToAimStart() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_ToAimStart() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_AimStart() {
    mAnm[MAIN_ANIM_ARROW_PEAK].setFrame(0.0f);
    mAnm[MAIN_ANIM_ARROW_KEEP].setFrame(0.0f);
    mAnm[MAIN_ANIM_ARROW_CURSOR].setAnimEnable(false);
    mAnm[MAIN_ANIM_LOOP_END].setAnimEnable(true);
    mAnm[MAIN_ANIM_LOOP_END].setFrame(0.0f);
    mpLyt->calc();
    mAnm[MAIN_ANIM_LOOP_END].setAnimEnable(false);
    mAnm[MAIN_ANIM_LOOP].setAnimEnable(false);
    mAnm[MAIN_ANIM_ARROW_ON].setAnimEnable(true);
    mAnm[MAIN_ANIM_ARROW_ON].setFrame(0.0f);
}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_AimStart() {
    if (mAnm[MAIN_ANIM_ARROW_ON].isEnabled()) {
        mAnm[MAIN_ANIM_ARROW_ON].play();
        if (mAnm[MAIN_ANIM_ARROW_ON].isEndReached()) {
            mpLyt->calc();
            mAnm[MAIN_ANIM_ARROW_ON].setAnimEnable(false);
            mAnm[MAIN_ANIM_ARROW_LOOP].setAnimEnable(true);
            mpLyt->calc();
        }
    }
}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_AimStart() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Draw() {
    mAnm[MAIN_ANIM_LOOP_END_2].setAnimEnable(true);
    mAnm[MAIN_ANIM_LOOP_END_2].setFrame(0.0f);
    mpLyt->calc();
    mAnm[MAIN_ANIM_LOOP_END_2].setAnimEnable(false);
    mAnm[MAIN_ANIM_ARROW_LOOP].setAnimEnable(false);
}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Draw() {
    // TODO ???
    mAnm[MAIN_ANIM_ARROW_CURSOR].setAnimEnable(true);
    f32 t = field_0x50;
    if (t < 0.0001f) {
        t = 0.0f;
    } else if (t > 0.9999f) {
        t = 1.0f;
    }
    mAnm[MAIN_ANIM_ARROW_CURSOR].setBackwardsRatio(t);
    if (t <= 0.0f) {
        mAnm[MAIN_ANIM_ARROW_CURSOR].setAnimEnable(false);
        mStateMgr.changeState(StateID_Charge);
    }
}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Draw() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Charge() {
    mAnm[MAIN_ANIM_ARROW_EFFECT].setAnimEnable(true);
    mAnm[MAIN_ANIM_ARROW_EFFECT].setFrame(0.0f);
}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Charge() {
    if (field_0x54 >= 1.0f) {
        if (!mAnm[MAIN_ANIM_ARROW_PEAK].isEnabled()) {
            mAnm[MAIN_ANIM_ARROW_PEAK].setAnimEnable(true);
            mAnm[MAIN_ANIM_ARROW_PEAK].setFrame(0.0f);
            SmallSoundManager::GetInstance()->playSound(SE_S_BW_ALIGN_SIGHT);
        }
        mAnm[MAIN_ANIM_ARROW_KEEP].setAnimEnable(false);
    } else {
        if (field_0x50 > 0.0001f) {
            mAnm[MAIN_ANIM_ARROW_CURSOR].setAnimEnable(true);
            mStateMgr.changeState(StateID_Draw);
            mAnm[MAIN_ANIM_ARROW_PEAK].setAnimEnable(false);
            mAnm[MAIN_ANIM_ARROW_KEEP].setAnimEnable(false);
        } else {
            mAnm[MAIN_ANIM_ARROW_PEAK].setAnimEnable(false);
            mAnm[MAIN_ANIM_ARROW_KEEP].setAnimEnable(true);
        }
    }
}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Charge() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Invisible() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Select() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Select() {
    if (!field_0x50) {
        return;
    }
    mStateMgr.changeState(StateID_ToDraw);
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Select() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_ToDraw() {
    mAnm[MAIN_ANIM_SLING_ON].setAnimEnable(true);
    mAnm[MAIN_ANIM_SLING_ON].setFrame(0.0f);
    field_0x58 = 0.0f;
}
extern "C" int getCurrentSlingshotType();
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_ToDraw() {
    if (field_0x50) {
        if (mAnm[MAIN_ANIM_SLING_ON].isEndReached()) {
            mpLyt->calc();
            mAnm[MAIN_ANIM_SLING_ON].setAnimEnable(false);
            if (getCurrentSlingshotType() == 2) {
                field_0x58 = field_0x54;
                mStateMgr.changeState(StateID_Charge);
            } else {
                mStateMgr.changeState(StateID_Draw);
            }
        } else {
            mAnm[MAIN_ANIM_SLING_ON].play();
        }
    } else {
        mAnm[MAIN_ANIM_SLING_ON].setAnimEnable(false);
        mStateMgr.changeState(StateID_Out);
    }
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_ToDraw() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Draw() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Draw() {
    if (field_0x50) {
        return;
    }
    mStateMgr.changeState(StateID_Out);
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Draw() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Out() {
    mAnm[MAIN_ANIM_SLING_OUT].setAnimEnable(true);
    mAnm[MAIN_ANIM_SLING_OUT].setFrame(0.0f);
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Out() {
    if (mAnm[MAIN_ANIM_SLING_OUT].isEndReached()) {
        mpLyt->calc();
        mAnm[MAIN_ANIM_SLING_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    } else {
        mAnm[MAIN_ANIM_SLING_OUT].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Out() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Charge() {
    mAnm[MAIN_ANIM_SLING_KEEP].setAnimEnable(true);
    mAnm[MAIN_ANIM_SLING_KEEP].setFrame(0.0f);
    mpLyt->calc();
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Charge() {
    if (field_0x50) {
        if (field_0x54 >= 1.0f) {
            mAnm[MAIN_ANIM_SLING_KEEP].setToEnd();
            mpLyt->calc();
            mAnm[MAIN_ANIM_SLING_KEEP].setAnimEnable(false);
            mStateMgr.changeState(StateID_ChargeFull);
        } else {
            // TODO we need a second load of anm control here
            mAnm[MAIN_ANIM_SLING_KEEP].setRatio((field_0x54 - field_0x58) / (1.0f - field_0x58));
        }
    } else {
        mAnm[MAIN_ANIM_SLING_KEEP].setAnimEnable(false);
        mStateMgr.changeState(StateID_Out);
    }
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Charge() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_ChargeFull() {
    mAnm[MAIN_ANIM_SLING_PEAK].setAnimEnable(true);
    mAnm[MAIN_ANIM_SLING_PEAK].setFrame(0.0f);
    mAnm[MAIN_ANIM_SLING_PEAK_RING].setAnimEnable(true);
    mAnm[MAIN_ANIM_SLING_PEAK_RING].setFrame(0.0f);
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_ChargeFull() {
    if (field_0x50) {
        if (mAnm[MAIN_ANIM_SLING_PEAK].isEndReached()) {
            mpLyt->calc();
        } else if (mAnm[MAIN_ANIM_SLING_PEAK].isEnabled()) {
            mAnm[MAIN_ANIM_SLING_PEAK].play();
        }
    } else {
        mpLyt->calc();
        mAnm[MAIN_ANIM_SLING_PEAK].setAnimEnable(false);
        mStateMgr.changeState(StateID_Out);
    }
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_ChargeFull() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_Normal() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_Normal() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_Normal() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_ToLock() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_ToLock() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_ToLock() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_Lock() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_Lock() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_Lock() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_ToNormal() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_ToNormal() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_ToNormal() {}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::initializeState_Normal() {
    mAnm[MAIN_ANIM_LOOP].setAnimEnable(true);
}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::executeState_Normal() {
    if (field_0x5C) {
        mAnm[MAIN_ANIM_VACUUM_LOCK].setAnimEnable(true);
        mAnm[MAIN_ANIM_VACUUM_LOCK].setForward();
        mAnm[MAIN_ANIM_VACUUM_LOCK].setToStart();
        mAnm[MAIN_ANIM_LOOP].setAnimEnable(false);
        mStateMgr.changeState(StateID_ToLock);
    } else {
        mAnm[MAIN_ANIM_LOOP].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::finalizeState_Normal() {}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::initializeState_ToLock() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::executeState_ToLock() {
    if (mAnm[MAIN_ANIM_VACUUM_LOCK].isEndReached()) {
        mAnm[MAIN_ANIM_VACUUM_LOCK].setAnimEnable(false);
        mpLyt->calc();
        mAnm[MAIN_ANIM_LOCK_LOOP].setAnimEnable(true);
        mAnm[MAIN_ANIM_LOCK_LOOP].setFrame(0.0f);
        mStateMgr.changeState(StateID_Lock);
    } else {
        mAnm[MAIN_ANIM_VACUUM_LOCK].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::finalizeState_ToLock() {}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::initializeState_Lock() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::executeState_Lock() {
    if (!field_0x5C) {
        mAnm[MAIN_ANIM_VACUUM_LOCK].setAnimEnable(true);
        mAnm[MAIN_ANIM_VACUUM_LOCK].setBackward();
        mAnm[MAIN_ANIM_VACUUM_LOCK].setToStart();
        mAnm[MAIN_ANIM_LOCK_LOOP].setAnimEnable(false);
        mStateMgr.changeState(StateID_ToNormal);
    } else {
        mAnm[MAIN_ANIM_LOCK_LOOP].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::finalizeState_Lock() {}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::initializeState_ToNormal() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::executeState_ToNormal() {
    if (mAnm[MAIN_ANIM_VACUUM_LOCK].isStop2()) {
        mpLyt->calc();
        mAnm[MAIN_ANIM_VACUUM_LOCK].setAnimEnable(false);
        mStateMgr.changeState(StateID_Normal);
    } else {
        mAnm[MAIN_ANIM_VACUUM_LOCK].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::finalizeState_ToNormal() {}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::enter() {
    mAnm[MAIN_ANIM_CURSOR].setFrame(4.0f);
    mAnm[MAIN_ANIM_LOCK_LOOP].setAnimEnable(false);
    mAnm[MAIN_ANIM_VACUUM_LOCK].setAnimEnable(true);
    mAnm[MAIN_ANIM_VACUUM_LOCK].setFrame(0.0f);
    mpLyt->animate();
    mpLyt->calc();
    mAnm[MAIN_ANIM_VACUUM_LOCK].setAnimEnable(false);
    mStateMgr.changeState(StateID_Normal);
}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::init() {
    field_0x5C = 0;
    enter();
    mStateMgr.changeState(StateID_Normal);
}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::execute() {
    mStateMgr.executeState();
}
