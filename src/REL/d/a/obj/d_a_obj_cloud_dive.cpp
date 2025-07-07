#include "d/a/obj/d_a_obj_cloud_dive.h"

#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_s.h"
#include "egg/math/eggMath.h"
#include "f/f_base.h"
#include "m/m_vec.h"

SPECIAL_ACTOR_PROFILE(OBJ_CLOUD_DIVE, dAcOcloudDive_c, fProfile::OBJ_CLOUD_DIVE, 0x1C2, 0, 0);

const f32 dAcOcloudDive_c::mRadius1 = 100.0f;
const f32 dAcOcloudDive_c::mRadius2 = 200.0f;
const f32 dAcOcloudDive_c::lbl_350_rodata_8 = 40.0f;

dCcD_SrcCyl dAcOcloudDive_c::sCylSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {AT_TYPE_BOMB, 0x111, {0, 0, 0x407}, 0, 0},
     /* mObjCo */ {0}},
    /* mCylInf */
    {100.f, 10.f}
};

STATE_DEFINE(dAcOcloudDive_c, Wait);
STATE_DEFINE(dAcOcloudDive_c, Dead);

bool dAcOcloudDive_c::createHeap() {
    return true;
}

int dAcOcloudDive_c::create() {
    CREATE_ALLOCATOR(dAcOcloudDive_c);
    mStts.mRank = 0xD;
    mCollider.Set(sCylSrc);
    mCollider.mStts = &mStts;
    field_0x4DC = getFromParams(0, 3);
    mItemID = getFromParams(2, 0xFF);
    if ((u16)getFromParams(2, 0xFF) == 0xFF) {
        mItemID = 0;
    }
    if (field_0x4DC == 1) {
        mRadius = mRadius2;
    } else {
        mRadius = mRadius1;
    }
    updateMatrix();
    mStateMgr.changeState(StateID_Wait);
    boundingBox.Set(mVec3_c(-0.0f, -0.0f, -0.0f), mVec3_c(0.0f, 0.0f, 0.0f));
    return SUCCEEDED;
}

int dAcOcloudDive_c::doDelete() {
    return SUCCEEDED;
}

int dAcOcloudDive_c::actorExecute() {
    mStateMgr.executeState();
    mCollider.SetR(mRadius);
    mCollider.SetH(40.0f);
    mVec3_c fStack = position - mVec3_c(0.0f, 20.0f, 0.0f);
    mCollider.SetC(fStack);
    dCcS::GetInstance()->Set(&mCollider);
    return SUCCEEDED;
}

int dAcOcloudDive_c::draw() {
    return SUCCEEDED;
}

void dAcOcloudDive_c::initializeState_Wait() {}

void dAcOcloudDive_c::executeState_Wait() {
    if (fn_350_6F0()) {
        mStateMgr.changeState(StateID_Wait);
    }
    if (fn_350_760()) {
        dAcItem_c::giveItem22((ITEM_ID)mItemID, 0, -1);
        mStateMgr.changeState(StateID_Wait);
    }
}

void dAcOcloudDive_c::finalizeState_Wait() {}

void dAcOcloudDive_c::initializeState_Dead() {
    deleteRequest();
}

void dAcOcloudDive_c::executeState_Dead() {}
void dAcOcloudDive_c::finalizeState_Dead() {}

bool dAcOcloudDive_c::fn_350_6F0() {
    return mCollider.mTg.mRPrm & 1 && mCollider.mTg.GetActor() != nullptr && mCollider.ChkTgAtHitType(0x20);
}

bool dAcOcloudDive_c::fn_350_760() {
    mVec3_c deltaPosition = dAcPy_c::GetLink()->position - position;
    f32 distance = EGG::Math<f32>::sqrt(deltaPosition.squareMagXZ());

    // return distance <= mRadius && deltaPosition.y <= 20.0f && deltaPosition.y >= -20.0f;

    bool temp = false;
    if (distance <= mRadius) {
        if (deltaPosition.y <= 20.0f) {
            temp = true;
        }
    }
    if (temp) {
        if (deltaPosition.y >= -20.0f) {
            return true;
        }
    }
    return false;
}
