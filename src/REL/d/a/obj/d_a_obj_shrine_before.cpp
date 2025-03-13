#include "d/a/obj/d_a_obj_shrine_before.h"

SPECIAL_ACTOR_PROFILE(OBJ_SHRINE_BEFORE, dAcOshrineBefore_c, fProfile::OBJ_SHRINE_BEFORE, 0x1EA, 0, 3);

bool dAcOshrineBefore_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("F401ShrineBef"));
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("F401ShrineBef");
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x120));
    return true;
}

int dAcOshrineBefore_c::create() {
    CREATE_ALLOCATOR(dAcOshrineBefore_c);
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    mModel.setPriorityDraw(0x1C, 0x9);
    boundingBox.Set(mVec3_c(-4000.0f, 7000.0f, 5000.0f), mVec3_c(4000.0f, 13000.0f, 14000.0f));
    mCullingDistance = 50000.0;
    return SUCCEEDED;
}

int dAcOshrineBefore_c::doDelete() {
    return SUCCEEDED;
}

int dAcOshrineBefore_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOshrineBefore_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}
