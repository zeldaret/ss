#include "d/a/obj/d_a_obj_fence_boko.h"

#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/d_stage.h"

SPECIAL_ACTOR_PROFILE(OBJ_FENCE_BOKO, dAcOfenceBoko_c, fProfile::OBJ_FENCE_BOKO, 0x1D7, 0, 3);

bool dAcOfenceBoko_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("FenceBoko"));
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("FenceBoko");
    TRY_CREATE(mModel.create(mdl, &mAllocator, 0x120));
    cBgD_t *dzb = (cBgD_t *)getOarcFile("FenceBoko", "dzb/FenceBoko.dzb");
    PLC *plc = (PLC *)getOarcFile("FenceBoko", "dat/FenceBoko.plc");
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mCollision.Set(dzb, plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mCollision.Lock();
    return mCollision.InitMapStuff(&mAllocator);
}

int dAcOfenceBoko_c::create() {
    CREATE_ALLOCATOR(dAcOfenceBoko_c);
    dBgS::GetInstance()->Regist(&mCollision, this);
    dBgS::GetInstance()->RegistBg(&mCollision, this);
    mCollision.mRoomId = dStage_c::GetInstance()->getCurrRoomId();
    roomid = mCollision.mRoomId;
    mModel.setPriorityDraw(0x1C, 0x9);
    boundingBox.Set(mVec3_c(-210.0f, -10.0f, -20.0f), mVec3_c(210.0f, 340.0f, 20.0f));
    mCullingDistance = 50000.0f;
    return SUCCEEDED;
}

int dAcOfenceBoko_c::doDelete() {
    dBgS::GetInstance()->UnRegist(&mCollision);
    return SUCCEEDED;
}

int dAcOfenceBoko_c::actorExecute() {
    // Putting these in a single if fails to match ;-;
    if (!field_0x560) {
        if (dBgS_ObjGndChk::CheckPos(mPosition + mVec3_c::Ey * 100.f)) {
            mCollision.mRoomId = dBgS_ObjGndChk::GetRoomID();
            roomid = mCollision.mRoomId;
            field_0x560 = true;
        }
    }
    return SUCCEEDED;
}

int dAcOfenceBoko_c::draw() {
    drawModelType1(&mModel);
    return SUCCEEDED;
}
