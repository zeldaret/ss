#include "d/a/obj/d_a_obj_girahim_foot.h"

#include "d/a/obj/d_a_obj_base.h"
#include "f/f_base.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

SPECIAL_ACTOR_PROFILE(OBJ_GIRAHIM_FOOT, dAcOgirahimFoot_c, fProfile::OBJ_GIRAHIM_FOOT, 0x210, 0, 6);

STATE_DEFINE(dAcOgirahimFoot_c, Wait);
STATE_DEFINE(dAcOgirahimFoot_c, Appear);

int dAcOgirahimFoot_c::create() {
    CREATE_ALLOCATOR(dAcOgirahimFoot_c);

    updateMatrix();

    mMdl.setLocalMtx(mWorldMtx);

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

bool dAcOgirahimFoot_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("Girahim_Foot"));

    nw4r::g3d::ResMdl resMdl = mRes.GetResMdl("Girahim_Foot");

    TRY_CREATE(mMdl.create(resMdl, &heap_allocator, 0x120, 1, nullptr));
}

int dAcOgirahimFoot_c::actorExecute() {
    return SUCCEEDED;
}

void dAcOgirahimFoot_c::initializeState_Wait() {}
void dAcOgirahimFoot_c::executeState_Wait() {}
void dAcOgirahimFoot_c::finalizeState_Wait() {}
void dAcOgirahimFoot_c::initializeState_Appear() {}
void dAcOgirahimFoot_c::executeState_Appear() {}
void dAcOgirahimFoot_c::finalizeState_Appear() {}
