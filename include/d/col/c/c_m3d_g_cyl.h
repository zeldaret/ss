#ifndef C_M3D_G_CYL_H
#define C_M3D_G_CYL_H

#include <m/m_vec.h>

class cM3dGCyl {
public:
    /* 0x00 */ mVec3_c mCenter;
    /* 0x0C */ f32 mRadius;
    /* 0x10 */ f32 mHeight;
    cM3dGCyl() {}

    void setC(const mVec3_c &, f32 radius, f32 height);
    void setC(const mVec3_c &);
    void setH(f32);
    void setR(f32);

    const mVec3_c &GetC() const {
        return mCenter;
    }

}; // Size = 0x14

#endif
