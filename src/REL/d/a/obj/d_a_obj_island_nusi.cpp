#include "d/a/obj/d_a_obj_island_nusi.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_stage.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"

const f32 dAcOislandNusi_c::someFloat = 100000.0f;

static const char *const sMdlNames[] = {
    "IslNusi",
    "IslNusi_s",
};

SPECIAL_ACTOR_PROFILE(OBJ_ISLAND_NUSI, dAcOislandNusi_c, fProfile::OBJ_ISLAND_NUSI, 0x204, 0, 3);

STATE_DEFINE(dAcOislandNusi_c, Wait);
STATE_DEFINE(dAcOislandNusi_c, NusiFight);

bool dAcOislandNusi_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("IslNusi"));
    dStage_c::bindStageResToFile(&mRes);
    dStage_c::bindSkyCmnToResFile(&mRes);
    for (int i = 0; i < 2; i++) {
        nw4r::g3d::ResMdl mdl = mRes.GetResMdl(sMdlNames[i]);
        TRY_CREATE(mMdls[i].create(mdl, &heap_allocator, 0x160));
    }

    mMdls[0].setOption(0x30001, 0);

    void *dzb = getOarcFile("IslNusi", "dzb/IslNusi.dzb");
    void *plc = getOarcFile("IslNusi", "dat/IslNusi.plc");
    updateMatrix();
    for (int i = 0; i < 2; i++) {
        mMdls[i].setLocalMtx(mWorldMtx);
    }
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBgW.Lock();
    return mBgW.InitMapStuff(&heap_allocator);
}

int dAcOislandNusi_c::create() {
    CREATE_ALLOCATOR(dAcOislandNusi_c);
    dBgS::GetInstance()->Regist(&mBgW, this);
    dBgS::GetInstance()->RegistBg(&mBgW, this);
    if (isInLeviasFight()) {
        mStateMgr.changeState(StateID_NusiFight);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }

    mMdls[0].setPriorityDraw(0x1C, 9);
    mMdls[1].setPriorityDraw(0x22, 9);
    boundingBox.Set(mVec3_c(-4000.0f, -1200.0f, -2700.0f), mVec3_c(4800.0f, 3400.0f, 1700.0f));

    mCullingDistance = 500000.0f;

    return SUCCEEDED;
}

int dAcOislandNusi_c::doDelete() {
    dBgS::GetInstance()->UnRegist(&mBgW);
    return SUCCEEDED;
}

int dAcOislandNusi_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcOislandNusi_c::draw() {
    for (int i = 0; i < 2; i++) {
        drawModelType1(&mMdls[i]);
    }
    return SUCCEEDED;
}

void dAcOislandNusi_c::initializeState_Wait() {
    nw4r::g3d::ScnMdl *m = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdl>(mMdls[0].getG3dObject());
    nw4r::g3d::ResMdl resM = mMdls[0].getResMdl();
    nw4r::g3d::ResNode resNd = resM.GetResNode("pPipe1");
    u32 id = resNd.GetID();
    nw4r::g3d::ScnMdl::CopiedVisAccess visAccess(m, id);
    visAccess.SetVisibility(true);
}
void dAcOislandNusi_c::executeState_Wait() {
    if (isInLeviasFight()) {
        mStateMgr.changeState(StateID_NusiFight);
    }
}
void dAcOislandNusi_c::finalizeState_Wait() {}

void dAcOislandNusi_c::initializeState_NusiFight() {
    nw4r::g3d::ScnMdl *m = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdl>(mMdls[0].getG3dObject());
    nw4r::g3d::ResMdl resM = mMdls[0].getResMdl();
    nw4r::g3d::ResNode resNd = resM.GetResNode("pPipe1");
    u32 id = resNd.GetID();
    nw4r::g3d::ScnMdl::CopiedVisAccess visAccess(m, id);
    visAccess.SetVisibility(false);
}
void dAcOislandNusi_c::executeState_NusiFight() {
    if (!isInLeviasFight()) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dAcOislandNusi_c::finalizeState_NusiFight() {}

bool dAcOislandNusi_c::isInLeviasFight() const {
    return StoryflagManager::sInstance->getCounterOrFlag(368) && !StoryflagManager::sInstance->getCounterOrFlag(200);
}
