#include "d/a/obj/d_a_obj_sword_candle.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_light_env.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_SWORD_CANDLE, dAcOSwordCandle_c, fProfile::OBJ_SWORD_CANDLE, 0x24C, 0, 7);

STATE_DEFINE(dAcOSwordCandle_c, Wait);
STATE_DEFINE(dAcOSwordCandle_c, WaitClear);

dCcD_SrcCyl dAcOSwordCandle_c::sCylSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {AT_TYPE_SWORD | AT_TYPE_BOMB | AT_TYPE_0x40 | AT_TYPE_SLINGSHOT | AT_TYPE_WHIP | AT_TYPE_ARROW |
          AT_TYPE_CLAWSHOT | AT_TYPE_0x800000,
      0x200111,
      {0, 0xb, 0x407},
      0x0,
      0x0},
     /* mObjCo */ {0x29}},
    /* mCylInf */
    {50.f, 130.f}
};

bool dAcOSwordCandle_c::createHeap() {
    void *data = getOarcResFile("SwordCandle");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("SwordCandle");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr));
    void *dzb = getOarcFile("SwordCandle", "dzb/SwordCandle.dzb");
    void *plc = getOarcFile("SwordCandle", "dat/SwordCandle.plc");
    updateMatrix();
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    return SUCCEEDED;
}

int dAcOSwordCandle_c::create() {
    mActivatedSceneflag = getFromParams(0, 0xFF);
    mRiseFromGroundSceneflag = getFromParams(0xC, 0xFF);
    mTransferToCrest = true;
    mSendToSwSB = 0.f;
    CREATE_ALLOCATOR(dAcOSwordCandle_c);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mStts.SetRank(0xD);
    mCyl.Set(sCylSrc);
    mCyl.SetStts(mStts);
    mAcceleration = -40.f;
    mMaxSpeed = 0.f;
    field_0x814 = getFromParams(0x8, 0xF);
    if (field_0x814 > 2) {
        field_0x814 = 0;
    }
    mEmitter.init(this);
    field_0x810 = lbl_530_bss_8[field_0x814];
    if (mRiseFromGroundSceneflag >= 0xFF ||
        !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRiseFromGroundSceneflag)) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        mStateMgr.changeState(StateID_WaitClear);
    }
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(true);
    nw4r::g3d::ResMdl mdl = mMdl.getResMdl();
    u32 id = mdl.GetResNode("loc_Effect").GetID();
    mMdl.getNodeWorldMtxMultVecZero(id, field_0x7F0);
    if (mRiseFromGroundSceneflag >= 0xff ||
        SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRiseFromGroundSceneflag)) {
        setCrestAtBone("SetGS", &mMdl, &mActivatedSceneflag);
        field_0x818 = 1;
    }
    if (mActivatedSceneflag < 0xff && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRiseFromGroundSceneflag)) {
        field_0x816 = 1;
        unsetActorProperty(1);
    }
    mMdl.setPriorityDraw(0x1C, 9);
    mBoundingBox.Set(mVec3_c(-150.f, -20.f, -150.f), mVec3_c(150.f, 500.f, 150.f));
    field_0x7DC.mPos = field_0x7F0;
    field_0x7DC.mRadius = 0.f;
    dLightEnv_c::GetPInstance()->shadow_set(&field_0x7DC);
    return SUCCEEDED;
}

void dAcOSwordCandle_c::initializeState_Wait() {}
void dAcOSwordCandle_c::executeState_Wait() {}
void dAcOSwordCandle_c::finalizeState_Wait() {}
void dAcOSwordCandle_c::initializeState_WaitClear() {}
void dAcOSwordCandle_c::executeState_WaitClear() {}
void dAcOSwordCandle_c::finalizeState_WaitClear() {}
