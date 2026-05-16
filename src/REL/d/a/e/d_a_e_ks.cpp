#include "d/a/e/d_a_e_ks.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_s_roof_chk.h"
#include "d/col/bg/d_bg_s_wtr_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_camera.h"
#include "d/d_light_env.h"
#include "d/d_pouch.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_ks.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/time_area_mgr.h"

struct dAcEKs_HIO_c {
    f32 _0x00; // 400.0
    s16 _0x04; // 0h
    s16 _0x06; // FFh
    s16 _0x08; // A0h
    s16 _0x0A; // 50h
    s16 _0x0C; // FFh
    s16 _0x0E; // 64h
    s16 _0x10; // FFh
    s16 _0x12; // FFh
    s16 _0x14; // FFh
    f32 _0x18; // 1.2
    f32 _0x1C; // 1.5
    f32 _0x20; // 1.0
    f32 _0x24; // 1.0
    f32 _0x28; // -35.0
    f32 _0x2C; // 30.0
    f32 _0x30; // 1.0
    s16 _0x34; // 71Ch
    s16 _0x36; // 0h
    f32 _0x38; // 30.0
    f32 _0x3C; // 15.0
    f32 _0x40; // 0.0
    f32 _0x44; // 100.0
    f32 _0x48; // 150.0
    f32 _0x4C; // 250.0
    f32 _0x50; // 150.0
    f32 _0x54; // 120.0
    f32 _0x58; // 3.5
    f32 _0x5C; // 3.5
    f32 _0x60; // 3.5
    f32 _0x64; // 10.0
    f32 _0x68; // 20.0
    f32 _0x6C; // 35.0
    f32 _0x70; // 7.0
    f32 _0x74; // 200.0
    f32 _0x78; // 10.0
    f32 _0x7C; // 0.0
    f32 _0x80; // 0.5
    s16 _0x84; // 1555h
    s16 _0x86; // 0h
    f32 _0x88; // 500.0
    f32 _0x8C; // 150.0
    f32 _0x90; // 150.0
    f32 _0x94; // 3000.0
    f32 _0x98; // 120.0
    f32 _0x9C; // 1.0
    s16 _0xA0; // Fh
    s16 _0xA2; // AABh
    f32 _0xA4; // 1.0
    s16 _0xA8; // 1Eh
    s16 _0xAA; // AABh
    f32 _0xAC; // 300.0
    f32 _0xB0; // 3.0
    s16 _0xB4; // 3h
    s16 _0xB6; // 1h
    f32 _0xB8; // 0.01
    f32 _0xBC; // 100.0
    s16 _0xC0; // AABh
    s16 _0xC2; // 0h
    f32 _0xC4; // -0.5

    const s32 getZero() const {
        s32 _weird_zero = 0;
        return _0x04;
    }
    static const dAcEKs_HIO_c sInstance;
};
const dAcEKs_HIO_c dAcEKs_HIO_c::sInstance = {
    400.0, 0,    0xFF,  0xA0, 0x50,  0xFF,  0x64,  0xFF,  0xFF,   0xFF,  1.2,          1.5,   1.0,   1.0,    -35.0,
    30.0,  1.0,  0x71C, 0,    30.0,  15.0,  0.0,   100.0, 150.0,  250.0, 150.0,        120.0, 3.5,   3.5,    3.5,
    10.0,  20.0, 35.0,  7.0,  200.0, 10.0,  0.0,   0.5,   0x1555, 0x0,   500.0,        150.0, 150.0, 3000.0, 120.0,
    1.0,   0xF,  0xAAB, 1.0,  0x1E,  0xAAB, 300.0, 3.0,   0x3,    0x1,   0.0099999998, 100.0, 0xAAB, 0,      -0.5,
};

SPECIAL_ACTOR_PROFILE(E_KS, dAcEKs_c, fProfile::E_KS, 0xFE, 0, 4099);

static dCcD_SrcSph sSrcSph = {{0x400}};

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

bool dAcEKs_c::lbl_155_bss_388;
bool dAcEKs_c::lbl_155_bss_389;

bool dAcEKs_c::restorePosRotFromCopy() {
    if (field_0xDCF == 0) {
        if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(mRoomID, mPosition) != 0) {
            mTimeArea.setField0x00(0);
            mSph.ClrCoSet();
            mSph.ClrTgSet();
            mSph.ClrAtSet();
            fn_800306D0();
            if (mType == EKS_FIRE || mType == EKS_ELECTRIC) {
                field_0xD20 = 0.f;
                mLightInfluence.SetScale(0);
                if (mEmitter1.hasEmitters()) {
                    mEmitter1.remove(true);
                }
            }
        } else {
            mTimeArea.setField0x00(1);
            fn_80030700();
            mSph.OnCoSet();
            mSph.OnTgSet();
        }
        mMdl.getModel().setScale(mScale * mTimeArea.getDistMaybe());
    }
    return true;
}

void dAcEKs_c::linkKiesuTag(dTgKiesuTag_c *pTgKs) {
    mTgRef.link(pTgKs);
}

void dAcEKs_c::setStartingState() {
    mTimer = cM::rndInt(1000);
    switch (mStartingState) {
        case EKS_STARTSTATE_Move: {
            changeState(StateID_Move);
            mCurrentState = mStartingState;
            mAcch.ClrRoofNone();
            mAcch.SetField_0xD4(55.f);
            mAcch.SetGroundUpY(30.f);
            mAcchCir.SetWall(20.f, 60.f);
        } break;
        case EKS_STARTSTATE_Wait: {
            changeState(StateID_Wait);
            mCurrentState = mStartingState;
            if ((s32)getFromParams(30, 0x3) == 1) {
                field_0xDB1 = 1;
            }
            mAcch.ClrRoofHit();
            mAcch.SetGroundUpY(0.f);
            mAcch.SetField_0xD4(0.f);
            mAcchCir.SetWall(0.f, 5.f);

        } break;
        case EKS_STARTSTATE_PathMove: {
            mCurrentState = mStartingState;
            mAcch.ClrRoofNone();
            mAcch.SetField_0xD4(55.f);
            mAcch.SetGroundUpY(30.f);
            mAcchCir.SetWall(20.f, 60.f);
            changeState(StateID_PathMove);
        } break;
        default: {
            changeState(StateID_Move);
            mCurrentState = mStartingState;
        } break;
    }

    mCurrentState = mNextState;
}

void dAcEKs_c::setIdleState() {
    switch (mStartingState) {
        case EKS_STARTSTATE_Move: {
            changeState(StateID_Move);
            mCurrentState = EKS_STATE_Move;
        } break;
        case EKS_STARTSTATE_Wait: {
            changeState(StateID_ReturnToWait);
            mCurrentState = EKS_STATE_ReturnToWait;
        } break;
        case EKS_STARTSTATE_PathMove: {
            changeState(StateID_PathMove);
            mCurrentState = EKS_STATE_PathMove;
        } break;
        default: {
            changeState(StateID_Move);
            mCurrentState = EKS_STATE_Move;
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
        return getStartingPos().y;
    }
}

void dAcEKs_c::fn_155_A60() {
    if (sLib::calcTimer(&field_0xDBD)) {
        return;
    }

    s32 s = fn_8002FDE0(mSph, nullptr);
    if (s == 6 || s == 7) {
        return;
    }
    switch (s) {
        case 12:
        case 13:
            if (mSph.ChkTgHit() && mSph.ChkTgAtHitType(AT_TYPE_SWORD)) {
                return;
            }
        default:
            field_0xDBD = 5;
            dJEffManager_c::spawnHitMarkEffect(0, mSph, nullptr, true);

            if (!mSph.ChkTgHit()) {
                return;
            }

            field_0xD96 = mSph.GetTgAtHitDir().atan2sX_Z();
            field_0xD98 = mSph.GetTgAtHitDir().atan2sY_XZ();
            field_0xD98 = -field_0xD98;

            if (mSph.ChkTgAtHitType(AT_TYPE_SWORD)) {
                s16 a = 0;
                s16 x = -cLib::targetAngleX(mPosition, dAcPy_c::GetLink()->mPosition);

                if (mSph.GetTgSoundID() == 0x01) {
                    x += 0x2000;
                } else if (mSph.GetTgSoundID() == 0x80) {
                    a = 0x1000;
                    x += 0x1000;
                } else if (mSph.GetTgSoundID() == 0x40) {
                    a = 0x2000;
                } else if (mSph.GetTgSoundID() == 0x20) {
                    a = 0x1000;
                    x += -0x1000;
                } else if (mSph.GetTgSoundID() == 0x10) {
                    x += -0x2000;
                } else if (mSph.GetTgSoundID() == 0x08) {
                    a = -0x1000;
                    x += -0x1000;
                } else if (mSph.GetTgSoundID() == 0x04) {
                    a = -0x2000;
                } else if (mSph.GetTgSoundID() == 0x02) {
                    a = -0x1000;
                    x += 0x1000;
                } else if (mSph.GetTgSoundID() == 0x100) {
                    x += -0x4000;
                }

                field_0xD92 = x * 0.5f;
                field_0xD94 = a * 0.5f;

                if (mHealth == 0) {
                    if (!isState(StateID_Wait)) {
                        mVelocity.y += 30.f;
                    }
                    changeState(StateID_Damage);
                    if (field_0xD86 > 0) {
                        field_0xDA9 = 1;
                    }
                    mSpeed = 65.f;
                    mAcchCir.SetWall(20.f, 60.f);
                    mAcceleration = -6.f;
                    field_0xD5E = 5;
                    mAcch.SetGroundUpY(0.f);

                    unsetActorProperty(AC_PROP_0x1);
                    mSph.ClrTgSet();
                } else {
                    if (!isState(StateID_Wait)) {
                        mVelocity.y += 30.f;
                    }
                    changeState(StateID_Damage);
                    if (field_0xD86 > 0) {
                        field_0xDA9 = 1;
                    }
                    mRotation.x += field_0xD98 * 0.5f;
                    mRotation.y += field_0xD96 * 0.5f;
                    mSpeed = 30.f;
                    field_0xDC6 = 8;
                    field_0xDBF = 8;
                }
            } else {
                if (mSph.ChkTgAtHitType(AT_TYPE_WHIP)) {
                    field_0xDB6 = 1;
                }
                mHealth = 0;
                if (isState(StateID_Wait)) {
                    changeState(StateID_Damage);
                    if (field_0xD86 > 0) {
                        field_0xDA9 = 1;
                    }
                    if (mHealth == 0) {
                        mAcceleration = -4.f;
                        field_0xD92 = field_0xD98 * 0.2f;
                        field_0xD94 = field_0xD96 * 0.1f;
                    }
                } else {
                    changeState(StateID_Damage);

                    if (field_0xD86 > 0) {
                        field_0xDA9 = 1;
                    }
                    if (mHealth == 0) {
                        mSpeed = 65.f;
                        mAcchCir.SetWall(20, 60);
                        mAcceleration = -6.f;
                        field_0xD5E = 5;
                        mAcch.SetGroundUpY(0);
                        unsetActorProperty(AC_PROP_0x1);
                        mSph.ClrTgSet();
                    }
                    field_0xD92 = field_0xD98 * 0.2f;
                    field_0xD94 = field_0xD96 * 0.1f;
                    mVelocity.y += 30.f;
                }
            }
            break;
        case 0: {
            return;
        }
    }
}

// TODO (I dont want to mess with Item stuff with the other PR open)
extern "C" void spawnDrop(ITEM_ID, s32 roomId, const mVec3_c &pos, const mAng3_c &rot);

void dAcEKs_c::kill(bool dropItem) {
    dLightEnv_c::GetPInstance()->setBPM8_Type4(&mPosition);
    mTgRef.unlink();

    if (field_0xDB6 != 0) {
        killWithFlagNoItemDrop();
    } else if (dropItem == true) {
        if (mType == EKS_CURSED) {
            if (tryDropItem(0.05f)) {
                spawnDrop(ITEM_EVIL_CRYSTAL, mRoomID, mPosition, mAngle);
            }
        } else {
            if (tryDropItem(0.05f)) {
                spawnDrop(ITEM_MONSTER_CLAW, mRoomID, mPosition, mAngle);
            }
        }
    } else {
        killWithFlagNoItemDrop();
    }

    static mVec3_c deathEffectScale(0.8, 0.8, 0.8);
    playDeathEffect0(mPosition, &deathEffectScale, false);
}

void dAcEKs_c::chaseTargetY(f32 ratio, f32 maxStepSize) {
    sLib::addCalcScaledDiff(&mPosition.y, mTargetPos.y, ratio, maxStepSize);
    sLib::addCalcAngle(mAngle.y.ref(), mYaw_0xC36, 10, 0x38E);
}

void dAcEKs_c::playBlinkAnm() {
    mAnmTexPat.play();
    if (mAnmTexPat.isStop(0)) {
        if (mCurrentAnmTexPat == EKS_TEXPAT_BLINK3) {
            setBlink(EKS_TEXPAT_BLINK1);
        }

        mBlinkTimer--;
        if (mBlinkTimer <= 0) {
            mAnmTexPat.setFrame(0, 0);
            mBlinkTimer = cM::rndInt(75) + 15;
        }
    }
}

void dAcEKs_c::soundSqueak() {
    if (mSqueakSoundTimer > 0) {
        mSqueakSoundTimer--;
    } else {
        startSound(SE_EKs_V_NAKU);
        mSqueakSoundTimer = cM::rndInt(20) + 20;
    }
}

void dAcEKs_c::soundStun() {
    if (mStunSoundTimer > 0) {
        mStunSoundTimer--;
    } else {
        startSound(SE_EKs_V_FAINT);
        mStunSoundTimer = cM::rndInt(20) + 20;
    }
}

void dAcEKs_c::fn_155_1430() {
    field_0xD6C = cM::rndInt(1) + 3;
}

void dAcEKs_c::fn_155_1470() {
    field_0xD52--;

    if (field_0xD54 > 0) {
        field_0xD54--;
    }

    if (field_0xD90 > 0) {
        field_0xD90--;
    }

    if (field_0xDA6 != 0) {
        if (!transitionToNextState()) {
            if (field_0xD54 <= 0) {
                f32 yDiff = getStartingPos().y - mPosition.y;
                if (yDiff >= 0.f && yDiff <= 150.f) {
                    field_0xDC1 = 0;
                    if (getStartingPos().squareDistanceToXZ(mPosition) < 6400.f) {
                        changeState(StateID_WaitReady);
                        mCurrentState = EKS_STATE_Wait;
                        return;
                    }
                } else {
                    if (yDiff < 0.f) {
                        field_0xDC1 = 2;
                    }
                }
            }
            adjustTargetGnd();
            adjustTargetRoof();
            setPitchYawToPoint(mTargetPos);
            chaseTargetY(0.1f, 3.0f);
            sLib::addCalcAngle(mRotation.y.ref(), mYaw_0xC36, 10, 0x38E);
            sLib::addCalcAngle(mRotation.x.ref(), mPitch_0xC34, 12, 0xB6);
            clampRotationX();
        }
    } else {
        field_0xB8C.set(mPnts[field_0xD9E]);
        f32 f = mPosition.squareDistance(field_0xB8C);

        if (checkWallCross()) {
            field_0xDB7++;
            if (field_0xDB7 < 5) {
                field_0xDC1 = 0;

                mTargetPos.x = field_0xB8C.x + cM::rndFX(800);
                mTargetPos.z = field_0xB8C.z + cM::rndFX(800);

                adjustTargetGnd();
                adjustTargetRoof();
            } else if (field_0xDB7 < 10) {
                field_0xDC1 = 1;

                fn_155_3BD0(mVec3_c(cM::rndFX(300), cM::rndFX(300), -cM::rndF(300)));

                field_0xD52 = cM::rndInt(20) + 20;

                adjustTargetGnd();
                adjustTargetRoof();
            }
            if (field_0xDB7 > 20) {
                field_0xDB7 = 0;
            }
        }

        if (field_0xD54 <= 0) {
            f32 yDiff = getStartingPos().y - mPosition.y;
            if (yDiff >= 0.f && yDiff <= 150.f) {
                field_0xDC1 = 0;
                if (field_0xD9E == 0) {
                    if (getStartingPos().squareDistanceToXZ(mPosition) < 6400.f) {
                        changeState(StateID_WaitReady);
                        return;
                    }
                }
            } else {
                if (yDiff < 0.f) {
                    field_0xDC1 = 2;
                }
            }
        }

        if (!checkBeyondRadius(getStartingPos(), 490000) && field_0xD54 <= 0 && field_0xD90 <= 0) {
            if (fn_155_2B80(true, 0)) {
                changeState(StateID_Chase);
                return;
            }
            field_0xD90 = 3;
        }

        if (field_0xDC1 == 0) {
            if (checkBeyondRadius(getStartingPos(), 490000) && field_0xD52 <= 0) {
                field_0xDA0++;
                if (field_0xDA0 > 5) {
                    mTargetPos.x = field_0xB8C.x + cM::rndFX(800);
                    mTargetPos.y = field_0xB8C.y + cM::rndFX(30);
                    mTargetPos.z = field_0xB8C.z + cM::rndFX(800);
                    field_0xDA0 = 0;
                } else {
                    mTargetPos.x = field_0xB8C.x + cM::rndFX(30);
                    mTargetPos.y = field_0xB8C.y + cM::rndFX(30);
                    mTargetPos.z = field_0xB8C.z + cM::rndFX(30);
                }
                field_0xD52 = cM::rndInt(20) + 20;
                adjustTargetGnd();
                adjustTargetRoof();
            }
        } else if (field_0xDC1 == 2) {
            field_0xDBA++;
            if (field_0xDBA < 20) {
                if (field_0xDBA % 2) {
                    field_0xDC1 = 0;
                    fn_155_3BD0(mVec3_c(cM::rndFX(800), cM::rndFX(400), cM::rndFX(800)));
                    adjustTargetGnd();
                    adjustTargetRoof();
                }
            } else if (field_0xDBA < 40) {
                if (field_0xDBA % 2) {
                    fn_155_3BD0(mVec3_c(cM::rndFX(800), -cM::rndF(400), cM::rndFX(800)));
                    field_0xD52 = cM::rndInt(20) + 20;
                    adjustTargetGnd();
                    adjustTargetRoof();
                }
            }

            if (field_0xDBA > 60) {
                field_0xDBA = 0;
                field_0xDC1 = 0;
            }
        }

        if (field_0xD52 <= 0) {
            if (field_0xD9E > 0) {
                mVec3_c startPos = mPosition;
                for (s32 i = 0; i <= field_0xD9E; ++i) {
                    if (!dBgS_ObjLinChk::LineCross(&startPos, &mPnts[i], nullptr)) {
                        field_0xD9E = i;
                        field_0xB8C.set(mPnts[field_0xD9E]);
                    }
                }
            }

            mTargetPos.x = field_0xB8C.x + cM::rndFX(800);
            mTargetPos.y = field_0xB8C.y + cM::rndFX(400);
            mTargetPos.z = field_0xB8C.z + cM::rndFX(800);

            field_0xDC1 = 0;

            field_0xD52 = cM::rndInt(20) + 20;

            adjustTargetGnd();
            adjustTargetRoof();
        }

        setPitchYawToPoint(mTargetPos);
        chaseTargetY(0.1f, 3.0f);
        sLib::addCalcAngle(mRotation.y.ref(), mYaw_0xC36, 10, 0x38E);
        sLib::addCalcAngle(mRotation.x.ref(), mPitch_0xC34, 12, 0xB6);
        clampRotationX();

        if (field_0xD9E != 0 && f < 2500.f) {
            field_0xD9E--;
        }
    }
}

void dAcEKs_c::fn_155_1C80() {
    if (field_0xDC8 != 0) {
        field_0xDC8--;
        field_0xDA5 = 1;
    } else {
        field_0xDA5 = 0;
    }

    if (field_0xD52 > 0) {
        field_0xD52--;
    }

    if (field_0xD90 > 0) {
        field_0xD90--;
    }

    if (field_0xDA6 != 0) {
        adjustTargetGnd();
        adjustTargetRoof();
        if (field_0xDC8 > 120 || !transitionToNextState()) {
            setPitchYawToPoint(mTargetPos);
            chaseTargetY(field_0xD00 + 0.1f, field_0xD04 + 3.0f);
            sLib::addCalcAngle(mRotation.y.ref(), mYaw_0xC36, 10, 0x38E);
            sLib::addCalcAngle(mRotation.x.ref(), mPitch_0xC34, 12, 0xB6);
            clampRotationX();
        }
    } else {
        if (field_0xD54 > 0) {
            field_0xD54--;
        }

        field_0xB8C.set(mPnts[field_0xD9E]);

        f32 f = mPosition.squareDistance(field_0xB8C);
        if (checkWallCross()) {
            field_0xDB7++;
            if (field_0xDB7 < 5) {
                field_0xDC1 = 0;

                mTargetPos.x = field_0xB8C.x + cM::rndFX(800);
                mTargetPos.y = field_0xB8C.y + cM::rndF(400);
                mTargetPos.z = field_0xB8C.z + cM::rndFX(800);

                adjustTargetGnd();
                adjustTargetRoof();
            } else if (field_0xDB7 < 10) {
                field_0xDC1 = 1;

                fn_155_3BD0(mVec3_c(cM::rndFX(200), cM::rndFX(200), cM::rndF(-300)));

                field_0xD52 = cM::rndInt(20) + 20;

                adjustTargetGnd();
                adjustTargetRoof();
            } else if (field_0xDB7 > 20) {
                field_0xDB7 = 0;
            }
        }

        if (field_0xDC1 == 0) {
            if (checkBeyondRadius(getStartingPos(), 490000)) {
                if (field_0xD52 <= 0) {
                    mTargetPos.x = field_0xB8C.x + cM::rndFX(800);
                    mTargetPos.y = field_0xB8C.y + cM::rndFX(400);
                    mTargetPos.z = field_0xB8C.z + cM::rndFX(800);
                    field_0xD52 = cM::rndInt(20) + 20;
                    adjustTargetGnd();
                    adjustTargetRoof();
                }
            } else {
                if (field_0xDC8 <= 120 && field_0xD54 <= 0 && field_0xD90 <= 0) {
                    if (fn_155_2B80(true, 0)) {
                        changeState(StateID_Chase);
                        return;
                    }
                    field_0xD90 = 3;
                }
            }
        }

        if (field_0xD52 <= 0) {
            if (field_0xD9E > 0) {
                mVec3_c startPos = mPosition;
                for (s32 i = 0; i <= field_0xD9E; ++i) {
                    if (!dBgS_ObjLinChk::LineCross(&startPos, &mPnts[i], nullptr)) {
                        field_0xD9E = i;
                        field_0xB8C.set(mPnts[field_0xD9E]);
                    }
                }
            }

            field_0xDC1 = 0;

            field_0xD52 = cM::rndInt(20) + 20;

            mTargetPos.x = field_0xB8C.x + cM::rndFX(800);
            mTargetPos.y = field_0xB8C.y + cM::rndFX(400);
            mTargetPos.z = field_0xB8C.z + cM::rndFX(800);

            field_0xD52 = cM::rndInt(20) + 20; // Set twice lol

            adjustTargetGnd();
            adjustTargetRoof();
        }

        if (mTargetPos.squareDistanceToXZ(mPosition) < 10000.f) {
            mTargetPos.x = field_0xB8C.x + cM::rndFX(800);
            mTargetPos.y = field_0xB8C.y + cM::rndFX(400);
            mTargetPos.z = field_0xB8C.z + cM::rndFX(800);

            adjustTargetGnd();
            adjustTargetRoof();
        }

        setPitchYawToPoint(mTargetPos);
        chaseTargetY(field_0xD00 + 0.1f, field_0xD04 + 3.0f);
        sLib::addCalcAngle(mRotation.y.ref(), mYaw_0xC36, 10, 0x38E);
        sLib::addCalcAngle(mRotation.x.ref(), mPitch_0xC34, 12, 0xB6);
        clampRotationX();

        if (field_0xD9E != 0 && f < 2500.f) {
            field_0xD9E--;
        }
    }
}

void dAcEKs_c::fn_155_2270() {
    // NONMATCHING
    if (field_0xD6A > 0) {
        field_0xD6A--;
    }

    dAcObjBase_c *pTarget = targetPlayerOrScrapper(0.f);
    field_0xBF8.set(pTarget->mPosition + field_0xBEC);

    mAng y = cLib::targetAngleY(mPosition, mTargetPos);
    y = y - mRotation.y;
    if (field_0xDA6 != 0) {
        if (transitionToNextState()) {
            if (mType == EKS_ELECTRIC) {
                fn_155_3EC0();
                field_0xDA9 = 1;
            }
        } else {
            if (field_0xDC5 != 0) {
                field_0xDC5--;
            } else {
                if (field_0xDAF != 0 && dAcPy_c::GetLink()->checkCurrentAction(/* SHIELD_BASH*/ 73)) {
                    ;
                    mWorldMtx.multVecSR(mVec3_c(0, 0, -250), field_0xC1C);
                    mTargetPos += field_0xC1C;
                    field_0xDC5 = 20;
                }
            }

            if (field_0xD6A <= 0) {
                if (lbl_155_bss_389) {
                    field_0xC70 = 0.f;
                    if (field_0xD6A <= 0) {
                        field_0xC70 = 0.f;
                        if (isTargeted()) {
                            field_0xD6C -= 2;
                        } else {
                            field_0xD6C -= 1;
                        }

                        if (field_0xD6C <= 0) {
                            fn_155_3460();
                            if (mTgRef.isLinked()) {
                                mTgRef.get()->incrementField_0x456();
                            }
                            changeState(StateID_AttackReady);
                            mCurrentState = EKS_STATE_AttackReady;

                            if (mType == EKS_ELECTRIC) {
                                fn_155_3EC0();
                                field_0xDA9 = 1;
                            }
                            return;
                        }
                    }
                }
                fn_155_3BE0(field_0xBC8);
                field_0xD6A = cM::rndInt(30) + 15;
                adjustTargetGnd();
                adjustTargetRoof();
            }

            if (field_0xD9A > 0) {
                field_0xC70 = 0.f;
            } else {
                sLib::addCalcScaledDiff(&field_0xC70, 35, 1, 3);
            }
            cLib::addCalcPos2(&mPosition, mTargetPos, 0.1, field_0xC70);
            setPitchYawToPoint(field_0xBF8);
            sLib::addCalcAngle(mAngle.y.ref(), mYaw_0xC36, 12, 0x1555);
            sLib::addCalcAngle(mRotation.y.ref(), mYaw_0xC36, 12, 0x1555);
            sLib::addCalcAngle(mRotation.x.ref(), 0, 12, 0x1555);
            // ??
            if (u16(y + 0x3FFF) <= 0x7FFE) {
                y = mRotation.x + 0xAAA;
            } else {
                y = mRotation.x - 0xAAA;
            }
            sLib::addCalcAngle(mRotation.x.ref(), y, 12, 0x1555);
        }
    } else {
        if (field_0xD90 > 0) {
            field_0xD90--;
        }
        if (field_0xDC5 != 0) {
            field_0xDC5--;
        } else {
            if (dAcPy_c::GetLink()->checkCurrentAction(/* SHIELD_BASH*/ 73)) {
                ;
                mWorldMtx.multVecSR(mVec3_c(0, 0, -250), field_0xC1C);
                mTargetPos += field_0xC1C;
                field_0xDC5 = 20;
            }
        }
        f32 r = field_0xC44 + 1600.f;
        if (checkBeyondRadius(getStartingPos(), r * r)) {
            setIdleState();
        } else {
            if (field_0xD90 <= 0) {
                if (!fn_155_2B80(true, 1)) {
                    setIdleState();
                    return;
                }
                field_0xD90 = 3;
            }

            if (dAcPy_c::GetLink()->checkFlags0x340(0x100)) {
                if (isOutsideRange()) {
                    changeState(StateID_Chase);
                    return;
                }
            } else {
                if (field_0xDB3 == 0 && dBgS_WtrChk::CheckPos(&mPosition, true, 100, -2000)) {
                    field_0xD10 = dBgS_WtrChk::GetWaterHeight();
                    field_0xDB3 = 1;
                }
            }
            if (field_0xD6A <= 0) {
                field_0xC70 = 0.f;
                if (lbl_155_bss_388) {
                    fn_155_1430();
                } else {
                    if (isTargeted()) {
                        field_0xD6C -= 2;
                    } else {
                        field_0xD6C -= 1;
                    }
                    if (field_0xD6C <= 0 && dAcPy_c::GetLink()->checkFlags0x340(0x100) &&
                        field_0xBF8.squareDistanceToXZ(mPosition) < 160000.f) {
                        fn_155_3460();
                        changeState(StateID_AttackReady);
                        return;
                    }
                }
                field_0xBC8.set(cM::rndFX(250), cM::rndF(150) + 100.f, cM::rndF(120) + 150.f);
                fn_155_3BE0(field_0xBC8);

                if (!dAcPy_c::GetLink()->checkFlags0x340(0x100)) {
                    mTargetPos.y = field_0xD10 + 50.f;
                }
                field_0xD6A = cM::rndInt(30) + 15;
                adjustTargetGnd();
                adjustTargetRoof();
            }
            if (field_0xD9A > 0) {
                field_0xC70 = 0.f;
            } else {
                sLib::addCalcScaledDiff(&field_0xC70, 20, 1, 1);
            }

            cLib::addCalcPos2(&mPosition, mTargetPos, 0.1, field_0xC70);
            setPitchYawToPoint(field_0xBF8);
            sLib::addCalcAngle(mAngle.y.ref(), mYaw_0xC36, 12, 0x1555);
            sLib::addCalcAngle(mRotation.y.ref(), mYaw_0xC36, 12, 0x1555);
            sLib::addCalcAngle(mRotation.x.ref(), 0, 12, 0x1555);

            // ??
            if (u16(y + 0x3FFF) <= 0x7FFE) {
                y = mRotation.x + 0xAAA;
            } else {
                y = mRotation.x - 0xAAA;
            }
            sLib::addCalcAngle(mRotation.x.ref(), y, 12, 0x1555);
        }
    }
}

bool dAcEKs_c::fn_155_29D0(s32 p1) {
    dAcObjBase_c *pTarget = targetPlayerOrScrapper(0.f);
    field_0xBF8.set(pTarget->mPosition);
    field_0xBF8.y += 100.f;

    dBgS_ObjLinChk linChk;
    if (isState(StateID_Wait)) {
        mVec3_c start = mPosition;
        start.y -= 70.f;
        linChk.Set(&start, &field_0xBF8, nullptr);
    } else {
        linChk.Set(&mPosition, &field_0xBF8, nullptr);
    }
    linChk.OnBackFlag();

    switch (p1) {
        case 0: {
            if (dBgS::GetInstance()->LineCross(&linChk)) {
                return true;
            }
        } break;
        case 1: {
            if (dBgS::GetInstance()->LineCross(&linChk)) {
                field_0xDCB++;
            } else {
                field_0xDCB = 0;
            }
            if (field_0xDCB > 10) {
                field_0xDCB = 0;

                return true;
            }
        } break;
    }
    return false;
}

bool dAcEKs_c::fn_155_2B80(bool b, s32 p1) {
    f32 f0 = field_0xDA5 != 0 ? field_0xC48 + 1200.f : 700.f;
    dAcObjBase_c *pTarget = targetPlayerOrScrapper(0.f);
    field_0xBF8.set(pTarget->mPosition);
    if (field_0xBF8.squareDistanceToXZ(mPosition) < f0 * f0) {
        f32 f2 = mPosition.y - field_0xBF8.y;
        if (f2 >= -300.f && f2 <= 1500.f) {
            if (dAcPy_c::GetLink()->isRecovering() ||
                /* cannot be EventManager::isInEventOtherThan0Or7 */
                (EventManager::isInEvent() && !EventManager::isInEvent0Or7())) {
                return false;
            }
            if (fn_155_3EF0()) {
                return false;
            }
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

bool dAcEKs_c::fn_155_2D60(bool b, s32 p1) {
    f32 f0 = field_0xC40 + 1500.f;
    f32 f1 = field_0xC3C + 700.f;
    dAcObjBase_c *pTarget = targetPlayerOrScrapper(0.f);
    field_0xBF8.set(pTarget->mPosition);
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
    dAcObjBase_c *pTarget = dAcPy_c::GetLink()->getBeetleInFlight();
    if (pTarget) {
        field_0xC28.set(pTarget->mPosition);
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
    return targetPlayerOrScrapper(0.f)->mPosition.squareDistanceToXZ(mPosition) > 360000.f;
}

bool dAcEKs_c::isNotWaitingNorDamage() {
    if (!(isState(StateID_Wait) || isState(StateID_Damage))) {
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

bool dAcEKs_c::checkWallCross() {
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

void dAcEKs_c::adjustTargetGnd() {
    dBgS_ObjGndChk gndChk;
    mVec3_c pos = mTargetPos;
    pos.y += 150.f;
    gndChk.SetPos(&pos);
    f32 chk = dBgS::GetInstance()->GroundCross(&gndChk);
    if (mTargetPos.y < chk + 120.f) {
        mTargetPos.y = chk + 120.f;
    }
}

void dAcEKs_c::adjustTargetRoof() {
    dBgS_ObjRoofChk roofChk;
    mVec3_c pos = mTargetPos;
    pos.y -= 150.f;
    roofChk.SetPos(&pos);
    f32 chk = dBgS::GetInstance()->RoofChk(&roofChk);
    if (mTargetPos.y > chk - 120.f) {
        mTargetPos.y = chk - 120.f;
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
            case EKS_STATE_Move:         changeState(StateID_Move); break;
            case EKS_STATE_Wait:         changeState(StateID_Wait); break;
            case EKS_STATE_WakeUp:       changeState(StateID_WakeUp); break;
            case EKS_STATE_ReturnToWait: changeState(StateID_ReturnToWait); break;
            case EKS_STATE_Chase:        changeState(StateID_Chase); break;
            case EKS_STATE_ChaseAttack:  changeState(StateID_ChaseAttack); break;
            case EKS_STATE_Fighting:     changeState(StateID_Fighting); break;
            case EKS_STATE_AttackReady:  changeState(StateID_AttackReady); break;
            case EKS_STATE_Attack:       changeState(StateID_Attack); break;
            case EKS_STATE_Damage:       changeState(StateID_Damage); break;
            case EKS_STATE_Stun:         changeState(StateID_Stun); break;
            case EKS_STATE_WindBlow:     changeState(StateID_WindBlow); break;
            case EKS_STATE_PathMove:     changeState(StateID_PathMove); break;
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
        case EKS_STATE_Move:     mNextState = state; break;
        case EKS_STATE_Wait:     mNextState = EKS_STATE_ReturnToWait; break;
        case EKS_STATE_PathMove: mNextState = state; break;
        default:                 mNextState = EKS_STATE_Move; break;
    }
}

void dAcEKs_c::fn_155_37A0() {
    mSph.OnAtSet();
    if (mType == EKS_ELECTRIC) {
        fn_155_3E90();
        field_0xDA9 = 0;
        field_0xD60 = 0;
    }
}

void dAcEKs_c::fn_155_37F0() {
    if (mType == EKS_ELECTRIC) {
        fn_155_3E90();
        field_0xDA9 = 0;
        field_0xD60 = 0;
    }
}

void dAcEKs_c::fn_155_3840() {
    mSph.ClrAtSet();
    if (mType == EKS_ELECTRIC) {
        fn_155_3EC0();
        field_0xDA9 = 1;
    }
}

void dAcEKs_c::fn_155_3890() {
    if (mType == EKS_ELECTRIC) {
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
            if (!(isState(StateID_Stun) || isState(StateID_Damage) || isState(StateID_ReturnToWait) ||
                  isState(StateID_Chase) || isState(StateID_ChaseAttack) || isState(StateID_Fighting) ||
                  isState(StateID_AttackReady) || isState(StateID_Attack) || isState(StateID_WindBlow))) {
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
    mWorldMtx.multVec(pnt, mTargetPos);
}

void dAcEKs_c::fn_155_3BE0(const mVec3_c &pnt) {
    if (isState(StateID_AttackReady) || isState(StateID_Attack)) {
        dAcObjBase_c *pAttack = targetPlayerOrScrapper(0.f);
        if (pAttack->isPlayer()) {
            mVec3_c v = static_cast<dAcPy_c *>(pAttack)->getCenterTranslation();
            pAttack->mWorldMtx.multVecSR(pnt, field_0xBBC);
            field_0xBBC += v;
        } else {
            pAttack->mWorldMtx.multVec(pnt, field_0xBBC);
        }
    } else {
        dAcObjBase_c *pAttack = targetPlayerOrScrapper(0.f);
        pAttack->mWorldMtx.multVec(pnt, mTargetPos);
    }
}

void dAcEKs_c::setBlink(u8 blink) {
    nw4r::g3d::ResAnmTexPat pat(nullptr);
    mCurrentAnmTexPat = blink;
    switch (mCurrentAnmTexPat) {
        case EKS_TEXPAT_BLINK1: pat = mResFile.GetResAnmTexPat("blink_1"); break;
        case EKS_TEXPAT_BLINK2: pat = mResFile.GetResAnmTexPat("blink_2"); break;
        case EKS_TEXPAT_BLINK3: pat = mResFile.GetResAnmTexPat("blink_3"); break;
    }
    mAnmTexPat.setAnm(mMdl.getModel(), pat, 0, m3d::PLAY_MODE_4);
}

bool dAcEKs_c::checkPathPntParam(u32 param) {
    return param != 0 && param < 0xFF;
}

void dAcEKs_c::fn_155_3E30() {
    if (field_0xDB2 != 0) {
        setActorProperty(AC_PROP_0x1);
    }
    field_0xDB2 = 0;
}

void dAcEKs_c::clampRotationX() {
    if (mRotation.x >= 0xE39) {
        mRotation.x.set(0xE39);
    } else if (mRotation.x <= -0xE39) {
        mRotation.x.set(-0xE39);
    }
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

bool dAcEKs_c::fn_155_3EF0() {
    if (field_0xDCF == 0 && dTimeAreaMgr_c::GetInstance()->fn_800B9B60(mRoomID, dAcPy_c::GetLink()->mPosition)) {
        return true;
    } else {
        return false;
    }
}

void dAcEKs_c::fn_155_3F50() {
    if ((field_0xD9C & 1) == 0 && field_0xD9E < 9) {
        mVec3_c end = mPosition;
        if (dBgS_ObjLinChk::LineCross(&mPnts[field_0xD9E], &end, nullptr)) {
            field_0xD9E++;
            mPnts[field_0xD9E].set(end);
        }
    }

    if (field_0xD9E != 0 && ((field_0xD9C + 4) & 7) == 0) {
        mVec3_c end = mPosition;
        if (!dBgS_ObjLinChk::LineCross(&mPnts[field_0xD9E - 1], &end, nullptr)) {
            field_0xD9E--;
        }
    }
}

bool dAcEKs_c::createHeap() {
    static const char *resFiles[] = {"Kiesu", "Kiesu_fire", "Kiesu_electric", "KiesuDevil"};
    static const char *mdlNames[] = {"kiesu", "F_kiesu", "EKiesu", "DKiesu"};

    mType = getFromParams(0, 0x7);
    nw4r::g3d::ResFile mdlResFile(getOarcResFile(resFiles[mType]));
    nw4r::g3d::ResFile anmResFile(getOarcResFile("Kiesu_anime"));

    TRY_CREATE(mMdl.create3(*this, mdlResFile.ptr(), anmResFile.ptr(), mdlNames[mType], "fly", 0x133));

    mResFile = mdlResFile;

    nw4r::g3d::ResAnmTexPat anmTexPat(mResFile.GetResAnmTexPat("blink_1"));

    TRY_CREATE(mAnmTexPat.create(mMdl.getModel().getResMdl(), anmTexPat, &mAllocator, nullptr, 1));

    fn_80030980(mMdl.getModel(), 1, false);
    mMdl.getModel().setAnm(mAnmTexPat);

    return true;
}

int dAcEKs_c::actorCreate() {
    if (StoryflagManager::sInstance->getFlag(STORYFLAG_GRATITUDE_QUEST_FINISHED)) {
        if (dScGame_c::isCurrentStage("F000") || dScGame_c::isCurrentStage("D000")) {
            return FAILED;
        }
    }

    CREATE_ALLOCATOR(dAcEKs_c);

    mAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(20, 60);

    mStts.SetRank(2);

    mSph.Set(sSrcSph);
    mSph.SetStts(mStts);

    mAcceleration = 0.f;
    mMaxSpeed = -40.f;

    setStartingPosition(mPosition);
    field_0xBE0 = mPosition;
    field_0xBEC.set(0, 100, 0);
    field_0xC10.set(0, 0, 0);
    field_0xCE4 = 500.f;

    field_0xDB7 = 0;
    field_0xDA5 = 0;
    field_0xDA9 = 0;
    field_0xDAA = 0;
    field_0xDB6 = 0;
    field_0xDAC = 1;
    field_0xDBE = 45;
    field_0xDBB = 0;
    field_0xDC1 = 0;

    mSph.ClrAtSet();
    mHealth = 4;
    field_0xD84 = 0;

    mEmitter1.init(this);
    mEmitter2.init(this);

    field_0xDA7 = 0;
    field_0xDAD = 0;
    field_0xDAE = 0;
    field_0xDC8 = 0;

    field_0xD04 = 0.f;
    field_0xD00 = 0.f;

    field_0xD88 = mRotation.y;
    field_0xD8A = mRotation.y - 0x8000;

    field_0xDBF = 0;
    field_0xD60 = 0;
    field_0xD62 = 10 + cM::rndInt(5);
    field_0xDB1 = 0;
    field_0xDCF = (mRotation.z >> 12) & 1;
    field_0xD9A = 0;
    if (field_0xDCF == 0) {
        field_0xDD0 = 0;
        mTimeArea.setField0x08(1);
        mTimeArea.setField0x00(1);

        if (dScGame_c::isCurrentStage("D301")) {
            setActorProperty(AC_PROP_0x400);
        }
    }
    field_0xC3C = getFromParams(6, 0xF) * 100.f;
    field_0xC40 = getFromParams(10, 0xF) * 100.f;
    field_0xC44 = ((mRotation.z >> 0) & 0xF) * 100.f;
    field_0xC48 = ((mRotation.z >> 4) & 0xF) * 100.f;

    if (!mTgRef.isLinked() || ((mRotation.z >> 8) & 0xF) < 3) {
        field_0xDA6 = 0;
    }

    mAngle.z = mRotation.z = 0;

    lbl_155_bss_388 = 0;
    lbl_155_bss_389 = 0;

    field_0xDB0 = mRail.init(getFromParams(22, 0xFF), mRoomID, 1, 0, false, 0.f, 0.f, 0.01f);
    if (field_0xDB0 != 0) {
        mRail.setSpeed(15.f);
    }
    if (field_0xDB0 != 0) {
        mStartingState = 2;
    } else {
        mStartingState = getFromParams(3, 0x7);
    }

    mVec3_c bboxMin(-250, -800, -250);
    mVec3_c bboxMax(250, 250, 250);
    mBoundingBox.Set(bboxMin, bboxMax);

    field_0xD9C = cM::rndF(0x10000);
    field_0xD9E = 0;
    mScale.set(1.2, 1.2, 1.2);

    mMdl.getModel().setScale(mScale);
    mWaterEmitter.init(this, 30, 1, 0);
    if (mType != EKS_NORMAL) {
        field_0xDA4 = 1;
        nw4r::g3d::ResNode node = mMdl.getModel().getResMdl().GetResNode("center");
        mCenterNode = node.GetID();
    } else {
        field_0xDA4 = 0;
    }

    if (mType == EKS_FIRE) {
        mSph.SetAtModifier(AT_MOD_FIRE);
        mSph.SetCo_0x8000();
        mTargetFiTextID = 0xE;
    } else if (mType == EKS_ELECTRIC) {
        mSph.SetAtModifier(AT_MOD_ELECTRIC);
        field_0xDA9 = 1;
        mTargetFiTextID = 0xD;
    } else if (mType == EKS_CURSED) {
        mSph.SetAtModifier(AT_MOD_CURSED);
        mTargetFiTextID = 0xC;
    }

    if (mType == EKS_FIRE) {
        mLightInfluence.mPos = mPosition;
        mLightInfluence.mClr.Set(0xFF, 0xA0, 0x50, 0xFF);
        mLightInfluence.SetScale(400);
        field_0xD20 = 400.f;
        field_0xD24 = 1.f;
        field_0xD28 = 40.f;
        dLightEnv_c::GetPInstance()->plight_set(&mLightInfluence);
    } else if (mType == EKS_ELECTRIC) {
        mLightInfluence.mPos = mPosition;
        mLightInfluence.mClr.Set(0x64, 0xFF, 0xFF, 0xFF);
        mLightInfluence.SetScale(300);
        field_0xD20 = 400.f;
        field_0xD24 = 1.f;
        field_0xD28 = 40.f;
        dLightEnv_c::GetPInstance()->plight_set(&mLightInfluence);
    }
    setStartingState();
    mAcch.CrrPos(*dBgS::GetInstance());

    return SUCCEEDED;
}

int dAcEKs_c::actorPostCreate() {
    if (field_0xDCF == 0) {
        if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(mRoomID, mPosition)) {
            mTimeArea.setField0x00(0.f);
        }
        mMdl.getModel().setScale(mScale * mTimeArea.getDistMaybe());
    }

    if (isState(StateID_Wait)) {
        mPosition.y = getLineCrossYRange(mPosition, 200);
        setStartingPosition(mPosition);
    }

    mPnts[0].set(mStartingPos);

    if (isState(StateID_Wait)) {
        mPnts[0].y -= 20.f;
    }

    updateMatrix();

    return SUCCEEDED;
}

int dAcEKs_c::doDelete() {
    if (mType == EKS_FIRE || mType == EKS_ELECTRIC) {
        dLightEnv_c::GetPInstance()->plight_cut(&mLightInfluence);
    }
    return SUCCEEDED;
}

int dAcEKs_c::actorExecute() {
    s32 _weird_zero = 0;

    mTimer++;

    if (field_0xDCF == 0) {
        int checkedTime = mTimeArea.check(mRoomID, mPosition, 0, 30, 0.2);
        if (checkedTime != 0) {
            if (checkedTime > 0) {
                startSound(SE_TIMESLIP_TIMESLIP);
                field_0xD9A = 15;
            } else {
                startSound(SE_TIMESLIP_TIMESLIP_REV);
                mSph.OnCoSet();
                mSph.OnTgSet();
            }

            static mVec3_c effScale(1, 1, 1);
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_464_, mPosition, nullptr, &effScale, nullptr, nullptr, 0, 0
            );
            fn_80030700();
        }

        if (mTimeArea.isNearZero()) {
            mSph.ClrCoSet();
            mSph.ClrTgSet();
            mSph.ClrAtSet();

            fn_800306D0();
            if (mType == EKS_FIRE || mType == EKS_ELECTRIC) {
                field_0xD20 = 0.f;
                mLightInfluence.SetScale(0.f);
            }

            return SUCCEEDED;
        }

        if (field_0xD9A > 0) {
            field_0xD9A--;
        }
    }
    if (mHealth != 0) {
        fn_155_A60();
    }

    mStateMgr.executeState();
    field_0xD9C++;

    if (isState(StateID_Chase) || isState(StateID_ChaseAttack) || isState(StateID_Fighting) ||
        isState(StateID_AttackReady) || isState(StateID_Attack) || isState(StateID_WindBlow)) {
        fn_155_3F50();
    }

    if (mType == EKS_FIRE) {
        if (field_0xDA9 != 0 || isState(StateID_Wait)) {
            field_0xD20 = 0.f;
            mLightInfluence.SetScale(0.f);
        } else {
            if (field_0xD60 > 0) {
                field_0xD20 = 0.f;
            } else {
                field_0xD20 = 400.f;
            }
        }

        mLightInfluence.mClr.Set(0xFF, 0xA0, 0x50, 0xFF);
        sLib::addCalcScaledDiff(&mLightInfluence.mScale, field_0xD20, field_0xD24, field_0xD28);
    } else if (mType == EKS_ELECTRIC) {
        if (field_0xDA9 != 0 || isState(StateID_Wait) || field_0xD60 > 0) {
            field_0xD20 = 0.f;
            mLightInfluence.SetScale(0.f);
        } else {
            if ((dScGame_c::getUpdateFrameCount() & 8) != 0) {
                mLightInfluence.SetScale(300.f);
            } else {
                mLightInfluence.SetScale(200.f);
            }
        }
        mLightInfluence.mClr.Set(0x64, 0xFF, 0xFF, 0xFF);
    }

    if (!isState(StateID_WindBlow)) {
        if (mSph.ChkTgHit() && mSph.ChkTgAtHitType(AT_TYPE_BELLOWS) && mSph.GetTgDamageFlags() == 0x8) {
            if (field_0xDA6 != 0) {
                mCurrentState = EKS_STATE_WindBlow;
                if (mTgRef.isLinked()) {
                    mTgRef.get()->setField_0x47A();
                }
            }
            mVelocity.set(0, 0, 0);
            mAngle.y = cLib::targetAngleY(dAcPy_c::GetLink()->mPositionCopy2, mPosition);
            mAngle.x = cLib::targetAngleX(dAcPy_c::GetLink()->mPositionCopy2, mPosition);

            // I Guess we funky
            s32 t = 7281;
            field_0xD66 = (cM::rndFX(0.25f + (0.01f * _weird_zero)) + (0.75f + (0.01f * _weird_zero))) * t;
            field_0xD68 = (cM::rndFX(0.25f + (0.01f * _weird_zero)) + (0.65f + (0.01f * _weird_zero))) * t;

            if (field_0xD66 < 0) {
                field_0xD64 = -910;
            } else {
                field_0xD64 = 910;
            }
            if (isState(StateID_Wait)) {
                mSpeed = 0.f;
                mVelocity.set(0, 0, 0);
                field_0xDAA = 1;
                mPosition.y -= 30.f;
                changeState(StateID_WindBlow);
            } else {
                mSpeed = 35.f;
                if (mAngle.x > 0xAAB) {
                    mVelocity.y = 15.f;
                } else if (mAngle.x < -0xAAB) {
                    mVelocity.y = -15.f;
                }
                changeState(StateID_WindBlow);
            }
        }
    }

    calcVelocity();

    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

    if (!(isState(StateID_Wait) || isState(StateID_WaitReady))) {
        mAcch.CrrPos(*dBgS::GetInstance());
        mWaterEmitter.execute(mAcch.GetWtrGroundH(), mAcch.GetGroundH());
    }

    if (field_0xDCF == 0) {
        mMdl.getModel().setScale(mScale * mTimeArea.getDistMaybe());
    } else {
        mMdl.getModel().setScale(mScale);
    }

    updateMatrix();
    mMdl.getModel().setLocalMtx(mWorldMtx);
    mMdl.getModel().calc(false);
    mSph.SetC(mPosition + field_0xC10);
    dCcS::GetInstance()->Set(&mSph);

    if (isNotWaitingNorDamage()) {
        mPositionCopy2.set(mPosition);
        mPositionCopy3.set(mPosition);
        if (!isState(StateID_Stun)) {
            mPositionCopy3.y += 60.f;
        } else {
            mPositionCopy3.y += 40.f;
        }
        fn_80030c20(0x2, 700, 50, -500, 500);
    }

    fn_80030400(mMdl.getModel(), 100, false, field_0xDBF);

    if (field_0xD30 != 0) {
        if (checkObjectProperty(OBJ_PROP_0x1)) {
            field_0xD31 = 0;
        } else {
            if ((mTimer % 12) == 0) {
                mVec3_c start = mPosition;
                mVec3_c end = dScGame_c::getCamera()->getPosition();
                end.y += 100.f;
                end += start;
                end *= 0.5f;

                if (dBgS_ObjLinChk::LineCross(&start, &end, nullptr)) {
                    field_0xD31 = 0;
                } else {
                    field_0xD31 = 1;
                }
            }
        }
    } else {
        field_0xD31 = 1;
        if (checkObjectProperty(OBJ_PROP_0x1)) {
            field_0xD31 = 0;
        }
    }

    if (field_0xDA9 == 0) {
        if (!isState(StateID_Wait) && field_0xDA4 != 0) {
            if (mType == EKS_FIRE) {
                if (field_0xD60 > 0) {
                    field_0xD60--;
                } else {
                    if (field_0xD31 != 0) {
                        holdSound(SE_EKs_FIRE_LV);
                        mMdl.getModel().getNodeWorldMtx(mCenterNode, mCenterWorldMtx);
                        mCenterWorldMtx.getTranslation(field_0xB98);
                        mEmitter1.holdEffect(
                            PARTICLE_RESOURCE_ID_MAPPING_402_, field_0xB98, nullptr, &mScale, nullptr, nullptr
                        );
                        mLightInfluence.mPos = field_0xB98;
                    }
                }
            } else if (mType == EKS_ELECTRIC) {
                if (field_0xD60 > 0) {
                    field_0xD60--;
                } else {
                    if (field_0xD31 != 0) {
                        fn_155_3E90();
                        holdSound(SE_EKs_ELEC_LV);
                        mMdl.getModel().getNodeWorldMtx(mCenterNode, mCenterWorldMtx);
                        mCenterWorldMtx.getTranslation(field_0xB98);
                        mEmitter1.holdEffect(
                            PARTICLE_RESOURCE_ID_MAPPING_401_, field_0xB98, nullptr, &mScale, nullptr, nullptr
                        );
                        mLightInfluence.mPos = field_0xB98;
                    }
                }
            } else if (mType == EKS_CURSED) {
                if (field_0xD60 > 0) {
                    field_0xD60--;
                } else {
                    if (field_0xD31 != 0) {
                        holdSound(SE_EKs_MA_LV);
                        mMdl.getModel().getNodeWorldMtx(mCenterNode, mCenterWorldMtx);
                        mCenterWorldMtx.getTranslation(field_0xB98);
                        mEmitter1.holdEffect(
                            PARTICLE_RESOURCE_ID_MAPPING_400_, field_0xB98, nullptr, &mScale, nullptr, nullptr
                        );
                        mEmitter1.setGlobalAlpha(dStageMgr_c::GetInstance()->getGlobalAlpha());
                    }
                }
            }
        }
    }

    return SUCCEEDED;
}
int dAcEKs_c::draw() {
    if (field_0xDCF == 0 && mTimeArea.isNearZero()) {
        return SUCCEEDED;
    }

    if (field_0xDA9 == 0 && !isState(StateID_Wait) && field_0xDA4 != 0) {
        if (mType == EKS_FIRE || mType == EKS_ELECTRIC) {
            mLightingInfo.mLightingCode = 0xF;
        }
    }

    drawModelType1(&mMdl.getModel());

    if (!isState(StateID_Wait)) {
        field_0xD18 = mPosition.y - mAcch.GetGroundH();
        // I hate this xD
        mQuat_c shadowRot(
            mVec3_c(0.f + dAcEKs_HIO_c::sInstance.getZero(), 5, 0.f + dAcEKs_HIO_c::sInstance.getZero()),
            50.f + dAcEKs_HIO_c::sInstance.getZero()
        );
        field_0xAE4.transS(mPosition);
        drawShadow(mShadow, nullptr, field_0xAE4, &shadowRot, -1, -1, -1, -1, -1, field_0xD18);
    }

    return SUCCEEDED;
}

void dAcEKs_c::initializeState_Wait() {
    // Float Ordering
    static const f32 __f[1] = {0.f};

    mAcchCir.SetWall(0, 5);
    setAnim("wait", 4, 1, 0);
    setBlink(EKS_TEXPAT_BLINK1);
    mAnmTexPat.setFrame(3, 0);
    mStts.SetRank(11);
    mSpeed = 0.f;
    field_0xC10.y = -35.f;
    field_0xD4A = 30;
    field_0xDA5 = 0;
    field_0xDAA = 0;
    field_0xDC4 = cM::rndInt(35);
    field_0xD90 = 3;
    field_0xDCE = 0;
    field_0xDB4 = 0;
    if (field_0xDB0 != 0) {
        field_0xDB1 = 1;
        field_0xD8E = field_0xDCA * 30;
    } else if ((s32)getFromParams(30, 3) == 1) {
        field_0xDB1 = 1;
        field_0xD8E = (cM::rndInt(20) + 10) * 30;
    }

    mRotation.y = field_0xD8A;
    mRotation.x = mAng(0);
    mAcch.ClrRoofHit();
    mAcch.SetField_0xD4(0);
    mAcch.SetGroundUpY(0);
    setBattleBgmRelated(0);

    if (field_0xDA6 != 0) {
        fn_155_3E30();
    } else {
        setActorProperty(AC_PROP_0x1);
    }

    if (mType == EKS_FIRE) {
        mSph.ClrCo_0x8000();
        field_0xD20 = 0.f;
        mLightInfluence.SetScale(0.f);
    } else if (mType == EKS_ELECTRIC) {
        fn_155_3EC0();
        field_0xD20 = 0.f;
        mLightInfluence.SetScale(0.f);
    }
    mPosition.y = getLineCrossYRange(mPosition, 200);
    setStartingPosition(mPosition);
}

// NONMATCHING
void dAcEKs_c::executeState_Wait() {
    mMdl.play();
    u16 flag = getFromParams(14, 0xFF);
    if (flag < 0xFF) {
        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, flag)) {
            if (field_0xDCE == 0) {
                bool b = false;
                mStartingState = EKS_STARTSTATE_Move;
                if (field_0xDA6 == 0) {
                    mMtx_c m;
                    m.YrotS(mRotation.y + 0x8000);
                    mVec3_c out, in(0, 0, 500);
                    m.multVecSR(in, out);

                    mVec3_c start(getStartingPos() + out);
                    mVec3_c out2 = out * 2.f;
                    mVec3_c end(getStartingPos() + out2);
                    if (dBgS_ObjLinChk::LineCross(&start, &end, nullptr) && dBgS_ObjLinChk::ChkWall()) {
                        b = true;
                    }
                    if (!b) {
                        out *= 2.f;
                    }
                    setStartingPosition(getStartingPos() + out);
                }
                field_0xCF8 = 20.f;
                field_0xDC8 = 150;
                field_0xD04 = 0.9f;
                field_0xD00 = 1.7f;
                field_0xDCE = 1;

            } else if (field_0xDCE == 1) {
                changeState(StateID_WakeUp);
                return;
            }
        } else if (field_0xDB4 != 0 && --field_0xD4A < 0xF) {
            mAnmTexPat.play();
            if (field_0xD4A < 0) {
                mStartingState = EKS_STARTSTATE_Move;
                field_0xDAA = 1;
                dTgKiesuTag_c *pTg = mTgRef.get();
                if (pTg) {
                    pTg->setField_0x47A();
                }
                changeState(StateID_WakeUp);
                return;
            }
        }

        if (fn_155_2EA0()) {
            playBlinkAnm();
            if (field_0xD62 > 0) {
                field_0xD62--;
            } else {
                field_0xD62 = cM::rndInt(5) + 10;
                f32 f = 800.f;
                if (field_0xD1C < f * f && !dBgS_ObjLinChk::LineCross(&mPosition, &field_0xC28, nullptr)) {
                    field_0xDB4 = 1;
                    field_0xD4A = cM::rndInt(15);
                }
            }
        } else if (field_0xDA5 == 0) {
            mAnmTexPat.setFrame(3, 0);
        }
        return;
    }

    if (field_0xDA6 != 0) {
        if (field_0xDA5 == 0) {
            if (field_0xDAD != 0) {
                if (field_0xDC4 != 0) {
                    field_0xDC4--;
                } else {
                    field_0xDA5 = 1;
                    field_0xD4C = cLib::targetAngleY(mPosition, targetPlayerOrScrapper(0)->mPosition) - 0x8000;
                }
            }
        } else {
            sLib::addCalcAngle(mRotation.y.ref(), field_0xD4C, 5, 0xAAB);

            if (--field_0xD4A < 15) {
                mAnmTexPat.play();
                if (field_0xD4A < 0) {
                    field_0xD4C = field_0xD4C + 0x8000;
                    changeState(StateID_WakeUp);
                    return;
                }
            }
        }
        if (field_0xDB4 != 0 || field_0xDAA != 0) {
            if (--field_0xD4A < 15) {
                mAnmTexPat.play();
                if (field_0xD4A < 0) {
                    field_0xDAA = 1;
                    if (mTgRef.isLinked()) {
                        mTgRef.get()->setField_0x47A();
                    }
                    changeState(StateID_WakeUp);
                    return;
                }
            }
        }
    } else {
        fn_155_3E30();
        if (field_0xDA5 == 0) {
            if (field_0xD90 > 0) {
                field_0xD90--;
            } else {
                if (fn_155_2D60(true, 0)) {
                    field_0xDA5 = 1;
                    field_0xD4C = cLib::targetAngleY(mPosition, targetPlayerOrScrapper(0)->mPosition) - 0x8000;
                } else {
                    field_0xD90 = 3;
                }
            }
        } else {
            sLib::addCalcAngle(mRotation.y.ref(), field_0xD4C, 5, 0xAAB);

            if (--field_0xD4A < 15) {
                mAnmTexPat.play();
                if (field_0xD4A < 0) {
                    field_0xD4C = field_0xD4C + 0x8000;
                    changeState(StateID_WakeUp);
                    return;
                }
            }
        }
        if (field_0xDAA != 0 || field_0xDB4 != 0) {
            if (--field_0xD4A < 15) {
                mAnmTexPat.play();
                if (field_0xD4A < 0) {
                    changeState(StateID_WakeUp);
                    return;
                }
            }
        }
        if (field_0xDB1 != 0) {
            if (field_0xD8E != 0) {
                field_0xD8E--;
            } else {
                field_0xDAA = 1;
                changeState(StateID_WakeUp);
                return;
            }
        }
    }

    if (dLightEnv_c::GetPInstance()->check_BPM8(&mPosition, nullptr) || field_0xDAE != 0) {
        field_0xDAA = 1;
        field_0xD4A = cM::rndInt(10);
    }
    if (fn_155_2EA0()) {
        playBlinkAnm();
        if (field_0xD62 > 0) {
            field_0xD62--;
        } else {
            field_0xD62 = cM::rndInt(5) + 10;
            f32 f = 800.f;
            if (field_0xD1C < f * f && !dBgS_ObjLinChk::LineCross(&mPosition, &field_0xC28, nullptr)) {
                field_0xDB4 = 1;
                field_0xD4A = cM::rndInt(15);
            }
        }
    } else {
        if (field_0xDA5 == 0) {
            mAnmTexPat.setFrame(3, 0);
        }
    }
}
void dAcEKs_c::finalizeState_Wait() {
    mStts.SetRank(2);
    field_0xC10.y = 0.f;
    setBattleBgmRelated(2);

    if (mType == EKS_FIRE) {
        mSph.SetCo_0x8000();
        field_0xD20 = 400.f;
        field_0xD24 = 1.f;
        field_0xD28 = 40.f;
    } else if (mType == EKS_ELECTRIC) {
        field_0xD20 = 400.f;
        field_0xD24 = 1.f;
        field_0xD28 = 40.f;
    }
}

void dAcEKs_c::initializeState_WakeUp() {
    setAnim("turn_st", 10, 1, 0);
    setBlink(EKS_TEXPAT_BLINK1);
    mBlinkTimer = cM::rndInt(75) + 15;
    field_0xCFC = mPosition.y - 70.f;
    field_0xD4E = 15;

    if (field_0xDAA) {
        field_0xDA5 = 0;
    } else {
        field_0xDA5 = 1;
    }
    mAcch.ClrRoofNone();
    mAcchCir.SetWall(-20, 5);
}
void dAcEKs_c::executeState_WakeUp() {
    mMdl.play();
    playBlinkAnm();
    sLib::addCalcScaledDiff(&mPosition.y, field_0xCFC, 0.1, 3);
    sLib::addCalcAngle(mRotation.y.ref(), field_0xD4C, 12, 0xAAB);
    sLib::addCalcAngle(mAngle.y.ref(), field_0xD4C, 12, 0xAAB);
    if (field_0xDA6 != 0) {
        if (--field_0xD4E < 0) {
            if (transitionToNextState()) {
                return;
            }
        }
    } else if (--field_0xD4E < 0) {
        if (field_0xDAA != 0) {
            if (fn_155_2B80(true, 0)) {
                changeState(StateID_Chase);
            } else if (field_0xDB0) {
                changeState(StateID_PathMove);
            } else {
                changeState(StateID_ReturnToWait);
            }
        } else if (field_0xDC8 != 0) {
            changeState(StateID_Move);
        } else {
            changeState(StateID_Chase);
        }
    }
}
void dAcEKs_c::finalizeState_WakeUp() {
    mAngle.y = mRotation.y;
    mAcch.ClrRoofNone();
    mAcchCir.SetWall(20, 60);
    mAcch.SetField_0xD4(55);
    mAcch.SetGroundUpY(30);
}

void dAcEKs_c::initializeState_ReturnToWait() {
    field_0xD30 = 1;
    setAnim("fly", 1.5, cM::rndFX(0.2) + 1.f, 0);
    if (mCurrentAnmTexPat == EKS_TEXPAT_BLINK2) {
        setBlink(EKS_TEXPAT_BLINK3);
    } else {
        setBlink(EKS_TEXPAT_BLINK1);
    }

    mBlinkTimer = cM::rndInt(75) + 15;
    field_0xD52 = cM::rndInt(20) + 20;
    field_0xD54 = cM::rndInt(30) + 30;

    if (field_0xDA6 == 0) {
        mTargetPos.x = getStartingPos().x + cM::rndFX(800);
        mTargetPos.y = getStartingPos().y + cM::rndFX(400);
        mTargetPos.z = getStartingPos().z + cM::rndFX(800);
    }

    setPitchYawToPoint(mTargetPos);
    mSpeed = 15.f;
    field_0xC70 = 0.f;
    field_0xDB7 = 0;
    field_0xDBA = 0;
    field_0xDC1 = 0;
    field_0xD90 = 3;
    field_0xDA5 = 0;
    mSqueakSoundTimer = cM::rndInt(20) + 20;
    unsetActorProperty(AC_PROP_0x1);
    field_0xD82 = 0;
    field_0xDA0 = 0;
}
void dAcEKs_c::executeState_ReturnToWait() {
    if ((s32)getFromParams(30, 0x3) == 2) {
        if (field_0xD82 > 1) {
            field_0xD82 = 0;
            if (!checkBeyondRadius(getStartingPos(), 490000.f)) {
                setActorProperty(AC_PROP_0x1);
            }
        } else {
            field_0xD82++;
        }
    }
    fn_155_3720(mSpeed, cM::rndFX(0.1) + 1.f);
    mMdl.play();
    playBlinkAnm();
    soundSqueak();
    if (field_0xD9A > 0) {
        mSpeed = 0.f;
    } else {
        sLib::addCalcScaledDiff(&mSpeed, 15, 0.1, 0.1);
    }
    fn_155_1470();
}
void dAcEKs_c::finalizeState_ReturnToWait() {
    mAngle.y = mRotation.y;
    field_0xDAD = 0;
    field_0xDAE = 0;
    field_0xD30 = 0;
}

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
void dAcEKs_c::finalizeState_PathMove() {
    field_0xD30 = 0;
}
