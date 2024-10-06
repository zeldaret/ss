#ifndef C_M3D_G_SPH_H
#define C_M3D_G_SPH_H

#include <m/m_vec.h>

class cM3dGSph {
public:
    /* 0x00 */ mVec3_c mCenter;
    /* 0x0C */ f32 mRadius;
    /* 0x10 */ f32 field_0x10;
    cM3dGSph();

    void setR(f32);
    void set(const mVec3_c &, f32);
    void setC(const mVec3_c &);
    void setC(f32, f32, f32);
    const mVec3_c &GetC() const {
        return mCenter;
    }

}; // Size = 0x14

#endif
