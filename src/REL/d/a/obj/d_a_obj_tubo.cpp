#include "d/a/obj/d_a_obj_tubo.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/dungeonflag_manager.h"
#include "egg/math/eggMath.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
#include "toBeSorted/event_manager.h"

void float_ordering() {
    f32 f[] = {15.f, .4f, .4f, 5.f};
}

#pragma explicit_zero_data on
u8 dAcOtubo_c::sUnk0 = 1;
u32 dAcOtubo_c::sUnk1 = 0;
#pragma explicit_zero_data off

SPECIAL_ACTOR_PROFILE(OBJ_TUBO, dAcOtubo_c, fProfile::OBJ_TUBO, 0x194, 0, 130);

STATE_DEFINE(dAcOtubo_c, Wait);
STATE_DEFINE(dAcOtubo_c, Grab);
STATE_DEFINE(dAcOtubo_c, Put);
STATE_DEFINE(dAcOtubo_c, Slope);
STATE_DEFINE(dAcOtubo_c, Rebirth);

dCcD_SrcSph dAcOtubo_c::sSphSrc = {
  /* mObjInf */
    {/* mObjAt */ {AT_TYPE_0x8, 0x12, {0, 0, 0}, 2, 0, 0, 0, 0, 0},
     /* mObjTg */ {0xFEF77FFF, 0x801111, {0xA, 0x40F}, 8, 0},
     /* mObjCo */ {0x1E9}},
 /* mSphInf */
    {30.f},
};

bool dAcOtubo_c::createHeap() {
    mRes = getOarcResFile("Tubo");
    const char *subtype = getSubtype() == 0 ? "Tubo00" : "Tubo01";
    TRY_CREATE(mMdl.create(mRes.GetResMdl(subtype), &heap_allocator, 0x120, 1, nullptr));
    return true;
}

int dAcOtubo_c::actorCreate() {
    CREATE_ALLOCATOR(dAcOtubo_c);
    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(20.f, 30.f);
    mEff_0x91C.init(this, 60.f, mScale.x, 0.f);
    mEff_0x91C.setField_0x01(1);
    int unkParam = getParams_0x3000();
    if (unkParam != 1 && unkParam != 2) {
        fn_8002EA30(-60.f, -20.f, 2.f, 1.f, 4.f, &mObjAcch);
    }
    mStts.SetRank(5);
    mSph.Set(sSphSrc);
    mSph.SetStts(mStts);
    mSph.ClrAtSet();

    forwardAccel = -4.f;
    forwardMaxSpeed = -40.f;
    mField_0x9EA = 0;
    mField_0x9D8 = 2.f;

    mQuat_0x98C.set(1.f, 0.f, 0.f, 0.f);
    mQuat_0x97C.set(1.f, 0.f, 0.f, 0.f);
    mQuat_0x99C.set(1.f, 0.f, 0.f, 0.f);

    mSceneflag = getSceneflag();
    mSubtype = getSubtype();
    mbField_0x9ED = getParams_0x3000() == 0;
    mbField_0x9EF = getParams_0x3000() == 2;
    if (getParams_0x3000() == 1) {
        mTimer_0x9F4 = 0xF;
        mSph.SetTgInfo_0x2(0);
    }
    mbField_0x9F0 = getParams_0xC000() == 0;

    if (mSceneflag < 0xFF && checkSceneflag()) {
        fillUpperParams2Byte();
    }

    mStateMgr.changeState(StateID_Wait);
    boundingBox.Set(mVec3_c(-100.f, -40.f, -100.f), mVec3_c(100.f, 80.f, 100.f));
    mActorCarryInfo.set(0xC8A0, 28.f, 50.f, 28.f, nullptr); // TODO (ActorCarry Flags)
    mActorCarryInfo.field_0x8C = 26.f;
    mActorCarryInfo.field_0x90 = 46.f;
    mActorCarryInfo.field_0x94 = 28.f;
    mActorCarryInfo.field_0x98 = 0.f;

    return SUCCEEDED;
}

int dAcOtubo_c::actorPostCreate() {
    mCeLady.link((dAcNpcCeLady_c *)fManager_c::searchBaseByProfName(fProfile::NPC_CE_LADY, nullptr));

    if (mbField_0x9EF) {
        if (DungeonflagManager::sInstance->getCounterOrFlag(3, 8) != 0) {
            return FAILED;
        }
        actor_properties |= 4; // TODO ( actor_properties )

        if (dBgS_ObjGndChk ::CheckPos(position + mVec3_c::Ey * 50.f)) {
            position.y = dBgS_ObjGndChk::GetGroundHeight();
        }
    }

    return SUCCEEDED;
}

int dAcOtubo_c::doDelete() {
    return SUCCEEDED;
}

int dAcOtubo_c::actorExecute() {
    mStateMgr.executeState();
    bool bOffGround = false;
    if (mTimer_0x9F4 != 0) {
        mField_0x9DC = 0.f;

        if (dBgS_ObjGndChk::CheckPos(position + mVec3_c::Ey * 500.f)) {
            if (dBgS_ObjGndChk::GetGroundHeight() - position.y > 5.f) {
                bOffGround = true;
                mSph.ClrTgSet();
            } else if (sLib::calcTimer(&mTimer_0x9F4) == 0) {
                mSph.SetTgInfo_0x2(8);
                mSph.OnTgSet();
                bOffGround = false;
            }
        }
    }
    if (mSph.ChkCoHit()) {
        mObjAcch.OnLineCheck();
    } else {
        mObjAcch.OffLineCheck();
    }

    if (!bOffGround) {
        if (!checkCarryType()) {
            calcVelocity();
            position += velocity;
            position += mStts.GetCcMove();
            mField_0x9DC += position.y - mOldPosition.y;
            adjustRoll();
        }
        mActorCarryInfo.bushTpFunc(mObjAcch);
    }

    if (mStateMgr.isState(StateID_Wait) || mStateMgr.isState(StateID_Slope)) {
        fn_272_2A10();
    }
    mSph.SetC(getCenter());
    dCcS::GetInstance()->Set(&mSph);

    mActorCarryInfo.fn_800511E0(this);
    mField_0x8F0.modifyMtx();
    calcRoll();

    mMdl.setLocalMtx(mWorldMtx);
    mMdl.calc(false);
    poscopy2 = position + mVec3_c(0.f, 28.f, 0.f);
    poscopy3 = poscopy2;
    poscopy3.y += 48.f;
    mField_0x9EB = 0;

    return SUCCEEDED;
}

extern "C" void fn_800A0550(dAcBase_c *);

void dAcOtubo_c::registerInEvent() {
    if (EventManager::isCurrentEvent("HoleIn") || EventManager::isCurrentEvent("HoleOut")) {
        fn_800A0550(this);
    }
}

int dAcOtubo_c::draw() {
    drawModelType1(&mMdl);

    static mQuat_c quat(0.f, 30.f, 0.f, 40.f);

    if (yoffset >= 0.f) {
        drawShadow(mShdw, nullptr, mWorldMtx, &quat, -1, -1, -1, -1, -1, position.y - mObjAcch.GetGroundH());
    }
    return SUCCEEDED;
}

void dAcOtubo_c::initializeState_Wait() {}
void dAcOtubo_c::executeState_Wait() {
    // TODO
    if (mObjAcch.ChkGroundLanding()) {
        if (!mbField_0x9EF && EventManager::isInEvent()) {
        } else {
        }
    } else {
    }
}
void dAcOtubo_c::finalizeState_Wait() {}

void dAcOtubo_c::initializeState_Grab() {}
void dAcOtubo_c::executeState_Grab() {}
void dAcOtubo_c::finalizeState_Grab() {}

void dAcOtubo_c::initializeState_Put() {}
void dAcOtubo_c::executeState_Put() {}
void dAcOtubo_c::finalizeState_Put() {}

void dAcOtubo_c::initializeState_Slope() {}
void dAcOtubo_c::executeState_Slope() {}
void dAcOtubo_c::finalizeState_Slope() {}

void dAcOtubo_c::initializeState_Rebirth() {}
void dAcOtubo_c::executeState_Rebirth() {}
void dAcOtubo_c::finalizeState_Rebirth() {}

void dAcOtubo_c::calcRoll() {
    bool onLog = checkOnLog_0xE4E();
    if (mField_0x9EA || onLog) {
        if (onLog) {
            velocity = position - mOldPosition;
            forwardSpeed = EGG::Math<f32>::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
            angle.y = cM::atan2s(velocity.x, velocity.z);
        }
        f32 vel_mag = PSVECMag(velocity);
        f32 f1 = mAng(vel_mag * (mField_0x9D4 + 200.f)).radian();
        f32 f2 = mAng(vel_mag * 182.0f * 0.2f).radian();
        f32 f0 = mAng(angle.y - rotation.y).radian();

        mQuat_c q0, q1, q2, q3;
        q1.setAxisRotation(mVec3_c::Ey, f0);
        q0.setAxisRotation(mVec3_c::Ey, -f0);
        q3.setAxisRotation(mVec3_c::Ex, f1);
        q2.setAxisRotation(mVec3_c::Ey, f2);

        mQuat_0x98C = q1 * q3 * q2 * q0 * mQuat_0x98C;

        if (onLog) {
            velocity = mVec3_c::Zero;
            forwardSpeed = 0.f;
        }
    }

    mMtx_c m;
    m.fromQuat(mQuat_0x99C * mQuat_0x98C);
}

mVec3_c dAcOtubo_c::getCenter() const {
    mMtx_c m;
    m.ZXYrotS(rotation.x, rotation.y, rotation.z);
    mVec3_c dir;
    PSMTXMultVecSR(m, mVec3_c::Ey, dir);

    return position + dir * 28.f;
}

void dAcOtubo_c::fn_272_2E60(const mVec3_c &vel) {
    if (mField_0x8F0.fn_800051780(mSph)) {
        if (mSph.ChkCoHit()) {
            position += mStts.GetCcMove();
            mField_0x8F0.fn_800051630();
        }
        forwardSpeed = 0.f;
        velocity = mVec3_c::Zero;
        mField_0x9F1 = 1;
        mField_0x9B8 = vel * 2.f;
    } else if (mSph.ChkTgAtHitType(AT_TYPE_0x10000)) {
        velocity.x += vel.x * 0.06f;
        velocity.y += vel.y * 0.2f;
        velocity.z += vel.z * 0.06f;
        mField_0x9CA = cM::atan2s(vel.x, vel.z);
        forwardSpeed = mField_0x9D0 * EGG::Math<f32>::sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
    }

    mField_0x9EA = 1;
}

void dAcOtubo_c::fn_272_3020() {
    mField_0x9EB = 1;
    static mVec3_c sRot = mVec3_c::Ey;

    mQuat_c quat;
    mVec3_c rot = mVec3_c::Ey + mVec3_c(0.f, 0.f, 1.f) * mField_0x9E0;

    rot.normalize();
    rot.rotY(mField_0x9CC);
    mField_0x9CC += 0x2000;

    quat.makeVectorRotation(sRot, rot);
    sLib::chase(&mField_0x9E0, 0.f, 0.005f);
    mQuat_0x99C.slerpTo(quat, 0.5f, mQuat_0x99C);
}
