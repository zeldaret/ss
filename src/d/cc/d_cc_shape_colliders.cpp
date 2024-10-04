#include <d/cc/d_cc_d.h>
#include <d/cc/d_cc_shape_colliders.h>
#include <m/m_vec.h>


dCcD_Unk1::dCcD_Unk1() {}
dCcD_Unk1::~dCcD_Unk1() {}

void dCcD_Unk1::init(const dCcD_SrcUnk &src) {
    dCcD_GObjInf::init(src.mObjInf);
    dCcD_ShapeAttr1::init(src.mUnk1Inf);
}

void *dCcD_Unk1::dCcD_GObjInf_0x0C() {
    if (this != nullptr) {
        return &UNK_0x00;
    }
    return this;
}

void dCcD_Unk1::unknownCalc() {
    PSVECSubtract(*&field_0x0C, *&field_0x00, *getAtVec());
}

dCcD_Empty::dCcD_Empty() {}
dCcD_Empty::~dCcD_Empty() {}

void dCcD_Empty::init(const dCcD_SrcEmpty &src) {
    dCcD_GObjInf::init(src.mObjInf);
}

void *dCcD_Empty::dCcD_GObjInf_0x0C() {
    if (this != nullptr) {
        return &UNK_0x00;
    }
    return this;
}

dCcD_Cyl::dCcD_Cyl() {}
dCcD_Cyl::~dCcD_Cyl() {}

void dCcD_Cyl::init(const dCcD_SrcCyl &src) {
    dCcD_GObjInf::init(src.mObjInf);
    dCcD_ShapeAttr3::init(src.mCylAttr);
}

void *dCcD_Cyl::dCcD_GObjInf_0x0C() {
    if (this != nullptr) {
        return &UNK_0x00;
    }
    return this;
}

void dCcD_Cyl::setCenter(const mVec3_c &c) {
    setAtVec(mVec3_c::Zero);
    setC(c);
}

void dCcD_Cyl::moveCenter(const mVec3_c &c) {
    setAtVec(c - GetC());
    setC(c);
}

dCcD_Sph::dCcD_Sph() {}
dCcD_Sph::~dCcD_Sph() {}

void dCcD_Sph::init(const dCcD_SrcSph &src) {
    dCcD_GObjInf::init(src.mObjInf);
    dCcD_ShapeAttr4::init(src.mSphAttr);
}

void dCcD_Sph::setCenter(const mVec3_c &c) {
    setAtVec(mVec3_c::Zero);
    setC(c);
}

void dCcD_Sph::moveCenter(const mVec3_c &c) {
    setAtVec(c - GetC());
    setC(c);
}

void dCcD_Sph::setCenterAndAtVec(const mVec3_c &a, const mVec3_c &b) {
    setAtVec(b);
    setC(a);
}

void *dCcD_Sph::dCcD_GObjInf_0x0C() {
    if (this != nullptr) {
        return &UNK_0x00;
    }
    return this;
}

dCcD_Aabb::dCcD_Aabb() {}
dCcD_Aabb::~dCcD_Aabb() {}

void dCcD_Aabb::init(const dCcD_SrcAabb &src) {
    dCcD_GObjInf::init(src.mObjInf);
    dCcD_ShapeAttr5::init(src.mAabbAttr);
}

void *dCcD_Aabb::dCcD_GObjInf_0x0C() {
    if (this != nullptr) {
        return &UNK_0x00;
    }
    return this;
}
