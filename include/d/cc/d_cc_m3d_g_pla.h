#ifndef D_CC_M3D_G_PLA_H_
#define D_CC_M3D_G_PLA_H_

#include "d/cc/d_cc_m3d.h"
#include "m/m_vec.h"

// Plane with a normal
class cM3dGPla {
public:
    /* 0x00 */ mVec3_c mNormal;
    /* 0x0C */ f32 mD;
    /* 0x10   vtable */

    cM3dGPla() {}
    cM3dGPla(const mVec3_c *, f32);
    bool crossInfLin(const mVec3_c &, const mVec3_c &, mVec3_c &) const;
    void SetupNP0(const Vec &, const Vec &);
    void SetupNP(const Vec &, const Vec &);
    bool getCrossY(const mVec3_c &, f32 *) const;
    bool getCrossYLessD(const Vec &, f32 *) const;
    void Set(const cM3dGPla *);

    virtual ~cM3dGPla() {}

    // f32 getPlaneFunc(const Vec *pPoint) const {
    //     return mD + VECDotProduct(mNormal, pPoint);
    // }
    mVec3_c *GetNP() {
        return &mNormal;
    }
    const mVec3_c *GetNP() const {
        return &mNormal;
    }
    f32 GetD() const {
        return mD;
    }
    // void SetupFrom3Vtx(const Vec *v1, const Vec *v2, const Vec *v3) {
    //     cM3d_CalcPla(v1, v2, v3, &mNormal, &mD);
    // }
    f32 getCrossY_NonIsZero(const mVec3_c *param_1) {
        return ((-mNormal.x * param_1->x - mNormal.z * param_1->z) - mD) / mNormal.y;
    }
};

#endif
