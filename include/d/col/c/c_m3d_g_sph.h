#ifndef C_M3D_G_SPH_H
#define C_M3D_G_SPH_H

#include "d/col/c/c_m3d_g_tri.h"
#include "m/m_vec.h"

class cM3dGSph {
public:
    /* 0x00 */ mVec3_c mCenter;
    /* 0x0C */ f32 mRadius;
    /* 0x10 */ f32 mRatio;
    cM3dGSph();

    void SetC(const mVec3_c &);
    void Set(const mVec3_c &, f32);
    void SetR(f32);
    void SetC(f32, f32, f32);

    f32 GetYDist(f32) const;
    bool Cross(const cM3dGTri *, f32 *, mVec3_c *);
    void Clamp(const mVec3_c &, mVec3_c &) const;

    bool fn_80338750(cM3dGUnk *, mVec3_c *);
    bool fn_80338760(cM3dGUnk *, f32 *);

    void SetRatio(f32);

    const mVec3_c &GetC() const {
        return mCenter;
    }

    const f32 &GetR() const {
        return mRadius;
    }

}; // Size = 0x14

#endif
