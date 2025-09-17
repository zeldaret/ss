#include "d/a/obj/d_a_obj_water_shield.h"

#include "d/col/bg/d_bg_s.h"
#include "nw4r/types_nw4r.h"

SPECIAL_ACTOR_PROFILE(OBJ_WATER_SHIELD, dAcOwaterShield_c, fProfile::OBJ_WATER_SHIELD, 0x218, 0, 0);

bool dAcOwaterShield_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("WaterWallF103"));
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl("WaterWallF103");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x32C));
    nw4r::g3d::ResAnmClr clr = mRes.GetResAnmClr("WaterWallF103");
    TRY_CREATE(mAnmClr.create(mdl, clr, &mAllocator, nullptr, 1));
    nw4r::g3d::ResAnmTexSrt srt = mRes.GetResAnmTexSrt("WaterWallF103");
    TRY_CREATE(mAnmSrt.create(mdl, srt, &mAllocator, nullptr, 1));
    cBgD_t *dzb = (cBgD_t *)getOarcFile("WaterWallF103", "dzb/WaterWallF103.dzb");
    PLC *plc = (PLC *)getOarcFile("WaterWallF103", "dat/WaterWallF103.plc");
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mBgW.Set(dzb, plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBgW.Lock();
    return true;
}

int dAcOwaterShield_c::create() {
    CREATE_ALLOCATOR(dAcOwaterShield_c);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mScnCallback.attach(mMdl);
    mMdl.setAnm(mAnmSrt);
    mMdl.setAnm(mAnmClr);

    mMdl.setPriorityDraw(0x1C, 0x9);

    boundingBox.Set(mVec3_c(-12000.0f, -2000.0f, -14000.0f), mVec3_c(12000.0f, 11000.0f, 12000.0f));

    return SUCCEEDED;
}

int dAcOwaterShield_c::doDelete() {
    return SUCCEEDED;
}

int dAcOwaterShield_c::actorExecute() {
    mAnmSrt.play();
    mAnmClr.play();
    return SUCCEEDED;
}

int dAcOwaterShield_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

// Weak function order issue IScnObjCallback
