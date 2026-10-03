#include "d/a/obj/d_a_obj_girahimu_sword_link.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_sphere.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_arithmetic.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_GH_SW_L, dAcObjGirahimuSwordLink_c, fProfile::OBJ_GH_SW_L, 0x114, 0, 2);

STATE_DEFINE(dAcObjGirahimuSwordLink_c, Hide);
STATE_DEFINE(dAcObjGirahimuSwordLink_c, Equip);
STATE_DEFINE(dAcObjGirahimuSwordLink_c, GetSword);
STATE_DEFINE(dAcObjGirahimuSwordLink_c, Throw);
STATE_DEFINE(dAcObjGirahimuSwordLink_c, AtThrow);
STATE_DEFINE(dAcObjGirahimuSwordLink_c, Stick);
STATE_DEFINE(dAcObjGirahimuSwordLink_c, Reflect);

bool dAcObjGirahimuSwordLink_c::createHeap() {
    nw4r::g3d::ResFile res = dAcPy_c::GetLink()->getSwordResFile();
    TRY_CREATE(mMdl.create(
        &mAllocator, res, dAcPy_c::getSwordName(), 0x130, mVec3_c::Zero, mVec3_c(100.0f, 0.0f, 0.0f), mStts, nullptr, 1,
        nullptr
    ));
    return true;
}

int dAcObjGirahimuSwordLink_c::create() {
    CREATE_ALLOCATOR(dAcObjGirahimuSwordLink_c);
    init();
    changeState(StateID_Hide);
    return SUCCEEDED;
}

void dAcObjGirahimuSwordLink_c::init() {
    mAcceleration = 0.0f;
    mStts.SetRank(12);
    mMaxSpeed = -6000.0f;
    mBoundingBox.Set(mVec3_c(-100.0f, -100.0f, -100.0f), mVec3_c(100.0f, 100.0f, 100.0f));
    mAcch.Set(&mPosition, &mOldPosition, this, 1, &mAcchCir, nullptr, nullptr, nullptr);
    mAcchCir.SetWall(0.0f, 50.0f);
    mMdl.fn_8006B660(4, 1, 0, 0, 0, 0, 0, 0x400, 12, 40.0f);
}

int dAcObjGirahimuSwordLink_c::actorExecute() {
    if (--field_0x8A8 <= 0) {
        field_0x8A8 = 0;
    }
    executeState();
    fn_238_19D0();
    mAcch.CrrPos(*dBgS::GetInstance());
    updateModelMatrix();
    if (getStateID().isEqual(StateID_Stick)) {
        mPositionCopy3 = mPosition;
        mPositionCopy3.y += 50.0f;
        mPositionCopy2 = mPositionCopy3;

        AttentionManager::GetInstance()->addUnk3Target(*this, 0x2 | 0x1, 600.0f, 50.0f, -200.0f, 200.0f);
    }
    return SUCCEEDED;
}

int dAcObjGirahimuSwordLink_c::draw() {
    if (getStateID().isEqual(StateID_Hide)) {
        return true;
    }

    mMdl.entry(this, nullptr, nullptr);
    static mSphere_c sph(mVec3_c(0.0f, 100.0f, 0.0f), 200.0f);
    fn_8002edb0(mShadow, mMdl.mMdl, &sph, -1, -1, 0.0f);
    return true;
}

void dAcObjGirahimuSwordLink_c::initializeState_Hide() {}
void dAcObjGirahimuSwordLink_c::executeState_Hide() {}
void dAcObjGirahimuSwordLink_c::finalizeState_Hide() {}

void dAcObjGirahimuSwordLink_c::initializeState_Equip() {
    mAcceleration = 0.0f;
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mRotation.set(mAng(0), mAng(0), mAng(0));
    mMdl.SetGrp(0xC);
}
void dAcObjGirahimuSwordLink_c::executeState_Equip() {}
void dAcObjGirahimuSwordLink_c::finalizeState_Equip() {}

void dAcObjGirahimuSwordLink_c::initializeState_GetSword() {
    mAcceleration = 3.0f;
    mVec3_c v(-7.0f, 112.0f, 36.0f);
    v.rotY(mAngleCopy.y);
    v += mGhirahimPos;
    calcVelocity(v, 29.0f);
    mMdl.SetGrp(0xC);
    mRotation.z = 0;
}
void dAcObjGirahimuSwordLink_c::executeState_GetSword() {
    mRotation.z -= 0x2000;
    mVelocity.y -= mAcceleration;
    mPosition += mVelocity;
}
void dAcObjGirahimuSwordLink_c::finalizeState_GetSword() {
    mRotation.z = 0;
}

void dAcObjGirahimuSwordLink_c::initializeState_Throw() {
    field_0x8B0.toQuat(field_0xD00);
    mMdl.enableAttack();
}
void dAcObjGirahimuSwordLink_c::executeState_Throw() {
    mVec3_c to(-1.0f, 0.0f, 0.0f);
    mQuat_c q;
    mVec3_c from(1.0f, 0.0f, 0.0f);
    q.slerp(from, to, 1.0f);
    field_0xD00.slerpTo(q, 0.2f, field_0xD00);
    field_0x8B0.fromQuat(field_0xD00);

    mVelocity.y -= mAcceleration;
    mRotation.z += field_0xD10;
    mPosition += mVelocity;

    if (lineCheck()) {
        changeState(StateID_Reflect);
        return;
    }
    if (mPosition.y <= mGhirahimPos.y - 500.0f) {
        mPosition.y = mGhirahimPos.y;
        field_0xD14.set(0.0f, 1.0f, 0.0f);
        changeState(StateID_Reflect);
    }
}
void dAcObjGirahimuSwordLink_c::finalizeState_Throw() {
    mMdl.setInactive();
}

void dAcObjGirahimuSwordLink_c::initializeState_AtThrow() {
    field_0x8B0.toQuat(field_0xD00);
    mMdl.enableAttack();
}
void dAcObjGirahimuSwordLink_c::executeState_AtThrow() {
    holdSound(SE_OGhSwL_FLY_LV);

    cCcD_Obj *pCcAt = mMdl.mCcList.findAtHit();
    if (pCcAt != nullptr) {
        if (pCcAt->GetAtActor()->isActorPlayer() && pCcAt->GetAtFlag0x2()) {
            field_0xD14.set(0.0f, 0.0f, 1.0f);
            field_0xD14.rotY(dAcPy_c::GetLink()->mRotation.y);

            changeState(StateID_Reflect);
            return;
        }
    }

    mVec3_c moveDir = mVelocity;
    moveDir.normalizeRS();
    (void)mVelocity.mag(); // lol

    mQuat_c q;
    mVec3_c from(1.0f, 0.0f, 0.0f);
    q.slerp(from, moveDir, 1.0f);
    field_0xD00.slerpTo(q, 0.5f, field_0xD00);
    field_0x8B0.fromQuat(field_0xD00);

    mVelocity.y -= mAcceleration;
    mPosition += mVelocity;
    if (lineCheck()) {
        changeState(StateID_Reflect);
        return;
    }

    if (mPosition.y <= mGhirahimPos.y + 5.0f) {
        field_0xD14.set(0.0f, 1.0f, 0.0f);
        changeState(StateID_Reflect);
        return;
    }

    mTrailEmitterID = PARTICLE_RESOURCE_ID_MAPPING_1_;
    mWorldMtx.transM(30.0f, 0.0f, 0.0f);
    mEmitter.holdEffect(mTrailEmitterID, mWorldMtx, nullptr, nullptr);
    mWorldMtx.transM(-30.0f, 0.0f, 0.0f);
}
void dAcObjGirahimuSwordLink_c::finalizeState_AtThrow() {
    mMdl.setInactive();
}

void dAcObjGirahimuSwordLink_c::initializeState_Stick() {
    startSound(SE_OGhSwL_BOUND);
    mMdl.setInactive();
    mAcceleration = 0.0f;
    mVelocity.set(0.0f, 0.0f, 0.0f);
}
void dAcObjGirahimuSwordLink_c::executeState_Stick() {
    mVec3_c to(-1.0f, 0.0f, 0.0f);
    field_0x8B0.multVec(to, to);
    to.y = 0.0f;
    to.normalizeRS();

    mQuat_c q;
    mVec3_c from(-1.0f, 0.0f, 0.0f);
    q.slerp(from, to, 1.0f);
    field_0xD00.slerpTo(q, 0.5f, field_0xD00);
    field_0x8B0.fromQuat(field_0xD00);

    from = mPosition - dAcPy_c::GetLink()->mPosition;
    if (from.absXZ() < 60.0f) {
        changeState(StateID_Hide);
        dAcPy_c::GetLinkM()->relatedToUsingItem0x11();
    }
}
void dAcObjGirahimuSwordLink_c::finalizeState_Stick() {
    mRotation.z = 0;
}

void dAcObjGirahimuSwordLink_c::initializeState_Reflect() {
    s32 _weird_zero = 0;
    reflect(field_0xD14, 3.0f, 10.f + _weird_zero, 0.5f);
    field_0x8AA = 0;
}
void dAcObjGirahimuSwordLink_c::executeState_Reflect() {
    sLib::chaseAngle(mRotation.z.ref(), 0, 0x1000);
    mVec3_c direction = mVelocity;

    if (field_0xD26) {
        mMdl.mCcList.findAtHit();
        mMdl.SetGrp(0x3E);
    }

    switch (field_0x8AA) {
        case 0:
            field_0xD24 += 0x1000;
            direction.normalizeRS();
            // fatll-through
        case 1:
            field_0xD24 += 0x1000;
            direction.normalizeRS();
            direction.y = field_0xD24.sin() * 0.3f;
            direction.normalizeRS();
            break;
        case 2:
            field_0xD24 += 0x1000;
            direction.normalizeRS();
            direction.y = field_0xD24.sin() * 0.3f;
            direction.normalizeRS();
    }
    mQuat_c q;
    mVec3_c from(1.0f, 0.0f, 0.0f);
    q.slerp(from, direction, 1.0f);
    field_0xD00.slerpTo(q, 0.5f, field_0xD00);
    field_0x8B0.fromQuat(field_0xD00);

    mVelocity.y -= mAcceleration;
    mPosition += mVelocity;
    if (lineCheck()) {
        if (mPosition.y < 40.0f) {
            field_0x8AA++;
        }
        if (field_0x8AA >= 3) {
            changeState(StateID_Stick);
        } else {
            reflect(field_0xD14, 3.0f, 5.0f, 0.5f);
        }
    } else {
        if (mPosition.y <= mGhirahimPos.y + 5.0f) {
            mPosition.y = mGhirahimPos.y + 5.0f;
            field_0xD14.set(0.0f, 1.0f, 0.0f);

            if (mPosition.y < 40.0f) {
                field_0x8AA++;
            }
            if (field_0x8AA < 3) {
                reflect(field_0xD14, 3.0f, 5.0f, 0.5f);
            } else {
                direction.set(1.0f, 0.0f, 0.0f);
                field_0x8B0.multVec(direction, direction);
                direction.y = 0.0f;
                direction.normalizeRS();
                mQuat_c q;
                mVec3_c from(1.0f, 0.0f, 0.0f);
                q.slerp(from, direction, 1.0f);
                field_0xD00.slerpTo(q, 0.5f, field_0xD00);
                field_0x8B0.fromQuat(field_0xD00);

                changeState(StateID_Stick);
            }
        }
    }

    mTrailEmitterID = PARTICLE_RESOURCE_ID_MAPPING_1_;
    mWorldMtx.transM(30.0f, 0.0f, 0.0f);
    mEmitter.holdEffect(mTrailEmitterID, mWorldMtx, nullptr, nullptr);
    mWorldMtx.transM(-30.0f, 0.0f, 0.0f);
}
void dAcObjGirahimuSwordLink_c::finalizeState_Reflect() {
    field_0xD26 = false;
}

void dAcObjGirahimuSwordLink_c::setEquip() {
    changeState(StateID_Equip);
}

void dAcObjGirahimuSwordLink_c::setGetSword() {
    changeState(StateID_GetSword);
}

void dAcObjGirahimuSwordLink_c::setHide() {
    changeState(StateID_Hide);
}

void dAcObjGirahimuSwordLink_c::setThrow(const mVec3_c &force, mAng angle, f32 rate) {
    mAcceleration = 3.0f;
    field_0xD10 = angle;
    calcVelocity(force, rate);
    changeState(StateID_Throw);
}

void dAcObjGirahimuSwordLink_c::setAtThrow(const mVec3_c &velocity) {
    mAcceleration = 0.0f;
    field_0xD10 = 0;
    mVelocity.set(velocity);
    changeState(StateID_AtThrow);
}

void dAcObjGirahimuSwordLink_c::fn_238_19D0() {
    return;
}

void dAcObjGirahimuSwordLink_c::updateModelMatrix() {
    mWorldMtx.transS(mPosition.x, mPosition.y, mPosition.z);
    mWorldMtx.concat(field_0x8B0);
    mWorldMtx.ZXYrotM(mRotation);
    mMdl.calc(mWorldMtx, mPosition - mOldPosition, false);
}

void dAcObjGirahimuSwordLink_c::setTransform(const mMtx_c &mtx, const mVec3_c &pos) {
    field_0x8B0 = mtx;
    field_0x8B0.ZrotM(-0x4000);
    field_0x8B0.XrotM(0x4000);
    setPosition(pos);
    setOldPosition(pos);
}

bool dAcObjGirahimuSwordLink_c::isHide() {
    return getStateID().isEqual(StateID_Hide);
}

bool dAcObjGirahimuSwordLink_c::isStick() {
    return getStateID().isEqual(StateID_Stick);
}

// NONMATCHING
void dAcObjGirahimuSwordLink_c::reflect(const mVec3_c &rot, f32 gravity, f32, f32 force) {
    if (rot.y == 1.0f) {
        startSound(SE_OGhSwL_BOUND);
    } else {
        startSound(SE_OGhSwL_HIT);
    }

    mVec3_c v0(-mVelocity.x, -mVelocity.y, -mVelocity.z);
    mAng a1 = v0.atan2sX_Z();
    mAng a2 = rot.atan2sX_Z();
    s32 s = a1 - a2;
    f32 mag = nw4r::math::FAbs(mAng(s).normal_c());
    if (mag <= 0.1f) {
        mag = 0.1f;
    }
    if (mag >= 1.0f) {
        mag = 1.0f;
    }

    f32 m = rot.mag();
    f32 dot = v0.dot(rot);
    f32 scale = dot * 2.0f / m;

    mVec3_c v;
    mVelocity = rot * scale - v0;
    mVelocity *= force;

    if (gravity < 0.0f) {
        gravity = -gravity;
    }
    mAcceleration = gravity;
}

bool dAcObjGirahimuSwordLink_c::lineCheck() {
    mMtx_c m = field_0x8B0;
    m.ZXYrotM(mRotation);

    mVec3_c start, end;
    start = mPosition;
    mVec3_c v0(100.0f, 0.0f, 0.0f);
    m.multVec(v0, v0);
    v0 += mPosition;
    end = v0;

    v0.set(0.0f, mVelocity.mag(), 0.0f);
    m.multVec(v0, v0);
    v0 += mPosition;
    start = v0;

    dBgS_ObjLinChk linChk;
    linChk.Set(&start, &end, nullptr);
    if (dBgS::GetInstance()->LineCross(&linChk)) {
        cM3dGPla pla;
        if (dBgS::GetInstance()->GetTriPla(linChk, &pla)) {
            field_0xD14.x = pla.GetN().x;
            field_0xD14.y = pla.GetN().y;
            field_0xD14.z = pla.GetN().z;
        }

        if (mVelocity.dot(field_0xD14) < 0.0f) {
            mVec3_c newPos = linChk.GetLinEnd();
            mVec3_c v1 = field_0xD14;
            v1.normalizeRS();
            v1 *= 5.0f;
            newPos += v1;
            mPosition = newPos;
            if (mPosition.y <= mGhirahimPos.y + 5.0f) {
                mPosition.y = mGhirahimPos.y + 5.0f;
            }
            return true;
        }
    } else {
        linChk.Set(&end, &start, nullptr);
        if (dBgS::GetInstance()->LineCross(&linChk)) {
            cM3dGPla pla;
            if (dBgS::GetInstance()->GetTriPla(linChk, &pla)) {
                field_0xD14.x = pla.GetN().x;
                field_0xD14.y = pla.GetN().y;
                field_0xD14.z = pla.GetN().z;
            }

            if (mVelocity.dot(field_0xD14) < 0.0f) {
                mVec3_c newPos = linChk.GetLinEnd();
                mVec3_c v1 = field_0xD14;
                v1.normalizeRS();
                v1.multScalar(5.0f);
                newPos += v1;
                mPosition = newPos;
                if (mPosition.y <= mGhirahimPos.y + 5.0f) {
                    mPosition.y = mGhirahimPos.y + 5.0f;
                }
                return true;
            }
        }
    }
    return false;
}
