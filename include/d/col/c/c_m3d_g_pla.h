#ifndef C_M3D_G_PLA_H
#define C_M3D_G_PLA_H

#include "d/col/c/c_m3d.h"
#include "m/m_vec.h"

// Plane with a normal
class cM3dGPla {
public:
    /* 0x00 */ mVec3_c mNormal;
    /* 0x0C */ f32 mD;

    cM3dGPla() {}
    cM3dGPla(const mVec3_c *v, f32 f) {
        mNormal = *v;
        f = mD;
    }
    bool crossInfLin(const mVec3_c &, const mVec3_c &, mVec3_c &) const;
    void SetupNP0(const mVec3_c &, const mVec3_c &);
    void SetupNP(const mVec3_c &, const mVec3_c &);
    bool getCrossY(const mVec3_c &, f32 *) const;
    bool getCrossYLessD(const mVec3_c &, f32 *) const;
    void Set(const cM3dGPla *);

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
