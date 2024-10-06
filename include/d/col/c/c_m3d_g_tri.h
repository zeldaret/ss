#ifndef C_M3D_G_TRI_H
#define C_M3D_G_TRI_H

#include "common.h"
#include "d/col/c/c_m3d_g_pla.h"

class cM3dGTri : public cM3dGPla {
public:
    /* 0x10 */ mVec3_c mA;
    /* 0x1C */ mVec3_c mB;
    /* 0x28 */ mVec3_c mC;

    cM3dGTri();
    void SetPos(const mVec3_c *, const mVec3_c *, const mVec3_c *);
};

#endif
