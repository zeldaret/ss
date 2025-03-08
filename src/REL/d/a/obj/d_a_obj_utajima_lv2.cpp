#include "d/a/obj/d_a_obj_utajima_lv2.h"

#include "common.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_stage.h"
#include "m/m_vec.h"

SPECIAL_ACTOR_PROFILE(OBJ_UTAJIMA_LV2, dAcOutajimaLv2_c, fProfile::OBJ_UTAJIMA_LV2, 0x1D5, 0, 3);

const f32 dAcOutajimaLv2_c::someFloat = 100000.0f;

bool dAcOutajimaLv2_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("IslCave"));
    dStage_c::bindStageResToFile(&mRes);
    dStage_c::bindSkyCmnToResFile(&mRes);
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl("IslCave");
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120));

    void *dzb = getOarcFile("IslCave", "dzb/IslCave.dzb");
    void *plc = getOarcFile("IslCave", "dat/IslCave.plc");
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBgW.Lock();
    // TODO InitMapStuff says it already returns a bool
    BOOL ok = mBgW.InitMapStuff(&heap_allocator);
    return ok;
}

int dAcOutajimaLv2_c::create() {
    CREATE_ALLOCATOR(dAcOutajimaLv2_c);

    dBgS::GetInstance()->Regist(&mBgW, this);
    dBgS::GetInstance()->RegistBg(&mBgW, this);
    mMdl.setPriorityDraw(0x1C, 9);
    boundingBox.Set(mVec3_c(-2800.0f, -4000.0f, -2600.0f), mVec3_c(2600.0f, 1900.0f, 2700.0f));
    mCullingDistance = 500000.0f;
    return SUCCEEDED;
}

int dAcOutajimaLv2_c::doDelete() {
    dBgS::GetInstance()->UnRegist(&mBgW);
    return SUCCEEDED;
}

int dAcOutajimaLv2_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOutajimaLv2_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
