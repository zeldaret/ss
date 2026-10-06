#include "d/a/d_a_girahimu_sword_beam.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_cc.h"
#include "d/d_vec.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_sphere.h"
#include "m/m_vec.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(GH_SWORD_BEAM, dAcGirahimuSwordBeam_c, fProfile::GH_SWORD_BEAM, 0x116, 0, 0);

STATE_DEFINE(dAcGirahimuSwordBeam_c, BulletMove);
STATE_DEFINE(dAcGirahimuSwordBeam_c, Damage);

static dCcD_SrcCps sSrcCps0 = {
    {
     {AT_TYPE_DAMAGE, 0x1000D, {0, 0, 0}, 4, 0, 0, 0, CUT_DIR_NONE, 0},
     {~(AT_TYPE_COMMON0 | AT_TYPE_DAMAGE), 0x111, {0, 0, 0x407}, 0, CUT_DIR_NONE},
     {0},
     },
    {20.0f},
};
static dCcD_SrcCps sSrcCps1 = {
    {
     {AT_TYPE_DAMAGE, 0x1001D, {0, 0, 0}, 4, 0, 0, 0, CUT_DIR_NONE, 0},
     {(AT_TYPE_0x800000 | AT_TYPE_0x40 | AT_TYPE_SWORD), 0x111, {0, 0, 0x407}, 0, CUT_DIR_NONE},
     {0},
     },
    {50.0f},
};

struct dAcGirahimuSwordBeam_HIO_c {
    f32 f0;
    f32 f1;
    u16 s0;
    u16 s1;

    static const dAcGirahimuSwordBeam_HIO_c sInstance;
};
const dAcGirahimuSwordBeam_HIO_c dAcGirahimuSwordBeam_HIO_c::sInstance = {1.17f, 1.21f, 20, 15};

static mAng sSomeAngle = 0;

bool dAcGirahimuSwordBeam_c::createHeap() {
    return true;
}

int dAcGirahimuSwordBeam_c::create() {
    mAngle.y = getXZAngleToPlayer();
    field_0xB1C = getFromParams(0, 0xF);
    CREATE_ALLOCATOR(dAcGirahimuSwordBeam_c);
    mAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(50.0f, 70.0f);
    mStts.SetRank(3);
    mCollider.addCc(mCps1, sSrcCps1);
    mCollider.addCc(mCps0, sSrcCps0);
    mCollider.SetStts(mStts);

    mCps0.ClrTgSet();
    mCps0.ClrCoSet();

    mCps1.ClrAtSet();
    mCps1.ClrCoSet();

    mRotation.x = 0;
    mRotation.z = 0;
    if (field_0xB1C == 0) {
        mRotation.z -= 0x4000;
    }
    transformMatrix();
    mMaxSpeed = -60.0f;

    mEmitter0.init(this);
    mEmitter1.init(this);

    field_0xB16 = -1;
    field_0xB24 = 0;
    field_0xB25 = 0;

    field_0xB00 = 0;
    field_0xB02 = 0;
    field_0xB04 = 0;
    field_0xB26 = 0;
    field_0xB27 = 0;
    field_0xB18 = 0;
    if (field_0xB1C == 1) {
        field_0xB0C = 250.0f;
    } else {
        field_0xB0C = 150.0f;
    }
    field_0xB06 = 0;
    field_0xB08 = 0;
    field_0xB0A = 0;
    field_0xB10 = 200;

    startSound(SE_BGh3_SW_BEAM_ATTACK);
    changeState(StateID_BulletMove);

    return SUCCEEDED;
}

int dAcGirahimuSwordBeam_c::doDelete() {
    return SUCCEEDED;
}

int dAcGirahimuSwordBeam_c::actorExecute() {
    s32 _weird_zero = 0;
    holdSound(SE_BGh3_SW_BEAM_LV);

    if (field_0xB20 == 0 && field_0xB25 == 1) {
        field_0xB16 = 0xF;
        field_0xB25 = 0;
    }

    if (field_0xB18 == 0) {
        if (mCps1.ChkTgSet() == 0) {
            mCps1.OnTgSet();
        }
        fn_240_11F0();
    }

    executeState();

    mMtx_c m;
    m.YrotS(mAngle.y);
    m.XrotM(-mAngle.x);
    mVec3_c vel(0.0f, 0.0f, mSpeed);
    m.multVec(vel, mVelocity);

    mPosition += mVelocity;

    if (field_0xB27 == 0) {
        mRotation.x = -mAngle.x;
        mRotation.y = mAngle.y;
    } else {
        mRotation.x += field_0xB06;
        mRotation.z += field_0xB0A;
        mRotation.y += field_0xB08;
        field_0xB14 += sSomeAngle;
    }

    transformMatrix();

    if (isState(StateID_BulletMove)) {
        startMoveEmitters();

        if (field_0xB24 != 0) {
            mAcch.SetGroundUpY(_weird_zero + -80.0f);
        } else if (field_0xB1C == 0) {
            mAcch.SetGroundUpY(_weird_zero + 60.0f);
        } else if (field_0xB1C == 1) {
            mAcch.SetGroundUpY(_weird_zero + 40.0f);
        }

        mAcch.CrrPos(*dBgS::GetInstance());
    }

    mMtx_c m1;
    m1.ZXYrotS(mRotation);
    m1.ZrotM(0x4000);
    mVec3_c b1;
    m1.getBase(1, b1);

    f32 scale1 = 50.0f;
    b1 *= 110.0f;
    b1 *= mScale.x;
    scale1 *= mScale.x;
    mVec3_c p1 = mPosition;
    if (field_0xB20 == 0) {
        getXZCirclePoint(p1, mAngle.y, scale1);
    }
    mCps0.Set(p1 + b1, p1 - b1);

    f32 scale0 = 45.0f;
    scale0 *= mScale.x;
    b1 *= 0.85f;
    mVec3_c p0 = p1;
    getXZCirclePoint(p0, mAngle.y, scale0);
    mCps1.Set(p0 + b1, p0 - b1);

    ;
    mCollider.registerColliders();
    if (field_0xB20 != 1) {
        if (field_0xB16 > 0) {
            field_0xB16--;
            if (field_0xB16 < 10) {
                if (field_0xB26 != 0) {
                    mEmitter0.setFading(10);
                } else {
                    mEmitter1.setFading(10);
                }
                field_0xB10 -= 10.0f;
            }
        } else if (field_0xB16 == 0) {
            deleteRequest();
        }
    }

    if (field_0xB18 != 0) {
        field_0xB18--;
        if (mCps1.ChkTgSet()) {
            mCps1.ClrTgSet();
        }
    }

    return SUCCEEDED;
}

int dAcGirahimuSwordBeam_c::draw() {
    mSphere_c sph(mVec3_c(0.0f, 0.0f, 0.0f), field_0xB0C);
    drawShadow(mShadow, nullptr, mWorldMtx, &sph, -1, field_0xB10, -1, -1, -1, mPosition.y - mAcch.GetGroundH());
    return SUCCEEDED;
}

void dAcGirahimuSwordBeam_c::initializeState_BulletMove() {
    s32 _weird_zero = 0;
    mSpeed = 29.0f + _weird_zero;
    mMtx_c m;
    m.YrotS(mAngle.y);
    m.XrotM(-mAngle.x);
    mVec3_c vel(0.0f, 0.0f, mSpeed);
    m.multVec(vel, mVelocity);
}
void dAcGirahimuSwordBeam_c::executeState_BulletMove() {
    if ((mCps0.ChkAtHit() && !mCps0.ChkAtShieldReflect()) || mAcch.ChkGndHit() || mAcch.ChkWallHit(nullptr) ||
        mAcch.ChkWaterIn()) {
        mCps0.SetAtRpm(0);
        mCps0.ClrAtActorInfo();
        mCps0.SubtractAtEffCounter();

        if (field_0xB26 != 0) {
            mEmitter0.remove(true);
        } else {
            mEmitter1.remove(true);
        }

        startDisappearEmitter();

        startSound(SE_BGh3_SW_BEAM_DISAPPEAR);
        changeState(StateID_Damage);
    }
}
void dAcGirahimuSwordBeam_c::finalizeState_BulletMove() {}

void dAcGirahimuSwordBeam_c::initializeState_Damage() {
    mSpeed = 0.0f;
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mCps0.ClrAtSet();
}
void dAcGirahimuSwordBeam_c::executeState_Damage() {
    deleteRequest();
}
void dAcGirahimuSwordBeam_c::finalizeState_Damage() {}

void dAcGirahimuSwordBeam_c::swordBeamReturn(s32 param1) {
    if (field_0xB20 == 1) {
        fn_240_17C0(param1);
        mAngle.y = getXZAngleToPlayer();
        mCps0.SetAtGrp(0xC);
        mCps0.OffAtGrp(0x2);
        fn_240_16C0();
        field_0xB20 = 0;
        field_0xB18 = 10;
        field_0xB0C += 30.0f;
        mEmitter0.remove(true);
        startSound(SE_BGh3_SW_BEAM_RETURN);
        field_0xB26 = 0;
    }
}

void dAcGirahimuSwordBeam_c::startMoveEmitters() {
    if (field_0xB26 != 0) {
        if (field_0xB27 == 0) {
            // the beam trail
            mEmitter0.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_882_, mWorldMtx, nullptr, nullptr);
        } else {
            // deflect sparkles?
            mEmitter0.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_940_, mWorldMtx, nullptr, nullptr);
        }
    } else {
        // the beam trail
        mEmitter1.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_882_, mWorldMtx, nullptr, nullptr);
    }
}

void dAcGirahimuSwordBeam_c::startDisappearEmitter() {
    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_881_, mWorldMtx, nullptr, nullptr, 0, 0);
}

void dAcGirahimuSwordBeam_c::transformMatrix() {
    mWorldMtx.transS(mPosition);
    mWorldMtx.ZXYrotM(mRotation);
    if (field_0xB27) {
        mWorldMtx.YrotM(field_0xB14);
    }
    mWorldMtx.scaleM(mScale);
}

void dAcGirahimuSwordBeam_c::fn_240_11F0() {
    if (field_0xB20 == 0) {
        fn_240_1380();
    }
    if (mCps1.ChkTgHit()) {
        if (mCps1.GetTgAtHitType() == AT_TYPE_0x800000) {
            if (field_0xB26 != 0) {
                mEmitter0.remove(true);
            } else {
                mEmitter1.remove(true);
            }

            startDisappearEmitter();
            startSound(SE_BGh3_SW_BEAM_DISAPPEAR);
            changeState(StateID_Damage);
        } else if (mCps1.GetTgAtHitType()) {
            // Assuming this was a another branch
        }
    }

    if (mCps0.ChkAtHit() && mCps0.ChkAtShieldReflect()) {
        mAng3_c a0;
        a0.x = a0.z = 0;
        if (field_0xAF4.isLinked()) {
            a0.y = dAcPy_c::GetLink()->mRotation.y;
        }
        field_0xB24 = 1;

        mAng3_c a1 = a0;
        fn_240_1810(0, a1, mRotation.y + 0x4000);
    }
}

void dAcGirahimuSwordBeam_c::fn_240_1380() {
    if (!mCps1.ChkTgHit()) {
        return;
    }

    if (mCps1.GetTgAtHitType() != AT_TYPE_SWORD) {
        return;
    }

    mAng3_c a;
    a.x = 0;
    a.y = mCps1.GetTgAtHitDir().atan2sX_Z();
    a.z = 0;
    mAng playerAngle = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
    mAng hitAngle = cLib::targetAngleY(mPosition, const_cast<const dCcD_Linked<dCcD_Cps> &>(mCps1).GetTgHitPos());
    if (field_0xAF4.isLinked()) {
        cLib::targetAngleY(mPosition, field_0xAF4.get()->mPosition);
    }

    s32 cutDir = dAcPy_c::GetLink()->getSpecificAttackDirection();
    switch (cutDir) {
        case CUT_DIR_U: {
            if (field_0xB1C == 0) {
                s32 a = mRotation.y - playerAngle;
                if (mAng::abs(a) < 0xAAB) {
                    fn_240_1710();
                } else {
                    field_0xB24 = 0;
                }
            } else if (field_0xB1C == 1) {
                field_0xB24 = 0;
            }
        } break;
        case CUT_DIR_D: {
            if (field_0xB1C == 0) {
                s32 a = mRotation.y - playerAngle;
                if (mAng::abs(a) < 0xAAB) {
                    fn_240_1710();
                } else {
                    field_0xB24 = 1;
                }
            } else if (field_0xB1C == 1) {
                field_0xB24 = 1;
            }
        } break;
        case CUT_DIR_L: {
            if (field_0xB1C == 0) {
                field_0xB24 = 0;
            } else if (field_0xB1C == 1) {
                s32 a = (mRotation.y + 0x1555) - playerAngle;
                if (mAng::abs(a) < 0x1C72) {
                    fn_240_1710();
                } else {
                    field_0xB24 = 0;
                }
            }
        } break;
        case CUT_DIR_R: {
            if (field_0xB1C == 0) {
                field_0xB24 = 0;
            } else if (field_0xB1C == 1) {
                s32 a = (mRotation.y - 0x1555) - playerAngle;
                if (mAng::abs(a) < 0x1C72) {
                    fn_240_1710();
                } else {
                    field_0xB24 = 0;
                }
            }
        } break;
        case CUT_DIR_LU:   field_0xB24 = 0; break;
        case CUT_DIR_LD:   field_0xB24 = 1; break;
        case CUT_DIR_RU:   field_0xB24 = 0; break;
        case CUT_DIR_RD:   field_0xB24 = 1; break;
        case CUT_DIR_STAB: field_0xB24 = 0; break;
        default:           field_0xB24 = 0; break;
    }

    mAng3_c a1 = a;
    mAng a2 = hitAngle;
    fn_240_1810(cutDir, a1, a2);
}

void dAcGirahimuSwordBeam_c::fn_240_16C0() {
    mSpeed *= 1.21f;
    mScale *= 1.17f;
}

void dAcGirahimuSwordBeam_c::fn_240_1710() {
    if (field_0xB20 == 0) {
        if (field_0xAF4.isLinked()) {
            mAngle.y = cLib::targetAngleY(mPosition, field_0xAF4.get()->mPosition);
        }
        mCps0.SetAtGrp(2);
        mCps0.OffAtGrp(0xC);
        field_0xB18 = 10;
        field_0xB20 = 1;
        mEmitter1.remove(true);
        startSound(SE_BGh3_SW_BEAM_RETURN);
        field_0xB26 = 1;
    }
}

void dAcGirahimuSwordBeam_c::fn_240_17C0(s32 val) {
    if (val == (field_0xB1C & 0xFF)) {
        return;
    }

    field_0xB1C = val;
    mRotation.z = 0;
    if (val == 0) {
        mRotation.z -= 0x4000;
    }
}

inline mAng rndAngle(f32 amp) {
    return cM::rndFX(amp);
}
void dAcGirahimuSwordBeam_c::fn_240_1810(u32 cutDir, mAng3_c &angle, const mAng &ay) {
    s32 _weird_zero = 0;
    mAng3_c inAngle = angle;
    mAng3_c playerAngle;
    mAng angleToPlayer = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
    mAng relativeAngle = (mRotation.y - 0x4000) - angleToPlayer;
    mAng angDiff = relativeAngle - mRotation.y;

    if (field_0xB20 != 0) {
        return;
    }

    field_0xB18 = 10;

    // ???
    mAng3_c m;
    mAng &linkedAngleY = m.x;
    if (field_0xAF4.isLinked()) {
        linkedAngleY = (cLib::targetAngleY(mPosition, field_0xAF4.get()->mPosition));
    }

    switch (cutDir) {
        case CUT_DIR_RD: {
            angle.y = linkedAngleY + 182 * rndAngle(11.0f) + 0x1C72;
            angle.x += 0x700;
            mRotation.y = angle.y + 0x4E39;
            s32 ang = relativeAngle.abs();
            if (ang < 0x38E4) {
                field_0xB0A = -0x1777;
                field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                field_0xB06 = 0x5B0;
                field_0xB08 = 0x5B0;
            } else {
                field_0xB0A = 0x1777;
                field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                field_0xB06 = -0x2D8;
                field_0xB08 = 0x889;
            }
        } break;
        case CUT_DIR_RU: {
            angle.y = linkedAngleY + 182 * rndAngle(11.0f) + 0x1C72;
            angle.x -= 0x900;
            mRotation.y = angle.y + 0x4E39;
            s32 ang = relativeAngle.abs();
            if (ang < 0x38E4) {
                field_0xB0A = 0x1777;
                field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                field_0xB06 = 0x5B0;
            } else {
                field_0xB0A = -0x1777;
                field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                field_0xB06 = 0x5B0;
            }
        } break;
        case CUT_DIR_R: {
            angle.y = linkedAngleY + 182 * rndAngle(11.0f) + 0x1C72;
            angle.x -= 0x900;
            if (field_0xB1C == 0) {
                s32 ang = relativeAngle.abs();
                if (ang - 0x3C73 <= 0x71Au) {
                    field_0xB08 = -0x1777;
                    angle.x = 0;
                } else {
                    field_0xB0A = 0x1777;
                    field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                    field_0xB06 = 0x5B0;
                }
            } else {
                field_0xB06 = -0x1777;
                field_0xB06 *= (0.8f + (0.1f * _weird_zero));
                field_0xB0A = 0x222;
            }
        } break;
        case CUT_DIR_LD: {
            angle.y = linkedAngleY + 182 * rndAngle(11.0f) + -0x1C72;
            angle.x += 0x700;
            mRotation.y = angle.y + 0x1555;
            s32 ang = relativeAngle.abs();
            if (ang < 0x471C) {
                field_0xB0A = 0x1777;
                field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                field_0xB06 = -0x222;
                field_0xB08 = -0x5B0;
            } else {
                field_0xB0A = -0x1777;
                field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                field_0xB06 = -0x5B0;
                field_0xB08 = -0x5B0;
            }
        } break;
        case CUT_DIR_LU: {
            angle.y = linkedAngleY + 182 * rndAngle(11.0f) + -0x1C72;
            angle.x -= 0x900;
            mRotation.y = angle.y + 0x1555;
            s32 ang = relativeAngle.abs();
            if (ang < 0x471C) {
                field_0xB0A = -0x1777;
                field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                field_0xB06 = 0x5B0;
            } else {
                field_0xB0A = 0x1777;
                field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                field_0xB06 = 0x5B0;
            }
        } break;
        case CUT_DIR_L: {
            angle.y = linkedAngleY + 182 * rndAngle(11.0f) - 0x1C72;
            angle.x -= 0x900;
            if (field_0xB1C == 0) {
                s32 ang = relativeAngle.abs();
                if (ang - 0x3C73 <= 0x71Au) {
                    field_0xB08 = 0x1777;
                    angle.x = 0;
                } else {
                    field_0xB0A = 0x1777;
                    field_0xB0A *= (0.8f + (0.1f * _weird_zero));
                    field_0xB06 = 0x5B0;
                }
            } else {
                field_0xB06 = 0x1777;
                field_0xB06 *= (0.8f + (0.1f * _weird_zero));
                field_0xB0A = 0x222;
            }
        } break;
        case CUT_DIR_U: {
            angle.y = linkedAngleY + (182 * rndAngle(11.0f));
            angle.x -= 0x1500;
            if (field_0xB1C == 1) {
                s32 ang = relativeAngle.abs();
                if (ang < 0x31C7) {
                    field_0xB0A = 0x1777;
                } else if (ang < 0x3C72) {
                    mRotation.y = angle.y + 0x6AAB;
                    field_0xB06 = 0x1777;
                    field_0xB06 *= (0.8f + (0.1f * _weird_zero));
                    field_0xB0A = 0x222;
                } else if (ang < 0x438E) {
                    field_0xB06 = 0x1777;
                } else if (ang < 0x4E39) {
                    mRotation.y = angle.y + 0x1C72;
                    field_0xB06 = -0x1777;
                    field_0xB06 *= (0.8f + (0.1f * _weird_zero));
                    field_0xB0A = 0x222;
                } else {
                    field_0xB0A = -0x1777;
                }
            } else {
                field_0xB08 = 0x1777;
                field_0xB08 *= (0.8f + (0.1f * _weird_zero));
                field_0xB0A = 0x222;
            }
        } break;
        case CUT_DIR_D: {
            angle.y = linkedAngleY + 182 * rndAngle(11.0f);
            angle.x += 0x1500;
            if (field_0xB1C == 1) {
                s32 ang = relativeAngle.abs();
                if (ang < 0x31C7) {
                    field_0xB0A = -0x1777;
                } else if (ang < 0x38E4) {
                    mRotation.y = angle.y + 0x6AAB;
                    field_0xB06 = -0x1777;
                    field_0xB06 *= (0.8f + (0.1f * _weird_zero));
                    field_0xB0A = 0x222;
                } else if (ang < 0x471C) {
                    field_0xB06 = -0x1777;
                } else if (ang < 0x4E39) {
                    mRotation.y = angle.y + 0x1C72;
                    field_0xB06 = 0x1777;
                    field_0xB06 *= (0.8f + (0.1f * _weird_zero));
                    field_0xB0A = 0x222;
                } else {
                    field_0xB0A = 0x1777;
                }
            } else {
                field_0xB08 = 0x1777;
                field_0xB08 *= (0.8f + (0.1f * _weird_zero));
                field_0xB0A = 0x222;
            }
        } break;
        default: {
            s32 ang = relativeAngle.abs();
            if (ang - 0x38E5 <= 0xE36u) {
                angle.y = mAng((s32)angleToPlayer) + 182 * rndAngle(31.0f) - 0x8000;
            } else {
                angle.y = mAng((s32)angleToPlayer) + 182 * rndAngle(31.0f) - 0x8000;
            }
            angle.x += 0x1500;
            if (field_0xB1C == 1) {
                field_0xB06 = -0x1777;
                field_0xB06 *= (0.8f + (0.1f * _weird_zero));
                field_0xB0A = 0x222;
            } else {
                field_0xB08 = -0x1777;
                field_0xB08 *= (0.8f + (0.1f * _weird_zero));
                field_0xB0A = 0x222;
            }
        } break;
    }
    sSomeAngle = mAng(0x1777);

    mAngle.y = dAcPy_c::GetLink()->mRotation.y;
    s32 ang = angDiff.abs();
    if (ang < 0x31C7) {
        field_0xB02 = angle.y;
        field_0xB00 = angle.x;
    } else {
        setB00(angle.x, inAngle.y, angle.z);
    }

    mAngle.x = field_0xB00;
    mAngle.y = field_0xB02;
    mAngle.z = field_0xB04;

    field_0xB16 = 20;
    field_0xB20 = 2;
    mEmitter1.remove(true);
    field_0xB27 = 1;
    field_0xB26 = 1;
}
