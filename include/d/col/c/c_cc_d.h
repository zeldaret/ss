#ifndef C_CC_D_H
#define C_CC_D_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/col/c/c_m3d.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/col/c/c_m3d_g_cps.h"
#include "d/col/c/c_m3d_g_cyl.h"
#include "d/col/c/c_m3d_g_sph.h"
#include "d/col/c/c_m3d_g_tri.h"
#include "d/col/c/c_m3d_g_unk.h"
#include "m/m_vec.h"
#include "nw4r/types_nw4r.h"

class dAcObjBase_c;

class cCcD_CpsAttr;
class cCcD_TriAttr;
class cCcD_UnkAttr;
class cCcD_CylAttr;
class cCcD_SphAttr;

class cCcD_DivideInfo {
private:
    /* 0x00 */ u32 mXDivInfo;
    /* 0x04 */ u32 mYDivInfo;
    /* 0x08 */ u32 mZDivInfo;
    /* 0x0C vtable */
public:
    cCcD_DivideInfo();
    virtual ~cCcD_DivideInfo();

    // Following two funcs were probably inlined based on how other classes went
    void Set(u32 xDivInfo, u32 yDivInfo, u32 zDivInfo) {
        mXDivInfo = xDivInfo;
        mYDivInfo = yDivInfo;
        mZDivInfo = zDivInfo;
    }

    bool Chk(cCcD_DivideInfo const &other) const {
        if ((mXDivInfo & other.mXDivInfo) == 0 || (mZDivInfo & other.mZDivInfo) == 0 ||
            (mYDivInfo & other.mYDivInfo) == 0) {
            return false;
        } else {
            return true;
        }
    }
};

class cCcD_DivideArea : public cM3dGAab {
private:
    /* 0x18 */ bool mXDiffIsZero;
    /* 0x1C */ f32 mScaledXDiff;
    /* 0x20 */ f32 mInvScaledXDiff;
    /* 0x24 */ bool mYDiffIsZero;
    /* 0x28 */ f32 mScaledYDiff;
    /* 0x2C */ f32 mInvScaledYDiff;
    /* 0x30 */ bool mZDiffIsZero;
    /* 0x34 */ f32 mScaledZDiff;
    /* 0x38 */ f32 mInvScaledZDiff;
    /* 0x3C vtable */

public:
    cCcD_DivideArea();
    virtual ~cCcD_DivideArea();
    void SetArea(cM3dGAab const &);
    void CalcDivideInfo(cCcD_DivideInfo *, cM3dGAab const &);
    void CalcDivideInfoOverArea(cCcD_DivideInfo *, cM3dGAab const &);
};

class cCcD_ShapeAttr {
public:
    cM3dGAab mAab;
    cCcD_DivideInfo mInfo;
    enum ShapeType {
        SHAPE_SPHERE = 0,
        SHAPE_CYLINDER = 1,
        SHAPE_CAPSULE = 2,
        SHAPE_TRIANGLE = 3,
        SHAPE_DEFAULT = 4,
    };
    struct Shape {
        ~Shape();

        /* 0x00 */ int mField_0x00;
        /* 0x04 */ mVec3_c mField_0x04;
        /* 0x10 */ mVec3_c mField_0x10;
        /* 0x1C */ mVec3_c mField_0x1C;
    };
    cCcD_ShapeAttr();
    /* vt 0x08 */ virtual ~cCcD_ShapeAttr();
    /* vt 0x0C */ virtual bool Calc(const mVec3_c &, const mVec3_c &, mVec3_c *);
    /* vt 0x10 */ virtual bool CrossAtTg(cCcD_ShapeAttr &, mVec3_c *);
    /* vt 0x14 */ virtual bool CrossAtTg(cCcD_CpsAttr &, mVec3_c *) = 0;
    /* vt 0x18 */ virtual bool CrossAtTg(cCcD_TriAttr &, mVec3_c *) = 0;
    /* vt 0x1C */ virtual bool CrossAtTg(cCcD_UnkAttr &, mVec3_c *) = 0;
    /* vt 0x20 */ virtual bool CrossAtTg(cCcD_CylAttr &, mVec3_c *) = 0;
    /* vt 0x24 */ virtual bool CrossAtTg(cCcD_SphAttr &, mVec3_c *) = 0;
    /* vt 0x28 */ virtual bool CrossCo(cCcD_ShapeAttr &, f32 *);
    /* vt 0x2C */ virtual bool CrossCo(cCcD_CpsAttr &, f32 *) = 0;
    /* vt 0x30 */ virtual bool CrossCo(cCcD_TriAttr &, f32 *) = 0;
    /* vt 0x34 */ virtual bool CrossCo(cCcD_UnkAttr &, f32 *) = 0;
    /* vt 0x38 */ virtual bool CrossCo(cCcD_CylAttr &, f32 *) = 0;
    /* vt 0x3C */ virtual bool CrossCo(cCcD_SphAttr &, f32 *) = 0;
    /* vt 0x40 */ virtual mVec3_c &GetCoP();
    /* vt 0x44 */ virtual void CalcAabBox() = 0;
    /* vt 0x48 */ virtual bool GetNVec(const mVec3_c &, mVec3_c *) const = 0;
    /* vt 0x4C */ virtual void GetShapeAccess(cCcD_ShapeAttr::Shape *) const;
    /* vt 0x50 */ virtual void GetPos(nw4r::math::VEC3 *) const = 0;
    /* vt 0x54 */ virtual bool GetPosBool(mVec3_c *) const;
    /* vt 0x58 */ virtual void TranslateXZ(f32, f32) = 0;

    cM3dGAab &GetWorkAab() {
        return mAab;
    }
    cM3dGAab const &GetWorkAab() const {
        return mAab;
    }

    static mVec3_c sVirtualCenter;
};

struct cCcD_SrcCpsAttr {
    f32 mRadius;
};
class cCcD_CpsAttr : public cCcD_ShapeAttr, public cM3dGCps {
public:
    mVec3_c mVirtCenter;

    cCcD_CpsAttr();
    void Set(const cCcD_SrcCpsAttr &src);
    void SetVirtCenter(mVec3_c *);

    f32 CrossCoCommon(cCcD_ShapeAttr &, f32);

    /* vt 0x08 */ virtual ~cCcD_CpsAttr();
    /* vt 0x0C */ virtual bool Calc(const mVec3_c &, const mVec3_c &, mVec3_c *) override;
    /* vt 0x10 */ virtual bool CrossAtTg(cCcD_ShapeAttr &, mVec3_c *) override;
    /* vt 0x14 */ virtual bool CrossAtTg(cCcD_CpsAttr &, mVec3_c *) override;
    /* vt 0x18 */ virtual bool CrossAtTg(cCcD_TriAttr &, mVec3_c *) override;
    /* vt 0x1C */ virtual bool CrossAtTg(cCcD_UnkAttr &, mVec3_c *) override;
    /* vt 0x20 */ virtual bool CrossAtTg(cCcD_CylAttr &, mVec3_c *) override;
    /* vt 0x24 */ virtual bool CrossAtTg(cCcD_SphAttr &, mVec3_c *) override;
    /* vt 0x28 */ virtual bool CrossCo(cCcD_ShapeAttr &, f32 *) override;
    /* vt 0x2C */ virtual bool CrossCo(cCcD_CpsAttr &, f32 *) override;
    /* vt 0x30 */ virtual bool CrossCo(cCcD_TriAttr &, f32 *) override;
    /* vt 0x34 */ virtual bool CrossCo(cCcD_UnkAttr &, f32 *) override;
    /* vt 0x38 */ virtual bool CrossCo(cCcD_CylAttr &, f32 *) override;
    /* vt 0x3C */ virtual bool CrossCo(cCcD_SphAttr &, f32 *) override;
    /* vt 0x40 */ virtual mVec3_c &GetCoP() override;
    /* vt 0x44 */ virtual void CalcAabBox() override;
    /* vt 0x48 */ virtual bool GetNVec(const mVec3_c &, mVec3_c *) const override;
    /* vt 0x4C */ virtual void GetShapeAccess(cCcD_ShapeAttr::Shape *) const override;
    /* vt 0x50 */ virtual void GetPos(nw4r::math::VEC3 *) const override;
    /* vt 0x58 */ virtual void TranslateXZ(f32, f32) override;
};

class cCcD_TriAttr : public cCcD_ShapeAttr, public cM3dGTri {
public:
    cCcD_TriAttr();
    /* vt 0x08 */ virtual ~cCcD_TriAttr();
    /* vt 0x0C */ virtual bool Calc(const mVec3_c &, const mVec3_c &, mVec3_c *) override;
    /* vt 0x10 */ virtual bool CrossAtTg(cCcD_ShapeAttr &, mVec3_c *) override;
    /* vt 0x14 */ virtual bool CrossAtTg(cCcD_CpsAttr &, mVec3_c *) override;
    /* vt 0x18 */ virtual bool CrossAtTg(cCcD_TriAttr &, mVec3_c *) override;
    /* vt 0x1C */ virtual bool CrossAtTg(cCcD_UnkAttr &, mVec3_c *) override;
    /* vt 0x20 */ virtual bool CrossAtTg(cCcD_CylAttr &, mVec3_c *) override;
    /* vt 0x24 */ virtual bool CrossAtTg(cCcD_SphAttr &, mVec3_c *) override;
    /* vt 0x28 */ virtual bool CrossCo(cCcD_ShapeAttr &, f32 *) override;
    /* vt 0x2C */ virtual bool CrossCo(cCcD_CpsAttr &, f32 *) override;
    /* vt 0x30 */ virtual bool CrossCo(cCcD_TriAttr &, f32 *) override;
    /* vt 0x34 */ virtual bool CrossCo(cCcD_UnkAttr &, f32 *) override;
    /* vt 0x38 */ virtual bool CrossCo(cCcD_CylAttr &, f32 *) override;
    /* vt 0x3C */ virtual bool CrossCo(cCcD_SphAttr &, f32 *) override;
    /* vt 0x44 */ virtual void CalcAabBox() override;
    /* vt 0x48 */ virtual bool GetNVec(const mVec3_c &, mVec3_c *) const override;
    /* vt 0x4C */ virtual void GetShapeAccess(cCcD_ShapeAttr::Shape *) const override;
    /* vt 0x50 */ virtual void GetPos(nw4r::math::VEC3 *) const override;
    /* vt 0x58 */ virtual void TranslateXZ(f32, f32) override;
};

struct cCcD_SrcUnkAttr {
    f32 mMinX, mMinY, mMinZ;
    f32 mMaxX, mMaxY, mMaxZ;

    mVec3_c &getMin() const {
        return *(mVec3_c *)&mMinX;
    }
    mVec3_c &getMax() const {
        return *(mVec3_c *)&mMaxX;
    }
};

class cCcD_UnkAttr : public cCcD_ShapeAttr, public cM3dGUnk {
public:
    cCcD_UnkAttr();

    void Set(const cCcD_SrcUnkAttr &src);
    /* vt 0x08 */ virtual ~cCcD_UnkAttr();
    /* vt 0x0C */ virtual bool Calc(const mVec3_c &, const mVec3_c &, mVec3_c *) override;
    /* vt 0x10 */ virtual bool CrossAtTg(cCcD_ShapeAttr &, mVec3_c *) override;
    /* vt 0x14 */ virtual bool CrossAtTg(cCcD_CpsAttr &, mVec3_c *) override;
    /* vt 0x18 */ virtual bool CrossAtTg(cCcD_TriAttr &, mVec3_c *) override;
    /* vt 0x1C */ virtual bool CrossAtTg(cCcD_UnkAttr &, mVec3_c *) override;
    /* vt 0x20 */ virtual bool CrossAtTg(cCcD_CylAttr &, mVec3_c *) override;
    /* vt 0x24 */ virtual bool CrossAtTg(cCcD_SphAttr &, mVec3_c *) override;
    /* vt 0x28 */ virtual bool CrossCo(cCcD_ShapeAttr &, f32 *) override;
    /* vt 0x2C */ virtual bool CrossCo(cCcD_CpsAttr &, f32 *) override;
    /* vt 0x30 */ virtual bool CrossCo(cCcD_TriAttr &, f32 *) override;
    /* vt 0x34 */ virtual bool CrossCo(cCcD_UnkAttr &, f32 *) override;
    /* vt 0x38 */ virtual bool CrossCo(cCcD_CylAttr &, f32 *) override;
    /* vt 0x3C */ virtual bool CrossCo(cCcD_SphAttr &, f32 *) override;
    /* vt 0x40 */ virtual mVec3_c &GetCoP() override;
    /* vt 0x44 */ virtual void CalcAabBox() override;
    /* vt 0x48 */ virtual bool GetNVec(const mVec3_c &, mVec3_c *) const override;
    /* vt 0x50 */ virtual void GetPos(nw4r::math::VEC3 *) const override;
    /* vt 0x58 */ virtual void TranslateXZ(f32, f32) override;
};

struct cCcD_SrcCylAttr {
    f32 mRadius;
    f32 mHeight;
};

class cCcD_CylAttr : public cCcD_ShapeAttr, public cM3dGCyl {
public:
    cCcD_CylAttr();
    void Set(const cCcD_SrcCylAttr &);
    /* vt 0x08 */ virtual ~cCcD_CylAttr();
    /* vt 0x0C */ virtual bool Calc(const mVec3_c &, const mVec3_c &, mVec3_c *) override;
    /* vt 0x10 */ virtual bool CrossAtTg(cCcD_ShapeAttr &, mVec3_c *) override;
    /* vt 0x14 */ virtual bool CrossAtTg(cCcD_CpsAttr &, mVec3_c *) override;
    /* vt 0x18 */ virtual bool CrossAtTg(cCcD_TriAttr &, mVec3_c *) override;
    /* vt 0x1C */ virtual bool CrossAtTg(cCcD_UnkAttr &, mVec3_c *) override;
    /* vt 0x20 */ virtual bool CrossAtTg(cCcD_CylAttr &, mVec3_c *) override;
    /* vt 0x24 */ virtual bool CrossAtTg(cCcD_SphAttr &, mVec3_c *) override;
    /* vt 0x28 */ virtual bool CrossCo(cCcD_ShapeAttr &, f32 *) override;
    /* vt 0x2C */ virtual bool CrossCo(cCcD_CpsAttr &, f32 *) override;
    /* vt 0x30 */ virtual bool CrossCo(cCcD_TriAttr &, f32 *) override;
    /* vt 0x34 */ virtual bool CrossCo(cCcD_UnkAttr &, f32 *) override;
    /* vt 0x38 */ virtual bool CrossCo(cCcD_CylAttr &, f32 *) override;
    /* vt 0x3C */ virtual bool CrossCo(cCcD_SphAttr &, f32 *) override;
    /* vt 0x40 */ virtual mVec3_c &GetCoP() override;
    /* vt 0x44 */ virtual void CalcAabBox() override;
    /* vt 0x48 */ virtual bool GetNVec(const mVec3_c &, mVec3_c *) const override;
    /* vt 0x4C */ virtual void GetShapeAccess(cCcD_ShapeAttr::Shape *) const override;
    /* vt 0x50 */ virtual void GetPos(nw4r::math::VEC3 *) const override;
    /* vt 0x58 */ virtual void TranslateXZ(f32, f32) override;
};

struct cCcD_SrcSphAttr {
    f32 mRadius;
};

class cCcD_SphAttr : public cCcD_ShapeAttr, public cM3dGSph {
public:
    cCcD_SphAttr();
    void Set(const cCcD_SrcSphAttr &);
    /* vt 0x08 */ virtual ~cCcD_SphAttr();
    /* vt 0x0C */ virtual bool Calc(const mVec3_c &, const mVec3_c &, mVec3_c *) override;
    /* vt 0x10 */ virtual bool CrossAtTg(cCcD_ShapeAttr &, mVec3_c *) override;
    /* vt 0x14 */ virtual bool CrossAtTg(cCcD_CpsAttr &, mVec3_c *) override;
    /* vt 0x18 */ virtual bool CrossAtTg(cCcD_TriAttr &, mVec3_c *) override;
    /* vt 0x1C */ virtual bool CrossAtTg(cCcD_UnkAttr &, mVec3_c *) override;
    /* vt 0x20 */ virtual bool CrossAtTg(cCcD_CylAttr &, mVec3_c *) override;
    /* vt 0x24 */ virtual bool CrossAtTg(cCcD_SphAttr &, mVec3_c *) override;
    /* vt 0x28 */ virtual bool CrossCo(cCcD_ShapeAttr &, f32 *) override;
    /* vt 0x2C */ virtual bool CrossCo(cCcD_CpsAttr &, f32 *) override;
    /* vt 0x30 */ virtual bool CrossCo(cCcD_TriAttr &, f32 *) override;
    /* vt 0x34 */ virtual bool CrossCo(cCcD_UnkAttr &, f32 *) override;
    /* vt 0x38 */ virtual bool CrossCo(cCcD_CylAttr &, f32 *) override;
    /* vt 0x3C */ virtual bool CrossCo(cCcD_SphAttr &, f32 *) override;
    /* vt 0x40 */ virtual mVec3_c &GetCoP() override;
    /* vt 0x44 */ virtual void CalcAabBox() override;
    /* vt 0x48 */ virtual bool GetNVec(const mVec3_c &, mVec3_c *) const override;
    /* vt 0x4C */ virtual void GetShapeAccess(cCcD_ShapeAttr::Shape *) const override;
    /* vt 0x50 */ virtual void GetPos(nw4r::math::VEC3 *) const override;
    /* vt 0x54 */ virtual bool GetPosBool(mVec3_c *) const override;
    /* vt 0x58 */ virtual void TranslateXZ(f32, f32) override;
};

// This has changed bit
class cCcD_Stts {
public:
    /* 0x00 */ int mAtApid;
    /* 0x04 */ int mAtOldApid;
    /* 0x08 */ int mTgApid;
    /* 0x0C */ int mTgOldApid;
    /* 0x10 */ mVec3_c mField_0x10;
    /* 0x1C */ mVec3_c mField_0x1C;
    /* 0x28 */ mVec3_c mCcMove;
    /* 0x34 */ dAcObjBase_c *mpActor;
    /* 0x38 */ int mRank;

    cCcD_Stts(dAcObjBase_c *);
    void Move();
    int GetID() const;
    void PlucCcMove(f32, f32, f32);
    void ClrCcMove();
    int GetWeight(int) const; // idk what to really call it but it removes the rank table

    void SetDefaultRank() {
        mRank = 0xD;
    }
    void SetRank(int rank) {
        mRank = rank;
    }
};

struct cCcD_SrcGObjTgInfo {
    /* 0x00 */ u16 mField_0x0;
    /* 0x02 */ u16 mField_0x2;
};

struct cCcD_SrcGObjTg {
    /* 0x00 */ u32 mType;
    /* 0x04 */ u32 mSPrm;
    /* 0x08 */ cCcD_SrcGObjTgInfo mInfo;
    /* 0x0C */ u16 mField_0x0C;
    /* 0x0E */ u16 mField_0x0E;
};

enum cCcD_AtModifiers_e {
    /* 0x 0000 0001 */ AT_MOD_FIRE = (1 << 0),
    /* 0x 0000 0002 */ AT_MOD_0x2 = (1 << 1),
    /* 0x 0000 0004 */ AT_MOD_ELECTRIC = (1 << 2),
    /* 0x 0000 0008 */ AT_MOD_WIND = (1 << 3),
    /* 0x 0000 0010 */ AT_MOD_0x10 = (1 << 4),
    /* 0x 0000 0020 */ AT_MOD_0x20 = (1 << 5),
    /* 0x 0000 0040 */ AT_MOD_WATER = (1 << 6),
    /* 0x 0000 0080 */ AT_MOD_0x80 = (1 << 7),
    /* 0x 0000 0100 */ AT_MOD_0x100 = (1 << 8),
    /* 0x 0000 0200 */ AT_MOD_STINKY = (1 << 9),
    /* 0x 0000 0400 */ AT_MOD_CURSED = (1 << 10),
};
struct cCcD_SrcGObjAtInfo {
    /* 0x00 */ u8 mField_0x0;
    /* 0x01 */ u8 mField_0x1;
    /* 0x02 */ u16 mModifier;
};

enum cCcD_AtSPrm {
    AT_SPRM_DAMAGE = 0x4,
};

struct cCcD_SrcGObjAt {
    /* 0x00 */ u32 mType;
    /* 0x04 */ u32 mSPrm;
    /* 0x08 */ cCcD_SrcGObjAtInfo mInfo;
    /* 0x0C */ u8 mDamage;
    /* 0x0D */ u8 mField_0xD;
    /* 0x0E */ u8 mField_0xE;
    /* 0x0F */ u8 mField_0xF;
    /* 0x10 */ u16 mField_0x10;
    /* 0x12 */ s16 mField_0x12;
};

struct cCcD_SrcGObjCo {
    /* 0x00 */ u32 mSPrm;
};

struct cCcD_SrcGObj {
    /* 0x00 */ cCcD_SrcGObjAt mObjAt;
    /* 0x14 */ cCcD_SrcGObjTg mObjTg;
    /* 0x1C */ cCcD_SrcGObjCo mObjCo;
};

class dAcObjBase_c;
class cCcD_GObj;
typedef bool (*cCcD_HitCallback)(
    dAcObjBase_c *i_actorA, cCcD_GObj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_GObj *i_objInfB
);

class cCcD_GAtTgCoCommonBase {
public:
    cCcD_HitCallback mHit_cb;
    s8 mEffCounter;
    dAcRef_c<dAcObjBase_c> mAc;
    u32 mRPrm;

    cCcD_GAtTgCoCommonBase();

    virtual ~cCcD_GAtTgCoCommonBase();
    void dt() {
        mEffCounter = 0;
        ClrActorInfo();
        mHit_cb = nullptr;
    }
    void ClrActorInfo();
    void SetHitActor(dAcObjBase_c *);
    dAcObjBase_c *GetActor();
    void SubtractEffCounter();

    u32 MskRPrm(u32 m) const {
        return mRPrm & m;
    }
    void SetRPrm(u32 f) {
        mRPrm = f;
    }
    void OnRPrm(u32 m) {
        mRPrm |= m;
    }
    void OffRPrm(u32 m) {
        mRPrm = (mRPrm & ~m) | m;
    }
};

class cCcD_GObjAt : public cCcD_GAtTgCoCommonBase {
public:
    cCcD_GObjAt();
    virtual ~cCcD_GObjAt();
    void Set(const cCcD_SrcGObjAt &);
    void SetAtFlag(u32);
    void AdjustHitPos(f32, f32);

    void SetVec(const mVec3_c &vec) {
        mVec = vec;
    }
    mVec3_c &GetVec() {
        return mVec;
    }

    void ClrSet() {
        OffSPrm(1);
    }

    void SetCallback(cCcD_HitCallback cb) {
        mHit_cb = cb;
    }

    u32 MskType(u32 mask) const {
        return mSrc.mType & mask;
    }
    void SetType(u32 flag) {
        mSrc.mType = flag;
    }
    void OnType(u32 m) {
        mSrc.mType |= m;
    }
    void OffType(u32 m) {
        mSrc.mType &= ~m;
    }
    u32 MskSPrm(u32 m) const {
        return mSrc.mSPrm & m;
    }
    void SetSPrm(u32 f) {
        mSrc.mSPrm = f;
    }
    void OnSPrm(u32 m) {
        mSrc.mSPrm |= m;
    }
    void OffSPrm(u32 m) {
        mSrc.mSPrm &= ~m;
    }
    u32 MskTgHitSPrm(u32 m) const {
        return mTgHitSrc.mSPrm & m;
    }
    void SetTgHitSPrm(u32 f) {
        mTgHitSrc.mSPrm = f;
    }
    void OnTgHitSPrm(u32 m) {
        mTgHitSrc.mSPrm |= m;
    }
    void OffTgHitSPrm(u32 m) {
        mTgHitSrc.mSPrm &= ~m;
    }

public:
    /* 0x1C */ cCcD_SrcGObjAt mSrc;
    /* 0x30 */ mVec3_c mHitPos;
    /* 0x3C */ mVec3_c mVec;
    /* 0x48 */ cCcD_SrcGObjTg mTgHitSrc;
    /* 0x58 */ cCcD_HitCallback mField_0x58;
};

class cCcD_GObjTg : public cCcD_GAtTgCoCommonBase {
public:
    cCcD_GObjTg();
    virtual ~cCcD_GObjTg();
    void Set(const cCcD_SrcGObjTg &);
    void AdjustHitPos(f32, f32);

    u32 GetAtHitType() const {
        return mAtHitSrc.mType;
    }

    void SetFlag_0xA(u16 flag) {
        mSrc.mField_0x0E = flag;
    }

    u16 GetFlag_0xA(u16 mask) const {
        return mSrc.mField_0x0E & mask;
    }
    void ClrSet() {
        OffSPrm(1);
    }

    void Set_0x4C(u32 f) {
        mField_0x4C = f;
    }

    u32 MskType(u32 mask) const {
        return mSrc.mType & mask;
    }
    void SetType(u32 flag) {
        mSrc.mType = flag;
    }
    void OnType(u32 m) {
        mSrc.mType |= m;
    }
    void OffType(u32 m) {
        mSrc.mType &= ~m;
    }
    u32 MskSPrm(u32 m) const {
        return mSrc.mSPrm & m;
    }
    void SetSPrm(u32 f) {
        mSrc.mSPrm = f;
    }
    void OnSPrm(u32 m) {
        mSrc.mSPrm |= m;
    }
    void OffSPrm(u32 m) {
        mSrc.mSPrm &= ~m;
    }
    u32 MskAtHitSPrm(u32 m) const {
        return mAtHitSrc.mSPrm & m;
    }
    void SetAtHitSPrm(u32 f) {
        mAtHitSrc.mSPrm = f;
    }
    void OnAtHitSPrm(u32 m) {
        mAtHitSrc.mSPrm |= m;
    }
    void OffAtHitSPrm(u32 m) {
        mAtHitSrc.mSPrm &= ~m;
    }

public:
    /* 0x1C */ cCcD_SrcGObjTg mSrc;
    /* 0x2C */ mVec3_c mField_0x2C;
    /* 0x38 */ mVec3_c mHitPos;
    /* 0x44 */ s16 *mShieldFrontRangeYAngle;
    /* 0x48 */ s16 mShieldRange;
    /* 0x4A */ u8 mField_0x4A;
    /* 0x4B */ u8 mField_0x4B;
    /* 0x4C */ u32 mField_0x4C;
    /* 0x50 */ u32 mField_0x50;
    /* 0x54 */ cCcD_HitCallback mField_0x54;
    /* 0x58 */ cCcD_SrcGObjAt mAtHitSrc;
    /* 0x6C */ mVec3_c mField_0x6C;
    /* 0x78 */ u32 mField_0x78;
};

class cCcD_GObjCo : public cCcD_GAtTgCoCommonBase {
public:
    cCcD_GObjCo();
    virtual ~cCcD_GObjCo();
    void Set(const cCcD_SrcGObjCo &);
    void SetCoFlag(u32);
    void AdjustHitPos(f32, f32);

    void ClrSet() {
        OffSPrm(1);
    }
    u32 MskSPrm(u32 m) const {
        return mSrc.mSPrm & m;
    }
    void SetSPrm(u32 f) {
        mSrc.mSPrm = f;
    }
    void OnSPrm(u32 m) {
        mSrc.mSPrm |= m;
    }
    void OffSPrm(u32 m) {
        mSrc.mSPrm &= ~m;
    }

public:
    /* 0x1C */ cCcD_SrcGObjCo mSrc;
    /* 0x20 */ u32 mField_0x20;
    /* 0x24 */ cCcD_SrcGObjCo mCoHitSrc;
    /* 0x28 */ cCcD_HitCallback mField_0x28_callback;
};

// Maybe ?
class cCcD_GObjInf {
public:
    /* 0x000 */ cCcD_GObjAt mObjAt;
    /* 0x05C */ cCcD_GObjTg mObjTg;
    /* 0x0D8 */ cCcD_GObjCo mObjCo;
};

class cCcD_GObj {
public:
    /* 0x000 */ cCcD_GObjAt mAt;
    /* 0x05C */ cCcD_GObjTg mTg;
    /* 0x0D8 */ cCcD_GObjCo mCo;
    /* 0x104 */ cCcD_Stts *mStts;
    /* 0x108 */ u32 mField_0x108;

public:
    cCcD_GObj();
    virtual ~cCcD_GObj();
    virtual cCcD_ShapeAttr *GetShapeAttr() = 0;
    virtual cCcD_GObjInf *GetGObjInfo();
    void ClrSet();
    void Set(const cCcD_SrcGObj &);
    void ClrAtHit();
    void ClrTgHit();
    void ClrCoHit();

    const mVec3_c &GetAtHitPos() const;
    mVec3_c &GetAtHitPos();
    bool GetAtFlag0x2() const;
    bool GetAtFlag0x4() const;
    bool GetAtFlag0x8() const;

    const mVec3_c &GetTgHitPos() const;
    mVec3_c &GetTgHitPos();
    bool GetTgFlag0x4() const;
    bool GetTgFlag0x8() const;

    bool ChkAtClawshot() const;
    bool ChkAtClawshotDebug() const;
    bool ChkAtElectrified() const;
    bool ChkAtElectrifiedExtra() const;
    bool ChkAtWhippable() const;
    bool ChkAtBit24() const;
    bool ChkAtArrowStick() const;
    bool ChkAtWaterScaleBonk() const;
    bool ChkAtSwordBonk() const;
    dAcObjBase_c *GetAtActor();

    bool ChkTgAtHitType(u32) const;
    u32 GetTg_0x58() const;
    bool ChkTgBit14() const;
    u8 GetTgDamage() const;
    u16 GetTgDamageFlags() const;
    bool ChkTgSkywardStrike() const;
    bool ChkTgBit17() const;
    bool ChkTgBit18() const;
    bool ChkTgBit19() const;
    bool ChkTgBit20() const;
    bool ChkTgBit23() const;
    bool ChkTgBit24() const;
    bool ChkTgBit25() const;
    u16 GetTgSoundID() const;
    s16 GetTg_0x6A() const;
    bool ChkTgBit8() const;
    u8 GetTg_0x4A() const;
    dAcObjBase_c *GetTgActor();
    const mVec3_c &GetTg_0x2C() const;

    dAcObjBase_c *GetCoActor();
    bool ChkCoBit4() const;

    void SetAtFlagsUpper(u32);
    bool ChkTgBit1() const;

    void AdjustHitPos(f32, f32);

    static bool fn_80328ad0(dAcObjBase_c *pObj, u32 attype);

    void SetAtVec(const mVec3_c &vec) {
        mAt.SetVec(vec);
    }

    void SetStts(cCcD_Stts &stts) {
        mStts = &stts;
    }

    void OnTgCoFlag(u32 f) {
        mTg.OnSPrm(f);
        mCo.OnSPrm(f);
    }

    void SetTgFlag(u32 flag) {
        mTg.SetType(flag);
    }
    void SetAtFlag(u32 flag) {
        mAt.SetSPrm(flag);
    }

    void SetTgFlag_0xA(u16 flag) {
        mTg.SetFlag_0xA(flag);
    }

    bool ChkTgHit() {
        return mTg.MskSPrm(1) != 0 && mTg.GetActor() != nullptr;
    }
    bool ChkAtHit() {
        return mAt.MskSPrm(1) != 0 && mAt.GetActor() != nullptr;
    }
    bool ChkCoHit() {
        return mCo.MskSPrm(1) != 0 && mCo.GetActor() != nullptr;
    }

    dAcObjBase_c *GetAc() {
        if (mStts == nullptr) {
            return nullptr;
        } else {
            return mStts->mpActor;
        }
    }

    void ClrCoSet() {
        mCo.ClrSet();
    }
    void ClrAtSet() {
        mAt.ClrSet();
    }
    void ClrTgSet() {
        mTg.ClrSet();
    }

    void SetTg_0x4C(u32 f) {
        mTg.Set_0x4C(f);
    }

    void SetAtCallback(cCcD_HitCallback cb) {
        mAt.SetCallback(cb);
    }

    u32 ChkTgNoAtHitInfSet() const {
        return mTg.MskSPrm(0x20);
    }
};

#endif
