#include <math.h>
#include <nw4r/math/math_types.h>
#include <rvl/MTX.h>

using namespace nw4r::math;

inline VEC3 cM3d_Cross(const VEC3 *pVecA, const VEC3 *pVecB, const VEC3 *pVecC, VEC3 *pVecOut) {
    VEC3 tmp1;
    VEC3 tmp2;
    VEC3Sub(&tmp1, pVecB, pVecA);
    VEC3Sub(&tmp2, pVecC, pVecA);

    VEC3Cross(pVecOut, &tmp1, &tmp2);
}

inline f32 cM3d_Dot(const VEC3 *pVecA, const VEC3 *pVecB) {
    return VEC3Dot(pVecA, pVecB);
}

void cM3d_CalcPla(const VEC3 *pVecA, const VEC3 *pVecB, const VEC3 *pVecC, VEC3 *pVecOut, f32 *pD) {
    cM3d_Cross(pVecA, pVecB, pVecC, pVecOut);
    f32 mag = VEC3Len(pVecOut);
    if (fabsf(mag) > 0.05f) {
        VEC3Scale(pVecOut, pVecOut, 1.0f / mag);
        *pD = -cM3d_Dot(pVecOut, pVecA);
    } else {
        pVecOut->y = 0.0f;
        *pD = 0.0f;
        pVecOut->z = 0.0f;
        pVecOut->x = 0.0f;
    }
}
