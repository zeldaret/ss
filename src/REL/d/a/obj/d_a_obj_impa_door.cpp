#include "d/a/obj/d_a_obj_impa_door.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_IMPA_DOOR, dAcOimpaDoor_c, fProfile::OBJ_IMPA_DOOR, 0x20C, 0, 3);

STATE_DEFINE(dAcOimpaDoor_c, Wait);

bool dAcOimpaDoor_c::createHeap() {
    void *data = getOarcFile("DoorG", "g3d/model.brres");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("DoorG");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));
    return true;
}

int dAcOimpaDoor_c::create() {
    CREATE_ALLOCATOR(dAcOimpaDoor_c);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mStateMgr.changeState(StateID_Wait);
    mMdl.setPriorityDraw(0x1C, 9);
    boundingBox.Set(mVec3_c(-250.0f, 0.0f, -30.0f), mVec3_c(250.0f, 940.0f, 30.0f));
    return SUCCEEDED;
}

int dAcOimpaDoor_c::doDelete() {
    return SUCCEEDED;
}

int dAcOimpaDoor_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcOimpaDoor_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOimpaDoor_c::initializeState_Wait() {}
void dAcOimpaDoor_c::executeState_Wait() {}
void dAcOimpaDoor_c::finalizeState_Wait() {}
