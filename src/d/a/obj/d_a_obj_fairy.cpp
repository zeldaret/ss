#include "d/a/obj/d_a_obj_fairy.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_player_act.h"
#include "d/d_stage_mgr.h"
#include "d/d_vec.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m3d/m_fanm.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_FAIRY, dAcOFairy_c, fProfile::OBJ_FAIRY, 0x22D, 0, 0x23);

static dCcD_SrcSph sSphSrc1 = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */ {AT_TYPE_BUGNET | AT_TYPE_BELLOWS | AT_TYPE_WIND, 0x200311, {0, 0, 0x40F}, 0, 0},
     /* mObjCo */ {0}},
    /* mSphInf */
    {30.f},
};

static dCcD_SrcSph sSphSrc2 = {
    /* mObjInf */
    {/* mObjAt */ {0, 0x0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */ {0, 0x0, {0, 0, 0x40F}, 0, 0},
     /* mObjCo */ {0x29}},
    /* mSphInf */
    {10.f},
};

static InteractionTargetDef sAttnTarget = {1, 0, 0, UNK_18, 0x2, 400.0f, 0.0f, 0.0f, -400.0f, 400.0f, 50.0f, 1.0f};

STATE_DEFINE(dAcOFairy_c, Wait);
STATE_DEFINE(dAcOFairy_c, Avoid);
STATE_DEFINE(dAcOFairy_c, PlayerAvoid);
STATE_DEFINE(dAcOFairy_c, CureStart);
STATE_DEFINE(dAcOFairy_c, Cure);
STATE_DEFINE(dAcOFairy_c, CureEnd);
STATE_DEFINE(dAcOFairy_c, CatchDemo);

bool dAcOFairy_c::createHeap() {
    void *file = getOarcResFile("PutFairy");
    TRY_CREATE(mModel.create(file, "PutFairy", "PutFairy_fly", &mAllocator, 0x120));

    return true;
}

int dAcOFairy_c::create() {
    CREATE_ALLOCATOR(dAcOFairy_c);

    mStts.SetRank(1);
    mCcSph1.Set(sSphSrc1);
    mCcSph1.SetStts(mStts);
    mCcSph2.Set(sSphSrc2);
    mCcSph2.SetStts(mStts);

    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(20.0f, 150.0f);
    mObjAcch.SetWaterNone();
    mObjAcch.SetGndThinCellingOff();
    mAcceleration = 0.0f;
    mMaxSpeed = -40.0f;

    setPosYWaveParams();
    int rnd = cM::rndInt(40);
    s16 tmp = mPosYWaveSpeed;
    mPosYWaveTime = tmp * rnd;
    mOrigPosition = mPosition;
    mMaxSpeedY = randMaxSpeedY();
    mSpeed = mMaxSpeedY;

    mSpawnType = getFromParams(0, 0xF);
    field_0xB81 = getFromParams(4, 0x3);

    setSpawnPosition(calcInitialSpawnPosition());

    mVec3_c crrPos = mPosition;
    mObjAcch.CrrPos(*dBgS::GetInstance());
    if (mObjAcch.ChkWallHit(nullptr)) {
        mAng tmp1 = mAcchCir.GetWallAngleY() + cM::rndFX(1.0f) * 1820.0f;
        mAng tmp2 = mAngle.y;
        mVelocity.rotY(tmp1 - tmp2);
        mAngle.y = tmp1;

        mVec3_c v(mVec3_c::Ez * (cM::rndFX(10.0f) + 150.0f));
        v.rotY(tmp1);

        mOrigPosition += v;
        setPosition(crrPos);
        mOldPosition = mPosition;
    }

    if (mSpawnType == SPAWN_MANUAL_RELEASE || mSpawnType == SPAWN_AUTO_RELEASE) {
        mStateMgr.changeState(StateID_CureStart);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }

    mModel.getModel().setPriorityDraw(0x7F, 0x7F);
    mBoundingBox.Set(mVec3_c(-20.0f, -200.0f, -20.0f), mVec3_c(20.0f, 20.0f, 20.0f));

    mVec3_c pos = mPosition;
    dBgS_ObjGndChk chk;
    pos.y += 10.0f;
    if (chk.CheckPos(pos)) {
        mOriginalGndHeight = chk.GetGroundHeight();
        mOrigPosition.y = mOriginalGndHeight + 20.0f + 100.0f;
    }
    mPreventCatchAfterSpawnTimer = 45;

    if (isWithinPlayerRadius(200.0f) && getXZAngleToPlayer().absDiff(mAng::fromVec(mVelocity)) < 0x2000) {
        mVelocity.rotY(0x8000);
        mAngle.y += mAng(0x8000);
    }

    mModel.setAnm("PutFairy_fly", m3d::PLAY_MODE_4, 0.0f);
    mModel.setRate(1.0f);
    mModel.setFrame(cM::rndInt(60));

    for (int i = 0; i < ARRAY_LENGTH(mEffects); i++) {
        mEffects[i].init(this);
    }

    return SUCCEEDED;
}

int dAcOFairy_c::doDelete() {
    if (!isCuring()) {
        setObjectProperty(OBJ_PROP_0x200);
        for (int i = 0; i < (int)ARRAY_LENGTH(mEffects); i++) {
            mEffects[i].remove(true);
        }
    }

    return SUCCEEDED;
}

int dAcOFairy_c::actorExecute() {
    if (!mStateMgr.isState(StateID_CatchDemo)) {
        if (mCcSph1.ChkTgHit() && !mStateMgr.isState(StateID_CatchDemo) && mCcSph1.ChkTgAtHitType(AT_TYPE_BUGNET)) {
            mStateMgr.changeState(StateID_CatchDemo);
            return SUCCEEDED;
        }

        if (!sLib::calcTimer(&mPreventCatchAfterSpawnTimer)) {
            if (mCcSph2.ChkCoHit() && !isCuring() && !dAcPy_c::GetLink()->checkCurrentAction(/* SCOOP */ 0x9E)) {
                if (!dAcItem_c::hasAnyFairy()) {
                    for (int i = 0; i < (int)ARRAY_LENGTH(mEffects); i++) {
                        mEffects[i].remove(true);
                    }
                    dAcItem_c::giveItem(ITEM_FAIRY, 0, 0);
                    deleteRequest();
                    return SUCCEEDED;
                }
                mStateMgr.changeState(StateID_CureStart);
                return SUCCEEDED;
            }
        }
        mPositionCopy3 = mPosition;
        mPositionCopy3.y += 20.0f;

        if (canTargetWithBugNet()) {
            AttentionManager::GetInstance()->addTarget(*this, sAttnTarget, 0, nullptr);
        }

        if (!isCuring()) {
            AttentionManager::GetInstance()->addCatchLikeTarget(*this, UNK_0x58, 1, 125.0f, -200.0f, 50.0f);

            if (dAcPy_c::GetLink()->isUsingBottle()) {
                AttentionManager::GetInstance()->addTarget(*this, sAttnTarget, 0, nullptr);
            }
        }

        mPositionCopy2 = mPosition;
    }

    mStateMgr.executeState();
    executeCheckForWindOrBellows();

    if (!isCuring()) {
        calcVelocity();
        mPosition += mVelocity;
        mPosition += mStts.GetCcMove();
    }

    mRotation.y = mAngle.y;

    if (isCuring()) {
        mPosition = mCurePosition;
    }

    if (!field_0xB89) {
        mEffects[0].holdEffect(PARTICLE_RESOURCE_ID_MAPPING_331_, mPosition, nullptr, nullptr, nullptr, nullptr);
        if (!isCuring() && !dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
            holdSound(SE_O_FAIRY_FLY_LEV);
        }
    }

    if (!isCuring()) {
        if (dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
            setObjectProperty(OBJ_PROP_0x200);
            f32 alpha = dStageMgr_c::GetInstance()->getGlobalAlpha();
            if (alpha < 255.0f) {
                mEffects[0].setGlobalAlpha(0);
            }
        } else {
            unsetObjectProperty(OBJ_PROP_0x200);
            mEffects[0].setGlobalAlpha(255);
        }
    }
    if (isCuring() && mRoomID != dAcPy_c::GetLink()->mRoomID) {
        // will delete when curing is done
        deleteRequest();
        return SUCCEEDED;
    }

    if (isCuring() && !field_0xB89) {
        mEffects[1].holdEffect(PARTICLE_RESOURCE_ID_MAPPING_327_, mPosition, nullptr, nullptr, nullptr, nullptr);
    }

    if (!mStateMgr.isState(StateID_CatchDemo) && !isCuring()) {
        mCcSph1.SetC(mPosition);
        dCcS::GetInstance()->Set(&mCcSph1);
        mCcSph2.SetC(mPosition);
        dCcS::GetInstance()->Set(&mCcSph2);
    }

    updateMatrix();
    mModel.getModel().setLocalMtx(mWorldMtx);
    mModel.getModel().setScale(mVec3_c(mScale));
    mModel.play();

    return SUCCEEDED;
}

int dAcOFairy_c::draw() {
    if (!mStateMgr.isState(StateID_CatchDemo)) {
        drawModelType1(&mModel.getModel());

        if (!isCuring()) {
            static mQuat_c rot(0.0f, 0.0f, 0.0f, 10.0f);
            f32 f = mObjAcch.GetGroundH();
            drawShadow(mShadow, nullptr, mWorldMtx, &rot, -1, -1, -1, -1, -1, mPosition.y - f);
        }
    }

    return SUCCEEDED;
}

void dAcOFairy_c::initializeState_Wait() {
    mSpeed = mMaxSpeedY;
    mPreventAvoidTimer = cM::rndInt(60) + 120;
    mHasSetTurnSpeedY = false;
    field_0xB86 = false;
    mModel.setRate(1.0f);
}
void dAcOFairy_c::executeState_Wait() {
    sLib::addCalc(&mSpeed, mMaxSpeedY, 0.1f, 2.0f, 0.01f);

    mVelocity.y = cM::minMaxLimit(mVelocity.y, -mMaxSpeedY, mMaxSpeedY);

    f32 diff = mOrigPosition.absXZTo(mPosition);
    f32 yDiff = mPosition.y - mOrigPosition.y;
    yDiff = nw4r::math::FAbs(yDiff);

    if (60.0f < diff) {
        // Fairy has moved away from its original position by more than 60 units,
        // time to turn it (in a random direction) until it's facing back to the
        // original position
        if (!mHasSetTurnSpeedY) {
            mTurnSpeedY = rndTurnSpeed();
        }
        mHasSetTurnSpeedY = true;

        mVec3_c v = mOrigPosition - mPosition;
        v.normalize();
        f32 factor = 1.0f / mVelocity.mag();
        if (v.dot(mVelocity * factor) < 0.9f) {
            mAngle.y += mTurnSpeedY;
        }
    } else {
        mHasSetTurnSpeedY = false;
        if (!sLib::calcTimer(&mPreventAvoidTimer)) {
            mAvoidTimer = cM::rndInt(30);
            mStateMgr.changeState(StateID_Avoid);
            return;
        } else if (shouldAvoidBugNet()) {
            mAvoidTimer = 30;
            mStateMgr.changeState(StateID_Avoid);
            return;
        }
    }

    if (shouldAvoidLink()) {
        mStateMgr.changeState(StateID_PlayerAvoid);
        return;
    }

    if (40.0f < yDiff) {
        if (isMovingAwayFromOrigY()) {
            f32 absVelY = mVelocity.y;
            absVelY = nw4r::math::FAbs(absVelY);
            f32 f = absVelY < 0.2f ? 0.3f : 0.1f;
            targetSpeedY = absVelY * 0.75f + f;
            if (mPosition.y - mOrigPosition.y >= 0.0f) {
                targetSpeedY *= -1.0f;
            }
            // @bug ineffective clamping
            (void)cM::minMaxLimit(targetSpeedY, -10.0f, 10.0f);
        }
        field_0xB86 = true;
        sLib::chase(&mVelocity.y, targetSpeedY, 0.1f);
    } else {
        field_0xB86 = false;
    }

    if (mVelocity.y < 0.0f && mPosition.y - mOriginalGndHeight <= 50.0f) {
        // slow down when approaching ground
        mVelocity.y *= 0.7f;
    }

    calcPosYWave();
    if (mHasSetTurnSpeedY || field_0xB86) {
        unsetActorProperty(AC_PROP_0x1);
    } else {
        setActorProperty(AC_PROP_0x1);
    }
}
void dAcOFairy_c::finalizeState_Wait() {}

void dAcOFairy_c::initializeState_Avoid() {
    mTurnSpeedY = rndTurnSpeed();
    field_0xB88 = cM::rnd() <= 0.2f;
    if (field_0xB88) {
        mAvoidTimer = cM::rndInt(30) + 45;
        mVec3_c res;

        const mVec3_c *v;
        if (mVelocity.dot(mVec3_c::Ez) >= 0.0f) {
            v = &mVec3_c::Ey;
        } else {
            // TODO - technically UB, but seems to be required
            mVec3_c tmp = -mVec3_c::Ey;
            v = &tmp;
        }
        mMtx_c mtx;
        mtx.makeRotationFromVecs(mVec3_c::Ez, *v, 1.0f);

        MTXMultVecSR(mtx, mVelocity, res);

        f32 f = (mAvoidTimer % 2) != 0 ? 4.0f : 2.0f;
        field_0xB24.makeRotationFromVecs(mVelocity, res, f / (mAvoidTimer - 1));
    }
    unsetActorProperty(AC_PROP_0x1);
}
void dAcOFairy_c::executeState_Avoid() {
    if (shouldAvoidLink()) {
        mStateMgr.changeState(StateID_PlayerAvoid);
    } else if (!sLib::calcTimer(&mAvoidTimer)) {
        mStateMgr.changeState(StateID_Wait);
    } else if (field_0xB88) {
        MTXMultVecSR(field_0xB24, mVelocity, mVelocity);
        mSpeed = mVelocity.absXZ();
        mAngle.y = mVelocity.atan2sX_Z();
    } else {
        mAngle.y += mTurnSpeedY;
        calcPosYWave();
    }
}
void dAcOFairy_c::finalizeState_Avoid() {
    setActorProperty(AC_PROP_0x1);
}

void dAcOFairy_c::initializeState_PlayerAvoid() {
    mTurnSpeedY *= 1.5f;
    unsetActorProperty(AC_PROP_0x1);
}
void dAcOFairy_c::executeState_PlayerAvoid() {
    s32 tmp = mTurnSpeedY;
    mAng ang = getXZAngleToPlayer();
    bool end = sLib::chaseAngle2(&mAngle.y.mVal, ang + mAng(0x8000), tmp);
    if (end) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        calcPosYWave();
    }
}
void dAcOFairy_c::finalizeState_PlayerAvoid() {
    setActorProperty(AC_PROP_0x1);
}

void dAcOFairy_c::initializeState_CureStart() {
    mCureAngularSpeed = cM::rndInt(0x100) + 0x2000;
    mCureAngle = getXZAngleToPlayer();
    // If the fairy saves the player from death, it spawns directly
    // at the player's position and circles outwards. Otherwise it directly
    // starts 50 units away.
    mCurePosXZOffset = mSpawnType == SPAWN_AUTO_RELEASE ? 0.0f : 50.0f;
    mCurePosYOffset = mSpawnType == SPAWN_AUTO_RELEASE ? 0.0f : 50.0f;

    mCcSph2.ClrCoSet();
    mCcSph2.ClrTgSet();
    mModel.setRate(12.0f);
    mSpeed = 0.0f;
    mVelocity = mVec3_c::Zero;
    dAcPy_c::fairyHeal(this);
    if (mSpawnType != SPAWN_AUTO_RELEASE) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_O_FAIRY_RECOVER);
    }
}
void dAcOFairy_c::executeState_CureStart() {
    if (mSpawnType == SPAWN_AUTO_RELEASE) {
        // Start circling away from the player up to the normal distance
        sLib::chase(&mAutoReleaseProgress, 1.0f, 0.04f);
        f32 f = cLib::easeInOut(mAutoReleaseProgress, 3.0f) * 50.0f;
        mCurePosXZOffset = f;
        mCurePosYOffset = f;

        const dAcPy_c *link = dAcPy_c::GetLink();

        mCurePosition = link->mPosition;
        mCurePosition.y += f;
        getXZCirclePoint(mCurePosition, mCureAngle, f);

        mAngle.y = getXZAngleToPlayer();

        if (link->checkActionFlags(daPlayerActBase_c::FLG0_IN_WATER)) {
            mCurePosition.y += -150.0f;
        }

        holdSound(SE_O_FAIRY_FLY_LEV);
        if (mAutoReleaseProgress == 1.0f) {
            mStateMgr.changeState(StateID_Cure);
        }
    } else {
        calcCurePosition(50.0f, 50.0f);

        s32 diff = mCureAngle.absDiff(0);
        if (diff < mCureAngularSpeed && mCurePosYOffset == 50.0f) {
            mStateMgr.changeState(StateID_Cure);
        }
    }
}
void dAcOFairy_c::finalizeState_CureStart() {}

void dAcOFairy_c::initializeState_Cure() {
    mCureAngularSpeed = cM::rndInt(0x100) + 0x1111;
    mCureAngle = getXZAngleToPlayer();
    mCurePosXZOffset = 50.0f;
    mCurePosYOffset = 50.0f;
    mModel.setRate(15.0f);
    mCurePosXZOffsetTarget = cM::rnd() * 80.0f + 20.0f;
    if (mSpawnType == SPAWN_AUTO_RELEASE) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_O_FAIRY_RECOVER);
    }
}
void dAcOFairy_c::executeState_Cure() {
    calcCurePosition(mCurePosXZOffsetTarget, 160.0f);
    if (mCureAngle < mCureAngularSpeed) {
        mCurePosXZOffsetTarget = cM::rnd() * 80.0f + 20.0f;
    }
    if (mCurePosYOffset == 160.0f) {
        mStateMgr.changeState(StateID_CureEnd);
    }
}
void dAcOFairy_c::finalizeState_Cure() {}

void dAcOFairy_c::initializeState_CureEnd() {}
void dAcOFairy_c::executeState_CureEnd() {
    calcCurePosition(mCurePosXZOffsetTarget, 180.0f);
    sLib::chase(&mScale.x, 0.0f, 0.07f);
    mScale.y = mScale.z = mScale.x;
    if (mScale.x <= 0.0f) {
        deleteRequest();
    }
}
void dAcOFairy_c::finalizeState_CureEnd() {}

void dAcOFairy_c::initializeState_CatchDemo() {
    field_0xB89 = true;
    dAcPy_c::GetLinkM()->bugNetCollectTreasure(ITEM_FAIRY);
    for (int i = 0; i < (int)ARRAY_LENGTH(mEffects); i++) {
        mEffects[i].remove(true);
    }
    setObjectProperty(OBJ_PROP_0x200);
}
void dAcOFairy_c::executeState_CatchDemo() {
    if (!field_0xB89) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dAcOFairy_c::finalizeState_CatchDemo() {
    field_0xB89 = false;
    unsetObjectProperty(OBJ_PROP_0x200);
}

bool dAcOFairy_c::shouldAvoidBugNet() const {
    if (dAcPy_c::LINK->isUsingBugnet()) {
        mVec3_c dist = dAcPy_c::LINK->getBugNetPos() - mPosition;
        return dist.mag() < 100.0f && mVelocity.dot(dist) > 0.0f;
    }

    return false;
}

bool dAcOFairy_c::isCuring() const {
    return mStateMgr.isState(StateID_CureStart) || mStateMgr.isState(StateID_Cure) ||
           mStateMgr.isState(StateID_CureEnd);
}

bool dAcOFairy_c::canTargetWithBugNet() const {
    bool canTarget = false;
    if (dAcPy_c::LINK != nullptr && dAcPy_c::LINK->isUsingBugnet() && !isCuring()) {
        canTarget = true;
    }
    return canTarget;
}

void dAcOFairy_c::setSpawnPosition(const mVec3_c &v) {
    mSpawnPosition = v;
    mAngle.y = cLib::targetAngleY(mPosition, mSpawnPosition);
    mVec3_c tmp(0.0f, 0.0f, mSpeed);
    mWorldMtx.YrotS(mAngle.y);
    mWorldMtx.XrotM(cLib::targetAngleX(mSpawnPosition, mPosition));
    MTXMultVec(mWorldMtx, tmp, mVelocity);
    targetSpeedY = mVelocity.y;
}

void dAcOFairy_c::setPosYWaveParams() {
    mPosYWaveSpeed = (cM::rndF(2.0f) + 1.0f) * 546.0f;
    mPosYWaveAmplitude = mSpeed * (cM::rndF(0.5f) + 0.5f);
}

mAng dAcOFairy_c::rndTurnSpeed() const {
    mAng ret = (cM::rnd() + 1.0f) * 546.0f;
    if (cM::rndF(1.0f) < 0.5f) {
        ret *= (s32)-1;
    }
    return ret;
}

mVec3_c dAcOFairy_c::calcInitialSpawnPosition() {
    mVec3_c ret = mOrigPosition;
    ret.x += (cM::rndF(60.0f) * 2.0f - 60.0f);
    ret.z += (cM::rndF(60.0f) * 2.0f - 60.0f);

    f32 tmp = (mOrigPosition - ret).absXZ();
    if (tmp > 60.0f) {
        // @bug maybe? assigning to local fixes stack order...
        mVec3_c _ignored = calcInitialSpawnPosition();
    }

    ret.y += (cM::rndF(40.0f) * 2.0f - 40.0f);
    return ret;
}

void dAcOFairy_c::calcCurePosition(const f32 &xzOffsetTarget, const f32 &yOffsetTarget) {
    const dAcPy_c *link = dAcPy_c::GetLink();

    mCurePosition = link->mPosition;
    mCurePosition.y += mCurePosYOffset;
    getXZCirclePoint(mCurePosition, mCureAngle, mCurePosXZOffset);

    sLib::chase(&mCurePosXZOffset, xzOffsetTarget, mCurePosXZOffsetTarget / 10.0f);
    sLib::chase(&mCurePosYOffset, yOffsetTarget, 2.0f);
    mCureAngle += mCureAngularSpeed;

    if (link->checkActionFlags(daPlayerActBase_c::FLG0_IN_WATER)) {
        mCurePosition.y += -150.0f;
    }
    mAngle.y = getXZAngleToPlayer() + mAng(-0x4000);
}

bool dAcOFairy_c::calcPosYWave() {
    if (0.9f < mPosYWaveTime.cos()) {
        setPosYWaveParams();
    }
    mPosYWaveTime += mPosYWaveSpeed;
    mPosition.y += mPosYWaveAmplitude * mPosYWaveTime.sin();
    return false;
}

void dAcOFairy_c::executeCheckForWindOrBellows() {
    if (mCcSph1.ChkTgHit() && mCcSph1.ChkTgAtHitType(AT_TYPE_BELLOWS | AT_TYPE_WIND) && !isCuring()) {
        mVec3_c dir = mCcSph1.GetTgAtHitDir();
        mAngle.x = dir.atan2sY_XZ();
        mAngle.y = dir.atan2sX_Z();

        // Blow the fairy in the wind direction, but slowly
        mVelocity.x += dir.x * 0.05f;
        mVelocity.z += dir.z * 0.05f;
        mVelocity.y += dir.y * 0.01f;

        f32 f = mMaxSpeedY * 5.0f;
        mSpeed = nw4r::ut::Min(mVelocity.absXZ(), f);
    }
}

f32 dAcOFairy_c::randMaxSpeedY() const {
    return cM::rndFX(0.5f) + 3.0f;
}

bool dAcOFairy_c::shouldAvoidLink() const {
    return isWithinPlayerRadius(150.0f) && mAngle.y.absDiff(getXZAngleToPlayer()) < 0x2000;
}

bool dAcOFairy_c::isMovingAwayFromOrigY() const {
    return (targetSpeedY >= 0.0f && mPosition.y - mOrigPosition.y > 0.0f) ||
           (targetSpeedY <= 0.0f && mPosition.y - mOrigPosition.y < 0.0f);
}
