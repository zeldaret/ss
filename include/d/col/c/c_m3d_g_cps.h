#ifndef C_M3D_G_CPS_H
#define C_M3D_G_CPS_H

#include "d/col/c/c_m3d.h"
#include "d/col/c/c_m3d_g_lin.h"
#include "d/col/c/c_m3d_g_unk.h"
#include "m/m_vec.h"

class cM3dGCps : public cM3dGLin {
public:
    /* 0x18 */ f32 mRadius;
    /* 0x1C */ f32 mField_0x1C;
    /* 0x20 */ u8 mField_0x20;
    /* 0x24 */ u32 mFlags;

    cM3dGCps();

    void Set(const mVec3_c &, const mVec3_c &, f32);
    void SetR(f32 r);
    void Set_0x1C(f32);
    bool Cross(cM3dGUnk &, mVec3_c *);
    bool fn_80337f40(const mVec3_c &) const;

    f32 GetR() const {
        return mRadius;
    }
    // bool Cross(cM3dGCps const *other, mVec3_c *xyz) const {
    //     return cM3d_Cross_CpsCps(*this, *other, xyz);
    // }
    // bool Cross(const cM3dGCyl *cyl, mVec3_c *xyz) const {
    //     return cM3d_Cross_CpsCyl(*this, *cyl, xyz);
    // }
    // bool Cross(const cM3dGSph *sph, mVec3_c *xyz) const {
    //     return cM3d_Cross_CpsSph(*this, *sph, xyz);
    // }
};

#endif
