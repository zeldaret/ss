#ifndef C_M3D_G_PLA_H
#define C_M3D_G_PLA_H

#include "c/c_math.h"
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
    bool CrossInfLin(const mVec3_c &, const mVec3_c &, mVec3_c &) const;
    bool getCrossYLessD(const mVec3_c &, f32 *) const;
    mAng GetAngle(mAng) const;
    mAng GetNegativeAngle(mAng) const;

    // Unused
    // void SetupNP0(const mVec3_c &, const mVec3_c &);
    // void SetupNP(const mVec3_c &, const mVec3_c &);
    // bool getCrossY(const mVec3_c &, f32 *) const;
    // void Set(const cM3dGPla *);

    f32 getPlaneFunc(const mVec3_c &pnt) const {
        return mD + VEC3Dot(mNormal, pnt);
    }
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

    u32 GetXZAngle() const {
        return cM::atan2s(mNormal.x, mNormal.z);
    }
};

#endif
