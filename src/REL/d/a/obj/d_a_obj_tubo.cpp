#include "d/a/obj/d_a_obj_tubo.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/dungeonflag_manager.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
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
