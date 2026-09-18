#include "d/a/obj/d_a_obj_sword_candle.h"

#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_stage_kraken.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/d_light_env.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"

f32 dAcOSwordCandle_c::lbl_530_data_0 = -0.f;

SPECIAL_ACTOR_PROFILE(OBJ_SWORD_CANDLE, dAcOSwordCandle_c, fProfile::OBJ_SWORD_CANDLE, 0x24C, 0, 7);

u16 dAcOSwordCandle_c::lbl_530_bss_8[6] = {
    PARTICLE_RESOURCE_ID_MAPPING_333_, PARTICLE_RESOURCE_ID_MAPPING_332_, PARTICLE_RESOURCE_ID_MAPPING_497_,
    PARTICLE_RESOURCE_ID_MAPPING_496_, PARTICLE_RESOURCE_ID_MAPPING_308_, PARTICLE_RESOURCE_ID_MAPPING_307_,
};

STATE_DEFINE(dAcOSwordCandle_c, Wait);
STATE_DEFINE(dAcOSwordCandle_c, WaitClear);

dCcD_SrcCyl dAcOSwordCandle_c::sCylSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {0, 0x111, {0, 0x0, 0x407}, 0x0, 0x0},
     /* mObjCo */ {0xE9}},
    /* mCylInf */
    {100.f, 50.f}
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

int dAcOSwordCandle_c::actorCreate() {
    mActivatedSceneflag = getFromParams(0, 0xFF);
    mRiseFromGroundSceneflag = getFromParams(0xC, 0xFF);
    mTransferToCrest = true;
    mSendToSwSB = lbl_530_data_0;
    CREATE_ALLOCATOR(dAcOSwordCandle_c);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mStts.SetRank(0xD);
    mCyl.Set(sCylSrc);
    mCyl.SetStts(mStts);
    mAcceleration = 0.f;
    mMaxSpeed = -40.f;
    field_0x814 = getFromParams(0x8, 0xF);
    if (field_0x814 >= 3) {
        field_0x814 = 0;
    }
    mEmitter.init(this);
    field_0x810 = lbl_530_bss_8[field_0x814 * 2 + 1];
    if (mRiseFromGroundSceneflag < 0xFF &&
        !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRiseFromGroundSceneflag)) {
        mStateMgr.changeState(StateID_WaitClear);
    } else {
        mStateMgr.changeState(StateID_Wait);
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
    if (mActivatedSceneflag < 0xff && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mActivatedSceneflag)) {
        unsetActorProperty(1);
        field_0x816 = 1;
    }
    mMdl.setPriorityDraw(0x1C, 9);
    mBoundingBox.Set(mVec3_c(-150.f, -20.f, -150.f), mVec3_c(150.f, 500.f, 150.f));
    field_0x7DC.mPos = field_0x7F0;
    field_0x7DC.mRadius = 0.f;
    dLightEnv_c::GetPInstance()->shadow_set(&field_0x7DC);
    return SUCCEEDED;
}

int dAcOSwordCandle_c::actorPostCreate() {
    if (field_0x814 == 1) {
        mMtx_c mtx;
        mtx.ZXYrotS(mRotation);
        mVec3_c out;
        mtx.multVecSR(mVec3_c::Ey, out);
        mVec3_c a = mPosition + out * 100.f;
        if (dBgS_ObjGndChk::CheckPos(a)) {
            mPosition.y = dBgS_ObjGndChk::GetGroundHeight();
            if (dBgS::GetInstance()->ChkMoveBG(dBgS_ObjGndChk::GetInstance(), false)) {
                field_0x7B0.SetPolyInfo(dBgS_ObjGndChk::GetInstance());
            }
        }
    }
    return SUCCEEDED;
}

int dAcOSwordCandle_c::doDelete() {
    dLightEnv_c::GetPInstance()->shadow_cut(&field_0x7DC);
    return SUCCEEDED;
}

int dAcOSwordCandle_c::actorExecute() {
    if (field_0x814 == 1 && dBgS::GetInstance()->ChkMoveBG(field_0x7B0, true)) {
        dBgS::GetInstance()->MoveBgTransPos(field_0x7B0, true, &mPosition, nullptr, nullptr);
        if (field_0x818) {
            setCrestPosRot(&mMdl);
        }
    }
    mStateMgr.executeState();
    if (field_0x814 == 1) {
        updateMatrix();
        mMdl.setLocalMtx(mWorldMtx);
        mMdl.calc(false);
        nw4r::g3d::ResMdl mdl = mMdl.getResMdl();
        nw4r::g3d::ResNode resNode = mdl.GetResNode("loc_Effect");
        mMdl.getNodeWorldMtxMultVecZero(resNode.GetID(), field_0x7F0);
    }
    mBgW.Move();
    return SUCCEEDED;
}

int dAcOSwordCandle_c::actorExecuteInEvent() {
    if (field_0x814 == 1 && dBgS::GetInstance()->ChkMoveBG(field_0x7B0, true)) {
        dBgS::GetInstance()->MoveBgTransPos(field_0x7B0, true, &mPosition, nullptr, nullptr);
        if (field_0x818) {
            setCrestPosRot(&mMdl);
        }
    }
    if (mEvent.isThisActorInEvent()) {
        switch (mEvent.getCurrentEventCommand()) {
            case 'fire': {
                field_0x810 = lbl_530_bss_8[field_0x814 * 2 + 1];
                mEvent.advanceNext();
                break;
            }
            case 'wait': {
                mEvent.advanceNext();
                break;
            }
            case 'demo': {
                fn_530_1180();
                break;
            }
        }
    }
    if (mActivatedSceneflag < 0xff && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mActivatedSceneflag) &&
        field_0x817) {
        mEmitter.holdEffect(field_0x810, field_0x7F0, &mRotation, &mScale, nullptr, nullptr);
        holdSound(SE_SdCdl_FIRE_LV);
        field_0x7DC.mPos = field_0x7F0;
        field_0x7DC.mRadius = 1000.f;
    }
    return SUCCEEDED;
}

void dAcOSwordCandle_c::registerInEvent() {
    if (EventManager::sInstance->isCurrentEvent("SwordCandle")) {
        dAcPy_c::LINK->mObjectActorFlags |= 0x200;
        return;
    }
    if (EventManager::sInstance->isCurrentEvent("GodsMark") || EventManager::sInstance->isCurrentEvent("SceneChange") ||
        EventManager::sInstance->isCurrentEvent("SwSwordBeam") || EventManager::sInstance->isCurrentEvent("STB")) {
        mEmitter.remove(true);
    } else {
        field_0x817 = 1;
    }
}

void dAcOSwordCandle_c::unkVirtFunc_0x6C() {
    if (dAcPy_c::GetLink()->mObjectActorFlags & 0x200) {
        dAcPy_c::LINK->mObjectActorFlags &= ~0x200;
    }
    field_0x817 = 0;
}

int dAcOSwordCandle_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOSwordCandle_c::initializeState_Wait() {
    dBgS::GetInstance()->Regist(&mBgW, this);
}

void dAcOSwordCandle_c::executeState_Wait() {
    if (mActivatedSceneflag < 0xff && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mActivatedSceneflag)) {
        mEmitter.holdEffect(field_0x810, field_0x7F0, &mRotation, &mScale, nullptr, nullptr);
        holdSound(SE_SdCdl_FIRE_LV);
        field_0x7DC.mPos = field_0x7F0;
        field_0x7DC.mRadius = 1000.f;
    }
}

void dAcOSwordCandle_c::finalizeState_Wait() {}

void dAcOSwordCandle_c::initializeState_WaitClear() {
    dBgS::GetInstance()->Release(&mBgW);
    mActorProperties &= ~0x1;
    mObjectActorFlags |= 0x200;
}

volatile u32 FLAGS_1 = 0x00000001;
u32 FLAGS_2 = 0x00100001;

void dAcOSwordCandle_c::executeState_WaitClear() {
    if (mRiseFromGroundSceneflag < 0xFF &&
        SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRiseFromGroundSceneflag)) {
        u32 f1 = ~FLAGS_1;
        u32 f2 = FLAGS_2;
        Event event("D301SwordCandle", getOarcZev("SwordCandle"), 500, f2 & f1, nullptr, nullptr);
        mEvent.scheduleEvent(event, 0);
    }
}

void dAcOSwordCandle_c::finalizeState_WaitClear() {
    mObjectActorFlags &= ~0x200;
    nw4r::g3d::ResMdl mdl = mMdl.getResMdl();
    nw4r::g3d::ResNode resNode = mdl.GetResNode("loc_Effect");
    mMdl.getNodeWorldMtxMultVecZero(resNode.GetID(), field_0x7F0);
    field_0x818 = 1;
}

f32 dAcOSwordCandle_c::lbl_530_data_F0 = 255.f;

void dAcOSwordCandle_c::fn_530_1180() {
    dAcOstageKraken_c *stageKraken = dAcOstageKraken_c::getStageKrakenPtr();
    mMtx_c mtx;
    mtx.ZXYrotS(mRotation);
    mVec3_c vec;
    mtx.multVecSR(mVec3_c::Ey, vec);
    int out;
    mEvent.getSingleIntData(&out, 'cut ', 0);
    switch (out) {
        case 0: {
            stageKraken->field_0x1A30 = 1;
            stageKraken->mActorProperties |= 4;
            mEvent.advanceNext();
            break;
        }
        case 1: {
            if (stageKraken->mMarkLeftRight != 0 && mTimer == 0) {
                dJEffManager_c::spawnEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_941_, mPosition, nullptr, nullptr, nullptr, nullptr, 0, 0
                );
                mTimer = 0x50;
                field_0x7DC.mPos = mPosition;
                field_0x7DC.mRadius = 1000.f;
            }
            if (stageKraken->mMarkLeftRight != 0 && sLib::calcTimer(&mTimer) == 0) {
                mMdl.setTevColorAll(GX_TEVREG2, mColor(0, 0, 0, 0xFF), false);
                field_0x7FC = mPosition;
                mPosition = mPosition - vec * 200.f;
                mOldPosition = mPosition;
                mObjectActorFlags &= ~0x200;
                field_0x818 = 1;
                field_0x808 = 0.f;
                updateMatrix();
                mMdl.setLocalMtx(mWorldMtx);
                mMdl.calc(false);
                setCrestAtBone("SetGS", &mMdl, &mActivatedSceneflag);
                vt_0x88(lbl_530_data_F0);
                dAcOSwSwordBeam_c *swSwordBeam = mSwSwordBeamRef.get();
                swSwordBeam->mActorProperties |= 4;
                mEvent.advanceNext();
            }
            break;
        }
        case 2: {
            if (field_0x808 == 120.f) {
                stageKraken->field_0x1A32 = 1;
                field_0x80C = 255.f;
            } else if (field_0x808 > 120.f) {
                sLib::chase(&field_0x80C, 0.f, 12.75);
                mMdl.setTevColorAll(GX_TEVREG2, mColor(0, 0, 0, field_0x80C), false);
                vt_0x88(field_0x80C);
            }
            if (sLib::chase(&field_0x808, 200.f, 2.f)) {
                mStateMgr.changeState(StateID_Wait);
                dAcOSwSwordBeam_c *swSwordBeam = mSwSwordBeamRef.get();
                swSwordBeam->mActorProperties &= ~0x4;
                stageKraken->mActorProperties &= ~0x4;
                mEvent.advanceNext();
            }
            mPosition = field_0x7FC - vec * (200.f - field_0x808);
            mOldPosition = mPosition;
            updateMatrix();
            mMdl.setLocalMtx(mWorldMtx);
            mMdl.calc(false);
        }
    }
}

void dAcOSwordCandle_c::vt_0x88(f32 &param) {
    dAcOSwSwordBeam_c *swSwordBeam = mSwSwordBeamRef.get();
    swSwordBeam->setField_0xC90(param);
    swSwordBeam->setField_0xCAA(true);
}
