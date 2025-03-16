#include "d/a/obj/d_a_obj_shrine_after.h"

SPECIAL_ACTOR_PROFILE(OBJ_SHRINE_AFTER, dAcOshrineAfter_c, fProfile::OBJ_SHRINE_AFTER, 0x1E9, 0, 3);

bool dAcOshrineAfter_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("F401ShrineAft"));
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("F401ShrineAft");
    TRY_CREATE(mModel.create(mdl, &heap_allocator, 0x120));
    return true;
}

int dAcOshrineAfter_c::create() {
    CREATE_ALLOCATOR(dAcOshrineAfter_c);
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    mModel.setPriorityDraw(0x1C, 0x9);
    boundingBox.Set(mVec3_c(-4000.0f, 7000.0f, 5000.0f), mVec3_c(5000.0f, 15000.0f, 14000.0f));
    mCullingDistance = 50000.0f;
    return SUCCEEDED;
}

int dAcOshrineAfter_c::doDelete() {
    return SUCCEEDED;
}

int dAcOshrineAfter_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOshrineAfter_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}
