#ifndef D_CC_SHAPE_COLLIDERS_H
#define D_CC_SHAPE_COLLIDERS_H

#include <m/m_vec.h>
#include <toBeSorted/cc/d_cc_d.h>

struct dCcD_SrcUnk {
    /* 0x00 */ dCcD_SrcGObjInf mObjInf;
    /* 0x44 */ cCcD_SrcUnk1Attr mUnk1Inf;
}; // Size: ???

class dCcD_Unk1 : public dCcD_GObjInf, public cCcD_ShapeAttr1 {
public:
    dCcD_Unk1();
    virtual ~dCcD_Unk1();

    virtual void *dCcD_GObjInf_0x0C() override;

    void init(const dCcD_SrcUnk &src);
    void unknownCalc();
};

struct dCcD_SrcEmpty {
    /* 0x00 */ dCcD_SrcGObjInf mObjInf;
}; // Size: 0x40

// Not sure what this is, the init function
// doesn't call the shape initializer
class dCcD_Empty : public dCcD_GObjInf, public cCcD_ShapeAttr2 {
public:
    dCcD_Empty();
    virtual ~dCcD_Empty();

    virtual void *dCcD_GObjInf_0x0C() override;

    void init(const dCcD_SrcEmpty &src);
};

struct dCcD_SrcCyl {
    /* 0x00 */ dCcD_SrcGObjInf mObjInf;
    /* 0x40 */ cCcD_SrcCylAttr mCylAttr;
}; // Size: 0x44

// Cylinder
class dCcD_Cyl : public dCcD_GObjInf, public cCcD_ShapeAttr3 {
public:
    dCcD_Cyl();
    virtual ~dCcD_Cyl();

    virtual void *dCcD_GObjInf_0x0C() override;
    void init(const dCcD_SrcCyl &src);
    void setCenter(const mVec3_c &);
    void moveCenter(const mVec3_c &);
};

struct dCcD_SrcSph {
    /* 0x00 */ dCcD_SrcGObjInf mObjInf;
    /* 0x40 */ cCcD_SrcSphAttr mSphAttr;
}; // Size: 0x44

// Sphere
class dCcD_Sph : public dCcD_GObjInf, public cCcD_ShapeAttr4 {
public:
    dCcD_Sph();
    virtual ~dCcD_Sph();

    virtual void *dCcD_GObjInf_0x0C() override;
    void init(const dCcD_SrcSph &src);
    void setCenter(const mVec3_c &);
    void moveCenter(const mVec3_c &);
    void setCenterAndAtVec(const mVec3_c &, const mVec3_c &);
};

struct dCcD_SrcAabb {
    /* 0x00 */ dCcD_SrcGObjInf mObjInf;
    /* 0x40 */ cCcD_SrcAabbAttr mAabbAttr;
}; // Size: 0x58

class dCcD_Aabb : public dCcD_GObjInf, public cCcD_ShapeAttr5 {
public:
    dCcD_Aabb();
    virtual ~dCcD_Aabb();

    virtual void *dCcD_GObjInf_0x0C() override;

    void init(const dCcD_SrcAabb &src);
};

#endif
