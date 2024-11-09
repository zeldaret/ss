#include "d/col/cc/d_cc_d.h"

#include "d/col/c/c_cc_d.h"
#include "m/m_vec.h"

dCcD_Cps::dCcD_Cps() {}
dCcD_Cps::~dCcD_Cps() {}

void dCcD_Cps::Set(const dCcD_SrcCps &src) {
    cCcD_Obj::Set(src.mObjInf);
    cCcD_CpsAttr::Set(src.mCpsInf);
}

cCcD_ShapeAttr *dCcD_Cps::GetShapeAttr() {
    return this;
}

void dCcD_Cps::unknownCalc() {
    PSVECSubtract(mEnd, mStart, mAt.mVec);
}

dCcD_Tri::dCcD_Tri() {}
dCcD_Tri::~dCcD_Tri() {}

void dCcD_Tri::Set(const dCcD_SrcTri &src) {
    cCcD_Obj::Set(src.mObjInf);
}

cCcD_ShapeAttr *dCcD_Tri::GetShapeAttr() {
    return this;
}

dCcD_Cyl::dCcD_Cyl() {}
dCcD_Cyl::~dCcD_Cyl() {}

void dCcD_Cyl::Set(const dCcD_SrcCyl &src) {
    cCcD_Obj::Set(src.mObjInf);
    cCcD_CylAttr::Set(src.mCylInf);
}

cCcD_ShapeAttr *dCcD_Cyl::GetShapeAttr() {
    return this;
}

void dCcD_Cyl::setCenter(const mVec3_c &c) {
    SetAtVec(mVec3_c::Zero);
    SetC(c);
}

void dCcD_Cyl::moveCenter(const mVec3_c &c) {
    SetAtVec(c - GetC());
    SetC(c);
}

dCcD_Sph::dCcD_Sph() {}
dCcD_Sph::~dCcD_Sph() {}

void dCcD_Sph::Set(const dCcD_SrcSph &src) {
    cCcD_Obj::Set(src.mObjInf);
    cCcD_SphAttr::Set(src.mSphInf);
}

void dCcD_Sph::setCenter(const mVec3_c &c) {
    SetAtVec(mVec3_c::Zero);
    SetC(&c);
}

void dCcD_Sph::moveCenter(const mVec3_c &c) {
    SetAtVec(c - GetC());
    SetC(&c);
}

void dCcD_Sph::setCenterAndAtVec(const mVec3_c &a, const mVec3_c &b) {
    SetAtVec(b);
    SetC(&a);
}

cCcD_ShapeAttr *dCcD_Sph::GetShapeAttr() {
    return this;
}

dCcD_Unk::dCcD_Unk() {}
dCcD_Unk::~dCcD_Unk() {}

void dCcD_Unk::Set(const dCcD_SrcUnk &src) {
    cCcD_Obj::Set(src.mObjInf);
    cCcD_UnkAttr::Set(src.mUnkInf);
}

cCcD_ShapeAttr *dCcD_Unk::GetShapeAttr() {
    return this;
}
