#include "d/a/obj/d_a_obj_grave.h"

#include "c/c_lib.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w_base.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_vec.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "m/types_m.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_GRAVE, dAcOGrave_c, fProfile::OBJ_GRAVE, 0x255, 0, 2);

STATE_DEFINE(dAcOGrave_c, Wait);
STATE_DEFINE(dAcOGrave_c, Move);

dCcD_SrcUnk dAcOGrave_c::sUnkSrc = {
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x111, {0, 6, 0x407}, 0, 0},
     /* mObjCo */ {0}},
    {-65.0f, 0.0f, -25.0f, 65.0f, 156.0f, 25.0f},
};

const u8 dAcOGrave_c::lbl_580_rodata_0 = 0xA;
const f32 dAcOGrave_c::lbl_580_rodata_30 = 10.f;

void dAcOGrave_c::fn_580_480(dAcObjBase_c *link, dBgW_Base::PushPullLabel label) {
    if (label & dBgW_Base::PPLABEL_PUSH) {
        mAngle.y = link->mRotation.y;
        if (link == dAcPy_c::GetLink()) {
            this->mPush = true;
        }
    } else if (label & dBgW_Base::PPLABEL_PULL) {
        mAngle.y = link->mRotation.y.mVal + 0x8000;
        if (link == dAcPy_c::GetLink()) {
            this->mPull = true;
        }
    }
}

dAcObjBase_c *dAcOGrave_c::pushPullCb(dAcObjBase_c *grave, dAcObjBase_c *link, dBgW_Base::PushPullLabel label) {
    ((dAcOGrave_c *)grave)->fn_580_480(link, label);
}

bool dAcOGrave_c::unkCb(dBgW *bgW, dAcObjBase_c *grave, dAcObjBase_c *link) {
    if (link->isActorPlayer() && dScGame_c::currentSpawnInfo.isNight()) {
        ((dAcOGrave_c *)grave)->field_0x7F7 = true;
    }
    return true;
}

bool dAcOGrave_c::createHeap() {
    void *data = CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres");
    mResFile = nw4r::g3d::ResFile(data);
    dStage_c::bindSkyCmnToResFile(&mResFile);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("StageF000Grave");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x128));
    nw4r::g3d::ResAnmClr anmClr = mResFile.GetResAnmClr("StageF000Grave");
    TRY_CREATE(mAnmMatClr.create(mdl, anmClr, &mAllocator, nullptr, 1));
    mMdl.setAnm(mAnmMatClr);
    void *dzb = CurrentStageArcManager::GetInstance()->getData("dzb/StageF000Grave.dzb");
    void *plc = CurrentStageArcManager::GetInstance()->getData("dat/StageF000Grave.plc");
    updateMatrix();
    return !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
}

int dAcOGrave_c::create() {
    mShouldGlow = getFromParams(0, 3) == 0;
    mSceneflag = getFromParams(2, 0xFF);
    CREATE_ALLOCATOR(dAcOGrave_c);
    mStts.SetRank(0xD);
    mCollider.Set(sUnkSrc);
    mCollider.SetStts(mStts);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    mBgW.SetPushPullCallback(pushPullCb);
    mBgW.SetUnkCallback(unkCb);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mAcceleration = 0.f;
    mMaxSpeed = -40.f;
    mOrigPos1 = mPosition;
    field_0x7F6 = true;
    if (mSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
        if (dScGame_c::currentSpawnInfo.isNight()) {
            mTimer = 1;
        }
        field_0x7F6 = false;
        mOrigPos2 = mPosition;
        getXZCirclePoint(mOrigPos2, mRotation.y.mVal + 0x8000, dAcPy_c::sPushRelatedConstant);
        mPosition.set(mOrigPos2.x, mOrigPos2.y, mOrigPos2.z);
        mOldPosition.set(mOrigPos2.x, mOrigPos2.y, mOrigPos2.z);
    }
    mStateMgr.changeState(StateID_Wait);
    mBoundingBox.Set(mVec3_c(-100.f, -0.f, -100.f), mVec3_c(100.f, 200.f, 100.f));
    return SUCCEEDED;
}

int dAcOGrave_c::doDelete() {
    return SUCCEEDED;
}

int dAcOGrave_c::actorExecute() {
    if (mTimer != 0 && mShouldGlow) {
        if (mAnmMatClr.getFrame(0) == 0.f || mAnmMatClr.getFrame(0) == 75.f) {
            startSound(SE_Grave_AKU_MARK);
        }
        if (mSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
            sLib::calcTimer(&mTimer);
        }
        mAnmMatClr.play();
    }
    if ((mObjectActorFlags & 1) == 0) {
        mStateMgr.executeState();
        calcVelocity();
        mPosition += mVelocity;
        mPosition += mStts.GetCcMove();
        updateMatrix();
        mMdl.setLocalMtx(mWorldMtx);
        if (mShouldGlow && dScGame_c::currentSpawnInfo.isNight()) {
            mCollider.Set(mPosition, mRotation.y);
            dCcS::GetInstance()->Set(&mCollider);
        }
        mBgW.Move();
        mPush = false;
        mPull = false;
        field_0x7F7 = false;
    }
    return SUCCEEDED;
}

int dAcOGrave_c::draw() {
    drawModelType1(&mMdl);
    static mQuat_c shadowRot(0.f, 50.f, 0.f, 50.f);
    drawShadow(mShadow, nullptr, mWorldMtx, &shadowRot, -1, -1, -1, -1, -1, 0.f);
    return SUCCEEDED;
}

void dAcOGrave_c::initializeState_Wait() {}

void dAcOGrave_c::executeState_Wait() {
    if (mShouldGlow && mTimer == 0) {
        if ((mCollider.ChkTgHit() &&
                 (mCollider.ChkTgAtHitType(AT_TYPE_SWORD) || mCollider.ChkTgAtHitType(AT_TYPE_BOMB)) ||
             field_0x7F7)) {
            mTimer = 300;
            mAnmMatClr.setFrame(0.f, 0);
            mAnmMatClr.setRate(1.f, 0);
        }
    }
    s32 diff = getXZAngleToPlayer().absDiff(mRotation.y);
    if (field_0x7F6) {
        mOrigPos2 = mPosition;
        getXZCirclePoint(mOrigPos2, mAngle.y, dAcPy_c::sPushRelatedConstant);
        if ((mPush && (s16)diff < 0x2000) || (mPull && (s16)diff > 0x6000)) {
            field_0x7F6 = false;
            mStateMgr.changeState(StateID_Move);
        }
    } else {
        mOrigPos2 = mOrigPos1;
        if ((mPush && (s16)diff > 0x6000) || (mPull && (s16)diff < 0x2000)) {
            field_0x7F6 = true;
            mStateMgr.changeState(StateID_Move);
        }
    }
}

void dAcOGrave_c::finalizeState_Wait() {}

void dAcOGrave_c::initializeState_Move() {
    dAcPy_c::LINK->onFlags_0x360(0x10000);
    startSound(SE_Grave_MOVE);
}

void dAcOGrave_c::executeState_Move() {
    if (cLib::chasePosXZ(mPosition, mOrigPos2, lbl_580_rodata_30)) {
        mStateMgr.changeState(StateID_Wait);
    }
}

void dAcOGrave_c::finalizeState_Move() {
    dAcPy_c::LINK->offFlags_0x360(0x10000);
    if (mShouldGlow && mTimer != 0) {
        if (mSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
            SceneflagManager::sInstance->setFlag(mRoomID, mSceneflag);
        }
    }
    itemDroppingAndGivingRelated(&mOrigPos1, 0);
    fillUpperParams2Byte();
}
