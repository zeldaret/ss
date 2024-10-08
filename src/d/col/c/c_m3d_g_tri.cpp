#include "d/col/c/c_m3d_g_tri.h"

using nw4r::math::VEC3;

void cM3dGTri::SetPos(const VecType *vA, const VecType *vB, const VecType *vC) {
    mA = *vA;
    mB = *vB;
    mC = *vC;
    cM3d_CalcPla(&mA, &mB, &mC, &mNormal, &mD);
}
