#include "d/a/obj/d_a_obj_megami_island.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/room_manager.h"

static const char *const sResFiles[] = {
    "F000Megami",
    "F000LastDungeon",
};

static const char *const sMdl1[] = {
    "F000Megami",
    "F000LastDungeon",
};

static const char *const sMdl2[] = {
    "F000Megami_s",
    "F000LastDungeon_s",
};

static const char *const sDzb[] = {
    "dzb/F000Meagami.dzb",
    "dzb/F000LastDungeon.dzb",
};

static const char *const sPlc[] = {
    "dat/F000Meagami.plc",
    "dat/F000LastDungeon.plc",
};

static const char *const sAnm[] = {
    "F000Megami",
    "F000LastDungeon",
};

SPECIAL_ACTOR_PROFILE(OBJ_MEGAMI_ISLAND, dAcOmegamiIsland_c, fProfile::OBJ_MEGAMI_ISLAND, 0x1F3, 0, 2);

bool dAcOmegamiIsland_c::createHeap() {
    mVariant = getVariant();

    mRes = nw4r::g3d::ResFile(getOarcResFile(sResFiles[mVariant]));
    RoomManager::bindStageResToFile(&mRes);
    RoomManager::bindSkyCmnToResFile(&mRes);

    nw4r::g3d::ResMdl mdl1 = mRes.GetResMdl(sMdl1[mVariant]);
    TRY_CREATE(mMdls[0].create(mdl1, &heap_allocator, 0x120));

    nw4r::g3d::ResAnmTexSrt anm = mRes.GetResAnmTexSrt(sAnm[mVariant]);
    TRY_CREATE(mAnm.create(mdl1, anm, &heap_allocator, nullptr, 1));

    nw4r::g3d::ResMdl mdl2 = mRes.GetResMdl(sMdl2[mVariant]);
    TRY_CREATE(mMdls[1].create(mdl2, &heap_allocator, 0x120));

    void *dzb = getOarcFile(sResFiles[mVariant], sDzb[mVariant]);
    void *plc = getOarcFile(sResFiles[mVariant], sPlc[mVariant]);

    updateMatrix();
    for (int i = 0; i < 2; i++) {
        mMdls[i].setLocalMtx(mWorldMtx);
    }

    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBgW.Lock();

    if (mVariant == 0) {
        mBgW.OnFlag0x20();
    }

    return (BOOL)mBgW.InitMapStuff(&heap_allocator);
}

int dAcOmegamiIsland_c::create() {
    if (hasGoddessStatueFallen()) {
        return FAILED;
    }

    CREATE_ALLOCATOR(dAcOmegamiIsland_c);

    dBgS::GetInstance()->Regist(&mBgW, this);
    dBgS::GetInstance()->RegistBg(&mBgW, this);
    mMdls[0].setAnm(mAnm);

    mMdls[0].setPriorityDraw(0x1C, 9);
    mMdls[1].setPriorityDraw(0x22, 9);

    boundingBox.Set(mVec3_c(-5000.0f, -5000.0f, -23000.0f), mVec3_c(5000.0f, 10000.0f, -9000.0f));
    mCullingDistance = 500000.0f;

    return SUCCEEDED;
}

int dAcOmegamiIsland_c::doDelete() {
    dBgS::GetInstance()->UnRegist(&mBgW);
    return SUCCEEDED;
}

int dAcOmegamiIsland_c::actorExecute() {
    mAnm.play();
    return SUCCEEDED;
}

int dAcOmegamiIsland_c::draw() {
    for (int i = 0; i < 2; i++) {
        drawModelType1(&mMdls[i]);
    }
    return SUCCEEDED;
}

bool dAcOmegamiIsland_c::hasGoddessStatueFallen() {
    return StoryflagManager::sInstance->getCounterOrFlag(0x11);
}
