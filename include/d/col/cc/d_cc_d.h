#ifndef D_CC_D_H
#define D_CC_D_H

#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d.h"
#include "m/m_vec.h"

struct dCcD_SrcCps {
    /* 0x00 */ cCcD_SrcGObj mObjInf;
    /* 0x44 */ cCcD_SrcCpsAttr mCpsInf;
};

// Capsule
class dCcD_Cps : public cCcD_Obj, public cCcD_CpsAttr {
public:
    dCcD_Cps();
    virtual ~dCcD_Cps();

    virtual cCcD_ShapeAttr *GetShapeAttr() override;

    using cM3dGCps::Set;
    void Set(const dCcD_SrcCps &src);
    void unknownCalc();
};

struct dCcD_SrcTri {
    /* 0x00 */ cCcD_SrcGObj mObjInf;
};

class dCcD_Tri : public cCcD_Obj, public cCcD_TriAttr {
public:
    dCcD_Tri();
    virtual ~dCcD_Tri();

    virtual cCcD_ShapeAttr *GetShapeAttr() override;

    void Set(const dCcD_SrcTri &src);
};

struct dCcD_SrcCyl {
    /* 0x00 */ cCcD_SrcGObj mObjInf;
    /* 0x40 */ cCcD_SrcCylAttr mCylInf;
};

// Cylinder
class dCcD_Cyl : public cCcD_Obj, public cCcD_CylAttr {
public:
    dCcD_Cyl();
    virtual ~dCcD_Cyl();

    virtual cCcD_ShapeAttr *GetShapeAttr() override;
    void Set(const dCcD_SrcCyl &src);
    void setCenter(const mVec3_c &);
    void moveCenter(const mVec3_c &);
};

struct dCcD_SrcSph {
    /* 0x00 */ cCcD_SrcGObj mObjInf;
    /* 0x40 */ cCcD_SrcSphAttr mSphInf;
};

// Sphere
class dCcD_Sph : public cCcD_Obj, public cCcD_SphAttr {
public:
    dCcD_Sph();
    virtual ~dCcD_Sph();

    virtual cCcD_ShapeAttr *GetShapeAttr() override;
    void Set(const dCcD_SrcSph &src);
    void setCenter(const mVec3_c &);
    void moveCenter(const mVec3_c &);
    void setCenterAndAtVec(const mVec3_c &, const mVec3_c &);
};

struct dCcD_SrcUnk {
    /* 0x00 */ cCcD_SrcGObj mObjInf;
    /* 0x40 */ cCcD_SrcUnkAttr mUnkInf;
};

class dCcD_Unk : public cCcD_Obj, public cCcD_UnkAttr {
public:
    dCcD_Unk();
    virtual ~dCcD_Unk();

    virtual cCcD_ShapeAttr *GetShapeAttr() override;

    // Have to bring these in scope. Consider renaming this
    // Set function instead or create an inline?
    using cM3dGUnk::Set;
    void Set(const dCcD_SrcUnk &src);
};

#endif
