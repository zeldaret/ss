#include "d/a/obj/d_a_obj_girahim_foot.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_GIRAHIM_FOOT, dAcOgirahimFoot_c, fProfile::OBJ_GIRAHIM_FOOT, 0x210, 0, 6);

STATE_DEFINE(dAcOgirahimFoot_c, Wait);
STATE_DEFINE(dAcOgirahimFoot_c, Appear);

bool dAcOgirahimFoot_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("Girahim_Foot"));

    nw4r::g3d::ResMdl resMdl = mRes.GetResMdl("Girahim_Foot");

    TRY_CREATE(mMdl.create(resMdl, &mAllocator, 0x120, 1, nullptr));

    return true;
}

int dAcOgirahimFoot_c::create() {
    CREATE_ALLOCATOR(dAcOgirahimFoot_c);

    updateMatrix();

    mMdl.setLocalMtx(mWorldMtx);

    mStateMgr.changeState(StateID_Wait);

    poscopy2.x = 13275.0f;
    poscopy2.y = 2370.0f;
    poscopy2.z = -11780.0f;

    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    boundingBox.Set(min, max);

    return SUCCEEDED;
}

int dAcOgirahimFoot_c::doDelete() {
    return SUCCEEDED;
};

int dAcOgirahimFoot_c::draw() {
    if (*mStateMgr.getStateID() == StateID_Appear) {
        drawModelType1(&mMdl);
        static mQuat_c rot(0.0f, 25.0f, 0.0f, 200.0f);
        fn_8002edb0(mShadow, mMdl, &rot, -1, -1, 0.0f);
    }
    return SUCCEEDED;
}

int dAcOgirahimFoot_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

void dAcOgirahimFoot_c::initializeState_Wait() {
    field_0x394 = false;
}

void dAcOgirahimFoot_c::executeState_Wait() {
    if (field_0x394) {
        mStateMgr.changeState(StateID_Appear);
    }
    return;
}

void dAcOgirahimFoot_c::finalizeState_Wait() {}

void dAcOgirahimFoot_c::initializeState_Appear() {
    field_0x394 = true;
}

// Not matching by one instruction
void dAcOgirahimFoot_c::executeState_Appear() {
    if (field_0x394) {
        return;
    }

    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_77_, mPosition, nullptr, &mScale, nullptr, nullptr, 0, 0);

    deleteRequest();
}

void dAcOgirahimFoot_c::finalizeState_Appear() {}
