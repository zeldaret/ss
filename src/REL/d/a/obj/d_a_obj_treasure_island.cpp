#include "d/a/obj/d_a_obj_treasure_island.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"

const f32 dAcOtreasureIsland_c::unkFloat0 = 100000.0f;
const f32 dAcOtreasureIsland_c::unkFloat1 = 50000.0f;

static const char *const AcOtreasureIsland__OarcNames[] = {
    "IslTreA", "IslTreB", "IslTreC", "IslTreD", "IslTreE", "IslTreF", "IslTreG", "IslTreH",
};
static const char *const AcOtreasureIsland__ModelNames[] = {
    "IslTreA", "IslTreB", "IslTreC", "IslTreD", "IslTreE", "IslTreF", "IslTreG", "IslTreH",
};
static const char *const AcOtreasureIsland__DbzNames[] = {
    "dzb/IslTreA.dzb", "dzb/IslTreB.dzb", "dzb/IslTreC.dzb", "dzb/IslTreD.dzb",
    "dzb/IslTreE.dzb", "dzb/IslTreF.dzb", "dzb/IslTreG.dzb", "dzb/IslTreH.dzb",
};
static const char *const AcOtreasureIsland__PlcNames[] = {
    "dat/IslTreA.plc", "dat/IslTreB.plc", "dat/IslTreC.plc", "dat/IslTreD.plc",
    "dat/IslTreE.plc", "dat/IslTreF.plc", "dat/IslTreG.plc", "dat/IslTreH.plc",
};

SPECIAL_ACTOR_PROFILE(OBJ_TREASURE_ISLAND, dAcOtreasureIsland_c, fProfile::OBJ_TREASURE_ISLAND, 0x1EE, 0, 3);

bool dAcOtreasureIsland_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile(AcOtreasureIsland__OarcNames[subtype]));
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);

    nw4r::g3d::ResMdl mdl;
    nw4r::g3d::ResAnmTexSrt anmSrt;

    mdl = mResFile.GetResMdl(AcOtreasureIsland__ModelNames[subtype]);
    TRY_CREATE(mModel.create(mdl, &mAllocator, 0x120));

    if (subtype == 2) {
        mdl = mResFile.GetResMdl("IslTreCWater00");
        TRY_CREATE(mWater00.create(mdl, &mAllocator, 0x324));

        if (dStage_c::getMA0IndirectSrt(&anmSrt, mdl)) {
            TRY_CREATE(mWaterSrt00.create(mdl, anmSrt, &mAllocator, 0x0, 1));
        }

        mdl = mResFile.GetResMdl("IslTreCWater01");
        d3d::UnkWithWater::linkMdl(mdl, &waterStuff);
        TRY_CREATE(mWater01.create(mdl, &mAllocator, 0x324));

        if (dStage_c::getMA0IndirectSrt(&anmSrt, mdl)) {
            TRY_CREATE(mWaterSrt01.create(mdl, anmSrt, &mAllocator, 0x0, 1));
        }
    }

    void *dzb = getOarcFile(AcOtreasureIsland__OarcNames[subtype], AcOtreasureIsland__DbzNames[subtype]);
    void *plc = getOarcFile(AcOtreasureIsland__OarcNames[subtype], AcOtreasureIsland__PlcNames[subtype]);
    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);

    if (subtype == 2) {
        mWater00.setLocalMtx(mWorldMtx);
        mWater01.setLocalMtx(mWorldMtx);
    }

    TRY_CREATE(!mCollision.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mCollision.Lock();
    return mCollision.InitMapStuff(&mAllocator);
}

int dAcOtreasureIsland_c::create() {
    subtype = mParams & 0xf;
    if (subtype == 2) {
        dStageMgr_c::GetInstance()->initUnkWithWater(0, &waterStuff);
    }

    CREATE_ALLOCATOR(dAcOtreasureIsland_c);
    dBgS::GetInstance()->Regist(&mCollision, this);
    dBgS::GetInstance()->RegistBg(&mCollision, this);
    mModel.setPriorityDraw(0x1C, 0x9);

    if (subtype == 2) {
        mWater00.setAnm(mWaterSrt00);
        mWater01.setAnm(mWaterSrt01);
        mWater01.setPriorityDraw(0x1C, 0x84);
        mWater00.setPriorityDraw(0x1C, 0x9);
    }

    mBoundingBox.Set(mVec3_c(-1850.0f, -2050.0f, -1900.0f), mVec3_c(1950.0f, 2500.0f, 1850.0f));
    mCullingDistance = 500000.0f;
    return SUCCEEDED;
}

int dAcOtreasureIsland_c::doDelete() {
    if (subtype == 2) {
        dStageMgr_c::GetInstance()->destroyUnkWithWater(0, &waterStuff);
    }
    dBgS::GetInstance()->UnRegist(&mCollision);
    return SUCCEEDED;
}

int dAcOtreasureIsland_c::actorExecute() {
    if (subtype == 2) {
        mWaterSrt00.play();
        mWaterSrt01.play();
    }
    return SUCCEEDED;
}

int dAcOtreasureIsland_c::draw() {
    drawModelType1(&mModel);
    if (subtype == 2) {
        drawModelType1(&mWater00);
        drawModelType1(&mWater01);
    }
    return SUCCEEDED;
}
