#include "d/a/obj/d_a_obj_move_elec.h"

#include "d/col/bg/d_bg_s.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/sceneflag_manager.h"
#include "m/m_vec.h"
#include "s/s_Math.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(OBJ_MOVE_ELEC, dAcOMoveElec_c, fProfile::OBJ_MOVE_ELEC, 0x1A8, 0, 6);

static dCcD_SrcUnk sSrcUnk = {
    {
     /* mObjAt */ {AT_TYPE_DAMAGE, 0x24020D, {0, 0, 0}, 4, 0, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */ {0, 0x310, {0, 6, 0x407}, 0, CUT_DIR_NONE},
     /* mObjCo */ {0xE8},
     },
    {-100.0f, -0.0f, -20.0f, 100.0f, 180.0f, 20.0f},
};

// Emitted but never read: the code below repeats these values as literals
struct dAcOMoveElec_HIO_c {
    /* 0x00 */ f32 field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ u8 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;

    static const dAcOMoveElec_HIO_c sHio;
};

const dAcOMoveElec_HIO_c dAcOMoveElec_HIO_c::sHio = {20.0f, 1000.0f, 1000.0f, 10.0f, 20, 800.0f, 800.0f};

STATE_DEFINE(dAcOMoveElec_c, WaitOff);
STATE_DEFINE(dAcOMoveElec_c, WaitOn);
STATE_DEFINE(dAcOMoveElec_c, Move);

bool dAcOMoveElec_c::createHeap() {
    mResFile = getOarcFile("D301DamageWall", "g3d/model.brres");
    TRY_CREATE(mMdl.create(mResFile, "D301DamageWall", "D301DamageWall", &mAllocator, 0x120));
    void *dzb = getOarcDZB("D301DamageWall", "D301DamageWall");
    void *plc = getOarcPLC("D301DamageWall", "D301DamageWall");
    updateMatrix();
    bool ok = !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
    if (!ok) {
        return true;
    }
    dzb = getOarcDZB("D301DamageWall", "D301DWGround");
    plc = getOarcPLC("D301DamageWall", "D301DWGround");
    mBgWGround.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
    return true;
}

int dAcOMoveElec_c::create() {
    mEventSceneflag = getFromParams(16, 0xFF);
    mSceneflag = getFromParams(8, 0xFF);
    mIsShort = getFromParams(24, 0x3) == 0;
    mEventId = mParams;
    CREATE_ALLOCATOR(dAcOMoveElec_c);
    mAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(20.0f, 50.0f);
    mAcch.SetWaterNone();
    mAcch.SetGndThinCellingOff();
    mAcch.SetWallNone();
    initPhase();
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mStts.SetRank(0xD);
    mCc.Set(sSrcUnk);
    mCc.SetStts(mStts);
    mAcceleration = 0.0f;
    mMaxSpeed = -40.0f;
    mCc.field_0xB4 = 1;
    mEffect.init(this);
    mMdl.setAnm("D301DamageWall", m3d::PLAY_MODE_4);
    mMdl.setRate(0.0f);
    if (mPhase == 3 || mPhase == 4) {
        mStateMgr.changeState(StateID_WaitOff);
    } else {
        mMdl.setFrame(mMdl.getAnm().getEndFrame() - 1.0f);
        mStateMgr.changeState(StateID_WaitOn);
    }
    mBoundingBox.Set(mVec3_c(-110.0f, -165.0f, -20.0f), mVec3_c(110.0f, 165.0f, 20.0f));
    return SUCCEEDED;
}

int dAcOMoveElec_c::doDelete() {
    return SUCCEEDED;
}

int dAcOMoveElec_c::actorExecute() {
    executeState();
    if (isState(StateID_Move)) {
        calcVelocity();
        mPosition += mVelocity;
        mPosition += mStts.GetCcMove();
        mAcch.CrrPos(*dBgS::GetInstance());
    }
    if (!isState(StateID_WaitOff)) {
        if (sLib::calcTimer(&mCcTimer) == 0) {
            mVec3_c dir = mVec3_c::Ez;
            dir.rotY(mRotation.y);
            mCc.SetAtVec(dir);
            mCc.cM3dGUnk::Set(mPosition, mRotation.y);
            mCcTimer = 0xF;
        }
        dCcS::GetInstance()->Set(&mCc);
    }
    updateMatrix();
    mMdl.getModel().setLocalMtx(mWorldMtx);
    mMdl.play();
    mMdl.getModel().calc(false);
    if (mEffect.hasEmitters()) {
        mEffect.setTransform(mWorldMtx);
    }
    if (!isState(StateID_WaitOff)) {
        mBgW.Move();
    } else {
        mBgWGround.Move();
    }
    return SUCCEEDED;
}

int dAcOMoveElec_c::draw() {
    drawModelType1(&mMdl.getModel());
    return SUCCEEDED;
}

void dAcOMoveElec_c::initializeState_WaitOff() {
    dBgS::GetInstance()->Release(&mBgW);
    dBgS::GetInstance()->Regist(&mBgWGround, this);
}
void dAcOMoveElec_c::executeState_WaitOff() {
    if (mPhase == 3 && mSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
        mPhase = 0;
        mStateMgr.changeState(StateID_WaitOn);
        mMdl.setRate(10.0f);
    }
}
void dAcOMoveElec_c::finalizeState_WaitOff() {}

void dAcOMoveElec_c::initializeState_WaitOn() {
    dBgS::GetInstance()->Regist(&mBgW, this);
    dBgS::GetInstance()->Release(&mBgWGround);
    mSpeed = 0.0f;
    mVelocity.y = 0.0f;
}
void dAcOMoveElec_c::executeState_WaitOn() {
    switch (mPhase) {
        case 0:
            if ((u16)(mSceneflag + 1) < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag + 1)) {
                mPhase = 1;
                mMdl.setAnm("D301DamageWall_Move", m3d::PLAY_MODE_4);
                mMdl.setRate(1.0f);
                mMoveDist = mIsShort ? 800.0f : 1000.0f;
                mStateMgr.changeState(StateID_Move);
            }
            break;
        case 1:
            if ((u16)(mSceneflag + 2) < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag + 2)) {
                mPhase = 2;
                mMdl.setAnm("D301DamageWall_Move", m3d::PLAY_MODE_4);
                mMdl.setRate(1.0f);
                mMoveDist = mIsShort ? 800.0f : 1000.0f;
                mEventTimer = 20;
                mStateMgr.changeState(StateID_Move);
            }
            break;
        case 2:
            if (mEventSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mEventSceneflag)) {
                if (mEventId != 0xFF) {
                    Event event(mEventId, mRoomID, 0x100001, nullptr, nullptr);
                    mEvent.scheduleEvent(event, 0);
                }
                if (mEvent.isThisActorInEvent() && sLib::calcTimer(&mEventTimer) == 0) {
                    mPhase = 4;
                    mMdl.setAnm("D301DamageWall", m3d::PLAY_MODE_4);
                    mMdl.setRate(-10.0f);
                    mMdl.setFrame(59.0f);
                    mStateMgr.changeState(StateID_WaitOff);
                }
            }
            break;
    }
}
void dAcOMoveElec_c::finalizeState_WaitOn() {}

void dAcOMoveElec_c::initializeState_Move() {
    mSpeed = 20.0f;
    mMoveTimer = mMoveDist / 20.0f;
}
void dAcOMoveElec_c::executeState_Move() {
    mEffect.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_678_, mPosition, nullptr, nullptr, nullptr, nullptr);
    if (sLib::calcTimer(&mMoveTimer) == 0) {
        mStateMgr.changeState(StateID_WaitOn);
    }
}
void dAcOMoveElec_c::finalizeState_Move() {}

void dAcOMoveElec_c::initPhase() {
    if (mEventSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mEventSceneflag)) {
        mPhase = 4;
    } else if ((u16)(mSceneflag + 2) < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag + 2)) {
        mPhase = 2;
    } else if ((u16)(mSceneflag + 1) < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag + 1)) {
        mPhase = 1;
    } else if (mSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
        mPhase = 0;
    } else {
        mPhase = 3;
    }
}
