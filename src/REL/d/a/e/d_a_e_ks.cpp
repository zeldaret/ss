#include "d/a/e/d_a_e_ks.h"

#include "c/c_math.h"
#include "common.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/t/d_t_ks.h"
#include "m/m_vec.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(E_KS, dAcEKs_c, fProfile::E_KS, 0xFE, 0, 4099);

STATE_DEFINE(dAcEKs_c, Wait);
STATE_DEFINE(dAcEKs_c, WakeUp);
STATE_DEFINE(dAcEKs_c, ReturnToWait);
STATE_DEFINE(dAcEKs_c, WaitReady);
STATE_DEFINE(dAcEKs_c, Move);
STATE_DEFINE(dAcEKs_c, Chase);
STATE_DEFINE(dAcEKs_c, ChaseAttack);
STATE_DEFINE(dAcEKs_c, Fighting);
STATE_DEFINE(dAcEKs_c, AttackReady);
STATE_DEFINE(dAcEKs_c, Attack);
STATE_DEFINE(dAcEKs_c, Damage);
STATE_DEFINE(dAcEKs_c, Stun);
STATE_DEFINE(dAcEKs_c, WindBlow);
STATE_DEFINE(dAcEKs_c, PathMove);

bool dAcEKs_c::restorePosRotFromCopy() {
    if (field_0xDCF == 0) {
        if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(mRoomID, mPosition) != 0) {
            field_0xAA8 = 0.f;
            mSph.ClrCoSet();
            mSph.ClrTgSet();
            mSph.ClrAtSet();
            fn_800306d0();
            if (mType == 1 || mType == 2) {
                field_0xD20 = 0.f;
                field_0xDE4 = 0.f;
                if (mEmitter1.hasEmitters()) {
                    mEmitter1.remove(true);
                }
            }
        } else {
            field_0xAA8 = 1.f;
            fn_80030700();
            mSph.OnCoSet();
            mSph.OnTgSet();
        }
        mMdl.getModel().setScale(mScale * field_0xAA8);
    }
    return true;
}

void dAcEKs_c::linkKiesuTag(dTgKiesuTag_c *pTgKs) {
    mTgRef.link(pTgKs);
}

void dAcEKs_c::setStartingState() {
    mTimer = cM::rndInt(1000);
    switch (mStartingState) {
        case 0: {
            mStateMgr.changeState(StateID_Move);
            mCurrentState = mStartingState;
            mAcch.ClrRoofNone();
            mAcch.SetField_0xD4(55.f);
            mAcch.SetGroundUpY(30.f);
            mAcchCir.SetWall(20.f, 60.f);
        } break;
        case 1: {
            mStateMgr.changeState(StateID_Wait);
            mCurrentState = mStartingState;
            if ((s32)getFromParams(30, 0x3) == 1) {
                field_0xDB1 = 1;
            }
            mAcch.ClrRoofHit();
            mAcch.SetGroundUpY(0.f);
            mAcch.SetField_0xD4(0.f);
            mAcchCir.SetWall(0.f, 5.f);

        } break;
        case 2: {
            mCurrentState = mStartingState;
            mAcch.ClrRoofNone();
            mAcch.SetField_0xD4(55.f);
            mAcch.SetGroundUpY(30.f);
            mAcchCir.SetWall(20.f, 60.f);
            mStateMgr.changeState(StateID_PathMove);
        } break;
        default: {
            mStateMgr.changeState(StateID_Move);
            mCurrentState = mStartingState;
        } break;
    }

    mCurrentState = mNextState;
}

void dAcEKs_c::setIdleState() {
    switch (mStartingState) {
        case 0: {
            mStateMgr.changeState(StateID_Move);
            mCurrentState = 0;
        } break;
        case 1: {
            mStateMgr.changeState(StateID_ReturnToWait);
            mCurrentState = 5;
        } break;
        case 2: {
            mStateMgr.changeState(StateID_PathMove);
            mCurrentState = 2;
        } break;
        default: {
            mStateMgr.changeState(StateID_Move);
            mCurrentState = 0;
        } break;
    }
}

f32 dAcEKs_c::getLineCrossYRange(const mVec3_c &pos, f32 range) {
    dBgS_ObjLinChk linChk;

    mVec3_c start, end;
    start.set(pos);
    start.y -= range;

    end.set(pos);
    end.y += range;

    linChk.Set(&start, &end, nullptr);
    if (dBgS::GetInstance()->LineCross(&linChk)) {
        return linChk.GetLinEnd().y;
    } else {
        return mStartingPos.y;
    }
}

// idk

void dAcEKs_c::initializeState_Wait() {}
void dAcEKs_c::executeState_Wait() {}
void dAcEKs_c::finalizeState_Wait() {}

void dAcEKs_c::initializeState_WakeUp() {}
void dAcEKs_c::executeState_WakeUp() {}
void dAcEKs_c::finalizeState_WakeUp() {}

void dAcEKs_c::initializeState_ReturnToWait() {}
void dAcEKs_c::executeState_ReturnToWait() {}
void dAcEKs_c::finalizeState_ReturnToWait() {}

void dAcEKs_c::initializeState_WaitReady() {}
void dAcEKs_c::executeState_WaitReady() {}
void dAcEKs_c::finalizeState_WaitReady() {}

void dAcEKs_c::initializeState_Move() {}
void dAcEKs_c::executeState_Move() {}
void dAcEKs_c::finalizeState_Move() {}

void dAcEKs_c::initializeState_Chase() {}
void dAcEKs_c::executeState_Chase() {}
void dAcEKs_c::finalizeState_Chase() {}

void dAcEKs_c::initializeState_ChaseAttack() {}
void dAcEKs_c::executeState_ChaseAttack() {}
void dAcEKs_c::finalizeState_ChaseAttack() {}

void dAcEKs_c::initializeState_Fighting() {}
void dAcEKs_c::executeState_Fighting() {}
void dAcEKs_c::finalizeState_Fighting() {}

void dAcEKs_c::initializeState_AttackReady() {}
void dAcEKs_c::executeState_AttackReady() {}
void dAcEKs_c::finalizeState_AttackReady() {}

void dAcEKs_c::initializeState_Attack() {}
void dAcEKs_c::executeState_Attack() {}
void dAcEKs_c::finalizeState_Attack() {}

void dAcEKs_c::initializeState_Damage() {}
void dAcEKs_c::executeState_Damage() {}
void dAcEKs_c::finalizeState_Damage() {}

void dAcEKs_c::initializeState_Stun() {}
void dAcEKs_c::executeState_Stun() {}
void dAcEKs_c::finalizeState_Stun() {}

void dAcEKs_c::initializeState_WindBlow() {}
void dAcEKs_c::executeState_WindBlow() {}
void dAcEKs_c::finalizeState_WindBlow() {}

void dAcEKs_c::initializeState_PathMove() {}
void dAcEKs_c::executeState_PathMove() {}
void dAcEKs_c::finalizeState_PathMove() {}
