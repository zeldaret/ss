#include "d/a/obj/d_a_obj_ivy_rope.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_lin.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/sceneflag_manager.h"
#include "egg/math/eggMath.h"
#include "m/m3d/m3d.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_triangular.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
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
      {0, 0x19, 0x407},
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
     {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x111, {0, 0, 0x407}, 0, 0},
     /* mObjCo */ {0x29}},
 /* mSphInf */
    {100.f}
};

dCcD_SrcCyl dAcOivyRope_c::sCylSrc = {
  /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {AT_TYPE_BELLOWS, 0x311, {0, 0, 8}, 8, 0},
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
    // TODO
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

void dAcOivyRope_c::fn_256_A2C0(bool bool0) {}

bool dAcOivyRope_c::fn_256_A750(bool bool0, f32 float0) {}

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

void dAcOivyRope_c::fn_256_C410() {
    fn_256_3E70();
    if (mStateMgr.isState(StateID_RopeReturn)) {
        mVec3_c diff = dAcPy_c::GetLink()->position - mTightropeEnd;
        if (diff.squareMagXZ() > 90000.f) {
            return;
        }
    }

    if (mStateMgr.isState(StateID_PlayerGrip) || mField_0xFCE != 0) {
        return;
    }

    int count = mSegmentCount - 9;
    f32 f2 = dAcPy_c::GetLink()->position.y - 100.f;
    f32 f = 150.f + dAcPy_c::GetLink()->position.y;
    if (count < 0) {
        count = 0;
    }
    for (int i = 0; i < mField_0xFEB; i++) {
        // TODO
    }
}

bool dAcOivyRope_c::fn_256_C6F0() {
    if (mField_0x105F) {
        mField_0x105F += mField_0x105E;
        if (mField_0x105F >= mSegmentCount - 19) {
            mField_0x105F = mSegmentCount - 20;
            return true;
        }
    }
    return false;
}

void dAcOivyRope_c::fn_256_C740() {
    const f32 f2 = (1.f - mField_0x1040) * 4.f + 1.1f;
    if (mField_0xFA4 <= f2) {
        const mVec3_c &pnt = fn_256_D730(mSegmentCount - 1);
        const mVec3_c diff = pnt - dAcPy_c::GetLink()->position;
        const f32 diffmag = diff.squareMagXZ();
        if (diffmag < 28900.f) {
            fn_256_C980(450.f, 200.f);
        }
    }
}

void dAcOivyRope_c::fn_256_C810(mVec3_c &pOut, int idx) {
    if (idx > 0) {
        mVec3_c end = fn_256_D730(idx);
        mVec3_c start = fn_256_D730(idx - 1);
        cLib::addCalcPos(&end, pOut, 0.65f, 75.f, 1.f);
        mVec3_c diff = end - start;
        diff.normalize();
        pOut = start + diff * 25.f;
    }
}

void dAcOivyRope_c::fn_256_C960() {
    fn_256_C980(150.f, 200.f);
}

void dAcOivyRope_c::fn_256_C980(f32 f0, f32 f1) {
    sInteraction.field_0x24 = mDistance + f1;
    sInteraction.field_0x14 = f0;
    AttentionManager::sInstance->addTarget(*this, sInteraction, 0, nullptr);
}

UNKTYPE dAcOivyRope_c::fn_256_C9B0(UNKTYPE) {}

void dAcOivyRope_c::fn_256_CD40() {
    position = poscopy3 = poscopy2 = fn_256_D730(mSegmentCount - 1);

    mCps1.Set(sCpsSrc);
    mCps1.SetStts(mStts);
    mCps1.SetR(15.f);

    mCps1.SetTgInfo_0x1(0xB);

    mVec3_c tmp = position;
    tmp.y -= 50.f;
    mCps1.cM3dGLin::Set(position, tmp);

    mCps1.OnTg_0x200000();
}

void dAcOivyRope_c::fn_256_CE20(f32 *pOut1, f32 *pOut2, f32 *pOut3) {
    f32 f2, f3;
    f32 f1 = mField_0xFD6;
    f2 = mField_0xFAC * mField_0xFAC + mField_0xFB0 * mField_0xFB0;
    f3 = nw4r::math::FSqrt(f2);
    f32 temp = f3 / f1;
    f2 = nw4r::math::FAbs(temp);
    if (f2 > 1.f) {
        mField_0xFAC = (mField_0xFD6 - 1) * mField_0xFCC.cos();
        mField_0xFB0 = (mField_0xFD6 - 1) * -mField_0xFCC.sin();

        f2 = 1.f;
    }

    *pOut1 = f1;
    *pOut2 = f3;
    *pOut3 = f2;
}

void dAcOivyRope_c::fn_256_CFA0(bool bool0) {
    if (bool0 && mField_0x1005 != 0) {
        mField_0x1005 = 0;
        mField_0x105F = mSegmentCount - 24;

        mField_0xFF7 = 1;
        mField_0xFF6 = 1;

        mField_0xFC4 = 0.05f;

        mField_0xFD6 = 20;

        mField_0xFA4 = 1.1f + (s8)mField_0x1005 * 0.1f;
        mField_0xFA8 = 1.1f + (s8)mField_0x1005 * 0.1f;

        mField_0xFD2 = 0;
        mField_0xFD4 = 0;
    }
}
void dAcOivyRope_c::fn_256_D050() {
    bool check = checkSubtype(0) || checkSubtype(4) || checkSubtype(3) || checkSubtype(7) || checkSubtype(6);
    if (check) {
        mCyl.Set(sCylSrc);
        mCyl.SetStts(mStts);
        mCyl.SetH(mDistance);
    }
}

void dAcOivyRope_c::fn_256_D110() {
    u32 third = (mSegmentCount - 1) / 3;
    mField_0xFF0[0] = 0;
    for (int i = 1; i <= 3; i++) {
        mField_0xFF0[i] = third * i;
        if (mField_0xFF0[i] > (mSegmentCount - 1)) {
            mField_0xFF0[i] = (mSegmentCount - 1);
        }
    }
}

void dAcOivyRope_c::fn_256_D1B0() {
    fn_256_D110();
    for (int i = 0; i < 3; i++) {
        mCpsArr[i].Set(sCpsSrc);
        mCpsArr[i].SetStts(mStts);
        mCpsArr[i].ClrCoSet();
        mCpsArr[i].SetR(15.f);
        mCpsArr[i].OffTgType(AT_TYPE_BEETLE | AT_TYPE_SLINGSHOT | AT_TYPE_SWORD);
        mCpsArr[i].OnTgType(AT_TYPE_0x40);
        mCpsArr[i].cM3dGLin::Set(fn_256_D730(mField_0xFF0[i]), fn_256_D730(mField_0xFF0[i + 1]));
    }
}

void dAcOivyRope_c::fn_256_D2B0() {
    if (checkObjectProperty(0x1)) {
        return;
    }

    if (mSubtype == 3 && mField_0x1039 == 3) {
        return;
    }

    if (mField_0xFEF != 0) {
        mField_0xFEF--;
    }

    for (int i = 0; i < 3; i++) {
        if (mCpsArr[i].ChkTgHit()) {
            mField_0xFEF = 2;
        }

        mCpsArr[i].cM3dGLin::Set(fn_256_D730(mField_0xFF0[i]), fn_256_D730(mField_0xFF0[i + 1]));

        dCcS::GetInstance()->Set(&mCpsArr[i]);
    }
}

void dAcOivyRope_c::fn_256_D3D0(mVec3_c &pOut1, mVec3_c &pOut2, s16 param2, bool bool0, f32 float0) {
    mVec3_c playerPos = dAcPy_c::GetLink()->position;
    mVec3_c playerCenter = dAcPy_c::GetLink()->getCenterTranslation();

    if (bool0) {
        f32 a = (playerCenter.x - playerPos.x);
        a *= 0.3f;

        f32 b = (playerCenter.z - playerPos.z);
        b *= 0.3f;

        playerCenter.x = playerPos.x + a;
        playerCenter.z = playerPos.z + b;
    }

    mMtx_c m;
    m.transS(playerCenter);
    mAng3_c playerRot = dAcPy_c::GetLink()->rotation;
    m.ZXYrotM(playerRot.x, playerRot.y, playerRot.z);

    // name 100% guess
    mAng swingAngle = (mField_0xFCC - 0x8000) - dAcPy_c::GetLink()->rotation.y;

    f32 something = param2 / 5461.f;
    if (something > 1.f) {
        something = 1.f;
    }

    mVec3_c somevec(0.f, 0.f, 0.f);
    somevec.x += something * swingAngle.sin();
    somevec.z += something * swingAngle.cos();

    mVec3_c somevec2(0.f, -25.f, float0);
    PSMTXMultVec(m, somevec, somevec2);
    somevec2 += somevec;
    if (somevec2.z < float0) {
        somevec2.z = float0;
    }

    // name 100% guess
    mVec3_c swingTarget;
    PSMTXMultVec(m, somevec, swingTarget);

    cLib::addCalcPos(&pOut2, swingTarget, 0.3f, 50.f, 1.f);

    if (pOut2.squareDistance(somevec2) > something * something) {
        pOut1 = pOut2 - somevec2;
        pOut1.normalize();
        pOut2 = somevec2 + pOut1 * something;
    }
    pOut1 = pOut2 - playerPos;
    pOut1.normalize();
}

// Get Path Point
const mVec3_c &dAcOivyRope_c::fn_256_D730(s32 idx) {
    if (mSubtype == 3) {
        return mCoilMdl.getLine(0)->getPathPoint(idx);
    } else {
        return mRopeMdl.getLine(0)->getPathPoint(idx);
    }
}

// Set Path Point
void dAcOivyRope_c::fn_256_D7A0(s32 idx, const mVec3_c &pnt) {
    if (mSubtype == 3) {
        mCoilMdl.getLine(0)->getPathPoint(idx) = pnt;
    } else {
        mRopeMdl.getLine(0)->getPathPoint(idx) = pnt;
    }
}

// Draw Shadow
void dAcOivyRope_c::fn_256_D850() {
    bool draw_shadow = false;
    switch (subtype) {
        case 1:
        case 2: break;
        case 4: {
            if (!mbField_0xFE8) {
                draw_shadow = true;
            }
        } break;
        case 6: break;
        case 7: {
            if (mbField_0x27C5) {
                draw_shadow = true;
            }
        } break;
        default: draw_shadow = true; break;
    }

    if (draw_shadow) {
        static mQuat_c shadow_q = mQuat_c(0.f, 0.f, 0.f, 30.f);
        if (mSubtype == 7) {
            // This section is just weird
            int idk;
            f32 f;
            f32 val;
            val = nw4r::math::FAbs(mField_0xF68.y - mField_0x27C0);
            if (!(val > 1000.f)) {
                f = 250.f;
                idk = f;
                if (val > 50.f) {
                    idk = (1000.f - val) * f * (1 / 950.f);
                    if (idk < 5.f) {
                        f = 5.f;
                        idk = f;
                    }
                }
                mMtx_c m;
                m.transS(mField_0xF68);
                drawShadow(mShadow, nullptr, m, &shadow_q, -1, idk, -1, -1, -1, val);
            }
        } else {
            mMtx_c m;
            m.transS(fn_256_D730(mSegmentCount - 1));
            drawShadow(mShadow, nullptr, m, &shadow_q, -1, -1, -1, -1, -1, 175.f);
        }
    }
}

extern "C" void *getCamera(int idx);
void dAcOivyRope_c::fn_256_DAA0(bool bool0, bool bool1, f32 float0, f32 float1) {
    bool cylHitBellows = false;
    if ((bool1 || mField_0xFEF != 0) || (mCyl.ChkTgHit() && mCyl.ChkTgAtHitType(AT_TYPE_BELLOWS))) {
        mbField_0x2793 = true;
        cylHitBellows = true;
        mField_0x2792 = 10;
        if (mField_0xFEF && mSubtype != 6) {
            float0 = 2.5f;
        }
    }

    if (mbField_0x2793) {
        if (mField_0x2790 != 0) {
            mField_0x2790--;
        }

        // This block is a mess
        else if (cylHitBellows) {
            mField_0x2790 = 30;
            f32 target = float0 - mField_0x1040 * 1.5f;
            f32 chasing = 0.f;
            f32 step = target * float1;
            int ang = dAcPy_c::GetLink()->rotation.y;
            u8 *cam = (u8 *)getCamera(0);
            if (cam) {
                ang = cLib::targetAngleY(*(mVec3_c *)(cam + 0x6C), mTightropeEnd) + 0x2000;
            }
            ang += cM::rndInt(0x1555);
            int i = 1;
            if (mSegmentCount > 1) {
                for (; i < mSegmentCount; i++) {
                    BOOL chased = sLib::chase(&chasing, target, step);
                    if (chased) {
                        target *= -1.f;
                    }
                    mPnts1[i].set(0.f, 0.f, 0.f);
                    // Inline?
                    mVec3_c &v = mPnts1[i];
                    f32 step = chasing;

                    v.x += step * nw4r::math::SinIdx(ang);
                    v.z += step * nw4r::math::CosIdx(ang);
                };
            }
        }

        if (fn_256_A750(bool1, 0.5f)) {
            if (!bool0) {
                mField_0xFD8 = 1;
                f32 target0 = -(mField_0xFD8 * mField_0xFCC.cos());
                f32 target1 = -(mField_0xFD8 * -mField_0xFCC.sin());
                sLib::addCalc(&mField_0xFAC, target0, 0.4f, 2.0f, 0.5f);
                sLib::addCalc(&mField_0xFB0, target1, 0.4f, 2.0f, 0.5f);

                mField_0xFD2 = mField_0xFAC;
                mField_0xFD4 = mField_0xFB0;

                if (mField_0x2792 != 0) {
                    mField_0x2792--;
                } else {
                    mbField_0x2793 = false;
                }

            } else {
                mbField_0x2793 = false;
            }
        }

    } else {
        mField_0x2790 = 0;
        if (bool0) {
            fn_256_9280();
        } else {
            fn_256_A2C0(true);
        }
    }
}

void dAcOivyRope_c::fn_256_DE80() {
    mField_0x1038 = mSegmentCount - 1;
    mField_0x101C = mPnts2[mField_0x1038];
    mField_0x102C = mField_0x1028;
    mField_0x1032 = 0x4000;
    mField_0x1039 = 2;
    mField_0x103D = 1;
}

void dAcOivyRope_c::fn_256_DEE0() {
    mField_0x101C = mPnts2[mField_0x1038];
    mField_0x102C = mField_0x1028;
    mField_0x1032 = 0x4000;
    mField_0x1039 = 4;
    mField_0x103D = 0;
}

u32 dAcOivyRope_c::fn_256_DF30() {}

void dAcOivyRope_c::fn_256_E3E0() {
    static const u8 l_some_table[] = {
        /* [0] */ 1, 10, 2,  0, 2, 40, 2,
        /* [7] */ 0, 3,  25, 2, 0, 3,
        // has 3 more zeros, probably padding
    };

    if (mStateMgr.isState(StateID_PlayerGrip)) {
        mField_0x1030 = 60;
    }

    switch (mField_0x1039) {
        case 1: {
            u32 var = fn_256_DF30();
            if (mStateMgr.isState(StateID_PlayerGrip)) {
                fn_256_DEE0();
            } else if (mField_0x103D != 0) {
                if (mField_0x103C != 0) {
                    mField_0x103C--;
                } else {
                    mField_0x103D = 0;
                    mField_0x103C = l_some_table[++mField_0x103B];
                }

            } else if (var == 0) {
                if (mField_0x103C != 0) {
                    mField_0x103C--;
                } else if (mField_0x103B >= 12) {
                    fn_256_DE80();
                } else {
                    mField_0x103C = l_some_table[++mField_0x103B];
                    mField_0x103D = 1;
                }
            }
        } break;
        case 2: {
            mField_0x1039 = fn_256_DF30();
            if (mStateMgr.isState(StateID_PlayerGrip)) {
                fn_256_DEE0();
            } else {
                if (fn_256_E790()) {
                    mField_0x103A = 0;
                    mField_0x1030 = 60;
                    mStateMgr.changeState(StateID_RopeReturn);
                    fn_256_DEE0();
                }
            }
        } break;
        case 3: {
            if (mField_0x103A == -1) {
                if (fn_256_E790()) {
                    if (mEventIdx != 0xFF && !mEvent.isThisActorInEvent()) {
                        static volatile u32 FLAGS_1 = 0x00000001;
                        static u32 FLAGS_2 = 0x00100001;
                        u32 f1 = FLAGS_1;
                        u32 f2 = FLAGS_2;
                        Event e(mEventIdx, roomid, f2 & ~f1, 0, 0);
                        mEvent.scheduleEvent(e, 0);
                    }
                }
            } else if (mField_0x103A > 0) {
                mField_0x103A--;
            } else if (mField_0x103A == 0) {
                mField_0x1030 = 60;
                mStateMgr.changeState(StateID_RopeReturn);
                fn_256_DEE0();
                playSound(0xB76 /* HANGDOWN */);
            }
        } break;
        case 4: {
            mField_0x1039 = fn_256_DF30();
        } break;
        case 5: {
            if (checkObjectProperty(0x1)) {
                mField_0x1039 = 2;
            } else {
                for (int i = 0; i < 0x23; i++) {
                    mField_0x1039 = fn_256_DF30();
                    if (mField_0x1039 != 5) {
                        break;
                    }
                }
                if (mField_0x1039 == 5) {
                    mField_0x1039 = 2;
                }
            }
        } break;
    }

    if (mField_0x1039 != 0) {
        mSph.SetC(mField_0x1010);
        dCcS::GetInstance()->Set(&mSph);
    }
}

bool dAcOivyRope_c::fn_256_E790() {
    bool ret = false;

    if (mDropped) {
        mField_0x103A = 0xF;
        ret = true;
    }

    if (mSph.ChkTgHit()) {
        mField_0x103A = 0x1;
        ret = true;
    }

    if (ret) {
        SceneflagManager::sInstance->setFlag(roomid, mWoodTagSceneflag);
    }

    return ret;
}

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
