#include "d/a/obj/d_a_obj_fence_konsai.h"

#include "d/col/bg/d_bg_s.h"

SPECIAL_ACTOR_PROFILE(OBJ_FENCE_KONSAI, dAcOfenceKonsai_c, fProfile::OBJ_FENCE_KONSAI, 0x1E3, 0, 3);

bool dAcOfenceKonsai_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("FenceKonsai"));
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("FenceKonsai");
    TRY_CREATE(mModel.create(mdl, &mAllocator, 0x120));
    cBgD_t *dzb = (cBgD_t *)getOarcFile("FenceKonsai", "dzb/FenceKonsai.dzb");
    PLC *plc = (PLC *)getOarcFile("FenceKonsai", "dat/FenceKonsai.plc");
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mCollision.Set(dzb, plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mCollision.Lock();
    return true;
}

int dAcOfenceKonsai_c::create() {
    CREATE_ALLOCATOR(dAcOfenceKonsai_c);
    dBgS::GetInstance()->Regist(&mCollision, this);
    mModel.setPriorityDraw(0x1C, 0x9);
    mBoundingBox.Set(mVec3_c(-100.0f, -10.0f, -30.0f), mVec3_c(100.0f, 270.0f, 40.0f));
    return SUCCEEDED;
}

int dAcOfenceKonsai_c::doDelete() {
    return SUCCEEDED;
}

int dAcOfenceKonsai_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOfenceKonsai_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}
