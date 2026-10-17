#include "d/a/obj/d_a_obj_display_only_nbs.h"

#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

static const char *const sMdlNames[] = {
    "StageD301Enkei",
};

SPECIAL_ACTOR_PROFILE(OBJ_DISPLAY_ONLY_NBS, dAcOdisplayOnlyNbs_c, fProfile::OBJ_DISPLAY_ONLY_NBS, 0x21B, 0, 3);

bool dAcOdisplayOnlyNbs_c::createHeap() {
    mSubtype = getFromParams(0, 0xF);
    mResFile = nw4r::g3d::ResFile(CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres"));
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl(sMdlNames[mSubtype]);
    TRY_CREATE(mModel.create(mdl, &mAllocator, 0x120));
    return true;
}

int dAcOdisplayOnlyNbs_c::create() {
    CREATE_ALLOCATOR(dAcOdisplayOnlyNbs_c);
    mModel.setPriorityDraw(0x1C, 9);
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    mVec3_c min, max;
    mModel.getBounds(&min, &max);
    mBoundingBox.Set(min, max);
    return SUCCEEDED;
}

int dAcOdisplayOnlyNbs_c::doDelete() {
    return SUCCEEDED;
}

int dAcOdisplayOnlyNbs_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOdisplayOnlyNbs_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}
