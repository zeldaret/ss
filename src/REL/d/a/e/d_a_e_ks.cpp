#include "d/a/e/d_a_e_ks.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_s_roof_chk.h"
#include "d/t/d_t_ks.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "toBeSorted/attention.h"
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

// idk.

bool dAcEKs_c::fn_155_2D60(bool b, s32 p1) {
    f32 f0 = field_0xC40 + 1500.f;
    f32 f1 = field_0xC3C + 700.f;
    dAcObjBase_c *pObj = attackPlayerOrScrapper(0.f);
    field_0xBF8.set(pObj->mPosition);
    if (field_0xBF8.squareDistanceToXZ(mPosition) < f1 * f1) {
        f32 f2 = field_0xBF8.y - mPosition.y;
        if (f2 <= 0.f && f2 > -f0) {
            if (b) {
                if (!fn_155_29D0(p1)) {
                    return true;
                }
            } else {
                return true;
            }
        }
    }
    return false;
}

bool dAcEKs_c::fn_155_2EA0() {
    f32 f0 = field_0xC40 + 1500.f;
    f32 f1 = field_0xC3C + 700.f;
    dAcObjBase_c *pObj = dAcPy_c::GetLink()->getBeetleInFlight();
    if (pObj) {
        field_0xC28.set(pObj->mPosition);
        field_0xD1C = field_0xC28.squareDistanceToXZ(mPosition);
        if (field_0xD1C < f1 * f1) {
            f32 f2 = field_0xC28.y - mPosition.y;
            if (f2 <= 0.f && f2 > -f0) {
                return true;
            }
        }
    }
    return false;
}

bool dAcEKs_c::isOutsideRange() {
    return attackPlayerOrScrapper(0.f)->mPosition.squareDistanceToXZ(mPosition) > 360000.f;
}

bool dAcEKs_c::isNotWaitingNorDamage() {
    if (!(mStateMgr.isState(StateID_Wait) || mStateMgr.isState(StateID_Damage))) {
        return true;
    }
    return false;
}

bool dAcEKs_c::isTargeted() {
    dAcObjBase_c *pTarget = AttentionManager::GetInstance()->getTargetedActor();
    if (!pTarget) {
        return false;
    }

    if (pTarget->mProfileName == fProfile::E_KS && pTarget == this) {
        return true;
    }
    return false;
}

bool dAcEKs_c::ChkWall() {
    field_0xDBB++;
    if (field_0xDBB > 5) {
        field_0xDBB = 0;
        mVec3_c offset(0, 0, 200.f);
        mVec3_c pos;
        mWorldMtx.multVec(offset, pos);
        if (dBgS_ObjLinChk::LineCross(&mPosition, &pos, nullptr) && dBgS_ObjLinChk::ChkWall()) {
            return true;
        }
    }
    return false;
}

void dAcEKs_c::ChkGnd() {
    dBgS_ObjGndChk gndChk;
    mVec3_c pos = field_0xBB0;
    pos.y += 150.f;
    gndChk.SetPos(&pos);
    f32 chk = dBgS::GetInstance()->GroundCross(&gndChk);
    if (field_0xBB0.y < chk + 120.f) {
        field_0xBB0.y = chk + 120.f;
    }
}

void dAcEKs_c::ChkRoof() {
    dBgS_ObjRoofChk roofChk;
    mVec3_c pos = field_0xBB0;
    pos.y -= 150.f;
    roofChk.SetPos(&pos);
    f32 chk = dBgS::GetInstance()->RoofChk(&roofChk);
    if (field_0xBB0.y > chk - 120.f) {
        field_0xBB0.y = chk - 120.f;
    }
}

bool dAcEKs_c::ChkHit() {
    if (mAcch.ChkWallHit(nullptr) || mAcch.ChkGndHit() || mAcch.ChkRoofHit()) {
        return true;
    } else {
        return false;
    }
}

void dAcEKs_c::fn_155_3460() {
    lbl_155_bss_388 = true;
    field_0xDA7 = true;
}

void dAcEKs_c::fn_155_3480() {
    if (field_0xDA7) {
        lbl_155_bss_388 = 0;
        field_0xDA7 = 0;
        if (field_0xDA6) {
            lbl_155_bss_389 = 0;
        }
    }
}

bool dAcEKs_c::transitionToNextState() {
    if (mCurrentState != mNextState) {
        switch (mNextState) {
            case 0:  mStateMgr.changeState(StateID_Move); break;
            case 1:  mStateMgr.changeState(StateID_Wait); break;
            case 2:  mStateMgr.changeState(StateID_PathMove); break;
            case 4:  mStateMgr.changeState(StateID_WakeUp); break;
            case 5:  mStateMgr.changeState(StateID_ReturnToWait); break;
            case 6:  mStateMgr.changeState(StateID_Chase); break;
            case 7:  mStateMgr.changeState(StateID_ChaseAttack); break;
            case 8:  mStateMgr.changeState(StateID_Fighting); break;
            case 9:  mStateMgr.changeState(StateID_AttackReady); break;
            case 10: mStateMgr.changeState(StateID_Attack); break;
            case 11: mStateMgr.changeState(StateID_Damage); break;
            case 12: mStateMgr.changeState(StateID_Stun); break;
            case 13: mStateMgr.changeState(StateID_WindBlow); break;
        }
        mCurrentState = mNextState;
        return true;
    }
    return false;
}

void dAcEKs_c::setAnim(char *anm, f32 f0, f32 rate, f32 frame) {
    mMdl.setAnm(anm, m3d::PLAY_MODE_4, f0);
    mMdl.setRate(rate);
    if (frame > 0.f) {
        mMdl.setFrame(frame);
    }
}

void dAcEKs_c::fn_155_3720(f32 scale, f32 rate) {
    f32 a = rate + scale * 0.01f;
    f32 endFrame = mMdl.getAnm().getEndFrame();
    if (a > endFrame) {
        a = endFrame;
    }
    mMdl.setRate(a);
}

void dAcEKs_c::fn_155_3750(u8 state) {
    switch (state) {
        case 0:  mNextState = state; break;
        case 1:  mNextState = 5; break;
        case 2:  mNextState = state; break;
        default: mNextState = 0; break;
    }
}

void dAcEKs_c::fn_155_37A0() {
    mSph.OnAtSet();
    if (mType == 2) {
        fn_155_3E90();
        field_0xDA9 = 0;
        field_0xD60 = 0;
    }
}

void dAcEKs_c::fn_155_37F0() {
    if (mType == 2) {
        fn_155_3E90();
        field_0xDA9 = 0;
        field_0xD60 = 0;
    }
}

void dAcEKs_c::fn_155_3840() {
    mSph.ClrAtSet();
    if (mType == 2) {
        fn_155_3EC0();
        field_0xDA9 = 1;
    }
}

void dAcEKs_c::fn_155_3890() {
    if (mType == 2) {
        fn_155_3EC0();
        field_0xDA9 = 1;
    }
}

void dAcEKs_c::on_lbl_155_bss_389() {
    lbl_155_bss_389 = true;
}

void dAcEKs_c::setBlinkChecked(u8 blink) {
    if (blink == mCurrentAnmTexPat) {
        return;
    }
    setBlink(blink);
}

void dAcEKs_c::fn_155_3900(s32 p0) {
    switch (p0) {
        case 0: {
            if (!(mStateMgr.isState(StateID_Stun) || mStateMgr.isState(StateID_Damage) ||
                  mStateMgr.isState(StateID_ReturnToWait) || mStateMgr.isState(StateID_Chase) ||
                  mStateMgr.isState(StateID_ChaseAttack) || mStateMgr.isState(StateID_Fighting) ||
                  mStateMgr.isState(StateID_AttackReady) || mStateMgr.isState(StateID_Attack) ||
                  mStateMgr.isState(StateID_WindBlow))) {
                setActorProperty(AC_PROP_0x1);
            } else {
                field_0xDB2 = 1;
            }
        } break;
        case 1: {
            unsetActorProperty(AC_PROP_0x1);
        } break;
    }
}

void dAcEKs_c::setPitchYawToPoint(const mVec3_c &pnt) {
    mPitch_0xC34 = -cLib::targetAngleX(mPosition, pnt);
    mYaw_0xC36 = cLib::targetAngleY(mPosition, pnt);
}

void dAcEKs_c::fn_155_3BD0(const mVec3_c &pnt) {
    mWorldMtx.multVec(pnt, field_0xBB0);
}

void dAcEKs_c::fn_155_3BE0(const mVec3_c &pnt) {
    if (mStateMgr.isState(StateID_AttackReady) || mStateMgr.isState(StateID_Attack)) {
        dAcObjBase_c *pAttack = attackPlayerOrScrapper(0.f);
        if (pAttack->isPlayer()) {
            mVec3_c v = static_cast<dAcPy_c *>(pAttack)->getCenterTranslation();
            pAttack->mWorldMtx.multVecSR(pnt, field_0xBBC);
            field_0xBBC += v;
        } else {
            pAttack->mWorldMtx.multVec(pnt, field_0xBBC);
        }
    } else {
        dAcObjBase_c *pAttack = attackPlayerOrScrapper(0.f);
        pAttack->mWorldMtx.multVec(pnt, field_0xBB0);
    }
}

void dAcEKs_c::setBlink(u8 blink) {
    nw4r::g3d::ResAnmTexPat pat(nullptr);
    mCurrentAnmTexPat = blink;
    switch (mCurrentAnmTexPat) {
        case 0: pat = mResFile.GetResAnmTexPat("blink_1"); break;
        case 1: pat = mResFile.GetResAnmTexPat("blink_2"); break;
        case 2: pat = mResFile.GetResAnmTexPat("blink_3"); break;
    }
    mAnmTexPat.setAnm(mMdl.getModel(), pat, 0, m3d::PLAY_MODE_4);
}

void dAcEKs_c::fn_155_3E90() {
    mSph.OnTgElectric();
    mSph.SetTg_0x50(2);
    mSph.OnCo_0x8000();
}

void dAcEKs_c::fn_155_3EC0() {
    mSph.ClrTgElectric();
    mSph.SetTg_0x50(0);
    mSph.ClrCo_0x8000();
}

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
