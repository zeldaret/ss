#include "d/col/c/c_cc_d.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_m3d.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/col/c/c_m3d_g_cyl.h"
#include "d/col/c/c_m3d_g_lin.h"
#include "d/col/c/c_m3d_g_unk.h"
#include "egg/math/eggMath.h"
#include "egg/math/eggVector.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/types_nw4r.h"
#include "rvl/MTX.h" // IWYU pragma: export
#include "s/s_Math.h"

// Very Hack ??
static inline bool IsZero(f32 in) {
    return fabsf(in) <= EGG::Math<f32>::epsilon();
}
static inline void MtxTransMinusXYZ(mMtx_c &mtx, f32 x, f32 y, f32 z) {
    PSMTXTrans(mtx, -x, -y, -z);
}

mVec3_c cCcD_ShapeAttr::sVirtualCenter = mVec3_c::Zero;

cCcD_DivideInfo::cCcD_DivideInfo() {}

cCcD_DivideInfo::~cCcD_DivideInfo() {}

cCcD_DivideArea::cCcD_DivideArea() {}

cCcD_DivideArea::~cCcD_DivideArea() {}

void cCcD_DivideArea::SetArea(cM3dGAab const &aab) {
    Set(aab.mMin, aab.mMax);

    mScaledXDiff = 1.0f / 32.0f * (GetMaxP()->x - GetMinP()->x);
    mXDiffIsZero = cM3d_IsZero(mScaledXDiff);
    if (!mXDiffIsZero) {
        mInvScaledXDiff = 1.0f / mScaledXDiff;
    }

    mScaledYDiff = 1.0f / 32.0f * (GetMaxP()->y - GetMinP()->y);
    mYDiffIsZero = cM3d_IsZero(mScaledYDiff);
    if (!mYDiffIsZero) {
        mInvScaledYDiff = 1.0f / mScaledYDiff;
    }

    mScaledZDiff = 1.0f / 32.0f * (GetMaxP()->z - GetMinP()->z);
    mZDiffIsZero = cM3d_IsZero(mScaledZDiff);
    if (!mZDiffIsZero) {
        mInvScaledZDiff = 1.0f / mScaledZDiff;
    }
}

static const u32 l_base[32] = {
    0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF,
    0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF,
    0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
    0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
};

void cCcD_DivideArea::CalcDivideInfo(cCcD_DivideInfo *pDivideInfo, const cM3dGAab &aab) {
    u32 xDivInfo, yDivInfo, zDivInfo;
    if (!mXDiffIsZero) {
        s32 var1 = mInvScaledXDiff * (aab.GetMinP()->x - GetMinP()->x);
        s32 var3 = mInvScaledXDiff * (aab.GetMaxP()->x - GetMinP()->x);
        if (31 < var3) {
            var3 = 31;
        }

        xDivInfo = l_base[var3];
        if (0 < var1) {
            var1--;
            xDivInfo &= ~l_base[var1];
        }
    } else {
        xDivInfo = 0xFFFFFFFF;
    }

    if (!mYDiffIsZero) {
        s32 var1 = mInvScaledYDiff * (aab.GetMinP()->y - GetMinP()->y);
        s32 var3 = mInvScaledYDiff * (aab.GetMaxP()->y - GetMinP()->y);
        if (31 < var3) {
            var3 = 31;
        }

        yDivInfo = l_base[var3];
        if (0 < var1) {
            var1--;
            yDivInfo &= ~l_base[var1];
        }
    } else {
        yDivInfo = 0xFFFFFFFF;
    }

    if (!mZDiffIsZero) {
        s32 var1 = mInvScaledZDiff * (aab.GetMinP()->z - GetMinP()->z);
        s32 var3 = mInvScaledZDiff * (aab.GetMaxP()->z - GetMinP()->z);
        if (31 < var3) {
            var3 = 31;
        }

        zDivInfo = l_base[var3];
        if (0 < var1) {
            var1--;
            zDivInfo &= ~l_base[var1];
        }
    } else {
        zDivInfo = 0xFFFFFFFF;
    }

    pDivideInfo->Set(xDivInfo, yDivInfo, zDivInfo);
}

void cCcD_DivideArea::CalcDivideInfoOverArea(cCcD_DivideInfo *pDivideInfo, const cM3dGAab &aab) {
    u32 xDivInfo, yDivInfo, zDivInfo;
    if (!mXDiffIsZero) {
        s32 var1 = mInvScaledXDiff * (aab.GetMinP()->x - GetMinP()->x);
        s32 var3 = mInvScaledXDiff * (aab.GetMaxP()->x - GetMinP()->x);
        if (var3 < 0 || 31 < var1) {
            xDivInfo = 0;
        } else {
            if (31 < var3) {
                var3 = 31;
            }
            xDivInfo = l_base[var3];
            if (0 < var1) {
                var1--;
                xDivInfo &= ~l_base[var1];
            }
        }
    } else {
        xDivInfo = 0xFFFFFFFF;
    }

    if (!mYDiffIsZero) {
        s32 var1 = mInvScaledYDiff * (aab.GetMinP()->y - GetMinP()->y);
        s32 var3 = mInvScaledYDiff * (aab.GetMaxP()->y - GetMinP()->y);
        if (var3 < 0 || 31 < var1) {
            yDivInfo = 0;
        } else {
            if (31 < var3) {
                var3 = 31;
            }
            yDivInfo = l_base[var3];
            if (0 < var1) {
                var1--;
                yDivInfo &= ~l_base[var1];
            }
        }
    } else {
        yDivInfo = 0xFFFFFFFF;
    }

    if (!mZDiffIsZero) {
        s32 var1 = mInvScaledZDiff * (aab.GetMinP()->z - GetMinP()->z);
        s32 var3 = mInvScaledZDiff * (aab.GetMaxP()->z - GetMinP()->z);
        if (var3 < 0 || 31 < var1) {
            zDivInfo = 0;
        } else {
            if (31 < var3) {
                var3 = 31;
            }
            zDivInfo = l_base[var3];
            if (0 < var1) {
                var1--;
                zDivInfo &= ~l_base[var1];
            }
        }
    } else {
        zDivInfo = 0xFFFFFFFF;
    }

    pDivideInfo->Set(xDivInfo, yDivInfo, zDivInfo);
}

cCcD_Stts::cCcD_Stts(dAcObjBase_c *pObj) {
    mAtApid = 0;
    mAtOldApid = 0;
    mTgApid = 0;
    mTgOldApid = 0;
    mRank = 0;
    mpActor = pObj;
    ClrCcMove();
}

void cCcD_Stts::Move() {
    mAtOldApid = mAtApid;
    mAtApid = 0;
    mTgOldApid = mTgApid;
    mTgApid = 0;
}

int cCcD_Stts::GetID() const {
    if (mpActor) {
        return mpActor->getID();
    }
    return 0;
}

void cCcD_Stts::PlucCcMove(f32 x, f32 y, f32 z) {
    if (mField_0x10.x > x) {
        mField_0x10.x = x;
    } else if (mField_0x1C.x < x) {
        mField_0x1C.x = x;
    }

    if (mField_0x10.y > y) {
        mField_0x10.y = y;
    } else if (mField_0x1C.y < y) {
        mField_0x1C.y = y;
    }

    if (mField_0x10.z > z) {
        mField_0x10.z = z;
    } else if (mField_0x1C.z < z) {
        mField_0x1C.z = z;
    }

    mCcMove.x = mField_0x10.x + mField_0x1C.x;
    mCcMove.y = mField_0x10.y + mField_0x1C.y;
    mCcMove.z = mField_0x10.z + mField_0x1C.z;
}

void cCcD_Stts::ClrCcMove() {
    mField_0x10 = mVec3_c::Zero;
    mField_0x1C = mVec3_c::Zero;
    mCcMove = mVec3_c::Zero;
}

static const u32 l_ranks[3] = {75, 90, 100};

int cCcD_Stts::GetWeight(int inRank) const {
    // Current Rank is highest and in is lowest
    // - No effect
    if (mRank == 13 || inRank == 0) {
        return 0;
    }

    // Current Rank is lowest and in is highest
    // - Fill effect
    if (mRank == 0 || inRank == 13) {
        return 100;
    }
    if (mRank == inRank) {
        return 50;
    }
    if (mRank == 12 || inRank == 1) {
        return 0;
    }
    if (mRank == 1 || inRank == 12) {
        return 100;
    }

    if (inRank != 12) {
        int diff = inRank - mRank;
        int idx;
        if (diff > 0) {
            idx = 3;
            if (diff <= 3) {
                idx = diff;
            }
            return l_ranks[idx - 1];
        } else {
            idx = 3;
            if (-diff <= 3) {
                idx = -diff;
            }
            return 100 - l_ranks[idx - 1];
        }
    }
    // Shouldnt reach here...
    return mRank;
}

cCcD_GAtTgCoCommonBase::cCcD_GAtTgCoCommonBase() : mHit_cb(nullptr), mEffCounter(0), mAc(), mRPrm(0) {}

cCcD_GAtTgCoCommonBase::~cCcD_GAtTgCoCommonBase() {
    ClrActorInfo();
    mRPrm = 0;
    ClrActorInfo();
    SubtractEffCounter();
    mHit_cb = nullptr;
}

void cCcD_GAtTgCoCommonBase::ClrActorInfo() {
    mAc.unlink();
}

void cCcD_GAtTgCoCommonBase::SetHitActor(dAcObjBase_c *pActor) {
    mRPrm |= 1;
    mAc.link(pActor);
}

dAcObjBase_c *cCcD_GAtTgCoCommonBase::GetActor() {
    return mAc.get();
}

void cCcD_GAtTgCoCommonBase::SubtractEffCounter() {
    if (mEffCounter <= 0) {
        return;
    }
    mEffCounter--;
}

///////////////////////////////////////////////////////////////////////////////
// Collision Object
///////////////////////////////////////////////////////////////////////////////

cCcD_GObj::cCcD_GObj() : mStts(0), mField_0x108(0) {}

cCcD_GObj::~cCcD_GObj() {
    mStts = 0;
}

bool cCcD_GObj::fn_80328ad0(dAcObjBase_c *pObj, u32 attype) {
    bool ret = false;
    if (pObj && pObj->isActorPlayer() && attype == 2) {
        ret = true;
    }
    return ret;
}

void cCcD_GObj::ClrSet() {
    mAt.mSrc.mSPrm &= ~1;
    mTg.mSrc.mSPrm &= ~1;
    mCo.mSrc.mSPrm &= ~1;
}

void cCcD_GObj::Set(const cCcD_SrcGObj &src) {
    mAt.Set(src.mObjAt);
    mTg.Set(src.mObjTg);
    mCo.Set(src.mObjCo);
}

void cCcD_GObj::ClrAtHit() {
    mAt.mRPrm = 0;
    mAt.ClrActorInfo();
    mAt.SubtractEffCounter();
    mAt.mEffCounter = 0;
}
void cCcD_GObj::ClrTgHit() {
    mTg.mRPrm = 0;
    mTg.ClrActorInfo();
    mTg.SubtractEffCounter();
    mTg.mEffCounter = 0;
}

void cCcD_GObj::ClrCoHit() {
    mCo.mRPrm = 0;
    mCo.ClrActorInfo();
    mCo.SubtractEffCounter();
    mCo.mEffCounter = 0;
}

const mVec3_c &cCcD_GObj::GetAtHitPos() const {
    return mAt.mHitPos;
}

mVec3_c &cCcD_GObj::GetAtHitPos() {
    return mAt.mHitPos;
}

bool cCcD_GObj::GetAtFlag0x2() const {
    return mAt.MskRPrm(2);
}

bool cCcD_GObj::GetAtFlag0x4() const {
    return mAt.MskRPrm(4);
}

bool cCcD_GObj::GetAtFlag0x8() const {
    return mAt.MskRPrm(8);
}

const mVec3_c &cCcD_GObj::GetTgHitPos() const {
    return mTg.mHitPos;
}

mVec3_c &cCcD_GObj::GetTgHitPos() {
    return mTg.mHitPos;
}

bool cCcD_GObj::GetTgFlag0x4() const {
    return mTg.MskRPrm(4);
}

bool cCcD_GObj::GetTgFlag0x8() const {
    return mTg.MskRPrm(0x8);
}

cCcD_GObjInf *cCcD_GObj::GetGObjInfo() {
    return (cCcD_GObjInf *)this;
}

bool cCcD_GObj::ChkAtClawshot() const {
    return mAt.MskTgHitSPrm(0x80000);
}

bool cCcD_GObj::ChkAtClawshotDebug() const {
    return false;
}

bool cCcD_GObj::ChkAtElectrified() const {
    return mAt.MskTgHitSPrm(0x40000);
}

bool cCcD_GObj::ChkAtElectrifiedExtra() const {
    return mAt.MskTgHitSPrm(0x8000000);
}

bool cCcD_GObj::ChkAtWhippable() const {
    return mAt.MskTgHitSPrm(0x800000);
}

bool cCcD_GObj::ChkAtBit24() const {
    return mAt.MskTgHitSPrm(0x1000000);
}

bool cCcD_GObj::ChkAtArrowStick() const {
    return mAt.MskTgHitSPrm(0x2000000);
}

bool cCcD_GObj::ChkAtWaterScaleBonk() const {
    return mAt.MskTgHitSPrm(0x4000000);
}

bool cCcD_GObj::ChkAtSwordBonk() const {
    return mAt.MskTgHitSPrm(0x10000);
}

dAcObjBase_c *cCcD_GObj::GetAtActor() {
    return mAt.GetActor();
}

bool cCcD_GObj::ChkTgAtHitType(u32 mask) const {
    return mTg.mAtHitSrc.mType & mask;
}

u32 cCcD_GObj::GetTg_0x58() const {
    return mTg.mAtHitSrc.mType;
}

bool cCcD_GObj::ChkTgBit14() const {
    return mTg.MskAtHitSPrm(0x4000);
}

u8 cCcD_GObj::GetTgDamage() const {
    return mTg.mAtHitSrc.mDamage;
}

u16 cCcD_GObj::GetTgDamageFlags() const {
    return mTg.mAtHitSrc.mInfo.mModifier;
}

bool cCcD_GObj::ChkTgSkywardStrike() const {
    return mTg.MskAtHitSPrm(0x10000);
}
bool cCcD_GObj::ChkTgBit17() const {
    return mTg.MskAtHitSPrm(0x20000);
}

bool cCcD_GObj::ChkTgBit18() const {
    return mTg.MskAtHitSPrm(0x40000);
}

bool cCcD_GObj::ChkTgBit19() const {
    return mTg.MskAtHitSPrm(0x80000);
}

bool cCcD_GObj::ChkTgBit23() const {
    return mTg.MskAtHitSPrm(0x800000);
}

bool cCcD_GObj::ChkTgBit20() const {
    return mTg.MskAtHitSPrm(0x100000);
}

bool cCcD_GObj::ChkTgBit24() const {
    return mTg.MskAtHitSPrm(0x1000000);
}

bool cCcD_GObj::ChkTgBit25() const {
    return mTg.MskAtHitSPrm(0x2000000);
}

u16 cCcD_GObj::GetTgSoundID() const {
    return mTg.mAtHitSrc.mField_0x10;
}

s16 cCcD_GObj::GetTg_0x6A() const {
    return mTg.mAtHitSrc.mField_0x12;
}

bool cCcD_GObj::ChkTgBit8() const {
    return mTg.MskAtHitSPrm(0x100);
}

u8 cCcD_GObj::GetTg_0x4A() const {
    return mTg.mField_0x4A;
}

dAcObjBase_c *cCcD_GObj::GetTgActor() {
    return mTg.GetActor();
}

const mVec3_c &cCcD_GObj::GetTg_0x2C() const {
    return mTg.mField_0x2C;
}

dAcObjBase_c *cCcD_GObj::GetCoActor() {
    return mCo.GetActor();
}

bool cCcD_GObj::ChkCoBit4() const {
    return mCo.mCoHitSrc.mSPrm >> 4 & 1;
}

void cCcD_GObj::SetAtFlagsUpper(u32 flags) {
    mAt.OffSPrm(0x3FF0000);
    mAt.OnSPrm(flags);
}

bool cCcD_GObj::ChkTgBit1() const {
    return mTg.MskRPrm(2);
}

void cCcD_GObj::AdjustHitPos(f32 x, f32 z) {
    mAt.AdjustHitPos(x, z);
    mTg.AdjustHitPos(x, z);
    mCo.AdjustHitPos(x, z);
}

///////////////////////////////////////////////////////////////////////////////
// Base Shape Attribute
///////////////////////////////////////////////////////////////////////////////

cCcD_ShapeAttr::cCcD_ShapeAttr() {}

cCcD_ShapeAttr::~cCcD_ShapeAttr() {}

bool cCcD_ShapeAttr::Calc(const mVec3_c &, const mVec3_c &, mVec3_c *pOut) {
    return false;
}

void cCcD_ShapeAttr::GetShapeAccess(cCcD_ShapeAttr::Shape *pShape) const {
    pShape->mField_0x00 = 4;
    pShape->mField_0x04 = mVec3_c::Zero;
    pShape->mField_0x10 = mVec3_c::Zero;
    pShape->mField_0x1C = mVec3_c::Zero;
}

bool cCcD_ShapeAttr::GetPosBool(mVec3_c *pOut) const {
    return false;
}

bool cCcD_ShapeAttr::CrossAtTg(cCcD_ShapeAttr &shapeAttr, mVec3_c *pOut) {
    return false;
}

bool cCcD_ShapeAttr::CrossCo(cCcD_ShapeAttr &shapeAttr, f32 *pOut) {
    return false;
}

mVec3_c &cCcD_ShapeAttr::GetCoP() {
    return sVirtualCenter;
}

///////////////////////////////////////////////////////////////////////////////
// Triangle Shape Attribute
///////////////////////////////////////////////////////////////////////////////

cCcD_TriAttr::cCcD_TriAttr() {}

cCcD_TriAttr::~cCcD_TriAttr() {}

bool cCcD_TriAttr::Calc(const mVec3_c &start, const mVec3_c &end, mVec3_c *pOut) {
    bool bVar1 = false;
    mVec3_c tmp0, tmp1;
    if (!CrossInfLin(start, end, *pOut)) {
        bVar1 = true;

    } else {
        mVec3_c a = GetPntA() - *pOut;
        mVec3_c b = GetPntB() - *pOut;
        mVec3_c c = GetPntC() - *pOut;

        mVec3_c a_x_b = a.cross(b);
        mVec3_c b_x_c = b.cross(c);
        mVec3_c c_x_a = c.cross(a);

        f32 dot0 = a_x_b.dot(b_x_c);
        f32 dot1 = b_x_c.dot(c_x_a);
        f32 dot2 = c_x_a.dot(a_x_b);

        if (!(dot0 > 0.0f && dot1 > 0.0f && dot2 > 0.0f)) {
            if (dot0 <= 0.f && dot1 <= 0.0f && dot2 <= 0.0f) {
                bVar1 = true;
            } else if (dot0 > 0.0f) {
                tmp0 = GetPntA();
                tmp1 = GetPntC();
            } else if (dot1 > 0.0f) {
                tmp0 = GetPntA();
                tmp1 = GetPntB();
            } else {
                tmp0 = GetPntC();
                tmp1 = GetPntB();
            }
            mVec3_c tmp2 = tmp1 - tmp0;
            f32 mag = tmp2.squaredLength();
            if (IsZero(mag)) {
                bVar1 = true;
            } else {
                mag = tmp2.dot(*pOut - tmp0) / mag;
                f32 scale = mag < 0.0f ? 0.0f : mag > 1.0f ? 1.0f : mag;
                tmp2 *= scale;
                *pOut = tmp0 + tmp2;
            }
        }
    }
    if (bVar1) {
        *pOut = (GetPntA() + GetPntB() + GetPntC()) * (1.f / 3.f);
    }
}

bool cCcD_TriAttr::CrossAtTg(cCcD_ShapeAttr &shapeAttr, mVec3_c *pOut) {
    return shapeAttr.CrossAtTg(*this, pOut);
}

bool cCcD_TriAttr::CrossAtTg(cCcD_CpsAttr &cpsAttr, mVec3_c *pOut) {
    return cM3d_Cross_CpsTri(cpsAttr, *this, pOut);
}

bool cCcD_TriAttr::CrossAtTg(cCcD_CylAttr &cylAttr, mVec3_c *pOut) {
    return cM3d_Cross_CylTri(cylAttr, *this, pOut);
}

bool cCcD_TriAttr::CrossAtTg(cCcD_SphAttr &sphAttr, mVec3_c *pOut) {
    return cM3d_Cross_SphTri(sphAttr, *this, pOut, nullptr, nullptr);
}

bool cCcD_TriAttr::CrossAtTg(cCcD_TriAttr &triAttr, mVec3_c *pOut) {
    return cM3d_Cross_TriTri(*this, triAttr, pOut);
}

bool cCcD_TriAttr::CrossAtTg(cCcD_UnkAttr &unkAttr, mVec3_c *pOut) {
    return cM3d_Cross_UnkTri(unkAttr, *this, pOut);
}

bool cCcD_TriAttr::CrossCo(cCcD_ShapeAttr &shapeAttr, f32 *pOut) {
    return shapeAttr.CrossCo(*this, pOut);
}

bool cCcD_TriAttr::CrossCo(cCcD_CpsAttr &cpsAttr, f32 *pOut) {
    return false;
}

bool cCcD_TriAttr::CrossCo(cCcD_TriAttr &triAttr, f32 *pOut) {
    return false;
}

bool cCcD_TriAttr::CrossCo(cCcD_UnkAttr &unkAttr, f32 *pOut) {
    return false;
}

bool cCcD_TriAttr::CrossCo(cCcD_CylAttr &cylAttr, f32 *pOut) {
    return false;
}

bool cCcD_TriAttr::CrossCo(cCcD_SphAttr &sphAttr, f32 *pOut) {
    return false;
}

void cCcD_TriAttr::CalcAabBox() {
    mVec3_c a, b, c;
    mAab.ClearForMinMax();

    a = GetPntA();
    mAab.SetMinMax(a);

    b = GetPntB();
    mAab.SetMinMax(b);

    c = GetPntC();
    mAab.SetMinMax(c);
}

bool cCcD_TriAttr::GetNVec(const mVec3_c &pnt, mVec3_c *pOut) const {
    if (getPlaneFunc(pnt) >= 0.f) {
        *pOut = GetN();
    } else {
        *pOut = GetN();
        PSVECScale(*pOut, *pOut, -1.0f);
    }
    return true;
}

void cCcD_TriAttr::GetShapeAccess(cCcD_ShapeAttr::Shape *pShape) const {
    pShape->mField_0x00 = SHAPE_TRIANGLE;
    pShape->mField_0x04 = GetPntA();
    pShape->mField_0x10 = GetPntB();
    pShape->mField_0x1C = GetPntC();
}

void cCcD_TriAttr::GetPos(nw4r::math::VEC3 *pOut) const {
    *pOut = GetPntA();
}

void cCcD_TriAttr::TranslateXZ(f32 x, f32 z) {
    mA.x += x;
    mA.z += z;
    mB.x += x;
    mB.z += z;
    mC.x += x;
    mC.z += z;
}

///////////////////////////////////////////////////////////////////////////////
// Unk Shape Attribute
///////////////////////////////////////////////////////////////////////////////

cCcD_UnkAttr::cCcD_UnkAttr() {}

cCcD_UnkAttr::~cCcD_UnkAttr() {}

bool cCcD_UnkAttr::Calc(const mVec3_c &start, const mVec3_c &end, mVec3_c *pOut) {
    mVec3_c tmp0, tmp1;
    const mMtx_c &inv = mInvMtx;
    PSMTXMultVec(inv, start, tmp0);
    PSMTXMultVec(inv, end, tmp1);
    for (int i = 0; i < 3; i++) {
        bool tmp0Min = tmp0(i) < mMin(i);
        bool tmp0Max = tmp0(i) > mMax(i);

        bool tmp1Min = tmp1(i) < mMin(i);
        bool tmp1Max = tmp1(i) > mMax(i);

        if (tmp0Min) {
            if (tmp1Max) {
                f32 t = (tmp0(i) + tmp1(i)) * 0.5f;
                (*pOut)(i) = t < mMin(i) ? mMin(i) : t > mMax(i) ? mMax(i) : t;

            } else {
                (*pOut)(i) = mMin(i);
            }
        } else if (tmp0Max) {
            if (tmp1Min) {
                f32 t = (tmp0(i) + tmp1(i)) * 0.5f;
                (*pOut)(i) = t < mMin(i) ? mMin(i) : t > mMax(i) ? mMax(i) : t;

            } else {
                (*pOut)(i) = mMax(i);
            }
        } else if (tmp1Min) {
            (*pOut)(i) = mMin(i);
        } else if (tmp1Max) {
            (*pOut)(i) = mMax(i);
        } else {
            (*pOut)(i) = (tmp0(i) + tmp1(i)) * 0.5f;
        }
    }
    PSMTXMultVec(mMtx, *pOut, *pOut);
    return true;
}

bool cCcD_UnkAttr::CrossAtTg(cCcD_ShapeAttr &shapeAttr, mVec3_c *pOut) {
    return shapeAttr.CrossAtTg(*this, pOut);
}

bool cCcD_UnkAttr::CrossAtTg(cCcD_CpsAttr &cpsAttr, mVec3_c *pOut) {
    return Cross(cpsAttr, pOut);
}

bool cCcD_UnkAttr::CrossAtTg(cCcD_TriAttr &triAttr, mVec3_c *pOut) {
    return Cross(triAttr, pOut);
}

bool cCcD_UnkAttr::CrossAtTg(cCcD_UnkAttr &unkAttr, mVec3_c *pOut) {
    return Cross(unkAttr, pOut);
}

bool cCcD_UnkAttr::CrossAtTg(cCcD_CylAttr &cylAttr, mVec3_c *pOut) {
    return Cross(cylAttr, pOut);
}

bool cCcD_UnkAttr::CrossAtTg(cCcD_SphAttr &sphAttr, mVec3_c *pOut) {
    return Cross(sphAttr, pOut);
}

bool cCcD_UnkAttr::CrossCo(cCcD_ShapeAttr &shapeAttr, f32 *pOut) {
    return shapeAttr.CrossCo(*this, pOut);
}

bool cCcD_UnkAttr::CrossCo(cCcD_CpsAttr &cpsAttr, f32 *pOut) {
    mVec3_c pos;
    if (cM3dGUnk::Cross(cpsAttr, &pos)) {
        cpsAttr.SetVirtCenter(&pos);
        *pOut = cpsAttr.CrossCoCommon(*this, mField_0xC4);
        mField_0xC4 = 0.0f;
    }
    return false;
}

bool cCcD_UnkAttr::CrossCo(cCcD_TriAttr &triAttr, f32 *pOut) {
    return false;
}

bool cCcD_UnkAttr::CrossCo(cCcD_UnkAttr &unkAttr, f32 *pOut) {
    return false;
}

bool cCcD_UnkAttr::CrossCo(cCcD_CylAttr &cylAttr, f32 *pOut) {
    return cM3dGUnk::Cross(cylAttr, pOut);
}

bool cCcD_UnkAttr::CrossCo(cCcD_SphAttr &sphAttr, f32 *pOut) {
    return cM3dGUnk::Cross(sphAttr, pOut);
}

void cCcD_UnkAttr::CalcAabBox() {
    mAab.ClearForMinMax();
    mVec3_c start, end;
    GetStartEnd(start, end);
    mMtx_c &mtx = mMtx;
    mVec3_c a;

    a = start;
    PSMTXMultVec(mtx, a, a);
    mAab.SetMinMax(a);

    a = end;
    PSMTXMultVec(mtx, a, a);
    mAab.SetMinMax(a);

    a.x = start.x;
    a.y = start.y;
    a.z = end.z;
    PSMTXMultVec(mtx, a, a);
    mAab.SetMinMax(a);

    a.x = end.x;
    a.y = end.y;
    a.z = start.z;
    PSMTXMultVec(mtx, a, a);
    mAab.SetMinMax(a);
}

bool cCcD_UnkAttr::GetNVec(const mVec3_c &, mVec3_c *pOut) const {
    pOut->set(0.f, 0.f, 0.f);
    return false;
}

void cCcD_UnkAttr::GetPos(nw4r::math::VEC3 *pPos) const {
    *pPos = mLin.GetStart();
}

void cCcD_UnkAttr::Set(const cCcD_SrcUnkAttr &src) {
    mVec3_c min = src.getMin();
    mVec3_c max = src.getMax();

    cM3dGUnk::Set(min, max);
}

mVec3_c &cCcD_UnkAttr::GetCoP() {
    return mVirtCenter;
}

void cCcD_UnkAttr::TranslateXZ(f32 x, f32 z) {
    cM3dGUnk::fn_80338f30(x, z);
}

///////////////////////////////////////////////////////////////////////////////
// Capsule Shape Attribute
///////////////////////////////////////////////////////////////////////////////

cCcD_CpsAttr::cCcD_CpsAttr() {}

cCcD_CpsAttr::~cCcD_CpsAttr() {}

void cCcD_CpsAttr::Set(const cCcD_SrcCpsAttr &src) {
    cM3dGCps::SetR(src.mRadius);
    // I dont like this
    nw4r::math::VEC3 a;
    a.z = 0.0f;
    a.y = 0.0f;
    a.x = 0.0f;
    GetEnd() = GetStart() = a;
}

mVec3_c &cCcD_CpsAttr::GetCoP() {
    return mVirtCenter;
}

bool cCcD_CpsAttr::CrossAtTg(cCcD_ShapeAttr &shapeAttr, mVec3_c *pOut) {
    return shapeAttr.CrossAtTg(*this, pOut);
}

bool cCcD_CpsAttr::CrossCo(cCcD_ShapeAttr &shapeAttr, f32 *pOut) {
    return shapeAttr.CrossCo(*this, pOut);
}

bool cCcD_CpsAttr::CrossCo(cCcD_TriAttr &triAttr, f32 *pOut) {
    return false;
}

void CalcCyl(const cM3dGCyl &cyl, const mVec3_c &start, const mVec3_c &end, mVec3_c *pOut) {
    cM3dGLin lin;
    mVec3_c pos;
    lin.Set(start, end);

    f32 radius = cyl.GetR();
    f32 height = cyl.GetC().y + cyl.GetH();

    if (cM3d_Cross_CylLin(cyl, lin, *pOut, pos)) {
        return;
    }

    if (height < start.y && height < end.y) {
        pOut->y = height;
    } else if (cyl.GetC().y > start.y && cyl.GetC().y > end.y) {
        pOut->y = cyl.GetC().y;
    } else {
        bool startValid = sLib::isInRange(start.y, cyl.GetC().y, height);
        bool endValid = sLib::isInRange(end.y, cyl.GetC().y, height);
        if (startValid && endValid) {
            pOut->y = (start.y + end.y) * 0.5f;
        } else if (startValid) {
            pOut->y = start.y;
        } else {
            pOut->y = end.y;
        }
    }
    mVec3_c start_center = start - cyl.GetC();
    f32 start_dist = EGG::Math<f32>::sqrt(start_center.x * start_center.x + start_center.z * start_center.z);

    mVec3_c end_center = end - cyl.GetC();
    f32 end_dist = EGG::Math<f32>::sqrt(end_center.x * end_center.x + end_center.z * end_center.z);

    if (start_dist < radius) {
        if (end_dist < radius) {
            pOut->x = (start.x + end.x) * 0.5f;
            pOut->z = (start.z + end.z) * 0.5f;
        } else {
            pOut->x = start.x;
            pOut->z = start.z;
        }
    } else if (end_dist < radius) {
        pOut->x = end.x;
        pOut->z = end.z;
    } else if (start_dist < end_dist) {
        start_center.normalize();
        start_center *= radius;
        pOut->x = start_center.x + cyl.GetC().x;
        pOut->z = start_center.z + cyl.GetC().z;
    } else {
        end_center.normalize();
        end_center *= radius;
        pOut->x = end_center.x + cyl.GetC().x;
        pOut->z = end_center.z + cyl.GetC().z;
    }
}

bool cCcD_CpsAttr::Calc(const mVec3_c &start, const mVec3_c &end, mVec3_c *pOut) {
    const mVec3_c &startRef = GetStart();
    const mVec3_c &endRef = GetEnd();

    mMtx_c mtx0;
    mVec3_c tmp0 = GetEnd() - GetStart();
    mVec3_c tmp1;
    mVec3_c tmp2;
    mVec3_c tmp3;
    mVec3_c tmp4;
    cM3dGCyl cyl;
    bool bInverse;

    f32 dist = EGG::Math<f32>::sqrt(tmp0.x * tmp0.x + tmp0.z * tmp0.z);
    if (IsZero(dist)) {
        tmp1 = start;
        tmp2 = end;
        if (endRef.y > startRef.y) {
            tmp3 = startRef;
            tmp4 = endRef;
        } else {
            tmp3 = endRef;
            tmp4 = startRef;
        }
        bInverse = false;
    } else {
        PSMTXTrans(mtx0, startRef.x, startRef.y, startRef.z);
        mtx0.XrotM(mAng::atan2s(-dist, tmp0.y));
        mtx0.YrotM(mAng::atan2s(-tmp0.x, tmp0.z));

        mMtx_c mtx1;

        MtxTransMinusXYZ(mtx1, startRef.x, startRef.y, startRef.z);
        PSMTXConcat(mtx0, mtx1, mtx0);
        PSMTXMultVec(mtx0, start, tmp1);
        PSMTXMultVec(mtx0, end, tmp2);
        tmp3 = GetStart();
        PSMTXMultVec(mtx0, GetEnd(), tmp4);
        bInverse = true;
    }
    tmp3.y -= GetR() * 0.5f;
    tmp4.y += GetR() * 0.5f;
    cyl.SetC(tmp3, GetR(), tmp4.y - tmp3.y);
    CalcCyl(cyl, tmp1, tmp2, pOut);
    if (bInverse) {
        PSMTXInverse(mtx0, mtx0);
        PSMTXMultVec(mtx0, *pOut, *pOut);
    }
    return true;
}

bool cCcD_CpsAttr::CrossAtTg(cCcD_CpsAttr &cpsAttr, mVec3_c *pOut) {
    return cM3d_Cross_CpsCps(*this, cpsAttr, pOut, nullptr, nullptr);
}

bool cCcD_CpsAttr::CrossAtTg(cCcD_CylAttr &cylAttr, mVec3_c *pOut) {
    return cM3d_Cross_CpsCyl(*this, cylAttr, pOut);
}

bool cCcD_CpsAttr::CrossAtTg(cCcD_SphAttr &sphAttr, mVec3_c *pOut) {
    return cM3d_Cross_CpsSph(*this, sphAttr, pOut);
}

bool cCcD_CpsAttr::CrossAtTg(cCcD_TriAttr &triAttr, mVec3_c *pOut) {
    return cM3d_Cross_CpsTri(*this, triAttr, pOut);
}

bool cCcD_CpsAttr::CrossAtTg(cCcD_UnkAttr &unkAttr, mVec3_c *pOut) {
    return cM3dGCps::Cross(unkAttr, pOut);
}

void cCcD_CpsAttr::SetVirtCenter(mVec3_c *pOut) {
    if (mField_0x20) {
        mVirtCenter = *pOut;
        mField_0x20 = 0;
    } else {
        f32 dist = cM3d_lineVsPosSuisenCross(*this, *pOut, &mVirtCenter);
        if (dist < 0.f) {
            mVirtCenter = GetStart();
        } else if (dist > 1.f) {
            mVirtCenter = GetEnd();
        }
    }
}

f32 cCcD_CpsAttr::CrossCoCommon(cCcD_ShapeAttr &attr, f32 r) {
    mVec3_c tmp = mVirtCenter - attr.GetCoP();
    f32 mag = (r + mField_0x1C) - EGG::Math<f32>::sqrt(tmp.x * tmp.x + tmp.z * tmp.z);

    if (mag < 0.0f) {
        mag = 0.0f;
    }
    Set_0x1C(mRadius);
    return mag;
}

bool cCcD_CpsAttr::CrossCo(cCcD_CpsAttr &cpsAttr, f32 *pOut) {
    mVec3_c pos;
    if (cM3d_Cross_CpsCps(*this, cpsAttr, &pos, nullptr, nullptr)) {
        SetVirtCenter(&pos);
        cpsAttr.SetVirtCenter(&pos);
        *pOut = CrossCoCommon(cpsAttr, cpsAttr.mField_0x1C);
        cpsAttr.Set_0x1C(cpsAttr.GetR());
        return true;
    }
    return false;
}

bool cCcD_CpsAttr::CrossCo(cCcD_CylAttr &cylAttr, f32 *pOut) {
    mVec3_c pos;
    if (cM3d_Cross_CpsCyl(*this, cylAttr, &pos)) {
        SetVirtCenter(&pos);
        *pOut = CrossCoCommon(cylAttr, cylAttr.GetR());
        return true;
    }
    return false;
}

bool cCcD_CpsAttr::CrossCo(cCcD_SphAttr &sphAttr, f32 *pOut) {
    mVec3_c pos;
    if (cM3d_Cross_CpsSph(*this, sphAttr, &pos)) {
        SetVirtCenter(&pos);
        *pOut = CrossCoCommon(sphAttr, sphAttr.GetYDist(pos.y));

        return true;
    }
    return false;
}

bool cCcD_CpsAttr::CrossCo(cCcD_UnkAttr &unkAttr, f32 *pOut) {
    mVec3_c pos;
    if (cM3dGCps::Cross(unkAttr, &pos)) {
        SetVirtCenter(&pos);
        *pOut = CrossCoCommon(unkAttr, unkAttr.mField_0xC4);
        unkAttr.mField_0xC4 = 0.0f;
        return true;
    }
    return false;
}

void cCcD_CpsAttr::CalcAabBox() {
    mAab.ClearForMinMax();
    mAab.SetMinMax(GetStart());
    mAab.SetMinMax(GetEnd());
    mAab.PlusR(GetR());
}

bool cCcD_CpsAttr::GetNVec(const mVec3_c &param0, mVec3_c *pOut) const {
    mVec3_c tmp1;
    PSVECSubtract(GetEnd(), GetStart(), tmp1);
    f32 sqMag = PSVECDotProduct(tmp1, tmp1);
    if (cM3d_IsZero(sqMag)) {
        return false;
    }

    mVec3_c tmp2;
    PSVECSubtract(param0, GetStart(), tmp2);
    f32 sqDist = PSVECDotProduct(tmp2, tmp1) / sqMag;
    mVec3_c tmp3;
    if (sqDist < 0.f) {
        tmp3 = GetStart();
    } else if (sqDist > 1.f) {
        tmp3 = GetEnd();
    } else {
        PSVECScale(tmp1, tmp1, sqDist);
        PSVECAdd(tmp1, GetStart(), tmp3);
    }

    PSVECSubtract(param0, tmp3, *pOut);
    if (cM3d_IsZero(PSVECMag(*pOut))) {
        pOut->set(0.f, 0.f, 0.f);
        return false;
    } else {
        PSVECNormalize(*pOut, *pOut);
        return true;
    }
}

void cCcD_CpsAttr::GetPos(nw4r::math::VEC3 *pOut) const {
    *pOut = GetStart();
}

void cCcD_CpsAttr::GetShapeAccess(cCcD_ShapeAttr::Shape *pShape) const {
    pShape->mField_0x00 = SHAPE_CAPSULE;
    pShape->mField_0x04 = GetStart();
    pShape->mField_0x10 = GetEnd();
    pShape->mField_0x1C.set(GetR(), 0.f, 0.f);
}

void cCcD_CpsAttr::TranslateXZ(f32 x, f32 z) {
    GetStart().x += x;
    GetStart().z += z;

    GetEnd().x += x;
    GetEnd().z += z;
}

///////////////////////////////////////////////////////////////////////////////
// Cylinder Shape Attribute
///////////////////////////////////////////////////////////////////////////////

cCcD_CylAttr::cCcD_CylAttr() {}

cCcD_CylAttr::~cCcD_CylAttr() {}

void cCcD_CylAttr::Set(const cCcD_SrcCylAttr &src) {
    SetR(src.mRadius);
    SetH(src.mHeight);
    mVec3_c a;
    a.z = 0.0f;
    a.y = 0.0f;
    a.x = 0.0f;
    SetC(a);
}

mVec3_c &cCcD_CylAttr::GetCoP() {
    return mCenter;
}

bool cCcD_CylAttr::Calc(const mVec3_c &param0, const mVec3_c &param1, mVec3_c *pOut) {
    CalcCyl(*this, param0, param1, pOut);
    return true;
}

bool cCcD_CylAttr::CrossAtTg(cCcD_ShapeAttr &shapeAttr, mVec3_c *pOut) {
    return shapeAttr.CrossAtTg(*this, pOut);
}

bool cCcD_CylAttr::CrossAtTg(cCcD_CpsAttr &cpsAttr, mVec3_c *pOut) {
    return cM3d_Cross_CpsCyl(cpsAttr, *this, pOut);
}

bool cCcD_CylAttr::CrossAtTg(cCcD_CylAttr &cylAttr, mVec3_c *pOut) {
    return cM3d_Cross_CylCyl(*this, cylAttr, pOut);
}

bool cCcD_CylAttr::CrossAtTg(cCcD_SphAttr &sphAttr, mVec3_c *pOut) {
    f32 f;
    return cM3d_Cross_CylSph(*this, sphAttr, pOut, &f);
}

bool cCcD_CylAttr::CrossAtTg(cCcD_TriAttr &triAttr, mVec3_c *pOut) {
    return cM3d_Cross_CylTri(*this, triAttr, pOut);
}

bool cCcD_CylAttr::CrossAtTg(cCcD_UnkAttr &unkAttr, mVec3_c *pOut) {
    return cM3dGCyl::Cross(unkAttr, pOut);
}

bool cCcD_CylAttr::CrossCo(cCcD_ShapeAttr &shapeAttr, f32 *pOut) {
    return shapeAttr.CrossCo(*this, pOut);
}

bool cCcD_CylAttr::CrossCo(cCcD_TriAttr &triAttr, f32 *pOut) {
    return false;
}

bool cCcD_CylAttr::CrossCo(cCcD_CylAttr &cylAttr, f32 *pOut) {
    return cM3d_Cross_CylCyl(*this, cylAttr, pOut);
}

bool cCcD_CylAttr::CrossCo(cCcD_SphAttr &sphAttr, f32 *pOut) {
    return cM3d_Cross_CylSph(*this, sphAttr, pOut);
}

bool cCcD_CylAttr::CrossCo(cCcD_CpsAttr &cpsAttr, f32 *pOut) {
    mVec3_c pos;
    if (cM3d_Cross_CpsCyl(cpsAttr, *this, &pos)) {
        cpsAttr.SetVirtCenter(&pos);
        *pOut = cpsAttr.CrossCoCommon(*this, GetR());
        return true;
    }
    return false;
}

bool cCcD_CylAttr::CrossCo(cCcD_UnkAttr &unkAttr, f32 *pOut) {
    cM3dGCyl::Cross(unkAttr, pOut);
}

void cCcD_CylAttr::CalcAabBox() {
    mVec3_c max;
    mVec3_c min;
    min.x = GetC().x - GetR();
    min.y = GetC().y;
    min.z = GetC().z - GetR();

    max.x = GetC().x + GetR();
    max.y = GetC().y + GetH();
    max.z = GetC().z + GetR();
    mAab.Set(min, max);
}

bool cCcD_CylAttr::GetNVec(const mVec3_c &param0, mVec3_c *pOut) const {
    mVec3_c vec;
    if (GetC().y > param0.y) {
        vec = GetC();
    } else if (GetC().y + GetH() < param0.y) {
        vec = GetC();
        vec.y = GetC().y + GetH();

    } else {
        vec = GetC();
        vec.y = param0.y;
    }
    PSVECSubtract(param0, vec, *pOut);
    if (cM3d_IsZero(PSVECMag(*pOut))) {
        pOut->set(0.f, 0.f, 0.f);
        return false;
    } else {
        PSVECNormalize(*pOut, *pOut);
        return true;
    }
}

void cCcD_CylAttr::GetShapeAccess(cCcD_ShapeAttr::Shape *pShape) const {
    pShape->mField_0x00 = SHAPE_CYLINDER;
    pShape->mField_0x04 = GetC();
    pShape->mField_0x10 = mVec3_c::Zero;
    pShape->mField_0x1C.set(GetR(), GetH(), 0.f);
}

void cCcD_CylAttr::GetPos(nw4r::math::VEC3 *pOut) const {
    *pOut = GetC();
}

void cCcD_CylAttr::TranslateXZ(f32 x, f32 z) {
    GetC().x += x;
    GetC().z += z;
}

///////////////////////////////////////////////////////////////////////////////
// Sphere Shape Attribute
///////////////////////////////////////////////////////////////////////////////

cCcD_SphAttr::cCcD_SphAttr() {}

cCcD_SphAttr::~cCcD_SphAttr() {}

void cCcD_SphAttr::Set(const cCcD_SrcSphAttr &src) {
    SetR(src.mRadius);
    mVec3_c a;
    a.z = 0.0f;
    a.y = 0.0f;
    a.x = 0.0f;
    SetC(&a);
}

mVec3_c &cCcD_SphAttr::GetCoP() {
    return mCenter;
}

bool cCcD_SphAttr::Calc(const mVec3_c &param0, const mVec3_c &param1, mVec3_c *pOut) {
    cM3dGLin lin;
    mVec3_c vec;
    lin.Set(param0, param1);

    if (!cM3d_Cross_LinSph_CrossPos(*this, lin, pOut, &vec)) {
        mVec3_c a = (param0 + param1) * 0.5f;
        f32 f;
        cM3d_Len3dSqPntAndSegLine(lin, GetC(), &a, &f, nullptr);
        a -= GetC();
        a.normalize();
        f32 radius = GetR();
        *pOut = GetC() + a * radius;
    }
    return true;
}

bool cCcD_SphAttr::CrossAtTg(cCcD_ShapeAttr &shapeAttr, mVec3_c *pOut) {
    return shapeAttr.CrossAtTg(*this, pOut);
}

bool cCcD_SphAttr::CrossAtTg(cCcD_CpsAttr &cpsAttr, mVec3_c *pOut) {
    return cM3d_Cross_CpsSph(cpsAttr, *this, pOut);
}

bool cCcD_SphAttr::CrossAtTg(cCcD_CylAttr &cylAttr, mVec3_c *pOut) {
    f32 f;
    return cM3d_Cross_CylSph(cylAttr, *this, pOut, &f);
}

bool cCcD_SphAttr::CrossAtTg(cCcD_SphAttr &sphAttr, mVec3_c *pOut) {
    return cM3d_Cross_SphSph(sphAttr, *this, pOut);
}

bool cCcD_SphAttr::CrossAtTg(cCcD_TriAttr &triAttr, mVec3_c *pOut) {
    return cM3d_Cross_SphTri(*this, triAttr, pOut, nullptr, nullptr);
}

bool cCcD_SphAttr::CrossAtTg(cCcD_UnkAttr &unkAttr, mVec3_c *pOut) {
    return cM3dGSph::Cross(unkAttr, pOut);
}

bool cCcD_SphAttr::CrossCo(cCcD_ShapeAttr &shapeAttr, f32 *pOut) {
    return shapeAttr.CrossCo(*this, pOut);
}

bool cCcD_SphAttr::CrossCo(cCcD_TriAttr &triAttr, f32 *pOut) {
    return false;
}

bool cCcD_SphAttr::CrossCo(cCcD_CylAttr &cylAttr, f32 *pOut) {
    return cM3d_Cross_CylSph(cylAttr, *this, pOut);
}

bool cCcD_SphAttr::CrossCo(cCcD_SphAttr &sphAttr, f32 *pOut) {
    return cM3d_Cross_SphSph(*this, sphAttr, pOut);
}

bool cCcD_SphAttr::CrossCo(cCcD_CpsAttr &cpsAttr, f32 *pOut) {
    mVec3_c pos;
    if (cM3d_Cross_CpsSph(cpsAttr, *this, &pos)) {
        cpsAttr.SetVirtCenter(&pos);
        *pOut = cpsAttr.CrossCoCommon(*this, cM3dGSph::GetYDist(pos.y));
        return true;
    }
    return false;
}

bool cCcD_SphAttr::CrossCo(cCcD_UnkAttr &unkAttr, f32 *pOut) {
    return cM3dGSph::Cross(unkAttr, pOut);
}

void cCcD_SphAttr::CalcAabBox() {
    mAab.Set(*this);
}

bool cCcD_SphAttr::GetNVec(const mVec3_c &param0, mVec3_c *param1) const {
    param1->x = param0.x - GetC().x;
    param1->y = param0.y - GetC().y;
    param1->z = param0.z - GetC().z;
    if (cM3d_IsZero(PSVECMag(*param1))) {
        param1->set(0.0f, 0.0f, 0.0f);
        return false;
    } else {
        PSVECNormalize(*param1, *param1);
        return true;
    }
}

void cCcD_SphAttr::GetShapeAccess(cCcD_ShapeAttr::Shape *pShape) const {
    pShape->mField_0x00 = SHAPE_SPHERE;
    pShape->mField_0x04 = GetC();
    pShape->mField_0x10 = mVec3_c::Zero;
    pShape->mField_0x1C.set(GetR(), 0.0f, 0.0f);
}

void cCcD_SphAttr::GetPos(nw4r::math::VEC3 *pPos) const {
    *pPos = GetC();
}

bool cCcD_SphAttr::GetPosBool(mVec3_c *pPos) const {
    *pPos = GetC();
    return true;
}

void cCcD_SphAttr::TranslateXZ(f32 x, f32 z) {
    GetC().x += x;
    GetC().z += z;
}

///////////////////////////////////////////////////////////////////////////////
// Attack/Defense/Collide Object Info
///////////////////////////////////////////////////////////////////////////////

cCcD_GObjAt::cCcD_GObjAt() {
    mField_0x58 = 0;
}

cCcD_GObjAt::~cCcD_GObjAt() {}

void cCcD_GObjAt::Set(const cCcD_SrcGObjAt &info) {
    mEffCounter = 0;
    mSrc = info;
    mHitPos = mVec3_c::Zero;
}

void cCcD_GObjAt::SetAtFlag(u32 flag) {
    mSrc.mSPrm = mSrc.mSPrm & ~0x3E | flag;
}

void cCcD_GObjAt::AdjustHitPos(f32 x, f32 z) {
    mHitPos.x += x;
    mHitPos.z += z;
}

cCcD_GObjTg::cCcD_GObjTg()
    : mShieldFrontRangeYAngle(nullptr), mField_0x4A(0), mField_0x4B(0), mField_0x4C(0), mField_0x50(0), mField_0x54(0),
      mField_0x6C(0.f, 0.f, 0.f), mField_0x78(0) {}

cCcD_GObjTg::~cCcD_GObjTg() {}

void cCcD_GObjTg::Set(const cCcD_SrcGObjTg &src) {
    mEffCounter = 0;
    mSrc = src;
    mField_0x4C = 0;
    mHitPos = mVec3_c::Zero;
    mShieldFrontRangeYAngle = nullptr;
    mShieldRange = 0x4000;
}

void cCcD_GObjTg::AdjustHitPos(f32 x, f32 z) {
    mHitPos.x += x;
    mHitPos.z += z;

    mField_0x6C.x += x;
    mField_0x6C.z += z;
}

cCcD_GObjCo::cCcD_GObjCo() {
    mField_0x20 = 0;
    mField_0x28_callback = 0;
}

cCcD_GObjCo::~cCcD_GObjCo() {}

void cCcD_GObjCo::Set(const cCcD_SrcGObjCo &src) {
    mEffCounter = 0;
    mSrc = src;
    SetCoFlag(mSrc.mSPrm & 0x1e0);
}

void cCcD_GObjCo::SetCoFlag(u32 flag) {
    mSrc.mSPrm = mSrc.mSPrm & ~0x1E0 | flag;
    mField_0x20 = (mSrc.mSPrm & 0x1E0) >> 4;
}

void cCcD_GObjCo::AdjustHitPos(f32, f32) {}
