#ifndef D_CC_D_H
#define D_CC_D_H

#include <d/a/d_a_base.h>
#include <d/col/c/c_m3d_g_aab.h>
#include <d/col/c/c_m3d_g_cps.h>
#include <d/col/c/c_m3d_g_cyl.h>
#include <d/col/c/c_m3d_g_sph.h>
#include <d/col/c/c_m3d_g_unk.h>
#include <f/f_base_id.h>
#include <m/m_mtx.h>
#include <m/m_vec.h>
#include <nw4r/math/math_types.h>

// Largely modeled off
// https://github.com/zeldaret/tp/blob/main/include/SSystem/SComponent/c_cc_d.h
// https://github.com/zeldaret/tp/blob/main/include/d/cc/d_cc_d.h
// - the SComponent/d split seems to not exist anymore in SS, so we'll just
//   go with the d names for this merger of the two

class dCcD_DivideInfo {
private:
    /* 0x00 */ u32 mXDivInfo;
    /* 0x04 */ u32 mYDivInfo;
    /* 0x08 */ u32 mZDivInfo;
    /* 0x0C vtable */
public:
    /* 80328120 */ dCcD_DivideInfo();
    /* 80328130 */ virtual ~dCcD_DivideInfo();
    // void Set(u32, u32, u32);
    // bool Chk(dCcD_DivideInfo const &) const;
}; // Size = 0x10

class dCcD_DivideArea : public cM3dGAab {
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

public:
    /* vt at 0x3C */
    dCcD_DivideArea();
    virtual ~dCcD_DivideArea();
    /* 803281c0 */ void SetArea(cM3dGAab const &);
    /* 803282d0 */ void CalcDivideInfo(dCcD_DivideInfo *, cM3dGAab const &, u32);
    /* 80328470 */ void CalcDivideInfoOverArea(dCcD_DivideInfo *, cM3dGAab const &);
}; // Size = 0x40

class dCcD_GAtTgCoCommonBase {
protected:
    /* 0x00 */ u32 field_0x00;
    /* 0x04 */ s8 mEffCounter;
    /* 0x08 */ dAcRef_c<dAcBase_c> mActor;
    /* 0x14 */ u32 field_0x14;

public:
    dCcD_GAtTgCoCommonBase();
    virtual ~dCcD_GAtTgCoCommonBase();

    void resetEffCounter() {
        mEffCounter = 0;
    }
    void resetField0x14() {
        field_0x14 = 0;
    }
    u32 getField0x14() {
        return field_0x14;
    }
    void decreaseCount();
    void unlink();
    dAcBase_c *getActor();
    void setActor(dAcBase_c *);
};

struct dCcD_SrcGAtTgCoCommonBase {
    /* 0x0 */ u32 mGFlag;
}; // Size: 0x4

struct dCcD_SrcGObjAt {
    /* 0x00 */ dCcD_SrcGAtTgCoCommonBase base;
    /* 0x04 */ u32 field_0x04;
    /* 0x08 */ u32 field_0x08;
    /* 0x0C */ u8 mSe;      // Sound Effect ID
    /* 0x0D */ u8 mHitMark; // Hit Mark particle ID
    /* 0x0E */ u8 mSpl;
    /* 0x0F */ u8 mMtrl;
    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ s16 field_0x12;
};

struct dCcD_SrcGObjTg {
    /* 0x00 */ dCcD_SrcGAtTgCoCommonBase base;
    /* 0x04 */ u32 field_0x04;
    /* 0x08 */ u16 field_0x08;
    /* 0x10 */ u16 field_0x0A;
    /* 0x0C */ u16 field_0x0C;
    /* 0x0E */ u16 field_0x0E;
};

struct dCcD_SrcGObjCo {
    /* 0x0 */ dCcD_SrcGAtTgCoCommonBase mBase;
};

struct dCcD_SrcGObjInf {
    /* 0x00 */ dCcD_SrcGObjAt mGObjAt;
    /* 0x14 */ dCcD_SrcGObjTg mGObjTg;
    /* 0x24 */ dCcD_SrcGObjCo mGObjCo;
};

class dCcD_GObjAt : public dCcD_GAtTgCoCommonBase {
private:
    /* 0x1C */ dCcD_SrcGObjAt mSrc;
    /* 0x30 */ mVec3_c mHitPos;
    /* 0x3C */ mVec3_c mVec;
    /* 0x48 */ mVec3_c mRVec;
    /* 0x54 */ u8 field_0x4C[0x58 - 0x54];
    /* 0x58 */ u32 field_0x58;

public:
    dCcD_GObjAt();
    virtual ~dCcD_GObjAt();

    void clearFlag() {
        mSrc.field_0x04 = mSrc.field_0x04 & 0xFFFFFFFE;
    }
    mVec3_c *GetHitPosP() {
        return &mHitPos;
    }
    void SetSomeVec(const mVec3_c &p) {
        mVec = p;
    }
    mVec3_c *GetSomeVec() {
        return &mVec;
    }
    void init(const dCcD_SrcGObjAt *);
    void setSomeAtFlags(u32 flags);
    void adjustHitPos(f32 dx, f32 dz);
};

class dCcD_GObjTg : public dCcD_GAtTgCoCommonBase {
private:
    /* 0x1C */ dCcD_SrcGObjTg mSrc;
    /* 0x2C */ u8 field_0x2C[0x38 - 0x2C];
    /* 0x38 */ mVec3_c mHitPos;
    /* 0x44 */ u32 field_0x44;
    /* 0x48 */ u16 field_0x48;
    /* 0x4A */ u8 field_0x4A;
    /* 0x4B */ u8 field_0x4B;
    /* 0x4C */ u32 field_0x4C;
    /* 0x50 */ u32 field_0x50;
    /* 0x54 */ u32 field_0x54;
    /* 0x58 */ u32 field_0x58;
    /* 0x5C */ u8 field_0x5C[0x6C - 0x5C];
    /* 0x6C */ mVec3_c field_0x6C;
    /* 0x78 */ u32 field_0x78;

public:
    dCcD_GObjTg();
    virtual ~dCcD_GObjTg();
    void clearFlag() {
        mSrc.field_0x04 = mSrc.field_0x04 & 0xFFFFFFFE;
    }
    void setFlag(u32 f) {
        mSrc.field_0x04 |= f;
    }
    void set0x4C(u32 val) {
        field_0x4C = val;
    }
    void setSrcFlag(u32 flag) {
        mSrc.base.mGFlag = flag;
    }
    void setSrc0x0A(u32 flag) {
        mSrc.field_0x0A = flag;
    }
    u32 get0x58() {
        return field_0x58;
    }
    void init(const dCcD_SrcGObjTg *);
    void adjustHitPos(f32 dx, f32 dz);
};

class dCcD_GObjCo : public dCcD_GAtTgCoCommonBase {
private:
    /* 0x1C */ dCcD_SrcGObjCo mSrc;
    /* 0x20 */ u32 field_0x20;
    /* 0x24 */ u16 field_0x24;
    /* 0x28 */ u32 field_0x28;

public:
    dCcD_GObjCo();
    virtual ~dCcD_GObjCo();
    void clearFlag() {
        mSrc.mBase.mGFlag = mSrc.mBase.mGFlag & 0xFFFFFFFE;
    }
    void setFlag(u32 f) {
        mSrc.mBase.mGFlag |= f;
    }
    void init(const dCcD_SrcGObjCo *);
    void setSomeAtFlags(u32 flags);
    void adjustHitPos(f32 dx, f32 dz);

    void SetGFlag(u32 flag) {
        mSrc.mBase.mGFlag = flag;
    }
};

class dCcD_ShapeAttr {
public:
    /* 0x00 */ u8 UNK_0x00[0x18 - 0x00];
    /* 0x18 */ dCcD_DivideInfo field_0x18;

    dCcD_ShapeAttr();

    /* vt 0x08 */ virtual ~dCcD_ShapeAttr();
    /* vt 0x0C */ virtual void dCcD_ShapeAttr_0x0C();
    /* vt 0x10 */ virtual void dCcD_ShapeAttr_0x10();
    /* vt 0x14 */ virtual void dCcD_ShapeAttr_0x14() = 0;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x18() = 0;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x1C() = 0;
    /* vt 0x20 */ virtual void dCcD_ShapeAttr_0x20() = 0;
    /* vt 0x24 */ virtual void dCcD_ShapeAttr_0x24() = 0;
    /* vt 0x28 */ virtual void dCcD_ShapeAttr_0x28(); // 0x80329110
    /* vt 0x2C */ virtual void dCcD_ShapeAttr_0x2C() = 0;
    /* vt 0x30 */ virtual void dCcD_ShapeAttr_0x30() = 0;
    /* vt 0x34 */ virtual void dCcD_ShapeAttr_0x34() = 0;
    /* vt 0x38 */ virtual void dCcD_ShapeAttr_0x38() = 0;
    /* vt 0x3C */ virtual void dCcD_ShapeAttr_0x3C() = 0;
    /* vt 0x40 */ virtual mVec3_c *getVirtualCenter(); // 0x80329120
    /* vt 0x44 */ virtual void dCcD_ShapeAttr_0x44() = 0;
    /* vt 0x48 */ virtual void dCcD_ShapeAttr_0x48() = 0;
    /* vt 0x4C */ virtual void getShapeAccess(); // 0x80329090
    /* vt 0x50 */ virtual void dCcD_ShapeAttr_0x50() = 0;
    /* vt 0x54 */ virtual void dCcD_ShapeAttr_0x54(); // 0x803290F0
    /* vt 0x58 */ virtual void dCcD_ShapeAttr_0x58() = 0;

    static mVec3_c m_virtual_center;
};

// This is in dAcObjBase_c @ 0x1c0
struct UnkCCDStruct {
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    /* 0x10 */ mVec3_c field_0x10;
    /* 0x1C */ mVec3_c field_0x1C;
    /* 0x28 */ mVec3_c posIncrements;
    dAcBase_c *field_0x34;
    s32 field_0x38;

    UnkCCDStruct(dAcBase_c *);

    void postExecute();
    void reset();
    fBaseID_e getId();
    u32 getSomething(int);

    void setField0x38ToMagicValue() {
        field_0x38 = 0xD;
    }

    void setField0x38(s32 val) {
        field_0x38 = val;
    }
};

// Object Info
class dCcD_GObjInf {
protected:
    /* 0x000 */ dCcD_GObjAt mGObjAt;
    /* 0x05C */ dCcD_GObjTg mGObjTg;
    /* 0x0D8 */ dCcD_GObjCo mGObjCo;
    /* 0x104 */ UnkCCDStruct *field_0x104;
    /* 0x108 */ u32 field_0x108;

public:
    dCcD_GObjInf();
    /* vt 0x08 */ virtual ~dCcD_GObjInf();
    /* vt 0x0C */ virtual void *dCcD_GObjInf_0x0C() = 0;
    /* vt 0x10 */ virtual void dCcD_GObjInf_0x10(); // 0x80328d30

    void clear();
    void init(const dCcD_SrcGObjInf &src);
    static bool weirdConditionCheck(dAcBase_c *ac, u32);

    inline void initUnk(UnkCCDStruct &src) {
        field_0x104 = &src;
    }

    inline void clearCoFlag() {
        mGObjCo.clearFlag();
    }

    void setTgCoFlag(u32 f) {
        mGObjTg.setFlag(f);
        mGObjCo.setFlag(f);
    }

    inline void setSomeDefendValue(u32 val) {
        mGObjTg.set0x4C(val);
    }

    inline void setAtVec(const mVec3_c &p) {
        mGObjAt.SetSomeVec(p);
    }

    inline void setTgFlag(u32 flag) {
        mGObjTg.setSrcFlag(flag);
    }

    inline mVec3_c *getAtVec() {
        return mGObjAt.GetSomeVec();
    }

    bool someInteractCheck() {
        return ((mGObjTg.getField0x14() & 1) != 0 && mGObjTg.getActor() != nullptr);
    }

    void setTgField0x0A(u32 val) {
        mGObjTg.setSrc0x0A(val);
    }

    void ResetAtHit();
    void ResetTgHit();
    void ResetCoHit();

    mVec3_c *GetAtHitPosP();
    mVec3_c *GetAtHitPosP2();

    dAcBase_c *GetAtActor();
    bool CheckCollidedMask(u32 mask);
    dAcBase_c *GetTgActor();
    dAcBase_c *GetCoActor();

    void adjustHitPos(f32 dx, f32 dz);
};

struct dCcD_SrcCylAttr {
public:
    /* 0x00 */ f32 mRadius;
    /* 0x04 */ f32 mHeight;
}; // Size = 0x8

struct dCcD_SrcSphAttr {
    /* 0x00 */ f32 mRadius;
}; // Size = 0x4

struct dCcD_SrcAabbAttr {
    /* 0x00 */ f32 minX, minY, minZ, maxX, maxY, maxZ;
}; // Size = 0x18

struct dCcD_SrcUnk1Attr {};

class cM3dGUnk1 {
public:
    /* 0x00 */ cM3dGCps mCps;
    /* 0x28 */ mVec3_c mVirtualCenter;
    cM3dGUnk1();
};

class cM3dGUnk2 {
public:
};

// Unk1
class dCcD_ShapeAttr1 : public dCcD_ShapeAttr, public cM3dGUnk1 {
public:
    dCcD_ShapeAttr1();

    void init(const dCcD_SrcUnk1Attr &);

    /* vt 0x08 */ virtual ~dCcD_ShapeAttr1();
    /* vt 0x0C */ virtual void dCcD_ShapeAttr_0x0C() override;
    /* vt 0x10 */ virtual void dCcD_ShapeAttr_0x10() override;
    /* vt 0x14 */ virtual void dCcD_ShapeAttr_0x14() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x18() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x1C() override;
    /* vt 0x20 */ virtual void dCcD_ShapeAttr_0x20() override;
    /* vt 0x24 */ virtual void dCcD_ShapeAttr_0x24() override;
    /* vt 0x28 */ virtual void dCcD_ShapeAttr_0x28() override;
    /* vt 0x2C */ virtual void dCcD_ShapeAttr_0x2C() override;
    /* vt 0x30 */ virtual void dCcD_ShapeAttr_0x30() override;
    /* vt 0x34 */ virtual void dCcD_ShapeAttr_0x34() override;
    /* vt 0x38 */ virtual void dCcD_ShapeAttr_0x38() override;
    /* vt 0x3C */ virtual void dCcD_ShapeAttr_0x3C() override;
    /* vt 0x40 */ virtual mVec3_c *getVirtualCenter() override;
    /* vt 0x44 */ virtual void dCcD_ShapeAttr_0x44() override;
    /* vt 0x48 */ virtual void dCcD_ShapeAttr_0x48() override;
    /* vt 0x4C */ virtual void getShapeAccess() override;
    /* vt 0x50 */ virtual void dCcD_ShapeAttr_0x50() override;
    // /* vt 0x54 */ virtual void dCcD_ShapeAttr_0x54() override;
    /* vt 0x58 */ virtual void dCcD_ShapeAttr_0x58() override;
};

// Unk1
class dCcD_ShapeAttr2 : public dCcD_ShapeAttr, public cM3dGUnk2 {
public:
    dCcD_ShapeAttr2();

    /* vt 0x08 */ virtual ~dCcD_ShapeAttr2();
    /* vt 0x0C */ virtual void dCcD_ShapeAttr_0x0C() override;
    /* vt 0x10 */ virtual void dCcD_ShapeAttr_0x10() override;
    /* vt 0x14 */ virtual void dCcD_ShapeAttr_0x14() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x18() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x1C() override;
    /* vt 0x20 */ virtual void dCcD_ShapeAttr_0x20() override;
    /* vt 0x24 */ virtual void dCcD_ShapeAttr_0x24() override;
    /* vt 0x28 */ virtual void dCcD_ShapeAttr_0x28() override;
    /* vt 0x2C */ virtual void dCcD_ShapeAttr_0x2C() override;
    /* vt 0x30 */ virtual void dCcD_ShapeAttr_0x30() override;
    /* vt 0x34 */ virtual void dCcD_ShapeAttr_0x34() override;
    /* vt 0x38 */ virtual void dCcD_ShapeAttr_0x38() override;
    /* vt 0x3C */ virtual void dCcD_ShapeAttr_0x3C() override;
    // /* vt 0x40 */ virtual mVec3_c *getVirtualCenter() override;
    /* vt 0x44 */ virtual void dCcD_ShapeAttr_0x44() override;
    /* vt 0x48 */ virtual void dCcD_ShapeAttr_0x48() override;
    /* vt 0x4C */ virtual void getShapeAccess() override;
    /* vt 0x50 */ virtual void dCcD_ShapeAttr_0x50() override;
    // /* vt 0x54 */ virtual void dCcD_ShapeAttr_0x54() override;
    /* vt 0x58 */ virtual void dCcD_ShapeAttr_0x58() override;
};

// Aabb
class dCcD_ShapeAttr5 : public dCcD_ShapeAttr, public cM3dGUnk {
public:
    dCcD_ShapeAttr5();

    void init(const dCcD_SrcAabbAttr &);

    /* vt 0x08 */ virtual ~dCcD_ShapeAttr5();
    /* vt 0x0C */ virtual void dCcD_ShapeAttr_0x0C() override;
    /* vt 0x10 */ virtual void dCcD_ShapeAttr_0x10() override;
    /* vt 0x14 */ virtual void dCcD_ShapeAttr_0x14() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x18() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x1C() override;
    /* vt 0x20 */ virtual void dCcD_ShapeAttr_0x20() override;
    /* vt 0x24 */ virtual void dCcD_ShapeAttr_0x24() override;
    /* vt 0x28 */ virtual void dCcD_ShapeAttr_0x28() override;
    /* vt 0x2C */ virtual void dCcD_ShapeAttr_0x2C() override;
    /* vt 0x30 */ virtual void dCcD_ShapeAttr_0x30() override;
    /* vt 0x34 */ virtual void dCcD_ShapeAttr_0x34() override;
    /* vt 0x38 */ virtual void dCcD_ShapeAttr_0x38() override;
    /* vt 0x3C */ virtual void dCcD_ShapeAttr_0x3C() override;
    /* vt 0x40 */ virtual mVec3_c *getVirtualCenter() override;
    /* vt 0x44 */ virtual void dCcD_ShapeAttr_0x44() override;
    /* vt 0x48 */ virtual void dCcD_ShapeAttr_0x48() override;
    // /* vt 0x4C */ virtual void getShapeAccess() override;
    /* vt 0x50 */ virtual void dCcD_ShapeAttr_0x50() override;
    // /* vt 0x54 */ virtual void dCcD_ShapeAttr_0x54() override;
    /* vt 0x58 */ virtual void dCcD_ShapeAttr_0x58() override;
};

// Cyl
class dCcD_ShapeAttr3 : public dCcD_ShapeAttr, public cM3dGCyl {
public:
    dCcD_ShapeAttr3();

    void init(const dCcD_SrcCylAttr &);

    /* vt 0x08 */ virtual ~dCcD_ShapeAttr3();
    /* vt 0x0C */ virtual void dCcD_ShapeAttr_0x0C() override;
    /* vt 0x10 */ virtual void dCcD_ShapeAttr_0x10() override;
    /* vt 0x14 */ virtual void dCcD_ShapeAttr_0x14() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x18() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x1C() override;
    /* vt 0x20 */ virtual void dCcD_ShapeAttr_0x20() override;
    /* vt 0x24 */ virtual void dCcD_ShapeAttr_0x24() override;
    /* vt 0x28 */ virtual void dCcD_ShapeAttr_0x28() override;
    /* vt 0x2C */ virtual void dCcD_ShapeAttr_0x2C() override;
    /* vt 0x30 */ virtual void dCcD_ShapeAttr_0x30() override;
    /* vt 0x34 */ virtual void dCcD_ShapeAttr_0x34() override;
    /* vt 0x38 */ virtual void dCcD_ShapeAttr_0x38() override;
    /* vt 0x3C */ virtual void dCcD_ShapeAttr_0x3C() override;
    /* vt 0x40 */ virtual mVec3_c *getVirtualCenter() override;
    /* vt 0x44 */ virtual void dCcD_ShapeAttr_0x44() override;
    /* vt 0x48 */ virtual void dCcD_ShapeAttr_0x48() override;
    /* vt 0x4C */ virtual void getShapeAccess() override;
    /* vt 0x50 */ virtual void dCcD_ShapeAttr_0x50() override;
    // /* vt 0x54 */ virtual void dCcD_ShapeAttr_0x54() override;
    /* vt 0x58 */ virtual void dCcD_ShapeAttr_0x58() override;
};

// Sph
class dCcD_ShapeAttr4 : public dCcD_ShapeAttr, public cM3dGSph {
public:
    dCcD_ShapeAttr4();

    void init(const dCcD_SrcSphAttr &);

    /* vt 0x08 */ virtual ~dCcD_ShapeAttr4();
    /* vt 0x0C */ virtual void dCcD_ShapeAttr_0x0C() override;
    /* vt 0x10 */ virtual void dCcD_ShapeAttr_0x10() override;
    /* vt 0x14 */ virtual void dCcD_ShapeAttr_0x14() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x18() override;
    /* vt 0x18 */ virtual void dCcD_ShapeAttr_0x1C() override;
    /* vt 0x20 */ virtual void dCcD_ShapeAttr_0x20() override;
    /* vt 0x24 */ virtual void dCcD_ShapeAttr_0x24() override;
    /* vt 0x28 */ virtual void dCcD_ShapeAttr_0x28() override;
    /* vt 0x2C */ virtual void dCcD_ShapeAttr_0x2C() override;
    /* vt 0x30 */ virtual void dCcD_ShapeAttr_0x30() override;
    /* vt 0x34 */ virtual void dCcD_ShapeAttr_0x34() override;
    /* vt 0x38 */ virtual void dCcD_ShapeAttr_0x38() override;
    /* vt 0x3C */ virtual void dCcD_ShapeAttr_0x3C() override;
    /* vt 0x40 */ virtual mVec3_c *getVirtualCenter() override;
    /* vt 0x44 */ virtual void dCcD_ShapeAttr_0x44() override;
    /* vt 0x48 */ virtual void dCcD_ShapeAttr_0x48() override;
    /* vt 0x4C */ virtual void getShapeAccess() override;
    /* vt 0x50 */ virtual void dCcD_ShapeAttr_0x50() override;
    /* vt 0x54 */ virtual void dCcD_ShapeAttr_0x54() override;
    /* vt 0x58 */ virtual void dCcD_ShapeAttr_0x58() override;
};

#endif
