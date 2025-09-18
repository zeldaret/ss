#include "d/a/d_a_invisible.h"

#include "d/col/bg/d_bg_s.h"
#include "f/f_base.h"
#include "m/m_vec.h"

SPECIAL_ACTOR_PROFILE(INVISIBLE, dAcInvisible, fProfile::INVISIBLE, 0x11D, 0, 0);

STATE_DEFINE(dAcInvisible, Wait);

int dAcInvisible::create() {
    mObjAcch.Set(this, 1, &mAcchCir);
    mObjAcch.SetRoofNone();
    mObjAcch.SetWaterNone();
    mAcchCir.SetWall(15.0f, 120.0f);
    mStts.SetRank(9);
    mSph.SetStts(mStts);
    mCps.SetStts(mStts);
    mAcceleration = 0.0;
    mMaxSpeed = -40.0f;
    mStateMgr.changeState(StateID_Wait);
    mBoundingBox.Set(mVec3_c(-0.0f, -0.0f, -0.0f), mVec3_c(0.0f, 0.0f, 0.0f));
    return SUCCEEDED;
}

int dAcInvisible::doDelete() {
    return SUCCEEDED;
}

int dAcInvisible::actorExecute() {
    mStateMgr.executeState();
    mObjAcch.CrrPos(*dBgS::GetInstance());
    return SUCCEEDED;
}

int dAcInvisible::draw() {
    return SUCCEEDED;
}

void dAcInvisible::initializeState_Wait() {}
void dAcInvisible::executeState_Wait() {}
void dAcInvisible::finalizeState_Wait() {}
