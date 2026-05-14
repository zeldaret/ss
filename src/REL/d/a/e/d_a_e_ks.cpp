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
#include "d/d_light_env.h"
#include "d/d_pouch.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_ks.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
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
    f32 _0xB8; // 0.0099999998
    f32 _0xBC; // 100.0
    s16 _0xC0; // AABh
    s16 _0xC2; // 0h
    f32 _0xC4; // -0.5

    static const dAcEKs_HIO_c sInstance;
};
const dAcEKs_HIO_c dAcEKs_HIO_c::sInstance = {
    400.0, 0,    0xFF,  0xA0, 0x50,  0xFF,  0x64,  0xFF,  0xFF,   0xFF,  1.2,          1.5,   1.0,   1.0,    -35.0,
    30.0,  1.0,  0x71C, 0,    30.0,  15.0,  0.0,   100.0, 150.0,  250.0, 150.0,        120.0, 3.5,   3.5,    3.5,
    10.0,  20.0, 35.0,  7.0,  200.0, 10.0,  0.0,   0.5,   0x1555, 0x0,   500.0,        150.0, 150.0, 3000.0, 120.0,
    1.0,   0xF,  0xAAB, 1.0,  0x1E,  0xAAB, 300.0, 3.0,   0x3,    0x1,   0.0099999998, 100.0, 0xAAB, 0,      -0.5,
};

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

bool dAcEKs_c::lbl_155_bss_388;
bool dAcEKs_c::lbl_155_bss_389;

bool dAcEKs_c::restorePosRotFromCopy() {
    if (field_0xDCF == 0) {
        if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(mRoomID, mPosition) != 0) {
            field_0xAA8 = 0.f;
            mSph.ClrCoSet();
            mSph.ClrTgSet();
            mSph.ClrAtSet();
            fn_800306D0();
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
            changeState(StateID_Move);
            mCurrentState = mStartingState;
            mAcch.ClrRoofNone();
            mAcch.SetField_0xD4(55.f);
            mAcch.SetGroundUpY(30.f);
            mAcchCir.SetWall(20.f, 60.f);
        } break;
        case 1: {
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
        case 2: {
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
        case 0: {
            changeState(StateID_Move);
            mCurrentState = 0;
        } break;
        case 1: {
            changeState(StateID_ReturnToWait);
            mCurrentState = 5;
        } break;
        case 2: {
            changeState(StateID_PathMove);
            mCurrentState = 2;
        } break;
        default: {
            changeState(StateID_Move);
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
        if (mType == 3) {
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
        if (mCurrentAnmTexPat == 2) {
            setBlink(0);
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
                        mCurrentState = 1;
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
            if (mType == 2) {
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
                            mCurrentState = 9;

                            if (mType == 2) {
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
            case 0:  changeState(StateID_Move); break;
            case 1:  changeState(StateID_Wait); break;
            case 4:  changeState(StateID_WakeUp); break;
            case 5:  changeState(StateID_ReturnToWait); break;
            case 6:  changeState(StateID_Chase); break;
            case 7:  changeState(StateID_ChaseAttack); break;
            case 8:  changeState(StateID_Fighting); break;
            case 9:  changeState(StateID_AttackReady); break;
            case 10: changeState(StateID_Attack); break;
            case 11: changeState(StateID_Damage); break;
            case 12: changeState(StateID_Stun); break;
            case 13: changeState(StateID_WindBlow); break;
            case 2:  changeState(StateID_PathMove); break;
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
        case 0: pat = mResFile.GetResAnmTexPat("blink_1"); break;
        case 1: pat = mResFile.GetResAnmTexPat("blink_2"); break;
        case 2: pat = mResFile.GetResAnmTexPat("blink_3"); break;
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
