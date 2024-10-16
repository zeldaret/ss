#ifndef C_M3D_G_CYL_H
#define C_M3D_G_CYL_H

#include "d/col/c/c_m3d_g_unk.h"
#include "m/m_vec.h"

class cM3dGCyl {
public:
    /* 0x00 */ mVec3_c mCenter;
    /* 0x0C */ f32 mRadius;
    /* 0x10 */ f32 mHeight;
    cM3dGCyl() {}

    void Set(const mVec3_c &, f32, f32);
    void SetC(const mVec3_c &, f32 radius, f32 height);
    void SetC(const mVec3_c &);
    void SetH(f32);
    void SetR(f32);

    bool fn_803380e0(cM3dGUnk *, mVec3_c *);
    bool fn_803380f0(cM3dGUnk *, f32 *);
    bool Cross(const mVec3_c &) const;
    void Clamp(const mVec3_c &, mVec3_c &) const;

    const mVec3_c &GetC() const {
        return mCenter;
    }
};

#endif
