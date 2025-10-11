#include "d/a/obj/d_a_obj_windmill.h"

#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_pinwheel.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_light_env.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_WINDMILL, dAcOwindmill_c, fProfile::OBJ_WINDMILL, 0x1DC, 0, 6);

STATE_DEFINE(dAcOwindmill_c, Wait);
STATE_DEFINE(dAcOwindmill_c, CameraMove);
STATE_DEFINE(dAcOwindmill_c, Move);
STATE_DEFINE(dAcOwindmill_c, Collect);
STATE_DEFINE(dAcOwindmill_c, CollectStick);

const s16 dAcOwindmill_c::sStateTimer1_0x50 = 0x50;
const f32 dAcOwindmill_c::sTargetHalf = 0.5f;
const f32 dAcOwindmill_c::sTargetOne = 1.f;
const s16 dAcOwindmill_c::sStateTimer1_0xC = 0xC;
const s16 dAcOwindmill_c::sStateTimer1_0x32 = 0x32;
const s16 dAcOwindmill_c::sStateTimer2_0xF = 0xF;
const s16 dAcOwindmill_c::s0xF_unused = 0xF;

bool dAcOwindmill_c::createHeap() {
    mHasPropellor = getFromParams(0x14, 1);
    void *data = CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres");
    mResFile = nw4r::g3d::ResFile(data);
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);

    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("StageF000Windmill");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120));

    nw4r::g3d::ResAnmChr resAnmChr = mResFile.GetResAnmChr("StageF000Windmill");
    TRY_CREATE(mAnmChr.create(mdl, resAnmChr, &mAllocator, nullptr));

    nw4r::g3d::ResAnmClr resAnmClr = mResFile.GetResAnmClr("StageF000Windmill");
    TRY_CREATE(mAnmClr.create(mdl, resAnmClr, &mAllocator, nullptr, 1));

    void *dzb = CurrentStageArcManager::GetInstance()->getData("dzb/StageF000Windmill.dzb");
    void *plc = CurrentStageArcManager::GetInstance()->getData("dat/StageF000Windmill.plc");
    updateMatrix();
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
}

int dAcOwindmill_c::create() {
    CREATE_ALLOCATOR(dAcOwindmill_c);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mSetRotation = getFromParams(0, 0xF);
    mSceneFlagInPlace = getFromParams(4, 0xFF);
    mSceneFlagBeingBlown = getFromParams(0xC, 0xFF);
    mWindmillRotation = mRotation.y;
    mWindmillPosition = mPosition;

    mWindmillPosition.x += -95.22334f * (mRotation.y + mAng(0xc17)).sin();
    mWindmillPosition.z += -95.22334f * (mRotation.y + mAng(0xc17)).cos();
    mWindmillPosition.y += -86.11143f;

    if (mHasPropellor || isWindmillRepaired()) {
        attachPinwheel();
    }
    mCallback.attach(mMdl);
    mMdl.setAnm(mAnmChr);
    mMdl.setAnm(mAnmClr);
    mAnmClr.setFrame(0.f, 0);
    field_0x634 = mAnmClr.getFrameMax(0) - 1.f;
    if (isLightTowerRaised2() || SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlagInPlace)) {
        s32 delta = mSetRotation - mCurrentStuckPos;
        if (delta < 0) {
            delta = 0xC;
        }
        mCurrentStuckPos = mSetRotation;
        mRotation.y += 0x1555 * delta;
        mStateMgr.changeState(StateID_CollectStick);
        mTimer1 = 0;
        mTimer2 = 0;
        mAnmClr.setFrame(field_0x634, 0);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }
    mMdl.setPriorityDraw(0x1C, 9);
    mBoundingBox.Set(mVec3_c(-440.f, -10.f, -160.f), mVec3_c(440.f, 910.f, 250.f));
    mCullingDistance = 500000.f;
    return SUCCEEDED;
}

s32 dAcOwindmill_c::sRotationIncrement1 = 0x44;
s32 dAcOwindmill_c::sRotationIncrement2 = 0x44;
s32 dAcOwindmill_c::sRotationIncrement3 = -0x73e9;

int dAcOwindmill_c::doDelete() {
    return SUCCEEDED;
}

int dAcOwindmill_c::actorExecute() {
    mStateMgr.executeState();
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mBgW.Move();
    mAnmChr.play();
    if (dScGame_c::currentSpawnInfo.isNight() &&
        SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlagBeingBlown)) {
        dLightEnv_c::GetInstance().set_override_spf(0xb, 0xb, 0.f);
    }
    return SUCCEEDED;
}

int dAcOwindmill_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOwindmill_c::initializeState_Wait() {
    mTimer1 = sStateTimer1_0x32;
}
void dAcOwindmill_c::executeState_Wait() {
    if (mAnmClr.getFrame(0) != 0.f) {
        mAnmClr.play();
    }
    if (!mHasPinwheel && isWindmillRepaired()) {
        attachPinwheel();
    }
    sLib::addCalc(&field_0x630, sTargetHalf, 0.5f, 0.1f, 0.f);
    mAnmChr.setRate(field_0x630);

    if (mPinwheel.get() != NULL && mPinwheel.get()->mBeingBlown) {
        mStateMgr.changeState(StateID_CameraMove);
    } else {
        const dAcPy_c *link = dAcPy_c::GetLink();
        if (sLib::calcTimer(&mTimer1) == 0 || !link->checkActionFlagsCont(0x100)) {
            mTimer1 = 0;
            SceneflagManager::sInstance->unsetFlag(mRoomID, mSceneFlagBeingBlown);
        }
        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlagBeingBlown)) {
            link->setWindMillPos(&mWindmillPosition);
        }
    }
}
void dAcOwindmill_c::finalizeState_Wait() {}

void dAcOwindmill_c::initializeState_CameraMove() {
    mTimer1 = sStateTimer1_0xC;
    dAcPy_c::GetLink()->setWindMillPos(&mWindmillPosition);
}

void dAcOwindmill_c::executeState_CameraMove() {
    if (mPinwheel.get() != NULL && mPinwheel.get()->mBeingBlown) {
        SceneflagManager::sInstance->setFlag(mRoomID, mSceneFlagBeingBlown);
    }
    sLib::addCalc(&field_0x630, sTargetHalf, 0.5f, 0.1f, 0.f);
    mAnmChr.setRate(field_0x630);
    const dAcPy_c *link = dAcPy_c::GetLink();
    if (!link->checkActionFlagsCont(0x100)) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, mSceneFlagBeingBlown);
        const sStateIDIf_c *oldState = mStateMgr.getOldStateID();
        mStateMgr.changeState(*oldState);
    } else {
        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlagBeingBlown)) {
            link->setWindMillPos(&mWindmillPosition);
        }
        if (sLib::calcTimer(&mTimer1) == 0) {
            if (mPinwheel.get() != NULL && mPinwheel.get()->mBeingBlown) {
                const sStateIDIf_c *oldState = mStateMgr.getOldStateID();
                oldState->isEqual(StateID_CollectStick);
                mStateMgr.changeState(StateID_Move);
            } else {
                const sStateIDIf_c *oldState = mStateMgr.getOldStateID();
                mStateMgr.changeState(*oldState);
            }
        }
    }
}

void dAcOwindmill_c::finalizeState_CameraMove() {}

void dAcOwindmill_c::initializeState_Move() {
    mTimer1 = sStateTimer1_0x50;
}

void dAcOwindmill_c::executeState_Move() {
    holdSound(SE_Windmil_ROLL_LV);
    if (field_0x63A) {
        if (field_0x634 != mAnmClr.getFrame(0)) {
            mAnmClr.play();
        }
        if (sLib::calcTimer(&mTimer3) == 0) {
            field_0x63A = false;
            mAnmClr.setRate(-1.f, 0);
        }
    } else {
        if (mAnmClr.getFrame(0) != 0.f) {
            mAnmClr.play();
        }
    }
    if (mPinwheel.get() != NULL && mPinwheel.get()->mBeingBlown) {
        SceneflagManager::sInstance->setFlag(mRoomID, mSceneFlagBeingBlown);
    }
    const dAcPy_c *link = dAcPy_c::GetLink();
    if (!link->checkActionFlagsCont(0x100)) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, mSceneFlagBeingBlown);
    }

    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlagBeingBlown)) {
        link->setWindMillPos(&mWindmillPosition);
    }
    sLib::addCalc(&field_0x630, sTargetOne, 0.5f, 0.1f, 0.f);
    mAnmChr.setRate(field_0x630);
    mRotation.y += sRotationIncrement1;
    if (sLib::calcTimer(&mTimer1) == 0) {
        mCurrentStuckPos++;
        if (mCurrentStuckPos == 0xC) {
            mCurrentStuckPos = 0;
        }
        if (mCurrentStuckPos == mSetRotation) {
            mStateMgr.changeState(StateID_Collect);
        } else {
            if (mPinwheel.get() != NULL && mPinwheel.get()->mBeingBlown) {
                mStateMgr.changeState(StateID_Move);
            } else {
                mStateMgr.changeState(StateID_Wait);
            }
        }
    }
}

void dAcOwindmill_c::finalizeState_Move() {
    field_0x63A = false;
    mAnmClr.setRate(-1.f, 0);
    mRotation.y = mWindmillRotation + (mAng)mAng::fromDeg(mCurrentStuckPos * 0x1e);
}

void dAcOwindmill_c::initializeState_Collect() {}

void dAcOwindmill_c::executeState_Collect() {
    const dAcPy_c *link = dAcPy_c::GetLink();
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlagBeingBlown)) {
        link->setWindMillPos(&mWindmillPosition);
    }
    if (mPinwheel.get() != NULL && mPinwheel.get()->mBeingBlown) {
        field_0x63A = true;
        startSound(SE_Windmil_LIGHT);
        mTimer3 = 15.f - (field_0x634 + 1.f);
        mAnmClr.setRate(1.f, 0);
        sLib::addCalc(&field_0x630, sTargetOne, 0.5f, 0.1f, 0.f);
        mAnmChr.setRate(field_0x630);
        mRotation.y += sRotationIncrement2;
        mStateMgr.changeState(StateID_Move);
        mTimer1--;
    } else {
        sLib::addCalc(&field_0x630, sTargetHalf, 0.5f, 0.1f, 0.f);
        mAnmChr.setRate(field_0x630);
        startSound(SE_Windmil_CORRECT);
        dRumble_c::start(dRumble_c::sRumblePreset3, 0x11);
        mStateMgr.changeState(StateID_CollectStick);
    }
}

void dAcOwindmill_c::finalizeState_Collect() {}

void dAcOwindmill_c::initializeState_CollectStick() {
    mTimer1 = sStateTimer1_0x32;
    mTimer2 = sStateTimer2_0xF;
    mAnmClr.setRate(1.f, 0);
}

void dAcOwindmill_c::executeState_CollectStick() {
    if (!EventManager::sInstance->isInEvent()) {
        SceneflagManager::sInstance->setFlag(mRoomID, mSceneFlagInPlace);
    }
    const dAcPy_c *link = dAcPy_c::GetLink();
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneFlagBeingBlown)) {
        link->setWindMillPos(&mWindmillPosition);
    }
    if (mPinwheel.get() != NULL && mPinwheel.get()->mBeingBlown) {
        sLib::addCalc(&field_0x630, sTargetOne, 0.5f, 0.1f, 0.f);
        mAnmChr.setRate(field_0x630);
        if (mTimer2 != 0) {
            field_0x63A = true;
            startSound(SE_Windmil_LIGHT);
            mTimer3 = 15.f - (field_0x634 + 1.f);
            mStateMgr.changeState(StateID_Move);
            mAnmClr.setRate(1.f, 0);
        } else {
            mStateMgr.changeState(StateID_CameraMove);
        }
    } else {
        sLib::addCalc(&field_0x630, sTargetHalf, 0.5f, 0.1f, 0.f);
        mAnmChr.setRate(field_0x630);
        if (mTimer2 == 1) {
            startSound(SE_Windmil_LIGHT);
        }
        if (sLib::calcTimer(&mTimer2) == 0) {
            if (field_0x634 != mAnmClr.getFrame(0)) {
                mAnmClr.play();
            }
        }
        if (sLib::calcTimer(&mTimer1) == 0 || !link->checkActionFlagsCont(0x100)) {
            mTimer1 = 0;
            SceneflagManager::sInstance->unsetFlag(mRoomID, mSceneFlagBeingBlown);
        }
    }
}

void dAcOwindmill_c::finalizeState_CollectStick() {
    SceneflagManager::sInstance->unsetFlag(mRoomID, mSceneFlagInPlace);
    mAnmClr.setRate(-1.f, 0);
}

bool dAcOwindmill_c::isWindmillRepaired() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_661);
}

bool dAcOwindmill_c::isLightTowerRaised2() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_662);
}

void dAcOwindmill_c::attachPinwheel() {
    mAng3_c pinwheelRot = mRotation;
    pinwheelRot.y += sRotationIncrement3;
    dAcOpinwheel_c *pinwheel = (dAcOpinwheel_c *)dAcObjBase_c::create(
        fProfile::OBJ_PINWHEEL, mRoomID, 0, &mWindmillPosition, &pinwheelRot, nullptr, 0xFFFFFFFF
    );
    mPinwheel.link(pinwheel);
    mHasPinwheel = true;
}
