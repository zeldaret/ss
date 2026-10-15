#include "d/a/obj/d_a_obj_siren_2dmap.h"

#include "d/col/bg/d_bg_s.h"

static const char *const sOarcNames[] = {
    "SirenMaikiMapS200",
    "F406Col",
};

static const char *const sDzbNames[] = {
    "dzb/SirenMaikiMapS200.dzb",
    "dzb/F406Col.dzb",
};

static const char *const sPlcNames[] = {
    "dat/SirenMaikiMapS200.plc",
    "dat/F406Col.plc",
};

SPECIAL_ACTOR_PROFILE(OBJ_SIREN_2DMAP, dAcOsiren2dmap_c, fProfile::OBJ_SIREN_2DMAP, 0x21A, 0, 0);

bool dAcOsiren2dmap_c::createHeap() {
    mSubtype = mParams & 0xF;
    if (mSubtype == 0xF) {
        mSubtype = 0;
    }

    void *dzb = getOarcFile(sOarcNames[mSubtype], sDzbNames[mSubtype]);
    void *plc = getOarcFile(sOarcNames[mSubtype], sPlcNames[mSubtype]);

    updateMatrix();

    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBgW.Lock();
    return mBgW.InitMapStuff(&mAllocator);
}

int dAcOsiren2dmap_c::create() {
    CREATE_ALLOCATOR(dAcOsiren2dmap_c);

    dBgS::GetInstance()->Regist(&mBgW, this);
    u8 roomId = mSubtype == 1;
    mBgW.SetRoomId(roomId);
    mRoomID = roomId;
    dBgS::GetInstance()->RegistBg(&mBgW, this);
    mBoundingBox.Set(mVec3_c(-0.0f, -0.0f, -0.0f), mVec3_c(0.0f, 0.0f, 0.0f));

    return SUCCEEDED;
}

int dAcOsiren2dmap_c::doDelete() {
    dBgS::GetInstance()->UnRegist(&mBgW);
    return SUCCEEDED;
}

int dAcOsiren2dmap_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOsiren2dmap_c::draw() {
    return SUCCEEDED;
}
