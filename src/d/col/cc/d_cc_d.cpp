#include <d/a/d_a_base.h>
#include <d/col/cc/d_cc_d.h>
#include <f/f_base_id.h>
#include <m/m_vec.h>

inline bool cM3d_IsZero(f32 f);

mVec3_c dCcD_ShapeAttr::m_virtual_center = mVec3_c::Zero;

dCcD_DivideInfo::dCcD_DivideInfo() {}
dCcD_DivideInfo::~dCcD_DivideInfo() {}

dCcD_DivideArea::dCcD_DivideArea() {}
dCcD_DivideArea::~dCcD_DivideArea() {}

void d_cc_d_float_order() {
    1.0f / 32.0f;
    1.0f;
    0.0f;
    FLT_EPSILON;
    1.0f / 3.0f;
    1.0e9f;
    -1.0e9f;
    -1.0f;
    0.5f;
}

// Very certain
void dCcD_DivideArea::SetArea(cM3dGAab const &aab) {
    Set(aab.mMin, aab.mMax);

    mScaledXDiff = 1.0f / 32.0f * (mMax.x - mMin.x);
    mXDiffIsZero = cM3d_IsZero(mScaledXDiff);
    if (!mXDiffIsZero) {
        mInvScaledXDiff = 1.0f / mScaledXDiff;
    }

    mScaledYDiff = 1.0f / 32.0f * (mMax.y - mMin.y);
    mYDiffIsZero = cM3d_IsZero(mScaledYDiff);
    if (!mYDiffIsZero) {
        mInvScaledYDiff = 1.0f / mScaledYDiff;
    }

    mScaledZDiff = 1.0f / 32.0f * (mMax.z - mMin.z);
    mZDiffIsZero = cM3d_IsZero(mScaledZDiff);
    if (!mZDiffIsZero) {
        mInvScaledZDiff = 1.0f / mScaledZDiff;
    }
}

inline bool cM3d_IsZero(f32 f) {
    return fabsf(f) < FLT_EPSILON;
}

static u32 const l_base[32] = {
    0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF,
    0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF,
    0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
    0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
};

// TODO

UnkCCDStruct::UnkCCDStruct(dAcBase_c *arg) : field_0x00(0), field_0x04(0), field_0x08(0), field_0x0C(0) {
    field_0x38 = 0;
    field_0x34 = arg;
    reset();
}

void UnkCCDStruct::postExecute() {
    field_0x04 = field_0x00;
    field_0x00 = 0;
    field_0x0C = field_0x08;
    field_0x08 = 0;
}

void UnkCCDStruct::reset() {
    field_0x10 = mVec3_c::Zero;
    field_0x1C = mVec3_c::Zero;
    posIncrements = mVec3_c::Zero;
}

fBaseID_e UnkCCDStruct::getId() {
    if (field_0x34 != nullptr) {
        return field_0x34->unique_ID;
    }
    return (fBaseID_e)0;
}

static u32 const sth[] = {0x4B, 0x5A, 0x64, 0x00};

u32 UnkCCDStruct::getSomething(int arg) {
    if (field_0x38 == 0x0D || arg == 0) {
        return 0;
    }

    if (field_0x38 == 0 || arg == 0x0D) {
        return 100;
    }

    if (field_0x38 == arg) {
        return 50;
    }

    if (field_0x38 == 0x0C || arg == 0x01) {
        return 0;
    }

    if (field_0x38 == 1 || arg == 0x0C) {
        return 100;
    }

    arg = arg - field_0x38;
    if (arg > 0) {
        return sth[(arg <= 3 ? arg : 3) - 1];
    } else {
        return 100 - sth[(-arg <= 3 ? -arg : 3) - 1];
    }
}

dCcD_GAtTgCoCommonBase::dCcD_GAtTgCoCommonBase() : field_0x00(0), field_0x14(0), mEffCounter(0) {}

dCcD_GAtTgCoCommonBase::~dCcD_GAtTgCoCommonBase() {
    unlink();
    field_0x14 = 0;
    unlink();
    decreaseCount();
    field_0x00 = 0;
}

void dCcD_GAtTgCoCommonBase::unlink() {
    mActor.unlink();
}

void dCcD_GAtTgCoCommonBase::setActor(dAcBase_c *actor) {
    field_0x14 |= 1;
    mActor.link(actor);
}

dAcBase_c *dCcD_GAtTgCoCommonBase::getActor() {
    return mActor.get();
}

void dCcD_GAtTgCoCommonBase::decreaseCount() {
    if (mEffCounter <= 0) {
        return;
    }
    mEffCounter--;
}

dCcD_GObjInf::dCcD_GObjInf() {
    field_0x104 = nullptr;
    field_0x108 = 0;
}

dCcD_GObjInf::~dCcD_GObjInf() {
    field_0x104 = nullptr;
}

bool dCcD_GObjInf::weirdConditionCheck(dAcBase_c *ac, u32 arg) {
    return ac != nullptr && ac->isActorPlayer() && arg == 2;
}

void dCcD_GObjInf::clear() {
    mGObjAt.clearFlag();
    mGObjTg.clearFlag();
    mGObjCo.clearFlag();
}

void dCcD_GObjInf::init(const dCcD_SrcGObjInf &src) {
    mGObjAt.init(&src.mGObjAt);
    mGObjTg.init(&src.mGObjTg);
    mGObjCo.init(&src.mGObjCo);
}

void dCcD_GObjInf::ResetAtHit() {
    mGObjAt.resetField0x14();
    mGObjAt.unlink();
    mGObjAt.decreaseCount();
    mGObjAt.resetEffCounter();
}

void dCcD_GObjInf::ResetTgHit() {
    mGObjTg.resetField0x14();
    mGObjTg.unlink();
    mGObjTg.decreaseCount();
    mGObjTg.resetEffCounter();
}

void dCcD_GObjInf::ResetCoHit() {
    mGObjCo.resetField0x14();
    mGObjCo.unlink();
    mGObjCo.decreaseCount();
    mGObjCo.resetEffCounter();
}

mVec3_c *dCcD_GObjInf::GetAtHitPosP() {
    return mGObjAt.GetHitPosP();
}

mVec3_c *dCcD_GObjInf::GetAtHitPosP2() {
    return mGObjAt.GetHitPosP();
}

void dCcD_GObjInf::dCcD_GObjInf_0x10() {}

dAcBase_c *dCcD_GObjInf::GetAtActor() {
    return mGObjAt.getActor();
}

dAcBase_c *dCcD_GObjInf::GetTgActor() {
    return mGObjTg.getActor();
}

dAcBase_c *dCcD_GObjInf::GetCoActor() {
    return mGObjCo.getActor();
}

void dCcD_GObjInf::adjustHitPos(f32 dx, f32 dz) {
    mGObjAt.adjustHitPos(dx, dz);
    mGObjTg.adjustHitPos(dx, dz);
    mGObjCo.adjustHitPos(dx, dz);
}

/**
 *
 * Geometric shape colliders
 *
 */

dCcD_ShapeAttr::dCcD_ShapeAttr() {}

dCcD_ShapeAttr::~dCcD_ShapeAttr() {}

mVec3_c *dCcD_ShapeAttr::getVirtualCenter() {
    return &m_virtual_center;
}

dCcD_ShapeAttr2::dCcD_ShapeAttr2() {}
dCcD_ShapeAttr2::~dCcD_ShapeAttr2() {}

dCcD_ShapeAttr5::dCcD_ShapeAttr5() {}
dCcD_ShapeAttr5::~dCcD_ShapeAttr5() {}

void dCcD_ShapeAttr5::init(const dCcD_SrcAabbAttr &src) {
    setMinMax(mVec3_c(src.minX, src.minY, src.minZ), mVec3_c(src.maxX, src.maxY, src.maxZ));
}

mVec3_c *dCcD_ShapeAttr5::getVirtualCenter() {
    return &mVirtualCenter;
}

dCcD_ShapeAttr1::dCcD_ShapeAttr1() {}
dCcD_ShapeAttr1::~dCcD_ShapeAttr1() {}

mVec3_c *dCcD_ShapeAttr1::getVirtualCenter() {
    return &mVirtualCenter;
}

dCcD_ShapeAttr3::dCcD_ShapeAttr3() {}

dCcD_ShapeAttr3::~dCcD_ShapeAttr3() {}

void dCcD_ShapeAttr3::init(const dCcD_SrcCylAttr &src) {
    setR(src.mRadius);
    setH(src.mHeight);
    mVec3_c v;
    v.z = 0.0f;
    v.y = 0.0f;
    v.x = 0.0f;
    setC(v);
}

mVec3_c *dCcD_ShapeAttr3::getVirtualCenter() {
    return &mCenter;
}

dCcD_ShapeAttr4::dCcD_ShapeAttr4() {}
dCcD_ShapeAttr4::~dCcD_ShapeAttr4() {}

void dCcD_ShapeAttr4::init(const dCcD_SrcSphAttr &src) {
    setR(src.mRadius);
    mVec3_c v;
    v.z = 0.0f;
    v.y = 0.0f;
    v.x = 0.0f;
    setC(v);
}

mVec3_c *dCcD_ShapeAttr4::getVirtualCenter() {
    return &mCenter;
}

/**
 *
 * At/Tg/Co collider
 *
 */

dCcD_GObjAt::dCcD_GObjAt() {
    field_0x58 = 0;
}

dCcD_GObjAt::~dCcD_GObjAt() {}

void dCcD_GObjAt::init(const dCcD_SrcGObjAt *src) {
    mEffCounter = 0;
    mSrc = *src;
    mHitPos = mVec3_c::Zero;
}

void dCcD_GObjAt::setSomeAtFlags(u32 flags) {
    mSrc.field_0x04 = mSrc.field_0x04 & 0xFFFFFFC1 | flags;
}

void dCcD_GObjAt::adjustHitPos(f32 dx, f32 dz) {
    mHitPos.x += dx;
    mHitPos.z += dz;
}

dCcD_GObjTg::dCcD_GObjTg() {
    field_0x44 = 0;
    field_0x4A = 0;
    field_0x4B = 0;
    field_0x4C = 0;
    field_0x50 = 0;
    field_0x54 = 0;
    field_0x6C.x = 0.0f;
    field_0x6C.y = 0.0f;
    field_0x6C.z = 0.0f;
    field_0x78 = 0;
}

dCcD_GObjTg::~dCcD_GObjTg() {}

void dCcD_GObjTg::init(const dCcD_SrcGObjTg *src) {
    mEffCounter = 0;
    mSrc = *src;
    field_0x4C = 0;
    mHitPos = mVec3_c::Zero;
    field_0x44 = 0;
    field_0x48 = 0x4000;
}

void dCcD_GObjTg::adjustHitPos(f32 dx, f32 dz) {
    mHitPos.x += dx;
    mHitPos.z += dz;
    field_0x6C.x += dx;
    field_0x6C.z += dz;
}

dCcD_GObjCo::dCcD_GObjCo() {
    field_0x20 = 0;
    field_0x28 = 0;
}

dCcD_GObjCo::~dCcD_GObjCo() {}

void dCcD_GObjCo::init(const dCcD_SrcGObjCo *src) {
    mEffCounter = 0;
    mSrc = *src;
    setSomeAtFlags(src->mBase.mGFlag & 0x1E0);
}

void dCcD_GObjCo::setSomeAtFlags(u32 flags) {}

void dCcD_GObjCo::adjustHitPos(f32 dx, f32 dz) {}
