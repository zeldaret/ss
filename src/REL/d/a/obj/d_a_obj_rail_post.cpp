#include "d/a/obj/d_a_obj_rail_post.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_RAIL_POST, dAcORailPost, fProfile::OBJ_RAIL_POST, 0x280, 0, 3);

dAcORailPost::dAcORailPost() {}
dAcORailPost::~dAcORailPost() {}

bool dAcORailPost::createHeap() {
    void *resFilePtr = getOarcResFile("RailPost");
    if (resFilePtr == nullptr) {
        return false;
    }

    nw4r::g3d::ResFile res(resFilePtr);
    if (res.IsValid() == false) {
        return false;
    }

    nw4r::g3d::ResMdl mdl = res.GetResMdl("RailPost");
    if (mdl.IsValid() == false) {
        return false;
    }

    if (!mMdl.create(mdl, &heap_allocator, 0x20, 1, nullptr)) {
        return false;
    }

    mMdl.setLocalMtx(mWorldMtx);
    void *dzb = getOarcDZB("RailPost", "RailPost");
    void *plc = getOarcPLC("RailPost", "RailPost");
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, 0x81, &mWorldMtx, &mScale));

    dBgS::GetInstance()->Regist(&mBgW, this);

    return true;
}

int dAcORailPost::create() {
    mVec3_c boundsMin;
    mVec3_c boundsMax;
    updateMatrix();
    CREATE_ALLOCATOR(dAcORailPost);

    mMdl.getBounds(&boundsMin, &boundsMax);
    boundingBox.Set(boundsMin, boundsMax);
    mCullingDistance = 25000.0f;

    return SUCCEEDED;
}

int dAcORailPost::doDelete() {
    dBgS::GetInstance()->Release(&mBgW);
    return SUCCEEDED;
}

int dAcORailPost::actorExecute() {
    return SUCCEEDED;
}

int dAcORailPost::draw() {
    mMdl.entry();
    return SUCCEEDED;
}
