#include "d/a/obj/d_a_obj_stopping_rope.h"

#include "d/a/obj/d_a_obj_base.h"
#include "f/f_base.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_STOPPING_ROPE, dAcOStoppingRope_c, fProfile::OBJ_STOPPING_ROPE, 0x23B, 0, 3);

STATE_DEFINE(dAcOStoppingRope_c, Wait);
STATE_DEFINE(dAcOStoppingRope_c, SwitchOn);

bool dAcOStoppingRope_c::createHeap() {
    void *data = getOarcResFile("Crane");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("CraneN");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));
    void *dzb = getOarcFile("Crane", "dzb/Crane.dzb");
    void *plc = getOarcFile("Crane", "dat/Crane.plc");
    updateMatrix();
    mMdl.setScale(mScale);
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, 1, &mWorldMtx, &mScale);
}

int dAcOStoppingRope_c::create() {
    return FAILED;
}

int dAcOStoppingRope_c::doDelete() {
    return SUCCEEDED;
}

int dAcOStoppingRope_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcOStoppingRope_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOStoppingRope_c::initializeState_Wait() {}
void dAcOStoppingRope_c::executeState_Wait() {}
void dAcOStoppingRope_c::finalizeState_Wait() {}
void dAcOStoppingRope_c::initializeState_SwitchOn() {}
void dAcOStoppingRope_c::executeState_SwitchOn() {}
void dAcOStoppingRope_c::finalizeState_SwitchOn() {}
