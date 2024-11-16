#include "d/a/obj/d_a_obj_ivy_rope.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/flag/sceneflag_manager.h"
#include "m/m_vec.h"
#include "toBeSorted/attention.h"

SPECIAL_ACTOR_PROFILE(OBJ_IVY_ROPE, dAcOivyRope_c, fProfile::OBJ_IVY_ROPE, 0x262, 0, 2);

STATE_DEFINE(dAcOivyRope_c, RopeWait);
STATE_DEFINE(dAcOivyRope_c, PlayerGrip);
STATE_DEFINE(dAcOivyRope_c, RopeReturn);
STATE_DEFINE(dAcOivyRope_c, RopeCut);
STATE_DEFINE(dAcOivyRope_c, TerryRopeWait);
STATE_DEFINE(dAcOivyRope_c, TerryRopeUp);
STATE_DEFINE(dAcOivyRope_c, TerryRopeDown);
STATE_DEFINE(dAcOivyRope_c, TerryRopeInit);
STATE_DEFINE(dAcOivyRope_c, TerryRopePlayerCarry);

InteractionTargetDef dAcOivyRope_c::sInteraction = {
    0, 0, 1, GRAB, 0, 150.f, 60.f, 0.f, 200.f, 650.f, 50.f, 1.f,
};

dCcD_SrcCps dAcOivyRope_c::sCpsSrc = {
  /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {AT_TYPE_0x800000 | AT_TYPE_BEETLE | AT_TYPE_CLAWSHOT | AT_TYPE_ARROW | AT_TYPE_WHIP | AT_TYPE_SLINGSHOT |
          AT_TYPE_BOMB | AT_TYPE_SWORD,
      0x111,
      {0x19, 0x407},
      0,
      0},
     /* mObjCo */ {0x4029}},
 /* mCpsInf */
    {10.f}
};

dCcD_SrcSph dAcOivyRope_c::sSphSrc = {
  /* mObjInf */
    {/* mObjAt */ {AT_TYPE_DAMAGE, 0x11, {0, 0, 0}, 1, 0, 0, 0, 0, 0},
     /* mObjTg */
     {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x111, {0, 0x407}, 0, 0},
     /* mObjCo */ {0x29}},
 /* mSphInf */
    {100.f}
};

dCcD_SrcCyl dAcOivyRope_c::sCylSrc = {
  /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {AT_TYPE_BELLOWS, 0x311, {0, 8}, 8, 0},
     /* mObjCo */ {0x109}},
 /* mCylInf */
    {10.f, 750.f}
};

dAcOivyRope_c::~dAcOivyRope_c() {
    if (mPnts1) {
        delete[] mPnts1;
        mPnts1 = nullptr;
    }
    if (mPnts2) {
        delete[] mPnts2;
        mPnts2 = nullptr;
    }
}

void dAcOivyRope_c::initTightRope() {}

UNKTYPE dAcOivyRope_c::fn_256_10C0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_11A0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_1240(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_1480(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_2160(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_26B0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_2C40(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_2D00(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_2DC0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_2EA0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_30A0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_33B0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_34B0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_3540(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_35E0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_36A0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_3770(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_3A20(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_3B90(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::calcDistance(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_3E30(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_3E70(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_3F60(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_43B0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_4520(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_4760(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_4C80(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_4CA0(UNKTYPE) {}

int dAcOivyRope_c::createBase() {
    rotation.set(0, 0, 0);
    f32 temp = 2000.f - 750.f;
    mDistance = 2000.f;
    mOldDistance = 2000.f;
    mSegmentCount = 81;
    mDropsceneFlag = params >> 12;
    mField_0xFEB = 1;
    if (temp < 0.f) {
        temp = 0.f;
    }
    temp /= 750.f;
    mField_0x1040 = temp;
    if (temp > 1.f) {
        mField_0x1040 = 1.f;
    }

    CREATE_ALLOCATOR(dAcOivyRope_c);
}

UNKTYPE dAcOivyRope_c::fn_256_5190(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_5590(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_57F0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_5B00(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_5B64(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_5B70(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_5DB0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_5FF0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_6100(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_61F0(UNKTYPE) {}

void dAcOivyRope_c::initializeState_TerryRopeInit() {}
void dAcOivyRope_c::executeState_TerryRopeInit() {}
void dAcOivyRope_c::finalizeState_TerryRopeInit() {}

void dAcOivyRope_c::initializeState_TerryRopeDown() {}
void dAcOivyRope_c::executeState_TerryRopeDown() {}
void dAcOivyRope_c::finalizeState_TerryRopeDown() {}

void dAcOivyRope_c::initializeState_TerryRopeUp() {}
void dAcOivyRope_c::executeState_TerryRopeUp() {}
void dAcOivyRope_c::finalizeState_TerryRopeUp() {}

void dAcOivyRope_c::initializeState_TerryRopePlayerCarry() {}
void dAcOivyRope_c::executeState_TerryRopePlayerCarry() {}
void dAcOivyRope_c::finalizeState_TerryRopePlayerCarry() {}

void dAcOivyRope_c::initializeState_TerryRopeWait() {}
void dAcOivyRope_c::executeState_TerryRopeWait() {}
void dAcOivyRope_c::finalizeState_TerryRopeWait() {}

bool dAcOivyRope_c::createHeap() {
    mPnts1 = new mVec3_c[mSegmentCount];
}

int dAcOivyRope_c::create() {}

UNKTYPE dAcOivyRope_c::fn_256_7300(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_7310(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_73D0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_7560(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_7680(UNKTYPE) {}

void dAcOivyRope_c::initializeState_RopeWait() {}
void dAcOivyRope_c::executeState_RopeWait() {}
void dAcOivyRope_c::finalizeState_RopeWait() {}

void dAcOivyRope_c::initializeState_PlayerGrip() {}
void dAcOivyRope_c::executeState_PlayerGrip() {}
void dAcOivyRope_c::finalizeState_PlayerGrip() {}

void dAcOivyRope_c::initializeState_RopeReturn() {}
void dAcOivyRope_c::executeState_RopeReturn() {}
void dAcOivyRope_c::finalizeState_RopeReturn() {}

void dAcOivyRope_c::initializeState_RopeCut() {}
void dAcOivyRope_c::executeState_RopeCut() {}
void dAcOivyRope_c::finalizeState_RopeCut() {}

UNKTYPE dAcOivyRope_c::fn_256_7D50(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_8060(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_8590(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_8990(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_8D10(UNKTYPE) {}

void dAcOivyRope_c::fn_256_9120(u16) {}

UNKTYPE dAcOivyRope_c::fn_256_9280(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_9450(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_9C80(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_A040(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_A2C0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_A750(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_AA40(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_AAB0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_AAF0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_ABA0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_AC00(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_AE00(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_B2B0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_BAB0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_BB70(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_BE80(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_BFF0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_C200(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_C410(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_C6F0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_C740(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_C810(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_C960(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_C980(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_C9B0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_CD40(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_CE20(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_CFA0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_D050(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_D110(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_D1B0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_D2B0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_D3D0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_D730(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_D7A0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_D850(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_DAA0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_DE80(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_DEE0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_DF30(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_E3E0(UNKTYPE) {}

UNKTYPE dAcOivyRope_c::fn_256_E790(UNKTYPE) {}

bool dAcOivyRope_c::canBeLinkedToWoodTag() {
    if (mSubtype == 3) {
        return !SceneflagManager::sInstance->checkBoolFlag(roomid, mWoodTagSceneflag);
    }
    return false;
}

bool dAcOivyRope_c::drop() {
    bool dropFlag;
    if (mDropsceneFlag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(roomid, mDropsceneFlag)) {
        dropFlag = false;
    } else {
        dropFlag = true;
    }

    if (dropFlag) {
        mDropped = true;
        return true;
    } else {
        return false;
    }
}
