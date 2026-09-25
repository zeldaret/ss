
#include "d/a/e/d_a_e_skytail.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_bird.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_cc.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(E_SKYTAIL, dAcESkytail_c, fProfile::E_SKYTAIL, 0xD7, 0, 2);

STATE_DEFINE(dAcESkytail_c, RandomMove);
STATE_DEFINE(dAcESkytail_c, Move);
STATE_DEFINE(dAcESkytail_c, ParallelMove);
STATE_DEFINE(dAcESkytail_c, FrontAttack);
STATE_DEFINE(dAcESkytail_c, Charge);
STATE_DEFINE(dAcESkytail_c, Dead);
STATE_DEFINE(dAcESkytail_c, Skytail);

static mVec3_c sArrVecs[5] = {
    mVec3_c(1500.0f, -300.0f, -500.0f),  //
    mVec3_c(1200.0f, 100.0f, -800.0f),   //
    mVec3_c(0.0f, 300.0f, 0.0f),         //
    mVec3_c(-1200.0f, 100.0f, -800.0f),  //
    mVec3_c(-1500.0f, -300.0f, -500.0f), //
};

/* clang-format off */
static dCcD_SrcSph sCcSrcHead = {
    {{AT_TYPE_DAMAGE,0x1D, {0, 0, 0}, 4, 0, 0, 0}, 
    {~(AT_TYPE_COMMON0 | AT_TYPE_PHYSICS), 0x303, {0, 0x18, 0x407}, 0, 0}, 
    {0xC5}},
    {180.0f},};
static dCcD_SrcSph sCcSrcSpine = {
    {{AT_TYPE_DAMAGE,0x1D, {0, 0, 0}, 4, 0, 0, 0}, 
    {~(AT_TYPE_COMMON0 | AT_TYPE_PHYSICS), 0x303, {0, 0x18, 0x407}, 0, 0}, 
    {0xC5}},
    {170.0f},};
static dCcD_SrcSph sCcSrcRear = {
    {{0, 0, {0, 0, 0}, 0, 0, 0, 0}, 
    {~(AT_TYPE_COMMON0 | AT_TYPE_PHYSICS), 0x303, {0, 0, 0x407}, 0, 0}, 
    {0xC5}},
    {200.0f},};
/* clang-format on */

class dAcESkytail_HIO_c {
public:
    f32 field_0x00;
    s32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2C;
    f32 field_0x30;
    f32 field_0x34;
    u16 field_0x38;
    u16 field_0x3A;
    f32 field_0x3C;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4C;
    u16 field_0x50;
    u16 field_0x52;
    u16 field_0x54;
    f32 field_0x58;
    f32 field_0x5C;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6C;
    u16 field_0x70;
    u16 field_0x72;
    f32 field_0x74;
    f32 field_0x78;

    static const dAcESkytail_HIO_c sInstance;
};
/* clang-format off */
 const dAcESkytail_HIO_c dAcESkytail_HIO_c::sInstance = {
    0.8f,
    1,
    70.0f,
    1.0f,
    3.0f,
    10000.0f,
    6000.0f,
    800.0f,
    100.0f,
    1.0f,
    1.0f,
    3000.0f,
    1000.0f,
    800.0f,
    5,
    5,
    80.0f,
    1.0f,
    1.0f,
    4000.0f,
    800.0f,
    12,
    15,
    15,
    12.0f,
    200.0f,
    3000.0f,
    150.0f,
    1.0f,
    1.0f,
    30,
    30,
    500.0f,
    5000.0f
};
/* clang-format on */

void dAcESkytail_c::callbackBody_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result, nw4r::g3d::ResMdl) {
    static const s32 spines[] = {
        BODY_NODE_Spine01, BODY_NODE_Spine02, BODY_NODE_Spine03, BODY_NODE_Spine04,
        BODY_NODE_Spine05, BODY_NODE_Spine06, BODY_NODE_Spine07, BODY_NODE_Spine08,
        BODY_NODE_Spine09, BODY_NODE_Spine10, BODY_NODE_Spine11,
    };

    mMtx_c m, m3, m4, m5, m6;
    s32 idx = -1;

    for (int i = 0; i < (s32)ARRAY_LENGTH(spines); ++i) {
        if ((s32)nodeId == spines[i]) {
            idx = i + 1; // Head is idx 0 in the callbackBody, we want to alter the spines
            break;
        }
    }
    // Center (The Head) is idx 0 - so only adjust the spines
    if (idx >= 1) {
        m.transS(mPos[idx]);
        m.YrotM(mAng[idx].y);
        m.XrotM(mAng[idx].x);
        m.ZrotM(-mAng[idx].z);
        if (idx >= 10 /* Last 2 nodes */) {
            mMtx_c scale;
            scale.scaleS(mScaleRear, mScaleRear, mScaleRear);
            m.concat(scale);
        }
        m.YrotM(-0x4000);
        result->SetMtx(m);
    }

    if (nodeId == BODY_NODE_LMouthBig) {
        result->GetMtx(m3);
        m3.YrotM(mMouthBigAngleY);
        result->SetMtx(m3);
    } else if (nodeId == BODY_NODE_LMouthSmall) {
        result->GetMtx(m4);
        m4.YrotM(mMouthSmallAngleY);
        result->SetMtx(m4);
    } else if (nodeId == BODY_NODE_RMouthBig) {
        result->GetMtx(m5);
        m5.YrotM(-mMouthBigAngleY);
        result->SetMtx(m5);
    } else if (nodeId == BODY_NODE_RMouthSmall) {
        result->GetMtx(m6);
        m6.YrotM(-mMouthSmallAngleY);
        result->SetMtx(m6);
    }
}
void dAcESkytail_c::callbackAntenna_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result, nw4r::g3d::ResMdl) {
    mMtx_c m;
    m.transS(mPos[nodeId]);
    m.YrotM(mAng[nodeId].y);
    m.XrotM(mAng[nodeId].x);
    m.YrotM(-0x4000);
    result->SetMtx(m);
}

void dAcESkytail_c::checkHit() {
    if (sLib::calcTimer(&mHitCooldown) == 0) {
        u32 col = someEnemyDamageCollisionStuffMaybe(mCollider, nullptr);

        cCcD_Obj *pCcObj = mCollider.findTgHit();
        if (pCcObj != nullptr) {
            if (pCcObj->GetTgAtHitType() == AT_TYPE_KOLOKTOS_SWORD) {
                dJEffManager_c::spawnHitMarkEffect(7, *pCcObj, nullptr, true);
                if (pCcObj == &mCc[COLLIDER_Rear]) {
                    startSound(SE_ESktl_DOWN);
                    mDeadSubstate = 0;
                } else {
                    mDeadSubstate = 1;
                }

                mCollider.ClrTg();
                mHitCooldown = 500;
                mHealth = 0;
                changeState(StateID_Dead);
            } else if (pCcObj == &mCc[COLLIDER_Rear]) {
                switch (col) {
                    case 0:
                    case 6:
                    case 7: dJEffManager_c::spawnHitMarkEffect(3, mCc[COLLIDER_Rear], nullptr, true); break;
                    default:
                        dJEffManager_c::spawnHitMarkEffect(7, mCc[COLLIDER_Rear], nullptr, true);
                        startSound(SE_ESktl_DOWN);
                        mDeadSubstate = 0;
                        mCollider.ClrTg();
                        mHitCooldown = 500;
                        mHealth = 0;
                        changeState(StateID_Dead);
                        break;
                }
            } else if (pCcObj == &mCc[COLLIDER_Head] || pCcObj == &mCc[COLLIDER_Spine4] ||
                       pCcObj == &mCc[COLLIDER_Spine6] || pCcObj == &mCc[COLLIDER_Spine8]) {
                dJEffManager_c::spawnHitMarkEffect(3, *pCcObj, nullptr, true);
                mCollider.ClrTg();
                mHitCooldown = 500;
            }
        }
    }
}

bool dAcESkytail_c::createHeap() {
    nw4r::g3d::ResFile res(getOarcResFile("Skytail"));

    TRY_CREATE(mBodyMdl.create3(*this, res.ptr(), "Skytail", "Fly", 0x130));

    mBodyMdl.getModel().setCallback(&mBodyTransform);
    for (int i = 0; i < (s32)ARRAY_LENGTH(mAntennaMdl); ++i) {
        TRY_CREATE(mAntennaMdl[i].create3(*this, res.ptr(), "SkytailAntenna", "SkytailAntenna", 0x130));
        mAntennaMdl[i].getModel().setCallback(&mAntennaTransform[i]);
    }
    return true;
}

int dAcESkytail_c::create() {
    CREATE_ALLOCATOR(dAcESkytail_c);

    mAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(20.0f, 100.0f);
    mStts.SetRank(11);

    mCollider.addCc(mCc[COLLIDER_Rear], sCcSrcRear);
    mCollider.addCc(mCc[COLLIDER_Head], sCcSrcHead);
    for (int i = 1; i < (s32)ARRAY_LENGTH(mCc) - 1; ++i) {
        mCollider.addCc(mCc[i], sCcSrcSpine);
    }
    mCollider.SetStts(mStts);
    for (int i = 0; i < (s32)ARRAY_LENGTH(mCc); ++i) {
        mCc[i].SetC(mPosition);
    }
    mAcceleration = -0.0f;
    mMaxSpeed = -80.0f;
    mHealth = 4;
    mSpeed = 70.0f;
    field_0x13B6 = 0;
    mbNearSkytail = false;
    mDeadSubstate = 2;
    setStartingPosition(mPosition);
    mBirthPos.set(mPosition);

    mCc[COLLIDER_Head].ClrAtSet();
    mCollider.ClrAt();

    mBodyTransform.mScaleRear = 1.0f;
    mLiveTimer = cM::rndF(65536.0f);
    mBodyScale = 1.0f;
    field_0x1382 = cM::rndF(65536.0f);

    mMouthBigPhase = cM::rndF(65536.0f);
    mMouthSmallPhase = mMouthBigPhase;
    mMouthSmallAmplitudeAdd = 0;
    mMouthBigAmplitudeAdd = 0;

    mBodyMdl.getModel().setScale(mBodyScale, mBodyScale, mBodyScale);

    field_0x13B7 = false;
    mFrontAttackSubstateTimer = 0;

    field_0x1338 = 0.0f;
    field_0x1334 = 0.0f;
    field_0x1330 = 0.0f;
    field_0x133C = 0.0f;

    field_0x1370 = 0;
    field_0x136C = 0;
    field_0x136A = 0;
    field_0x136E = 0;

    mParallelMoveRateShiftTarget = 0.0f;
    mParallelMoveRateShift = 0.0f;

    mScaleRearPhase = 0;

    mWorldMtx.transS(mPosition);
    mWorldMtx.YrotM(mRotation.y);
    mWorldMtx.XrotM(mRotation.x);
    mWorldMtx.ZrotM(mRotation.z);
    mBodyMdl.getModel().setLocalMtx(mWorldMtx);

    wiggleBody();
    mBodyMdl.getModel().calc(true);

    for (int i = 0; i < (s32)ARRAY_LENGTH(mAntennaMdl); ++i) {
        wiggleAntenna(i);
        mAntennaMdl[i].getModel().calc(true);
    }

    changeState(StateID_RandomMove);

    mBodyMdl.setAnm("Fly", m3d::PLAY_MODE_4);

    mBoundingBox.Set(mVec3_c(-2000.0f, -2000.0f, -2000.0f), mVec3_c(2000.0f, 2000.0f, 2000.0f));

    return SUCCEEDED;
}

int dAcESkytail_c::doDelete() {
    return SUCCEEDED;
}

int dAcESkytail_c::actorExecute() {
    mLiveTimer++;

    for (int i = 0; i < 3; ++i) {
        if (field_0x13A4[i] != 0) {
            field_0x13A4[i]--;
        }
    }

    if (field_0x13A4[0] == 0) {
        mbNearSkytail = false;
    }

    setStartingPosition(mPosition);

    dBird_c *pBird = dBird_c::getInstance();
    mBirdPosition.set(pBird->getPosition());
    mDistanceToBird = mPosition.distance(mBirdPosition);
    field_0x13B7 = pBird->getRef_0xEB8().isLinked();
    mBirdRotation = pBird->getAngle();

    if (!field_0x13B7 || field_0x13B6 == 0) {
        if (!isState(StateID_RandomMove) && !isState(StateID_Dead)) {
            changeState(StateID_RandomMove);
        }
    }
    if (mHitCooldown > 0) {
        mCollider.ClrTg();
        if (!pBird->fn_129_CFB0() && !isState(StateID_Dead)) {
            mHitCooldown = 0;
            mCollider.TgSet();
        }
    } else {
        if (!isState(StateID_Dead)) {
            mCollider.TgSet();
        }
    }

    executeState();

    // Pulse the Rear Bulb
    if (!isState(StateID_Dead)) {
        mBodyTransform.mScaleRear = 1.3f + mAng(mScaleRearPhase).sin() * 0.2f;
        f32 rate = mBodyMdl.getAnm().getRate();
        if (rate > 2.0f) {
            rate = 2.0f;
        }
        mScaleRearPhase += mAng::fromDeg(180.0f / (20.001f - 5.0f * rate));
    }

    if (!isState(StateID_Dead)) {
        checkHit();
    }
    fn_172_45E0();
    mRotation.set(mAngle);
    clampRotationX();

    // Bug oops!
    if (!(mLiveTimer & 0xF) & (cM::rndF(1.0f) < 0.2f)) {
        field_0x1386 = 1000.0f + cM::rndF(1000.f);
        mMouthSmallPhaseIncrementTarget = mMouthBigPhaseIncrementTarget = field_0x1398 + 3000.0f + cM::rndF(3000.0f);
    }

    field_0x1382 += (f32)field_0x1384;
    sLib::addCalcAngle(&field_0x1384, field_0x1386, 2, 200);
    mMouthBigPhase += mMouthBigPhaseIncrement;
    sLib::addCalcAngle(&mMouthBigPhaseIncrement, mMouthBigPhaseIncrementTarget, 2, 200);
    mMouthSmallPhase += (f32)mMouthBigPhaseIncrement * 2.0f;
    sLib::addCalcAngle(&mMouthSmallPhaseIncrement, mMouthSmallPhaseIncrementTarget, 2, 200);

    if (!isState(StateID_ParallelMove) || mParallelMoveSubstate != 2) {
        sLib::addCalcScaled(&mParallelMoveRateShift, 0.1f, 0.1f);
        sLib::addCalcScaled(&field_0x133C, 0.1f, 0.1f);
        sLib::addCalcAngle(&field_0x1372, 0, 1, 1000);
    }

    mAng sin = mAng(field_0x1382).sin() * 3000.0f;
    if (isState(StateID_ParallelMove) && (mParallelMoveSubstate == 3 || mParallelMoveSubstate == 4)) {
        mVelocity = mAccel * mSpeed;
    } else {
        mMtx_c m;
        m.YrotS(mRotation.y);
        m.XrotM(sin + mRotation.x);
        mVec3_c v(0.0f, 0.0f, mSpeed);
        v.x += field_0x1348;
        v.y += field_0x134C;
        m.multVecSR(v);
        mVelocity.set(v);
        if (isState(StateID_Dead)) {
            mVelocity.y = -60.0f;
            mVelocity.y += v.y;
        }
    }
    mPosition += mVelocity;
    mPosition += mStts.GetCcMove() * 0.3f;
    mVec3_c v = mPosition;
    mAcch.CrrPos(*dBgS::GetInstance());
    mPosition.set(v);
    cBgS_PolyInfo info;
    if (!mAcch.fn_80340ca0(info)) {
        dBgS::GetInstance()->ChkMoveBG(info, true);
        const dAcObjBase_c *pObj = dBgS::GetInstance()->GetActorPointer(info);
        if (pObj != nullptr && pObj->mProfileName != fProfile::OBJ_CUMUL_CLOUD) {
            mVelocity.set(0.0f, 0.0f, 0.0f);
            static mVec3_c scale(2.0f, 2.0f, 2.0f);
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_320_, mBodyTransform.mPos[6], &mRotation, &scale, nullptr, nullptr, 0, 0
            );
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_320_, mBodyTransform.mPos[4], &mRotation, &scale, nullptr, nullptr, 0, 0
            );
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_320_, mBodyTransform.mPos[2], &mRotation, &scale, nullptr, nullptr, 0, 0
            );
            playDeathEffect0(mPosition, &scale, true);
            killWithFlagNoItemDrop();
        }
    }

    mBodyMdl.play();
    mWorldMtx.transS(mPosition);
    mWorldMtx.YrotM(mRotation.y);
    mWorldMtx.XrotM(mRotation.x);
    mWorldMtx.ZrotM(mRotation.z);
    mBodyMdl.getModel().setLocalMtx(mWorldMtx);
    wiggleBody();
    mBodyMdl.getModel().calc(false);
    for (int i = 0; i < (s32)ARRAY_LENGTH(mAntennaMdl); ++i) {
        wiggleAntenna(i);
        mAntennaMdl[i].getModel().calc(false);
    }

    cCcD_Obj *pCcObj = mCollider.findCoHit();
    if (pCcObj != nullptr && pCcObj == &mCc[COLLIDER_Head]) {
        dAcObjBase_c *pObj = pCcObj->GetCoActor();
        if (pObj->mProfileName == fProfile::E_SKYTAIL) {
            mbNearSkytail = true;
            mMtx_c m;
            mVec3_c v = pObj->mPosition - mPosition;
            m.YrotS(v.atan2sX_Z());
            mVec3_c in(0.0f, 0.0f, 430.0f);
            m.multVecSR(in);
            mCloseSkytailAdjustment.set(in);
            if (mPosition.y < pObj->mPosition.y) {
                mCloseSkytailAdjustment.y -= 350.f;
            } else {
                mCloseSkytailAdjustment.y += 350.f;
            }

            mCloseSkytailTarget.set(field_0x12A0 + mCloseSkytailAdjustment);
            mAngleTargetY = cLib::targetAngleY(mPosition, mCloseSkytailTarget);
            mAngleTargetX = -cLib::targetAngleX(mPosition, mCloseSkytailTarget);
            fn_172_45D0();
            field_0x13A4[0] += 5;
        } else if (pObj->mProfileName == fProfile::B_NUSI) {
            mVelocity.set(0.0f, 0.0f, 0.0f);
            static mVec3_c scale(2.0f, 2.0f, 2.0f);
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_320_, mBodyTransform.mPos[6], &mRotation, &scale, nullptr, nullptr, 0, 0
            );
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_320_, mBodyTransform.mPos[4], &mRotation, &scale, nullptr, nullptr, 0, 0
            );
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_320_, mBodyTransform.mPos[2], &mRotation, &scale, nullptr, nullptr, 0, 0
            );
            playDeathEffect0(mPosition, &scale, true);
            killWithFlagNoItemDrop();
        }
    }

    if (mbNearSkytail) {
        cLib::addCalcPos2(&field_0x12A0, mCloseSkytailTarget, 1.0f, 131.0f);
    }
    mCc[COLLIDER_Head].SetC(mPosition);
    mCc[COLLIDER_Spine4].SetC(mBodyTransform.mPos[4 /* BODY_NODE_Spine04 */]);
    mCc[COLLIDER_Spine6].SetC(mBodyTransform.mPos[6 /* BODY_NODE_Spine06 */]);
    mCc[COLLIDER_Spine8].SetC(mBodyTransform.mPos[8 /* BODY_NODE_Spine08 */]);
    mCc[COLLIDER_Rear].SetC(mBodyTransform.mPos[11 /* BODY_NODE_Spine11 */]);
    mCollider.registerColliders();
    return SUCCEEDED;
}

int dAcESkytail_c::draw() {
    drawModelType1(&mBodyMdl.getModel());
    for (int i = 0; i < (s32)ARRAY_LENGTH(mAntennaMdl); ++i) {
        drawModelType1(&mAntennaMdl[i].getModel());
    }
    return SUCCEEDED;
}

void dAcESkytail_c::initializeState_RandomMove() {
    field_0x12A0.set(
        mBirthPos.x + cM::rndFX(2000.0f), //
        mBirthPos.y + cM::rndFX(2000.0f), //
        mBirthPos.z + cM::rndFX(2000.0f)  //
    );
    setActorProperty(AC_PROP_0x1);
}
void dAcESkytail_c::executeState_RandomMove() {
    if (field_0x13A4[0] == 0) {
        field_0x12A0.x = mBirthPos.x + cM::rndFX(2000.0f);
        field_0x12A0.y = mBirthPos.y + +2000.0f + cM::rndFX(1000.0f);
        field_0x12A0.z = mBirthPos.z + cM::rndFX(2000.0f);

        mAngleStep = 0;
        field_0x137E = cM::rndF(1000.0f) + 1000.0f;
        mAngleTargetY = cLib::targetAngleY(mPosition, field_0x12A0);
        mAngleTargetX = -cLib::targetAngleX(mPosition, field_0x12A0);
        fn_172_45D0();
        mRandomMoveSpeedMultiplier = cM::rndF(0.2f) + 1.0f;
        field_0x13A4[0] = cM::rndF(60.0f) + 30.0f;
    }
    s16 zTarget = mAngle.y;
    sLib::addCalcAngle(mAngle.y.ref(), mAngleTargetY, 8, mAngleStep);
    sLib::addCalcAngle(mAngle.x.ref(), mAngleTargetX, 8, mAngleStep);
    zTarget -= mAngle.y;
    sLib::addCalcAngle(mAngle.z.ref(), zTarget * 8, 2, 3000);
    sLib::addCalcAngle(&mAngleStep, field_0x137E, 1, 100);
    sLib::addCalcScaledDiff(&mSpeed, mRandomMoveSpeedMultiplier * 50.0f, 0.1f, 1.0f);
    mBodyMdl.setRate(mSpeed / 50.0f);
    if (field_0x13B6 != 0 && field_0x13B7 != 0) {
        changeState(StateID_Move);
    }
}
void dAcESkytail_c::finalizeState_RandomMove() {
    unsetActorProperty(AC_PROP_0x1);
}

void dAcESkytail_c::initializeState_Move() {
    mParallelMoveSubstate = 0;
}

void dAcESkytail_c::executeState_Move() {
    mVec3_c v0;
    f32 distf0 = 6000.0f;
    f32 f = 12.0f;
    dBird_c::getInstance()->fn_129_C4A0(v0, f);
    f32 distXZ = v0.absXZTo(mPosition);
    switch (mParallelMoveSubstate) {
        case 0: {
            if (field_0x13A4[0] == 0) {
                mScratchVec.set(0.0f, 0.0f, 10000.0f);
                field_0x12A0.set(0.0f, 0.0f, 0.0f);
                mScratchMtx.transS(mBirdPosition);
                mScratchMtx.ZXYrotM(mBirdRotation);
                mScratchMtx.multVec(mScratchVec, field_0x12A0);
                field_0x137E = 500.0f + cM::rndF(1000.0f);
                mAngleTargetY = cLib::targetAngleY(mPosition, field_0x12A0);
                mAngleTargetX = -cLib::targetAngleX(mPosition, field_0x12A0);
                fn_172_45D0();
                field_0x13A4[0] = 10.0f + cM::rndF(5.0f);
            }
            if (isWithinCameraView(0.0f, 0.0f)) {
                if (distXZ > distf0) {
                    mParallelMoveSubstate = 1;
                    field_0x13A4[0] = 0;
                }
            } else {
                changeState(StateID_ParallelMove);
                return;
            }
        } break;
        case 1: {
            if (field_0x13A4[0] == 0) {
                field_0x137E = 1000.0f + cM::rndF(500.0f);
                mAngleTargetY = cLib::targetAngleY(mPosition, v0);
                mAngleTargetX = -cLib::targetAngleX(mPosition, v0);
                fn_172_45D0();
            }
            if (isWithinCameraView(0.0f, 0.0f)) {
                if (distXZ < 4000.0f) {
                    mVec3_c v1 = mPosition - v0;
                    mFrontAttackSubstateTimer = 30 + (s32)(v1.mag() / mSpeed);
                    changeState(StateID_FrontAttack);
                    return;
                }
            } else {
                changeState(StateID_ParallelMove);
                return;
            }
        } break;
    }

    s16 zTarget = mAngle.y;
    sLib::addCalcAngle(mAngle.y.ref(), mAngleTargetY, 8, mAngleStep);
    sLib::addCalcAngle(mAngle.x.ref(), mAngleTargetX, 8, mAngleStep);
    zTarget -= mAngle.y;
    sLib::addCalcAngle(mAngle.z.ref(), zTarget * 8, 2, 3000);
    sLib::addCalcAngle(&mAngleStep, field_0x137E, 1, 100);
    sLib::addCalcScaledDiff(&mSpeed, 70.0f, 1.0f, 3.0f);
    mBodyMdl.setRate(mSpeed / 50.0f);
}
void dAcESkytail_c::finalizeState_Move() {}

void dAcESkytail_c::initializeState_ParallelMove() {
    mParallelMoveSubstate = 0;
    field_0x13AA = 90 + cM::rndInt(60);
    field_0x130C = 100.0f;
    field_0x139A = 5 + cM::rndInt(5);
    field_0x13B5 = 50 + cM::rndInt(20);
    mParallelMoveRateShiftTarget = 0.0f;
    mParallelMoveRateShift = 0.0f;
    field_0x1380 = 0;
    field_0x133C = 0.0f;
    field_0x139C = 0;
}

void dAcESkytail_c::executeState_ParallelMove() {
    mVec3_c v0;
    f32 distf0 = 1000.0f;
    f32 f = 12.0f;
    dBird_c::getInstance()->fn_129_C4A0(v0, f);
    f32 distXZ = v0.absXZTo(mPosition);
    switch (mParallelMoveSubstate) {
        case 0: {
            if (field_0x13A4[0] == 0) {
                mScratchVec.set(0.0f, 0.0f, 3000.0f);
                field_0x12A0.set(0.0f, 0.0f, 0.0f);
                mScratchMtx.transS(mBirdPosition);
                mScratchMtx.ZXYrotM(mBirdRotation);
                mScratchMtx.multVec(mScratchVec, field_0x12A0);
                field_0x137E = 500.0f + cM::rndF(1000.0f);
                mAngleTargetY = cLib::targetAngleY(mPosition, field_0x12A0);
                mAngleTargetX = -cLib::targetAngleX(mPosition, field_0x12A0);
                fn_172_45D0();
                field_0x13A4[0] = 10.0f + cM::rndF(5.0f);
            }
            if (isWithinCameraView(0.0f, 0.0f)) {
                if (distXZ > distf0) {
                    mParallelMoveSubstate = 1;
                    field_0x13A4[0] = 0;
                    field_0x139A = 5 + cM::rndInt(5);
                    field_0x139C = 0;
                    field_0x1360 = 1 + cM::rndInt(3);
                }
            }
        } break;
        case 1: {
            if (field_0x13AA != 0) {
                field_0x13AA--;
            } else {
                field_0x13AA = 90 + cM::rndInt(60);
                field_0x1360 = 1 + cM::rndInt(3);
            }

            mParallelMoveScratchVec.set(sArrVecs[field_0x1360]);
            mScratchMtx.transS(mBirdPosition);
            mScratchMtx.ZXYrotM(mBirdRotation);
            mScratchMtx.multVec(mParallelMoveScratchVec, mParallelMoveScratchVec);
            field_0x12A0.set(mParallelMoveScratchVec);
            mScratchVec.set(0.0f, 0.0f, 3000.0f);
            mScratchMtx.multVecSR(mScratchVec, mScratchVec);
            field_0x12A0 += mScratchVec;

            distXZ = mBirdPosition.distance(field_0x12A0);

            if (field_0x13A4[0] == 0) {
                field_0x137E = 500.0f + cM::rndF(1000.0f);
                mAngleTargetY = cLib::targetAngleY(mPosition, field_0x12A0);
                mAngleTargetX = -cLib::targetAngleX(mPosition, field_0x12A0);
                fn_172_45D0();
                field_0x13A4[0] = 25.0f + cM::rndF(10.0f);
                if (field_0x139A > 0) {
                    field_0x139A--;
                }
            }
            if (isWithinCameraView(0.0f, 0.0f)) {
                f32 dist = mPosition.distance(field_0x12A0);
                field_0x130C = (dBird_c::getInstance()->getField_0xFAC().mag() * (1.0f + dist / distXZ)) * 0.5f;

                if (field_0x130C <= 30.0f) {
                    field_0x130C = 30.0f;
                }
                field_0x139C = 0;
                if (field_0x139A <= 0) {
                    mParallelMoveSubstate = 2;
                    mParallelMoveRateShift = 0.0f;
                    mParallelMoveRateShiftTarget = 1.0f;
                    field_0x13B5 = 50 + cM::rndInt(20);
                    field_0x139A = 5 + cM::rndInt(5);
                }
            } else if (dBird_c::getInstance()->getField_0xFCC() > 0 && --field_0x139A < 0) {
                mParallelMoveSubstate = 2;
                mParallelMoveRateShift = 0.0f;
                mParallelMoveRateShiftTarget = 1.0f;
                field_0x13B5 = 50 + cM::rndInt(20);
                field_0x139A = 5 + cM::rndInt(5);
                field_0x139C = 0;
            } else if (++field_0x139C > 0) {
                mParallelMoveSubstate = 0;
                field_0x13A4[0] = 0;
                field_0x130C = 100.0f;
                field_0x139C = 0;
            }
        } break;
        case 2: {
            mParallelMoveScratchVec.set(sArrVecs[field_0x1360]);
            mScratchMtx.transS(mBirdPosition);
            mScratchMtx.ZXYrotM(mBirdRotation);
            mScratchMtx.multVec(mParallelMoveScratchVec, mParallelMoveScratchVec);
            field_0x12A0.set(mParallelMoveScratchVec);
            mScratchVec.set(0.0f, 0.0f, 3000.0f);
            mScratchMtx.multVecSR(mScratchVec, mScratchVec);
            field_0x12A0 += mScratchVec;

            distXZ = mBirdPosition.distance(field_0x12A0);

            if (field_0x13A4[0] == 0) {
                field_0x137E = 500.0f + cM::rndF(1000.0f);
                mAngleTargetY = cLib::targetAngleY(mPosition, field_0x12A0);
                mAngleTargetX = -cLib::targetAngleX(mPosition, field_0x12A0);
                fn_172_45D0();
                field_0x13A4[0] = 20.0f + cM::rndF(10.0f);
            }

            f32 dist = mPosition.distance(field_0x12A0);
            field_0x130C = (dBird_c::getInstance()->getField_0xFAC().mag() * (1.0f + dist / distXZ)) * 0.5f;
            if (field_0x130C <= 30.0f) {
                field_0x130C = 30.0f;
            }
            if (field_0x13B5 != 0) {
                field_0x13B5--;
            } else {
                field_0x13B4 = 60;
                mCollider.AtSet();
                startSound(SE_ESktl_V_ATTACK);
                mParallelMoveSubstate = 3;
                mAccel.set(v0 - mPosition);
                mAccel.normalizeRS();
                field_0x130C = 80.0f;
                if (dBird_c::getInstance()->getField_0xFAC().mag() > field_0x130C) {
                    field_0x130C = dBird_c::getInstance()->getField_0xFAC().mag();
                }
                mAngleTargetY = cLib::targetAngleY(mPosition, v0);
                mAngleTargetX = -cLib::targetAngleX(mPosition, v0);
                fn_172_45D0();
            }

            sLib::addCalcScaledDiff(&mParallelMoveRateShift, mParallelMoveRateShiftTarget, 0.1f, 0.1f);
            sLib::addCalcScaledDiff(&field_0x133C, 2.0f, 0.1f, 0.1f);
            sLib::addCalcAngle(&field_0x1372, 4000, 1, 1000);
        } break;
        case 3: {
            mVec3_c v2 = v0 - mPosition;
            v2.normalizeRS();
            cLib::chasePos(mAccel, v2, 0.01f);

            field_0x137E = 500.0f + cM::rndF(1000.0f);
            mAngleTargetY = cLib::targetAngleY(mPosition, mPosition + mAccel * mSpeed);
            mAngleTargetX = -cLib::targetAngleX(mPosition, mPosition + mAccel * mSpeed);
            fn_172_45D0();
            if (field_0x13B4 != 0) {
                field_0x13B4--;
            } else {
                if (isWithinCameraView(0.0f, 0.0f)) {
                    changeState(StateID_Move);
                } else {
                    changeState(StateID_ParallelMove);
                }
                return;
            }

            cCcD_Obj *pCcObj = mCollider.findAtHit();
            if (pCcObj != nullptr) {
                if (pCcObj->GetAtActor()->mProfileName == fProfile::BIRD) {
                    mCollider.ClrAt();
                    mParallelMoveSubstate = 4;
                } else if (pCcObj->GetAtActor()->isActorPlayer()) {
                    mCollider.ClrAt();
                    mParallelMoveSubstate = 4;
                }
            }
        } break;
        case 4: {
            if (field_0x13B4 != 0) {
                field_0x13B4--;
            } else {
                if (isWithinCameraView(0.0f, 0.0f)) {
                    changeState(StateID_Move);
                } else {
                    changeState(StateID_ParallelMove);
                }
                return;
            }
        } break;
    }

    s16 zTarget = mAngle.y;
    sLib::addCalcAngle(mAngle.y.ref(), mAngleTargetY, 8, mAngleStep);
    sLib::addCalcAngle(mAngle.x.ref(), mAngleTargetX, 8, mAngleStep);
    zTarget -= mAngle.y;
    sLib::addCalcAngle(mAngle.z.ref(), zTarget * 8, 2, 3000);
    sLib::addCalcAngle(&mAngleStep, field_0x137E, 1, 100);
    sLib::addCalcScaledDiff(&mSpeed, field_0x130C, 1.0f, 3.0f);
    f32 rate = mSpeed / 50.0f + mParallelMoveRateShift;
    if (rate > 5.0f) {
        mBodyMdl.setRate(5.0f);
    } else {
        mBodyMdl.setRate(rate);
    }
}
void dAcESkytail_c::finalizeState_ParallelMove() {
    field_0x133C = 0.0f;
    field_0x1372 = 0;
    mCollider.ClrAt();
}

void dAcESkytail_c::initializeState_FrontAttack() {
    mFrontAttackSubstate = 1;
    mMouthBigAmplitudeAdd = 2000;
    mMouthSmallAmplitudeAdd = 0;
    field_0x1398 = 4000;
    mCollider.AtSet();
    field_0x13A4[0] = 10.0f + cM::rndF(5.0f);
}
void dAcESkytail_c::executeState_FrontAttack() {
    f32 f = 12.0f;
    mVec3_c v_unused = mBirdPosition;
    mVec3_c v0;
    dBird_c::getInstance()->fn_129_C4A0(v0, f);
    f32 distXZ = v0.absXZTo(mPosition);
    distXZ = mPosition.distance(v0);
    switch (mFrontAttackSubstate) {
        case 1: {
            if (field_0x13A4[0] == 0) {
                field_0x137E = 1000.0f + cM::rndF(500.0f);
                mAngleTargetY = cLib::targetAngleY(mPosition, v0);
                mAngleTargetX = -cLib::targetAngleX(mPosition, v0);
                fn_172_45D0();
                field_0x13A4[0] = 10.0f + cM::rndF(5.0f);
            }
            if (isWithinCameraView(0.0f, 0.0f)) {
                if (distXZ < 2500.0f) {
                    mFrontAttackSubstate = 0;
                    mVec3_c v = mPosition - v0;
                    mFrontAttackSubstateTimer = 30 + (s32)(v.mag() / mSpeed);
                }
            } else {
                changeState(StateID_ParallelMove);
                return;
            }
        } break;
        case 0: {
            mFrontAttackSubstateTimer--;
            mAngleTargetY = cLib::targetAngleY(mPosition, v0);
            mAngleTargetX = -cLib::targetAngleX(mPosition, v0);
            fn_172_45D0();

            if (isWithinCameraView(0.0f, 0.0f)) {
                if (distXZ < 1300.0f) {
                    startSound(SE_ESktl_V_ATTACK);
                    mFrontAttackSubstate = 2;
                    mVec3_c v = mPosition - v0;
                    mFrontAttackSubstateTimer = 30 + (s32)(v.mag() / mSpeed);
                }
            } else if (!isWithinCameraView(0.0f, 0.0f) || mFrontAttackSubstateTimer <= 0) {
                changeState(StateID_ParallelMove);
                return;
            }
        } break;
        case 2: {
            mFrontAttackSubstateTimer--;
            if (isWithinCameraView(0.0f, 0.0f)) {
                if (distXZ < 800.0f) {
                    mFrontAttackSubstate = 4;
                    mVec3_c v = mPosition - v0;
                    mFrontAttackSubstateTimer = 120 + (s32)(v.mag() / mSpeed);
                }
            } else if (!isWithinCameraView(0.0f, 0.0f) || mFrontAttackSubstateTimer <= 0) {
                changeState(StateID_ParallelMove);
                return;
            }
        } break;
        case 4: {
            mFrontAttackSubstateTimer--;
            isWithinCameraView(0.0f, 0.0f);
            if (mFrontAttackSubstateTimer <= 0) {
                if (isWithinCameraView(0.0f, 0.0f)) {
                    changeState(StateID_Move);
                } else {
                    changeState(StateID_ParallelMove);
                }
                return;
            }
        } break;
    }
    s16 zTarget = mAngle.y;
    sLib::addCalcAngle(mAngle.y.ref(), mAngleTargetY, 8, mAngleStep);
    sLib::addCalcAngle(mAngle.x.ref(), mAngleTargetX, 8, mAngleStep);
    zTarget -= mAngle.y;
    sLib::addCalcAngle(mAngle.z.ref(), zTarget * 8, 2, 3000);
    sLib::addCalcAngle(&mAngleStep, field_0x137E, 1, 100);
    cCcD_Obj *pCcObj = mCollider.findAtHit();
    if (pCcObj != nullptr) {
        if (pCcObj->GetAtActor()->mProfileName == fProfile::BIRD) {
            mCollider.ClrAt();
        } else if (pCcObj->GetAtActor()->isActorPlayer()) {
            mCollider.ClrAt();
        }
    }
    sLib::addCalcScaledDiff(&mSpeed, 80.0f, 1.0f, 1.0f);
    mBodyMdl.setRate(mSpeed / 50.0f);
}
void dAcESkytail_c::finalizeState_FrontAttack() {
    mCollider.ClrAt();
    mMouthSmallAmplitudeAdd = 0;
    mMouthBigAmplitudeAdd = 0;
    field_0x1398 = 0;
}

void dAcESkytail_c::initializeState_Charge() {}
void dAcESkytail_c::executeState_Charge() {}
void dAcESkytail_c::finalizeState_Charge() {
    mCc[COLLIDER_Head].ClrAtSet();
}

void dAcESkytail_c::initializeState_Dead() {
    field_0x13AF = 0;
    mDeadStateTimer = 0;
    field_0x1334 = 8.0f + cM::rndFX(1.0f);
    field_0x1330 = 8.0f + cM::rndFX(1.0f);
    field_0x1338 = 7.0f + cM::rndFX(1.0f);
    field_0x136E = 6000 + cM::rndInt(1000);
    field_0x136A = 6000 + cM::rndInt(1000);
    field_0x136C = 7500 + cM::rndInt(500);
    field_0x1370 = 6000 + cM::rndInt(1000);
    mBodyMdl.setRate(mSpeed / 50.0f);
    mCollider.ClrTg();
    mCollider.ClrCo();
    mCollider.ClrAt();
}

void dAcESkytail_c::executeState_Dead() {
    mDeadStateTimer++;
    mHitCooldown = 500;
    mScratchVec.set(0.0f, 0.0f, 5000.0f);
    field_0x12A0.set(0.0f, 0.0f, 0.0f);

    mScratchMtx.transS(mBirdPosition);
    mScratchMtx.ZXYrotM(mBirdRotation);
    mScratchMtx.multVec(mScratchVec, field_0x12A0);

    field_0x137E = 500.0f + cM::rndF(1000.0f);
    mAngleTargetY = cLib::targetAngleY(mPosition, field_0x12A0);
    mAngleTargetX = -cLib::targetAngleX(mPosition, field_0x12A0);

    fn_172_45D0();

    sLib::addCalcAngle(mAngle.y.ref(), mAngleTargetY, 8, mAngleStep);
    sLib::addCalcAngle(mAngle.x.ref(), mAngleTargetX, 8, mAngleStep);
    mAngle.z += -0xAAA;

    field_0x1374 += -4000;
    field_0x1376 += -4000;

    field_0x1348 = mAng(field_0x1374).cos() * 20.0f;
    field_0x134C = mAng(field_0x1376).sin() * 20.0f;

    sLib::addCalcScaledDiff(&mSpeed, 150.0f, 1.0f, 5.0f);
    mBodyMdl.setRate(mSpeed / 50.0f);

    bool bVar = false;
    if (mDeadStateTimer > 15) {
        bVar = fn_80030570(mBodyMdl.getModel(), 1, 2.0f);
        for (int i = 0; i < (s32)ARRAY_LENGTH(mAntennaMdl); ++i) {
            fn_80030570(mAntennaMdl[i].getModel(), 0, 2.0f);
        }
    }

    static mVec3_c scale(2.0f, 2.0f, 2.0f);
    switch (mDeadSubstate) {
        case 0: {
            mBodyTransform.mScaleRear = 0.0f;
            if (mBodyTransform.mScaleRear == 0.0f) {
                mBodyMdl.getModel().setCullMode(3, GX_CULL_ALL, false);
                dJEffManager_c::spawnEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_460_, mBodyTransform.mPos[10], &mRotation, &scale, nullptr, nullptr, 0,
                    0
                );
                mDeadSubstate = 2;
            }
        } break;
        case 1: {
            if (mDeadStateTimer > 15) {
                mBodyMdl.getModel().setCullMode(3, GX_CULL_ALL, false);
                mDeadSubstate = 2;
            }

        } break;
    }

    if (bVar) {
        itemDroppingAndGivingRelated(&mBodyTransform.mPos[11], 4);
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_320_, mBodyTransform.mPos[6], &mRotation, &scale, nullptr, nullptr, 0, 0
        );
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_320_, mBodyTransform.mPos[4], &mRotation, &scale, nullptr, nullptr, 0, 0
        );
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_320_, mBodyTransform.mPos[2], &mRotation, &scale, nullptr, nullptr, 0, 0
        );
        playDeathEffect0(mPosition, &scale, false);
    }
}
void dAcESkytail_c::finalizeState_Dead() {}

void dAcESkytail_c::initializeState_Skytail() {}
void dAcESkytail_c::executeState_Skytail() {
    if (dBird_c::getInstance()->fn_129_CFB0()) {
        mSpeed = 50.0f;
        return;
    }

    mSpeed = 150.0f;
    mVec3_c v0(0.0f, -200.0f, 2100.0f);
    mScratchMtx.transS(mBirdPosition);
    mScratchMtx.ZXYrotM(mBirdRotation);
    mScratchMtx.multVec(v0, mPosition);
    mAngleTargetY = mBirdRotation.y;
    mAngleTargetX = mBirdRotation.x;
    fn_172_45D0();
    mAngle.y = mBirdRotation.y;
    mAngle.x = mBirdRotation.x;

    mBodyMdl.setRate(mSpeed / 50.0f);
    mBodyMdl.setRate(mSpeed / 50.0f);
}
void dAcESkytail_c::finalizeState_Skytail() {}

void dAcESkytail_c::wiggleBody() {
    mMtx_c m1, m0;
    mVec3_c v3, v0, v1, v2;
    mVec3_c *pos;
    mAng3_c *ang;

    mBodyTransform.mPos[0] = mPosition;
    mBodyTransform.mAng[0].y = mRotation.y;
    mBodyTransform.mAng[0].x = mRotation.x;

    pos = &mBodyTransform.mPos[1];
    ang = &mBodyTransform.mAng[1];

    v0.set(0.0f, 0.0f, 0.0f);

    m0.YrotS(mRotation.y);
    m0.XrotM(mRotation.x);

    f32 fVar2 = field_0x1334 + 1.3f + field_0x133C;
    f32 fVar14 = field_0x1330 + 1.3f;

    for (u32 i = 1; i < ARRAY_LENGTH(mBodyTransform.mPos); ++i, ++pos, ++ang) {
        if (i >= 11) {
            s16 ang0 = mLiveTimer * 0.85f * field_0x136A - (i * field_0x136C);
            f32 sin0 = mAng(ang0).sin();

            s16 ang1 = mLiveTimer * (field_0x136E + field_0x1372 + 0x600) - (i * 8000);
            f32 sin1 = mAng(ang1).sin();

            v1.set(
                fVar14 * (sin0 * 1.5f), mBodyTransform.mScaleRear * (fVar2 * (sin1 * 5.0f)),
                mBodyTransform.mScaleRear * -10.0f
            );
        } else {
            s16 ang0 = mLiveTimer * 0.85f * field_0x136A - (i * field_0x136C);
            f32 sin0 = mAng(ang0).sin();

            s16 ang1 = mLiveTimer * (field_0x136E + field_0x1372 + 0x600) - (i * 8000);
            f32 sin1 = mAng(ang1).sin();

            sin1 = fVar2 * (sin1 * 5.0f);
            sin0 = fVar14 * (sin0 * 1.5f);
            v1.set(sin0, sin1, -10.f);
        }

        m0.multVec(v1, v2);
        v3 = (pos[0] - pos[-1]) + v2;

        ang[0].y = v3.atan2sX_Z();
        ang[0].x = -cM::atan2s(v3.y, nw4r::math::FSqrt(v3.squareMagXZ()));

        m1.YrotS(ang[0].y);
        m1.XrotM(ang[0].x);
        m1.multVec(v0, v1);

        pos[0] = pos[-1] + v1;

        if (i >= 11) {
            v0.z = mBodyTransform.mScaleRear * 100.0f;
        } else {
            v0.z = 100.0f;
        }

        ang[0].z = mRotation.z;

        fVar2 *= 1.15f;
        fVar14 *= 1.15f;
    }

    mBodyTransform.mMouthBigAngleY =
        (mMouthBigAmplitudeAdd + 6000) * ((mAng(mMouthBigPhase).sin() * 0.6f - 0.4f) - 0.1f);
    mBodyTransform.mMouthSmallAngleY =
        (mMouthSmallAmplitudeAdd + 4000) * ((mAng(mMouthSmallPhase).sin() * 0.6f - 0.4f) - 0.8f);

    f32 scale = 0.8f;
}

void dAcESkytail_c::wiggleAntenna(s32 idx) {
    mMtx_c m0, m1;
    mVec3_c v3, v0, v1, v2;
    mVec3_c *pos;
    mAng3_c *ang;

    m0.YrotS(mRotation.y);
    m0.XrotM(mRotation.x);
    m0.ZrotM(mRotation.z);

    v0.y = -24.0f;
    v0.z = -28.0f;

    f32 fVar1;
    if (idx == 0) {
        v0.x = 122.0f;
        fVar1 = 100.0f;
    } else {
        v0.x = -122.0f;
        fVar1 = -100.0f;
    }

    m0.multVec(v0, mAntennaTransform[idx].mPos[0]);
    mAntennaTransform[idx].mPos[0] += mPosition;

    pos = &mAntennaTransform[idx].mPos[1];
    ang = &mAntennaTransform[idx].mAng[1];

    v0.set(0.0f, 0.0f, 70.0f);

    m1.YrotS(mRotation.y);
    m1.XrotM(mRotation.x);
    m1.ZrotM(mRotation.z);

    f32 fVar2 = 1.0f;
    f32 fVar3 = field_0x1338 + 1.0f;
    for (u32 i = 1; i < ARRAY_LENGTH(mAntennaTransform[idx].mPos); ++i, ++ang, ++pos) {
        s16 angle = mLiveTimer * (field_0x1370 + 0x600) - (i * 8000);
        v1.set(fVar1 * fVar2, 0.0f, 50.0f * mAng(angle).sin() * fVar3);

        m1.multVec(v1, v2);

        v3 = (pos[0] - pos[-1]) + v2;

        ang[0].y = v3.atan2sX_Z();
        ang[0].x = -cM::atan2s(v3.y, nw4r::math::FSqrt(v3.squareMagXZ()));

        m0.YrotS(ang[0].y);
        m0.XrotM(ang[0].x);
        m0.multVec(v0, v1);

        pos[0] = pos[-1] + v1;

        v0.z = 130.0f;

        fVar2 *= 0.8f;
        fVar3 *= 0.8f;
    }

    mAntennaTransform[idx].mAng[0] = mAntennaTransform[idx].mAng[1];
}

void dAcESkytail_c::fn_172_45D0() {
    return;
}

void dAcESkytail_c::fn_172_45E0() {
    return;
}

void dAcESkytail_c::clampRotationX() {
    if (mRotation.x - 7282 <= (u32)mAng::d2s(140) - 1) {
        mRotation.x = 7281; // 40 deg
    } else if (mRotation.x + 0x8000 <= (u32)mAng::d2s(140)) {
        mRotation.x = -7281; // -40 deg
    }
}
