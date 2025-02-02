#include "d/a/obj/d_a_obj_stage_cover.h"

#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_STAGE_COVER, dAcOstageCover_c, fProfile::OBJ_STAGE_COVER, 0x01E1, 0, 0);

STATE_DEFINE(dAcOstageCover_c, Wait);

bool dAcOstageCover_c::createHeap() {
    mBrres = static_cast<nw4r::g3d::ResFile>(CurrentStageArcManager::sInstance->getData("g3d/stage.brres"));
    nw4r::g3d::ResMdl mdl = mBrres.GetResMdl("StageCover");
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x120));
    return true;
}

int dAcOstageCover_c::create() {
    CREATE_ALLOCATOR(dAcOstageCover_c);

    mStateMgr.changeState(StateID_Wait);
    mModel.setPriorityDraw(0x1C, 9);
    boundingBox.Set(mVec3_c(-0.0f, -0.0f, -0.0f), mVec3_c(0.0f, 0.0f, 0.0f));
    return SUCCEEDED;
}

int dAcOstageCover_c::doDelete() {
    return SUCCEEDED;
}

int dAcOstageCover_c::actorExecute() {
    mStateMgr.executeState();
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    return SUCCEEDED;
}

int dAcOstageCover_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}

void dAcOstageCover_c::initializeState_Wait() {}
void dAcOstageCover_c::executeState_Wait() {
    if (mDeletionRequested) {
        deleteRequest();
    }
}
void dAcOstageCover_c::finalizeState_Wait() {}
