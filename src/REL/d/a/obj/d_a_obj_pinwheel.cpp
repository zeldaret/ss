#include "d/a/obj/d_a_obj_pinwheel.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_stage.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"

SPECIAL_ACTOR_PROFILE(OBJ_PINWHEEL, dAcOpinwheel_c, fProfile::OBJ_PINWHEEL, 0x1DD, 0, 2);

STATE_DEFINE(dAcOpinwheel_c, Wait);
STATE_DEFINE(dAcOpinwheel_c, Acceleration);
STATE_DEFINE(dAcOpinwheel_c, RollMaxSpeed);
STATE_DEFINE(dAcOpinwheel_c, Deceleration);

dCcD_SrcSph dAcOpinwheel_c::sSphSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x111, {0, 6, 0x8}, 8, 0},
     /* mObjCo */ {0xE9}},
    /* mSphInf */
    {50.f},
};

const u32 dAcOpinwheel_c::unused = 0x1E1388;

bool dAcOpinwheel_c::createHeap() {
    void *data = getOarcResFile("Pinwheel");
    mResFile = nw4r::g3d::ResFile(data);
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("Pinwheel");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));

    void *dzb = getOarcFile("Pinwheel", "dzb/Pinwheel.dzb");
    void *plc = getOarcFile("Pinwheel", "dat/Pinwheel.plc");
    updateMatrix();
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
}

int dAcOpinwheel_c::create() {
    CREATE_ALLOCATOR(dAcOpinwheel_c);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mStts.SetRank(0xD);
    mSph.Set(sSphSrc);
    mSph.SetStts(mStts);
    mStateMgr.changeState(StateID_Wait);
    mMdl.setPriorityDraw(0x1C, 9);
    mBoundingBox.Set(mVec3_c(-75.f, -75.f, -10.f), mVec3_c(75.f, 75.f, 50.f));
    return SUCCEEDED;
}

int dAcOpinwheel_c::doDelete() {
    return SUCCEEDED;
}

int dAcOpinwheel_c::actorExecute() {
    mStateMgr.executeState();
    mSph.SetC(mPosition);
    dCcS::GetInstance()->Set(&mSph);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    return SUCCEEDED;
}

int dAcOpinwheel_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOpinwheel_c::initializeState_Wait() {
    mRotationSpeed = 0;
}

void dAcOpinwheel_c::executeState_Wait() {
    if (isBeingBlown()) {
        mStateMgr.changeState(StateID_Acceleration);
    }
}

void dAcOpinwheel_c::finalizeState_Wait() {}

void dAcOpinwheel_c::initializeState_Acceleration() {
    mStateTimer = 0x1E;
}

void dAcOpinwheel_c::executeState_Acceleration() {
    holdSound(SE_Pinwhel_ROLL_LV);
    mRotationSpeed += 0xA6;
    mRotation.z += mRotationSpeed;
    if (sLib::calcTimer(&mStateTimer) == 0) {
        mStateMgr.changeState(StateID_RollMaxSpeed);
    } else if (!isBeingBlown()) {
        u16 stateTimer = 0x1E - mStateTimer;
        mStateMgr.changeState(StateID_Deceleration);
        mStateTimer = stateTimer;
    }
}

void dAcOpinwheel_c::finalizeState_Acceleration() {}

void dAcOpinwheel_c::initializeState_RollMaxSpeed() {
    mBeingBlown = true;
}

void dAcOpinwheel_c::executeState_RollMaxSpeed() {
    holdSound(SE_Pinwhel_ROLL_LV);
    mRotation.z += mRotationSpeed;
    if (!isBeingBlown()) {
        if (sLib::calcTimer(&mStateTimer) == 0) {
            mStateMgr.changeState(StateID_Deceleration);
        }

    } else {
        mStateTimer = 2;
    }
}

void dAcOpinwheel_c::finalizeState_RollMaxSpeed() {
    mBeingBlown = false;
}

void dAcOpinwheel_c::initializeState_Deceleration() {
    mStateTimer = 0x1E;
}

void dAcOpinwheel_c::executeState_Deceleration() {
    holdSound(SE_Pinwhel_ROLL_LV);
    mRotationSpeed -= 0xA6;
    mRotation.z += mRotationSpeed;
    if (sLib::calcTimer(&mStateTimer) == 0) {
        mStateMgr.changeState(StateID_Wait);
    } else if (isBeingBlown()) {
        u16 stateTimer = 0x1E - mStateTimer;
        mStateMgr.changeState(StateID_Acceleration);
        mStateTimer = stateTimer;
    }
}

void dAcOpinwheel_c::finalizeState_Deceleration() {}

bool dAcOpinwheel_c::isBeingBlown() {
    return mSph.ChkTgHit() && mSph.ChkTgAtHitType(AT_TYPE_BELLOWS);
}
