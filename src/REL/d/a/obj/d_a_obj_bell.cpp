#include "d/a/obj/d_a_obj_bell.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_plc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_stage.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/dowsing_target.h"

SPECIAL_ACTOR_PROFILE(OBJ_BELL, dAcObell_c, fProfile::OBJ_BELL, 0x208, 0, 2);

STATE_DEFINE(dAcObell_c, Wait);
STATE_DEFINE(dAcObell_c, WaitRupee);
STATE_DEFINE(dAcObell_c, After);

static dCcD_SrcSph sSrcSph = {
    // TODO
};

bool dAcObell_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres"));
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);

    nw4r::g3d::ResMdl resMdl = mResFile.GetResMdl("StageF000Bell");
    TRY_CREATE(mMdl.create(resMdl, &mAllocator, 0x120));

    cBgD_t *dzb = (cBgD_t *)CurrentStageArcManager::GetInstance()->getData("dzb/StageF000Bell.dzb");
    PLC *plc = (PLC *)CurrentStageArcManager::GetInstance()->getData("dat/StageF000Bell.plc");
    updateMatrix();

    TRY_CREATE(!(BOOL)mBgW.Set(dzb, plc, 1, &mWorldMtx, &mScale));

    return true;
}

int dAcObell_c::create() {
    CREATE_ALLOCATOR(dAcObell_c);

    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);

    mStts.SetRank(13);
    mSph.Set(sSrcSph);
    mSph.SetStts(mStts);

    mRupeeDir = mVec3_c(0.f, -300.f, 0.f);
    field_0x718 = mVec3_c(0.f, -210.f, 0.f);

    mDowsingTarget.initialize(DowsingTarget::SLOT_RUPEE, 0, nullptr, 0.f);
    mDowsingTarget.doRegister();
    mDowsingTarget.setOffset(mVec3_c::Ey * -150.f);

    mStateMgr.changeState(StateID_Wait);

    mMdl.setPriorityDraw(0x1C, 9);
    mBoundingBox.Set(mVec3_c(-160.f, -320.f, -160.f), mVec3_c(160.f, 10.f, 160.f));
    mCullingDistance = 50000.f;
    return SUCCEEDED;
}

void dAcObell_c::initializeState_Wait() {}
void dAcObell_c::executeState_Wait() {}
void dAcObell_c::finalizeState_Wait() {}
void dAcObell_c::initializeState_WaitRupee() {}
void dAcObell_c::executeState_WaitRupee() {}
void dAcObell_c::finalizeState_WaitRupee() {}
void dAcObell_c::initializeState_After() {}
void dAcObell_c::executeState_After() {}
void dAcObell_c::finalizeState_After() {}
