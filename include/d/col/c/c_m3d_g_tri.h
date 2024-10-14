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

    void SetBg(const VecType *, const VecType *, const VecType *, const cM3dGPla *pla);

    bool cross(const cM3dGLin *lin, VecType *xyz, bool param_2, bool param_3) const {
        return cM3d_Cross_LinTri(lin, this, xyz, param_2, param_3);
    }
};

#endif
