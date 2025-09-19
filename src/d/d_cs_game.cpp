#include "d/d_cs_game.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_cs_base.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_gfx.h"
#include "d/d_pad.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_map.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_BASE_PROFILE(C_GAME, dCsGame_c, fProfile::C_GAME, 0x2BF, 0x06F9);

static dCsGame_HIO_c sHio;

dCsGame_HIO_c::dCsGame_HIO_c() {
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

dCsGame_c::dCsGame_c() : mCursorType(lytItemCursor_c::CS_NONE) {
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
    mLyt1.init();
    mLyt2.setResAcc(&mCursorResAcc);
    mLyt2.init();
    mCursor.setResAcc(&mMain2DResAcc);
    mCursor.init();

    setNextCursorType(lytItemCursor_c::CS_NONE);
    mCursorType = lytItemCursor_c::CS_NONE;

    mCursor.setField0x9A0(0);
    mCursorIf.setCursorMask(2);
    dCsMgr_c::GetInstance()->registCursor(&mCursorIf);
    dCsBase_c::GetInstance()->setVisible(false);
    return SUCCEEDED;
}

int dCsGame_c::doDelete() {
    mCursor.remove();
    mLyt2.remove();
    mLyt1.remove();
    dCsMgr_c::GetInstance()->unregistCursor(&mCursorIf);
    mCursorResAcc.detach();
    mMain2DResAcc.detach();
    return SUCCEEDED;
}

#pragma push
#pragma pool_data off
int dCsGame_c::execute() {
    mCursorType = mCursor.mNextCursorType;
    mCursor.mCursorActive = mCursor.mNextCursor;
    if (mCursor.isCursorActive() && mCursorType == lytItemCursor_c::CS_DOWSING) {
        mCursor.mDowsing.moveEffectsIn();
    } else {
        mCursor.mDowsing.moveEffectsOut();
    }

    bool active = mCursor.isCursorActive();
    if (!active) {
        if (!mCursor.field_0x9A2) {
            mCursor.field_0x9A1 = active;
        }
        mCursor.field_0x9A2 = false;
        return SUCCEEDED;
    }

    if (active != mCursor.field_0x9A1 || mCursorType != mCursor.mActiveCursorType) {
        switch (mCursorType) {
            case lytItemCursor_c::CS_BOW:       mCursor.changeState(lytItemCursor_c::StateID_Bow); break;
            case lytItemCursor_c::CS_DOWSING:   mCursor.changeState(lytItemCursor_c::StateID_Dowsing); break;
            case lytItemCursor_c::CS_VACUUM:    mCursor.changeState(lytItemCursor_c::StateID_Vacuum); break;
            case lytItemCursor_c::CS_PACHINKO:  mCursor.changeState(lytItemCursor_c::StateID_Pachinko); break;
            case lytItemCursor_c::CS_HOOKSHOT:  mCursor.changeState(lytItemCursor_c::StateID_HookShot); break;
            case lytItemCursor_c::CS_PLAYERCAM: mCursor.changeState(lytItemCursor_c::StateID_PlayerCam); break;
            default:                            break;
        }

        if (!mCursor.field_0x9A2) {
            mCursor.mActiveCursorType = mCursorType;
            mCursor.field_0x9A1 = mCursor.mCursorActive;
        }
    }

    const mVec2_c &cursorPosition = mCursorIf.getCursorPos();

    switch (mCursorType) {
        case lytItemCursor_c::CS_POINTER_CAT: {
            mLyt2.setPosition(mVec2_c(cursorPosition.x, cursorPosition.y + 10.0f));
            mLyt2.execute();
            break;
        }
        case lytItemCursor_c::CS_POINTER_DEF: {
            mLyt1.setPosition(cursorPosition);
            mLyt1.execute();
            break;
        }
        case lytItemCursor_c::CS_BOW:
        case lytItemCursor_c::CS_DOWSING:
        case lytItemCursor_c::CS_VACUUM:
        case lytItemCursor_c::CS_PACHINKO:
        case lytItemCursor_c::CS_PLAYERCAM:
        case lytItemCursor_c::CS_HOOKSHOT:  {
            mCursor.setPosition(cursorPosition);
            mCursor.execute();
        } break;
        default: break;
    }

    mCursor.mNextCursor = false;
    mCursor.field_0x9A2 = false;

    return SUCCEEDED;
}
#pragma pop

int dCsGame_c::draw() {
    if (!mCursor.isCursorActive() || isForcedHidden()) {
        return SUCCEEDED;
    }

    switch (mCursorType) {
        case lytItemCursor_c::CS_POINTER_CAT: mLyt2.draw(); break;
        case lytItemCursor_c::CS_POINTER_DEF: mLyt1.draw(); break;
        case lytItemCursor_c::CS_BOW:
        case lytItemCursor_c::CS_DOWSING:
        case lytItemCursor_c::CS_VACUUM:
        case lytItemCursor_c::CS_PACHINKO:
        case lytItemCursor_c::CS_PLAYERCAM:
        case lytItemCursor_c::CS_HOOKSHOT:    mCursor.draw(); break;
        default:                              break;
    }

    return SUCCEEDED;
}

bool dCsGame_c::fn_801BF5E0() const {
    return mCursor.isCursorActive() & !isForcedHidden();
}

void dCsGame_c::setNextCursorType(lytItemCursor_c::CursorType_e cs) {
    mCursor.setNextCursorType(cs);
}

void dCsGame_c::setCursorTypePlayerCam() {
    setNextCursorType(lytItemCursor_c::CS_PLAYERCAM);
}

mVec2_c &dCursorInterfaceGame_c::getCursorPos() {
    // TODO explain this math
    mVec2_c pos = dPad::getDpdPosScreen();
    f32 f0 = dGfx_c::getWidth4x3() / dGfx_c::getWidth16x9F() * 368.0f;
    f32 f1 = sHio.field_0x04;
    f32 f2 = sHio.field_0x08 + 6.0f;
    f32 f3 = sHio.field_0x10 + 4.0f;
    f32 f4 = sHio.field_0x0C + -15.0f;

    if (dLytMap_c::GetInstance() != nullptr && dLytMap_c::GetInstance()->isOpenMaybe()) {
        if (pos.x < -f0 + f4) {
            pos.x = -f0 + f4;
        }
        if (pos.x > f0 + f3) {
            pos.x = f0 + f3;
        }
        if (pos.y < f1 + -209.0f) {
            pos.y = f1 + -209.0f;
        }
        if (pos.y < f2 + -209.0f) {
            pos.y = f2 + -209.0f;
        }
    }

    field_0x14 = pos;

    return field_0x14;
}

bool dCsGameLytBase2_c::init() {
    loadResAcc();
    build();
    return true;
}

bool dCsGameLytBase2_c::execute() {
    nw4r::lyt::Pane *p = getLyt()->getLayout()->GetRootPane();
    mVec3_c pos;
    pos.x = mPosition.x;
    pos.y = mPosition.y;
    pos.z = 0.0f;
    p->SetTranslate(pos);
    getLyt()->calc();
    return true;
}

bool dCsGameLytBase2_c::draw() {
    getLyt()->addToDrawList();
    return true;
}

void dCsGameLyt1_c::loadResAcc() {
    mLyt.setResAcc(mpResAcc);
}

void dCsGameLyt1_c::build() {
    mLyt.build("P1_Def.brlyt", nullptr);
}

bool dCsGameLyt1_c::init() {
    dCsGameLytBase2_c::init();
    mLyt.setPriority(0x88);
    mLyt.calc();
    return true;
}

bool dCsGameLyt1_c::remove() {
    return true;
}

void dCsGameLyt2_c::loadResAcc() {
    mLyt.setResAcc(mpResAcc);
}

void dCsGameLyt2_c::build() {
    mLyt.build("P1_Cat.brlyt", nullptr);
}

bool dCsGameLyt2_c::init() {
    dCsGameLytBase2_c::init();
    mLyt.setPriority(0x88);
    mLyt.calc();
    return true;
}

bool dCsGameLyt2_c::remove() {
    return true;
}

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

void dCsGame_c::lytItemCursor_c::initializeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::executeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::finalizeState_Invisible() {}

void dCsGame_c::lytItemCursor_c::initializeState_Bow() {
    mBow.enter();
    mBow.select();
}
void dCsGame_c::lytItemCursor_c::executeState_Bow() {
    mDrawCursorTrailThisFrame = true;
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
    mDrawCursorTrailThisFrame = true;
    mVacuum.execute();
}
void dCsGame_c::lytItemCursor_c::finalizeState_Vacuum() {}

void dCsGame_c::lytItemCursor_c::initializeState_Pachinko() {}
void dCsGame_c::lytItemCursor_c::executeState_Pachinko() {
    mDrawCursorTrailThisFrame = true;
    mPachinko.execute();
}
void dCsGame_c::lytItemCursor_c::finalizeState_Pachinko() {}

void dCsGame_c::lytItemCursor_c::initializeState_HookShot() {
    mCrawShot.enter();
}
void dCsGame_c::lytItemCursor_c::executeState_HookShot() {
    mDrawCursorTrailThisFrame = true;
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

void dCsGame_c::lytItemCursor_c::loadResAcc() {
    mLyt.setResAcc(mpResAcc);
}

void dCsGame_c::lytItemCursor_c::build() {
    mLyt.build("itemCursor_00.brlyt", nullptr);
}

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

bool dCsGame_c::lytItemCursor_c::init() {
    mEffects.init(dCsGame_c::sInstance);
    dCsGameLytBase2_c::init();
    mLyt.setPriority(0x88);
    mLyt.calc();
    mAnm.init(mAnmGroups.tmp.mAnmGroups, lytItemCursorMap, 0x17, mpResAcc, mLyt.getLayout());

    for (int i = 0; i < 0x17; i++) {
        d2d::AnmGroup_c &grp = mAnmGroups[i];
        grp.bind(false);
        grp.setAnimEnable(false);
    }

    mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_CURSOR].setAnimEnable(true);
    d2d::AnmGroup_c &mainLoop = mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_LOOP];
    mainLoop.setFrame(0.0f);
    mainLoop.setAnimEnable(true);

    mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_ARROW_LOOP].setAnimEnable(true);
    d2d::AnmGroup_c &arrowLoop = mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_ARROW_LOOP];
    arrowLoop.setFrame(0.0f);

    mBow.init();
    mDowsing.init();
    mPachinko.init();
    mCrawShot.init();
    mVacuum.init();

    mAnmGroups.tmp.mAnmGroups[MAIN_ANIM_VACUUM_LOCK].setAnimEnable(false);
    mEffects.setEmitterCallback(&mTrailCb);

    mLyt.setPriority(0x88);

    mLyt.calc();
    arrowLoop.setAnimEnable(false);
    mStateMgr.changeState(StateID_Invisible);
    return true;
}

bool dCsGame_c::lytItemCursor_c::remove() {
    mAnm.remove();
    return true;
}

bool dCsGame_c::lytItemCursor_c::execute() {
    mPositionThisFrame.x = mPosition.x;
    mPositionThisFrame.y = mPosition.y;
    mPositionThisFrame.z = 0.0f;

    mDrawCursorTrailThisFrame = false;
    mAnmGroups[MAIN_ANIM_LOOP].setAnimEnable(true);
    mStateMgr.executeState();

    if (dCsGame_c::GetInstance()->fn_801BF5E0() && mDrawCursorTrailThisFrame) {
        f32 sqDist = mPositionThisFrame.squareDistance(mPositionLastFrame);
        if (sqDist > sHio.field_0x34 * sHio.field_0x34 ||
            (mEffects.hasEmitters() && sqDist > sHio.field_0x38 * sHio.field_0x38)) {
            mEffects.createContinuousUIEffect(
                PARTICLE_RESOURCE_ID_MAPPING_31_, mPositionThisFrame, nullptr, nullptr, nullptr, nullptr
            );
        }
    }

    dCsGameLytBase2_c::execute();
    mPositionLastFrame = mPositionThisFrame;
    return true;
}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_NotFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_NotFind() {
    if (field_0xC0 < 0.9999f) {
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
        mAnm[MAIN_ANIM_DOWSE_ZOOM].setFrame((1.0f - field_0xC0) * mAnm[MAIN_ANIM_DOWSE_ZOOM].getLastFrame());
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
    f32 f = mAnm[MAIN_ANIM_DOWSE_ZOOM].getLastFrame();
    mAnm[MAIN_ANIM_DOWSE_ZOOM].setFrame((1.0f - field_0xC0) * f);

    if (field_0xC0 > 0.9999f) {
        mAnm[MAIN_ANIM_DOWSE_ZOOM].setFrame(f);
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

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::enter() {
    mAnm[MAIN_ANIM_CURSOR].setFrame(3.0f);
    mAnm[MAIN_ANIM_DOWSE_IN_OUT].setFrame(0.0f);
    mAnm[MAIN_ANIM_DOWSE_IN_OUT].setAnimEnable(true);
    mAnm[MAIN_ANIM_DOWSE_ZOOM].setAnimEnable(false);
    mAnm[MAIN_ANIM_LOOP].setAnimEnable(false);
    mAnm[MAIN_ANIM_DOWSE_LOOP_1].setAnimEnable(false);
    mAnm[MAIN_ANIM_DOWSE_LOOP_2].setAnimEnable(false);
    mAnm[MAIN_ANIM_DOWSE_LOOP_3].setAnimEnable(false);

    mpLyt->calc();

    mAnm[MAIN_ANIM_DOWSE_ZOOM].setAnimEnable(false);
    mAnm[MAIN_ANIM_DOWSE_IN_OUT].setAnimEnable(false);
    mAnm[MAIN_ANIM_DOWSE_LOOP_1].setAnimEnable(true);
}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::moveEffectsIn() {
    if (mAlpha < sHio.field_0x40) {
        mAlpha++;
    }
    updateEffects();
}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::moveEffectsOut() {
    if (mAlpha > 0) {
        mAlpha--;
        updateEffects();
    }
}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::updateEffects() {
    u8 alpha = (f32)mAlpha / (f32)sHio.field_0x40 * 255.0f;
    if (alpha != 0) {
        mVec2_c csPos = dPad::getDpdPosScreen();
        mVec3_c effectPos(csPos.x, csPos.y, 0.0f);
        mVec3_c effectScale(sHio.field_0x3C, sHio.field_0x3C, 1.0f);
        mEffects1.createContinuousUIEffect(
            PARTICLE_RESOURCE_ID_MAPPING_725_, mVec3_c::Zero, nullptr, nullptr, nullptr, nullptr
        );
        mEffects2.createContinuousUIEffect(
            PARTICLE_RESOURCE_ID_MAPPING_726_, effectPos, &mAng3_c::Zero, &effectScale, nullptr, nullptr
        );
        mEffects1.setGlobalAlpha(alpha);
        mEffects2.setGlobalAlpha(alpha);
    } else {
        mEffects1.remove(true);
        mEffects2.remove(true);
    }
}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::init() {
    mpDowsingPane = mpLyt->findPane("N_dowsing_00");
    mpRingAllPane = mpLyt->findPane("N_ringAll_00");
    mEffects1.init(dCsGame_c::GetInstance());
    mEffects2.init(dCsGame_c::GetInstance());
    field_0xC0 = 0.0f;
    enter();
    mStateMgr.changeState(StateID_NotFind);
}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::execute() {
    mAnm[MAIN_ANIM_LOOP].setAnimEnable(false);
    f32 f2 = (sHio.field_0x18 - sHio.field_0x1C) * 0.95f;
    if (field_0xD0 > 0.0f) {
        field_0xD0 -= f2 / sHio.field_0x14;
        if (field_0xD0 < 0.0f) {
            field_0xD0 = 0.0f;
        }
    } else {
        field_0xD0 += f2 / sHio.field_0x14;
        if (field_0xD0 > 0.0f) {
            field_0xD0 = 0.0f;
        }
    }

    f32 scale = sHio.field_0x1C + (sHio.field_0x18 - sHio.field_0x1C) * (field_0xCC + field_0xD0);
    mpDowsingPane->SetScale(mVec2_c(scale, scale));

    mVec3_c rot = mpRingAllPane->GetRotate();
    rot.z = -mRotZ.degree2();
    mpRingAllPane->SetRotate(rot);

    mStateMgr.executeState();

    if (mAnm[MAIN_ANIM_DOWSE_LOOP_1].isEnabled()) {
        mAnm[MAIN_ANIM_DOWSE_LOOP_1].play();
    }

    if (mAnm[MAIN_ANIM_DOWSE_LOOP_2].isEnabled()) {
        if (mAnm[MAIN_ANIM_DOWSE_LOOP_2].getFrame() >= 60.0f) {
            mAnm[MAIN_ANIM_DOWSE_LOOP_2].setFrame(60.0f);
        } else {
            mAnm[MAIN_ANIM_DOWSE_LOOP_2].play();
        }
    }

    if (mAnm[MAIN_ANIM_DOWSE_LOOP_3].isEnabled()) {
        mAnm[MAIN_ANIM_DOWSE_LOOP_3].play();
        if (mAnm[MAIN_ANIM_DOWSE_LOOP_3].isEndReached()) {
            mAnm[MAIN_ANIM_DOWSE_LOOP_3].setAnimEnable(false);
        }
    }

    const sStateIDIf_c *state = mStateMgr.getStateID();

    if (*state == StateID_NotFind || *state == StateID_ToNotFind) {
        mAnm[MAIN_ANIM_DOWSE_LOOP_2].setFrame(0.0f);
        mpLyt->calc();
        mAnm[MAIN_ANIM_DOWSE_LOOP_2].setAnimEnable(false);
    }
}

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
    f32 f = mAnm[MAIN_ANIM_ARROW_CURSOR].getLastFrame();
    mAnm[MAIN_ANIM_ARROW_CURSOR].setFrame(f - (t * f));
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
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_BW_ALIGN_SIGHT);
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
    if (mIsCharging) {
        mStateMgr.changeState(StateID_ToDraw);
    }
}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Select() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_ToDraw() {
    mAnm[MAIN_ANIM_SLING_ON].setAnimEnable(true);
    mAnm[MAIN_ANIM_SLING_ON].setFrame(0.0f);
    mSavedOnProgress = 0.0f;
}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_ToDraw() {
    if (mIsCharging) {
        if (mAnm[MAIN_ANIM_SLING_ON].isEndReached()) {
            mpLyt->calc();
            mAnm[MAIN_ANIM_SLING_ON].setAnimEnable(false);
            if (dAcPy_c::getCurrentSlingshotType() == 2) {
                mSavedOnProgress = mDrawProgress;
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
    if (!mIsCharging) {
        mStateMgr.changeState(StateID_Out);
    }
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
    if (mIsCharging) {
        if (mDrawProgress >= 1.0f) {
            mAnm[MAIN_ANIM_SLING_KEEP].setToEnd();
            mpLyt->calc();
            mAnm[MAIN_ANIM_SLING_KEEP].setAnimEnable(false);
            mStateMgr.changeState(StateID_ChargeFull);
        } else {
            f32 frame = ((mDrawProgress - mSavedOnProgress) / (1.0f - mSavedOnProgress));
            f32 len = mAnm[MAIN_ANIM_SLING_KEEP].getLastFrame();
            mAnm[MAIN_ANIM_SLING_KEEP].setFrame(len * frame);
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
    if (mIsCharging) {
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

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::init() {
    mAnm[MAIN_ANIM_SLING_ON].setFrame(0.0f);
    mAnm[MAIN_ANIM_SLING_ON].setAnimEnable(true);
    mAnm[MAIN_ANIM_SLING_KEEP].setFrame(0.0f);
    mAnm[MAIN_ANIM_SLING_KEEP].setAnimEnable(true);
    mAnm[MAIN_ANIM_SLING_PEAK].setFrame(0.0f);
    mAnm[MAIN_ANIM_SLING_PEAK].setAnimEnable(true);
    mAnm[MAIN_ANIM_SLING_OUT].setFrame(0.0f);
    mAnm[MAIN_ANIM_SLING_OUT].setAnimEnable(true);
    mpLyt->calc();

    mAnm[MAIN_ANIM_SLING_KEEP].setAnimEnable(false);
    mAnm[MAIN_ANIM_SLING_PEAK].setAnimEnable(false);
    mAnm[MAIN_ANIM_SLING_OUT].setAnimEnable(false);

    mIsCharging = false;
    mDrawProgress = false;

    mStateMgr.changeState(StateID_Select);
}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::execute() {
    mAnm[MAIN_ANIM_CURSOR].setFrame(5.0f);
    mStateMgr.executeState();
    if (mAnm[MAIN_ANIM_LOOP].isEnabled()) {
        // ??? order
        mpLyt->calc();
        mAnm[MAIN_ANIM_LOOP].play();
    }

    if (mAnm[MAIN_ANIM_SLING_PEAK_RING].isEndReached()) {
        mAnm[MAIN_ANIM_SLING_PEAK_RING].setAnimEnable(false);
        mpLyt->calc();
    } else {
        mAnm[MAIN_ANIM_SLING_PEAK_RING].play();
    }
}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::setCharging(bool charging, f32 progress) {
    mIsCharging = charging;
    mDrawProgress = progress;
}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_Normal() {
    mAnm[MAIN_ANIM_LOOP].setAnimEnable(true);
}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_Normal() {
    if (mLocked) {
        mAnm[MAIN_ANIM_HOOK_FIX].setAnimEnable(true);
        mAnm[MAIN_ANIM_HOOK_FIX].setForward();
        mAnm[MAIN_ANIM_HOOK_FIX].setToStart();
        mAnm[MAIN_ANIM_LOOP].setAnimEnable(false);
        mStateMgr.changeState(StateID_ToLock);
    } else {
        mAnm[MAIN_ANIM_LOOP].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_Normal() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::setRotate(f32 rot) {
    mVec3_c v;
    v.set(0.0f, 0.0f, rot);
    mpPaneCraws[0]->SetRotate(v);
    v.z -= 120.0f;
    mpPaneCraws[1]->SetRotate(v);
    v.z += 240.0f;
    mpPaneCraws[2]->SetRotate(v);
}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_ToLock() {
    setRotate(0.0f);
}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_ToLock() {
    if (mAnm[MAIN_ANIM_HOOK_FIX].isEndReached()) {
        mAnm[MAIN_ANIM_HOOK_FIX].setAnimEnable(false);
        mpLyt->calc();
        mAnm[MAIN_ANIM_LOCK_LOOP].setAnimEnable(true);
        mAnm[MAIN_ANIM_LOCK_LOOP].setFrame(0.0f);
        mStateMgr.changeState(StateID_Lock);
    } else {
        mAnm[MAIN_ANIM_HOOK_FIX].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_ToLock() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_Lock() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_Lock() {
    if (!mLocked) {
        mAnm[MAIN_ANIM_HOOK_FIX].setAnimEnable(true);
        mAnm[MAIN_ANIM_HOOK_FIX].setBackward();
        mAnm[MAIN_ANIM_HOOK_FIX].setToStart();
        mAnm[MAIN_ANIM_LOCK_LOOP].setAnimEnable(false);
        field_0x68 = mpPaneCrawFix->GetRotate().z;
        setRotate(field_0x68);
        mStateMgr.changeState(StateID_ToNormal);
    } else {
        mAnm[MAIN_ANIM_LOCK_LOOP].play();
    }
}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_Lock() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_ToNormal() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_ToNormal() {
    if (mAnm[MAIN_ANIM_HOOK_FIX].isStop2()) {
        setRotate(0.0f);
        mpLyt->calc();
        mAnm[MAIN_ANIM_HOOK_FIX].setAnimEnable(false);
        mStateMgr.changeState(StateID_Normal);
    } else {
        mAnm[MAIN_ANIM_HOOK_FIX].play();
        f32 frame = mAnm[MAIN_ANIM_HOOK_FIX].getFrame();
        f32 len = mAnm[MAIN_ANIM_HOOK_FIX].getLastFrame();
        f32 offset = (-120.0f - field_0x68);
        offset /= len;
        if (offset < -10.0f) {
            offset = -10.0f;
        }
        setRotate(field_0x68 + offset * (len - frame));
    }
}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_ToNormal() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::enter() {
    mAnm[MAIN_ANIM_CURSOR].setFrame(6.0f);
    mAnm[MAIN_ANIM_HOOK_FIX].setAnimEnable(true);
    mAnm[MAIN_ANIM_HOOK_FIX].setFrame(0.0f);
    mAnm[MAIN_ANIM_LOCK_LOOP].setAnimEnable(false);
    mpLyt->getLayout()->Animate(0);
    mpLyt->calc();
    mAnm[MAIN_ANIM_HOOK_FIX].setAnimEnable(false);
    mStateMgr.changeState(StateID_Normal);
}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::setLocked(bool locked) {
    mLocked = locked;
}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::init() {
    mpPaneCrawFix = mpLyt->findPane("N_crawFix_00");
    mpPaneCraws[0] = mpLyt->findPane("N_craw_03");
    mpPaneCraws[1] = mpLyt->findPane("N_craw_04");
    mpPaneCraws[2] = mpLyt->findPane("N_craw_05");
    mLocked = false;
    enter();
    mStateMgr.changeState(StateID_Normal);
}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::execute() {
    mStateMgr.executeState();
}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::initializeState_Normal() {
    mAnm[MAIN_ANIM_LOOP].setAnimEnable(true);
}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::executeState_Normal() {
    if (mLocked) {
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
    if (!mLocked) {
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

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::setUnkFloat(f32 f) {
    if (f < 0.0f) {
        f = 0.0f;
    } else if (f > 100.0f) {
        f = 100.0f;
    }
    field_0x54 = f;
}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::setLocked(bool locked) {
    mLocked = locked;
}

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
    mLocked = false;
    enter();
    mStateMgr.changeState(StateID_Normal);
}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::execute() {
    mStateMgr.executeState();
}
