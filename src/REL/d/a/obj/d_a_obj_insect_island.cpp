#include "d/a/obj/d_a_obj_insect_island.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_plc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_INSECT_ISLAND, dAcOinsectIsland_c, fProfile::OBJ_INSECT_ISLAND, 0x1E8, 0, 3);

void float_ordering() {
    100000.0f;
}

bool dAcOinsectIsland_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("IslIns"));
    dStage_c::bindStageResToFile(&mRes);
    dStage_c::bindSkyCmnToResFile(&mRes);

    nw4r::g3d::ResMdl resMdl;
    nw4r::g3d::ResAnmTexSrt resTexSrt;

    resMdl = mRes.GetResMdl("IslIns");
    TRY_CREATE(mMainMdl.create(resMdl, &mAllocator, 0x324));

    resTexSrt = mRes.GetResAnmTexSrt("IslIns");
    TRY_CREATE(mMainAnm.create(resMdl, resTexSrt, &mAllocator, nullptr, 1));

    resMdl = mRes.GetResMdl("IslInsWater00");
    TRY_CREATE(mWtrMdl0.create(resMdl, &mAllocator, 0x324));
    if (dStage_c::getMA0IndirectSrt(&resTexSrt, resMdl)) {
        TRY_CREATE(mWtrAnm0.create(resMdl, resTexSrt, &mAllocator, nullptr, 1));
    }

    resMdl = mRes.GetResMdl("IslInsWater01");
    mWtr.linkMdl(resMdl, &mWtr);
    TRY_CREATE(mWtrMdl1.create(resMdl, &mAllocator, 0x324));
    if (dStage_c::getMA0IndirectSrt(&resTexSrt, resMdl)) {
        TRY_CREATE(mWtrAnm1.create(resMdl, resTexSrt, &mAllocator, nullptr, 1));
    }

    cBgD_t *pDzb = static_cast<cBgD_t *>(getOarcFile("IslIns", "dzb/IslIns.dzb"));
    PLC *pPLC = static_cast<PLC *>(getOarcFile("IslIns", "dat/IslIns.plc"));
    updateMatrix();
    mMainMdl.setLocalMtx(mWorldMtx);
    mWtrMdl0.setLocalMtx(mWorldMtx);
    mWtrMdl1.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mBgW.Set(pDzb, pPLC, 1, &mWorldMtx, &mScale));
    mBgW.Lock();

    pDzb = static_cast<cBgD_t *>(getOarcFile("IslIns", "dzb/IslIns_Bird.dzb"));
    pPLC = static_cast<PLC *>(getOarcFile("IslIns", "dat/IslIns_Bird.plc"));
    TRY_CREATE(!mBgWBird.Set(pDzb, pPLC, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBgWBird.Lock();

    return true;
}

int dAcOinsectIsland_c::create() {
    dStageMgr_c::GetInstance()->initUnkWithWater(0, &mWtr);

    CREATE_ALLOCATOR(dAcOinsectIsland_c);

    dBgS::GetInstance()->Regist(&mBgW, this);
    dBgS::GetInstance()->Regist(&mBgWBird, this);

    mBgWBird.SetField_0x22(2);

    mMainMdl.setAnm(mMainAnm);
    mWtrMdl0.setAnm(mWtrAnm0);
    mWtrMdl1.setAnm(mWtrAnm1);

    mWtrMdl1.setPriorityDraw(0x1C, 0x84);
    mWtrMdl0.setPriorityDraw(0x1C, 9);
    mMainMdl.setPriorityDraw(0x1C, 9);

    mBoundingBox.Set(mVec3_c(-4700.0f, -2500.0f, -3000.0f), mVec3_c(3300.0f, 1800.0f, 2400.0f));
    mCullingDistance = 500000.0f;

    return SUCCEEDED;
}

int dAcOinsectIsland_c::doDelete() {
    dStageMgr_c::GetInstance()->destroyUnkWithWater(0, &mWtr);
    return SUCCEEDED;
}

int dAcOinsectIsland_c::actorExecute() {
    mMainAnm.play();
    mWtrAnm0.play();
    mWtrAnm1.play();
    return SUCCEEDED;
}

int dAcOinsectIsland_c::draw() {
    drawModelType1(&mMainMdl);
    drawModelType1(&mWtrMdl0);
    drawModelType1(&mWtrMdl1);
    return SUCCEEDED;
}
