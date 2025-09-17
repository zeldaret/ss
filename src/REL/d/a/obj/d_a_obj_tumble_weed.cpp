#include "d/a/obj/d_a_obj_tumble_weed.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_light_env.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/math/eggMath.h"
#include "egg/math/eggQuat.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/dowsing_target.h"

void float_ordering() {
    const f32 arr[] = {5.f, 15.f, 7.f, 0.5f, 0.1f};
}

SPECIAL_ACTOR_PROFILE(OBJ_TUMBLE_WEED, dAcOTumbleWeed_c, fProfile::OBJ_TUMBLE_WEED, 0x243, 0, 2);

STATE_DEFINE(dAcOTumbleWeed_c, Wait);
STATE_DEFINE(dAcOTumbleWeed_c, Slope);

dCcD_SrcSph dAcOTumbleWeed_c::sSphSrc = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */ {~(AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000), 0x111, {0, 8, 0x40F}, 0, 0},
     /* mObjCo */ {0xE9}},
    /* mSphInf */
    {60.f},
};

bool dAcOTumbleWeed_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("GrassRollDry"));
    TRY_CREATE(mMdl.create(mResFile.GetResMdl("GrassRollDry"), &mAllocator, 0x120, 1, nullptr));
    return true;
}

int dAcOTumbleWeed_c::actorCreate() {
    CREATE_ALLOCATOR(dAcOTumbleWeed_c);

    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(20.f, 60.f);
    mObjAcch.OffClrSpeedY();
    mObjAcch.SetGndThinCellingOff();

    mStts.SetRank(3);

    mSph.Set(sSphSrc);
    mSph.SetStts(mStts);

    mDowsingTarget.initialize(DowsingTarget::SLOT_TREASURE, 0, nullptr, 0.0f);
    mDowsingTarget.doRegister();

    mAcceleration = -2.f;
    mMaxSpeed = -40.f;

    mField_0x974 = 0.0f;
    mField_0x98B = true;

    mWind = mVec3_c::Zero;

    mSpeedTarget = cM::rndFX(2.5f) + 15.f;
    mField_0x978 = cM::rndF(40.f);
    mField_0x97C = cM::rndFX(0.25f) + 1.f;
    mTumbleTimer = (150);

    mField_0x910.set(1.f, 0.f, 0.f, 0.f);
    mScale.set(1.f, 1.f, 1.f);

    mStateMgr.changeState(StateID_Wait);

    mBoundingBox.Set(mVec3_c(-50.f, -20.f, -50.f), mVec3_c(50.f, 60.f, 50.f));

    return SUCCEEDED;
}

int dAcOTumbleWeed_c::actorPostCreate() {
    return SUCCEEDED;
}

int dAcOTumbleWeed_c::doDelete() {
    return SUCCEEDED;
}

int dAcOTumbleWeed_c::actorExecute() {
    if (!mField_0x98C && !checkStopped()) {
        mField_0x968 = mVelocity;
        mField_0x968.y = 0.0f;
        mField_0x968.normalize();
        mField_0x98C = true;
    }

    mStateMgr.executeState();
    calcWind();

    sLib::chase(&mField_0x980, 1.f, 0.1f);
    dLightEnv_c::GetInstance().setWind(mField_0x968, mField_0x980);

    calcVelocity();
    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

    mObjAcch.CrrPos(*dBgS::GetInstance());
    mField_0x974 += mPosition.y - mOldPosition.y;
    if (checkCollect()) {
        dAcPy_c::LINK->bugNetCollectTreasure(ITEM_TUMBLE_WEED);
        killNoItemDrop();
        return SUCCEEDED;
    }
    if (checkBreak()) {
        doBreak();
        return SUCCEEDED;
    }

    if (mSph.ChkTgHit() && mSph.ChkTgAtHitType(AT_TYPE_BELLOWS | AT_TYPE_WIND)) {
        mField_0x974 = 0.f;
        adjustTumble(mSph.GetTgAtHitDir() * 0.06f);
    }
    adjustTimeScale();

    if (checkObjectProperty(OBJ_PROP_0x2)) {
        // Weak function not being placed right
        if (sLib::calcTimer(&mTumbleTimer) == 0) {
            killNoItemDrop();
            return SUCCEEDED;
        }
    } else {
        mTumbleTimer = 150;
    }
    mSph.SetC(getPosition());
    mSph.SetR(mScale.x * 60.f);
    dCcS::GetInstance()->Set(&mSph);
    updateMatrix();
    calcMatrix();
    holdSound(SE_TWeed_ROLL_LV);
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.setScale(mScale);
    mMdl.calc(false);

    return SUCCEEDED;
}

int dAcOTumbleWeed_c::draw() {
    drawModelType1(&mMdl);

    static mQuat_c shadowRot(0.f, 30.f, 0.f, 50.f);
    if (0.f < mScale.x) {
        drawShadow(mShdw, nullptr, mShadowMtx, &shadowRot, -1, -1, -1, -1, -1, mPosition.y - mObjAcch.GetGroundH());
    }

    return SUCCEEDED;
}

void dAcOTumbleWeed_c::initializeState_Wait() {
    mbOnGround = false;
}

void dAcOTumbleWeed_c::executeState_Wait() {
    if (mObjAcch.ChkGroundLanding() && !mbOnGround) {
        tumbleBounceMaybe();
        return;
    }

    if (!mObjAcch.ChkGndHit()) {
        return;
    }

    mVelocity.y = 0.f;
    mField_0x974 = 0.f;
    if (checkSlope()) {
        mStateMgr.changeState(StateID_Slope);
    } else {
        adjustSpeed();
        if (mSpeed < 0.1f) {
            mField_0x98B = false;
            sLib::chase(&mSpeed, 0.f, 0.05f);
        }
    }
}

void dAcOTumbleWeed_c::finalizeState_Wait() {}

void dAcOTumbleWeed_c::initializeState_Slope() {}

void dAcOTumbleWeed_c::executeState_Slope() {
    if (mObjAcch.ChkGroundLanding() && !mbOnGround) {
        tumbleBounceMaybe();
        return;
    }

    if (!mObjAcch.ChkGndHit()) {
        return;
    }

    mVelocity.y = 0.f;
    adjustAngle();
    adjustSpeed();
    if (!checkSlope()) {
        mStateMgr.changeState(StateID_Wait);
    }
}

void dAcOTumbleWeed_c::finalizeState_Slope() {}

bool dAcOTumbleWeed_c::checkBreak() {
    if (mObjAcch.ChkWallHit(nullptr) && mSpeed > 5.f) {
        return true;
    }

    if (mSph.ChkTgHit() && !mSph.ChkTgAtHitType(AT_TYPE_BUGNET)) {
        return true;
    }

    if (mObjAcch.ChkGndHit() && mField_0x974 < -300.f) {
        return true;
    }

    if (fn_475_E40()) {
        return true;
    }

    if (mObjAcch.ChkGndHit() && checkInvalidGround()) {
        return true;
    }

    return false;
}

bool dAcOTumbleWeed_c::fn_475_E40() const {
    return mField_0x974 < -10000.f;
}

bool dAcOTumbleWeed_c::checkSlope() {
    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(mObjAcch.mGnd, &pla);

    mAng ang = mAng::fromRad(mVec3_c::Ey.angle(pla.GetN()));
    return sLib::absDiff(ang, 0) > mAng::deg2short(1);
}

bool dAcOTumbleWeed_c::checkCollect() {
    return mSph.ChkTgHit() && mSph.ChkTgAtHitType(AT_TYPE_BUGNET) ? true : false;
}

bool dAcOTumbleWeed_c::checkInvalidGround() const {
    u32 code = dBgS::GetInstance()->GetSpecialCode(mObjAcch.mGnd);

    return code == POLY_ATTR_LAVA || code == POLY_ATTR_SAND_MED || code == POLY_ATTR_SAND_DEEP_INSTANT ||
           code == POLY_ATTR_SAND_DEEP_SLOW;
}

void dAcOTumbleWeed_c::doBreak() {
    startSound(SE_TWeed_CUT);
    mVec3_c pos = getPosition();
    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_743_, pos, nullptr, nullptr, nullptr, nullptr, 0, 0);
    deleteRequest();
}

void dAcOTumbleWeed_c::calcMatrix() {
    if (mField_0x98B) {
        f32 vel_mag = VEC3Len(mVelocity);
        f32 f1 = mAng(vel_mag * (mField_0x978 + 200.f)).radian();
        f32 f2 = mAng(vel_mag * 182.0f * 0.2f).radian();
        f32 f0 = mAng(mAngle.y - mRotation.y).radian();

        mQuat_c q0, q1, q2, q3;
        q1.setAxisRotation(mVec3_c::Ey, f0);
        q0.setAxisRotation(mVec3_c::Ey, -f0);
        q3.setAxisRotation(mVec3_c::Ex, f1);
        q2.setAxisRotation(mVec3_c::Ey, f2);

        mField_0x910 = q1 * q3 * q2 * q0 * mField_0x910;
    }

    mMtx_c mtx0, mtx1, mtx2;
    mShadowMtx.copyFrom(mWorldMtx);
    mtx1.transS(getPosition() - mPosition);
    mShadowMtx += mtx1;
    mtx0.fromQuat(mField_0x910);
    mtx2.transS(0.f, 40.f, 0.f);
    mWorldMtx += mtx2;
    mWorldMtx += mtx0;
}

void dAcOTumbleWeed_c::adjustAngle() {
    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(mObjAcch.mGnd, &pla);
    mVec3_c vel = mVelocity; // ok?
    mAngle.y = mAng::fromVec(mVelocity);
}

void dAcOTumbleWeed_c::adjustSpeed() {
    cM3dGPla pla;

    // @bug
    // the ground angle calculation happens before retrieving the grounds normal
    // this is probably hard to notice due to the little effect it has.
    // Effect:
    //  Tumbleweed slows to the target much quicker than intended
    //  Tumbleweed speeds to the target + 5.0f much quicker than intended
    mAng gndAngle = mAng::fromRad(mVec3_c::Ey.angle(pla.GetN()));

    dBgS::GetInstance()->GetTriPla(mObjAcch.mGnd, &pla);

    f32 speedStep = 0.5f;
    f32 step = gndAngle.cos() * speedStep;

    // Flat Ground or not in the direction of the slope
    //  Slows down
    if (gndAngle < mAng::deg2short(1) ||
        sLib::absDiff(cM::atan2s(pla.GetN().x, pla.GetN().z), getAngle().y) > mAng::deg2short(90)) {
        sLib::chase(&mSpeed, mSpeedTarget, step);
    }
    // Sloped Ground and in the direction of the slope
    //  Speeds up
    else {
        mSpeed = cM::minMaxLimit(mSpeed + step, mSpeedTarget, mSpeedTarget + 5.0f);
    }
}

void dAcOTumbleWeed_c::tumbleBounceMaybe() {
    mVelocity.y *= -0.75f;
    mSpeed *= 0.9f;

    if (mSpeed < 15.f) {
        mSpeed = mSpeedTarget;
    }

    mbOnGround = false;
    if (mVelocity.y < 5.f) {
        mVelocity.y = 0.f;
        mbOnGround = true;
    }
}

void dAcOTumbleWeed_c::adjustTimeScale() {
    mTimeArea.check(getRoomId(), getPosition(), 0, 30.f, 0.1f);
    if (0.f < mTimeArea.getDistMaybe()) {
        sLib::chase(&mScale.y, 0.f, 0.07f);
        mScale.z = mScale.y;
        mScale.x = mScale.y;
    } else {
        sLib::chase(&mScale.y, 1.f, 0.07f);
        mScale.z = mScale.y;
        mScale.x = mScale.y;
    }
}

void dAcOTumbleWeed_c::adjustTumble(const mVec3_c &dir) {
    mField_0x98B = true;

    mVelocity.x += dir.x;
    mVelocity.z += dir.z;

    mSpeed = mField_0x97C * EGG::Math<f32>::sqrt(mVelocity.x * mVelocity.x + mVelocity.z * mVelocity.z);
    mAngle.y = mAng::fromVec(dir);
}

void dAcOTumbleWeed_c::setWind(const mVec3_c &wind) {
    mWind = wind;
    mWindTimer = cM::rndInt(10);
}

void dAcOTumbleWeed_c::calcWind() {
    if (!isWindStop() && sLib::calcTimer(&mWindTimer) == 0) {
        mVelocity.y = cM::rndFX(3.0f) + 15.f;
        mSpeed = cM::rndFX(5.0f) + 15.f;

        mWind = mVec3_c::Zero;
        mField_0x980 = 0.f;
    }
}

mVec3_c dAcOTumbleWeed_c::getPosition() const {
    mMtx_c mtx;
    mVec3_c vec;
    mtx.ZXYrotS(mRotation.x, mRotation.y, mRotation.z);
    PSMTXMultVecSR(mtx, mVec3_c::Ey, vec);
    return mPosition + vec * 40.f;
}

void float_ordering2() {
    const f32 arr[] = {30.f, 0.07f, 20.f, -10000.f, 5.f};
}
