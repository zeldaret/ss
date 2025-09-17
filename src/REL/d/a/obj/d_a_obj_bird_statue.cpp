#include "d/a/obj/d_a_obj_bird_statue.h"

SPECIAL_ACTOR_PROFILE(OBJ_BIRD_STATUE, dAcOBirdStatue_c, fProfile::OBJ_BIRD_STATUE, 0x285, 0, 3);

bool dAcOBirdStatue_c::createHeap() {
    void *data = getOarcResFile("BirdStatue");
    if (data == nullptr) {
        return false;
    }

    nw4r::g3d::ResFile file(data);
    if (!file.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl = file.GetResMdl("BirdStatue");
    if (!mdl.IsValid()) {
        return false;
    }

    return mMdl.create(mdl, &mAllocator, 0, 1, nullptr);
}

int dAcOBirdStatue_c::create() {
    if (!initAllocatorWork1Heap(0xffffffff, "dAcOBirdStatue_c::m_allocator", 0x20)) {
        return FAILED;
    }
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    mBoundingBox.Set(min, max);
    return SUCCEEDED;
}

int dAcOBirdStatue_c::draw() {
    dAcObjBase_c::drawModelType1(&mMdl);
    return SUCCEEDED;
}
