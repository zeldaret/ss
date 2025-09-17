#include "d/a/obj/d_a_obj_iron_stage.h"

#include "d/col/bg/d_bg_s.h"
#include "f/f_base.h"

SPECIAL_ACTOR_PROFILE(OBJ_IRON_STAGE, dAcOironStage_c, fProfile::OBJ_IRON_STAGE, 0x1CE, 0, 3);

bool dAcOironStage_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("IronStage"));
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("IronStage");
    TRY_CREATE(mModel.create(mdl, &mAllocator, 0x120));
    cBgD_t *dzb = (cBgD_t *)getOarcFile("IronStage", "dzb/IronStage.dzb");
    PLC *plc = (PLC *)getOarcFile("IronStage", "dat/IronStage.plc");
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mCollision.Set(dzb, plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mCollision.Lock();
    return true;
}

int dAcOironStage_c::create() {
    CREATE_ALLOCATOR(dAcOironStage_c);
    dBgS::GetInstance()->Regist(&mCollision, this);
    mModel.setPriorityDraw(0x1C, 0x9);
    mBoundingBox.Set(mVec3_c(-460.0f, -210.0f, -260.0f), mVec3_c(460.0f, 210.0f, 260.0f));
    mCullingDistance = 15000.0f;
    return SUCCEEDED;
}

int dAcOironStage_c::doDelete() {
    return SUCCEEDED;
}

int dAcOironStage_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOironStage_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}
