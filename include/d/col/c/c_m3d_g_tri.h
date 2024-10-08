#ifndef C_M3D_G_TRI_H
#define C_M3D_G_TRI_H

#include "common.h"
#include "d/col/c/c_m3d_g_pla.h"

class cM3dGTri : public cM3dGPla {
    typedef nw4r::math::VEC3 VecType;

public:
    /* 0x10 */ VecType mA;
    /* 0x1C */ VecType mB;
    /* 0x28 */ VecType mC;

    cM3dGTri() {}
    void SetPos(const VecType *, const VecType *, const VecType *);
};

#endif
