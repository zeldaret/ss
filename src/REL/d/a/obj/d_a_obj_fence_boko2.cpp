#include "d/a/obj/d_a_obj_fence_boko2.h"

#include "d/col/bg/d_bg_s.h"

SPECIAL_ACTOR_PROFILE(OBJ_FENCE_BOKO2, dAcOfenceBoko2_c, fProfile::OBJ_FENCE_BOKO2, 0x1D8, 0, 3);

bool dAcOfenceBoko2_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("FenceBoko2"));
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("FenceBoko2");
    TRY_CREATE(mModel.create(mdl, &mAllocator, 0x120));
    cBgD_t *dzb = (cBgD_t *)getOarcFile("FenceBoko2", "dzb/FenceBoko2.dzb");
    PLC *plc = (PLC *)getOarcFile("FenceBoko2", "dat/FenceBoko2.plc");
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mCollision.Set(dzb, plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mCollision.Lock();
    return mCollision.InitMapStuff(&mAllocator);
}

int dAcOfenceBoko2_c::create() {
    CREATE_ALLOCATOR(dAcOfenceBoko2_c);
    dBgS::GetInstance()->Regist(&mCollision, this);
    dBgS::GetInstance()->RegistBg(&mCollision, this);
    mModel.setPriorityDraw(0x1C, 0x9);
    boundingBox.Set(mVec3_c(-160.0f, -40.0f, -70.0f), mVec3_c(180.0f, 300.0f, 60.0f));
    mCullingDistance = 50000.0f;
    return SUCCEEDED;
}

int dAcOfenceBoko2_c::doDelete() {
    dBgS::GetInstance()->UnRegist(&mCollision);
    return SUCCEEDED;
}

int dAcOfenceBoko2_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOfenceBoko2_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}
