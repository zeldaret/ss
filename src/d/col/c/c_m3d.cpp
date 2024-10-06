#include "d/col/c/c_m3d.h"

#include "d/col/c/c_m3d_g_aab.h"
#include "d/col/c/c_m3d_g_cir.h"
#include "d/col/c/c_m3d_g_cps.h"
#include "d/col/c/c_m3d_g_cyl.h"
#include "d/col/c/c_m3d_g_lin.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/col/c/c_m3d_g_sph.h"
#include "d/col/c/c_m3d_g_tri.h"
#include "d/col/c/c_m3d_g_unk.h"
#include "egg/math/eggMath.h"
#include "m/m_vec.h"
#include "math.h"
#include "nw4r/math/math_types.h"
#include "rvl/MTX.h"

using namespace nw4r::math;
using namespace EGG;

const f32 G_CM3D_F_ABS_MIN = 32 * FLT_EPSILON;

inline VEC3 cM3d_Cross(const VEC3 *pVecA, const VEC3 *pVecB, const VEC3 *pVecC, VEC3 *pVecOut) {
    VEC3 tmp1;
    VEC3 tmp2;
    VEC3Sub(&tmp1, pVecB, pVecA);
    VEC3Sub(&tmp2, pVecC, pVecA);

    VEC3Cross(pVecOut, &tmp1, &tmp2);
}

bool cM3d_Len2dSqPntAndSegLine(f32, f32, f32, f32, f32, f32, f32 *, f32 *, f32 *) {}
bool cM3d_Len3dSqPntAndSegLine(const cM3dGLin *, const VEC3 *, VEC3 *, f32 *, f32 *) {}
f32 cM3d_SignedLenPlaAndPos(const cM3dGPla *, const VEC3 *) {}

void cM3d_CalcPla(const VEC3 *pVecA, const VEC3 *pVecB, const VEC3 *pVecC, VEC3 *pVecOut, f32 *pD) {
    cM3d_Cross(pVecA, pVecB, pVecC, pVecOut);
    f32 mag = VEC3Len(pVecOut);
    if (fabsf(mag) > 0.05f) {
        VEC3Scale(pVecOut, pVecOut, Math<f32>::inv(mag));
        f32 dot = VEC3Dot(pVecOut, pVecA);
        *pD = -dot;
    } else {
        pVecOut->y = 0.0f;
        *pD = 0.0f;
        pVecOut->z = 0.0f;
        pVecOut->x = 0.0f;
    }
}
int cM3d_Check_LinLin(const cM3dGLin *, const cM3dGLin *, f32 *, f32 *) {}
bool cM3d_Cross_LinPla(const cM3dGLin *, const cM3dGPla *, VEC3 *, bool, bool) {}
bool cM3d_Cross_MinMaxBoxLine(const VEC3 *, const VEC3 *, const VEC3 *, const VEC3 *) {}
bool cM3d_InclusionCheckPosIn3PosBox3d(const VEC3 *, const VEC3 *, const VEC3 *, const VEC3 *, f32) {}
bool cM3d_CrossX_Tri(const cM3dGTri *, const VEC3 *, f32) {}
bool cM3d_CrossX_Tri(const cM3dGTri *, const VEC3 *) {}
bool cM3d_CrossY_Tri(const cM3dGTri *, const VEC3 *) {}
bool cM3d_CrossY_Tri_Front(const VEC3 &, const VEC3 &, const VEC3 &, const VEC3 *) {}
bool cM3d_CrossY_Tri(const cM3dGTri *, const VEC3 *, f32 *) {}
bool cM3d_CrossY_Tri(const cM3dGTri *, const VEC3 *, f32) {}
bool cM3d_CrossY_Tri(const cM3dGTri *, const VEC3 *, const cM3d_Range *, f32 *) {}
bool cM3d_CrossZ_Tri(const cM3dGTri *, const VEC3 *, f32) {}
bool cM3d_CrossZ_Tri(const cM3dGTri *, const VEC3 *) {}
bool cM3d_CrossZ_Tri(const cM3dGTri *, const VEC3 *, f32 *) {}
bool cM3d_Cross_LinTri(const cM3dGLin *, const cM3dGTri *, VEC3 *, bool, bool) {}
bool cM3d_Cross_LinTri(
    const cM3dGLin *, const VEC3 *, const VEC3 *, const VEC3 *, const cM3dGTri *, VEC3 *, bool, bool
) {}

bool cM3d_Cross_LinTri_Easy(const cM3dGTri *pTri, const VEC3 *pPos) {
    if ((cM3d_IsZero(pTri->mNormal.x) || cM3d_CrossX_Tri(pTri, pPos)) &&
        (cM3d_IsZero(pTri->mNormal.y) || cM3d_CrossY_Tri(pTri, pPos)) &&
        (cM3d_IsZero(pTri->mNormal.z) || cM3d_CrossZ_Tri(pTri, pPos))) {
        return true;
    }
    return false;
}

bool cM3d_Cross_SphPnt(const cM3dGSph *pSph, const VEC3 *pPos) {
    f32 r2 = pSph->GetR();
    return VEC3DistSq(pSph->GetC(), pPos) < r2 * r2;
}

bool cM3d_Cross_LinSph(const cM3dGLin *, const cM3dGSph *, VEC3 *) {}
bool cM3d_Cross_LinSph_CrossPos(const cM3dGSph &, const cM3dGLin &, VEC3 *, VEC3 *) {}
bool cM3d_Cross_CylSph(const cM3dGCyl *, const cM3dGSph *, f32 *) {}
bool cM3d_Cross_CylSph(const cM3dGCyl *, const cM3dGSph *, VEC3 *, f32 *) {}
bool cM3d_Cross_SphSph(const cM3dGSph *, const cM3dGSph *, f32 *) {}
bool cM3d_Cross_SphSph(const cM3dGSph *, const cM3dGSph *, f32 *, f32 *) {}
bool cM3d_Cross_SphSph(const cM3dGSph *, const cM3dGSph *, VEC3 *) {}

inline void cM3d_InDivPos1(const VEC3 *pVecA, const VEC3 *pVecB, f32 pF, VEC3 *pOut) {
    VEC3 tmp;
    VEC3Scale(&tmp, pVecB, pF);
    VEC3Add(&tmp, pVecA, pOut);
}
inline void cM3d_InDivPos2(const VEC3 *pVecA, const VEC3 *pVecB, f32 pF, VEC3 *pOut) {
    VEC3 tmp;
    VEC3Sub(&tmp, pVecB, pVecA);
    cM3d_InDivPos1(pVecA, &tmp, pF, pOut);
}

void cM3d_CalcSphVsTriCrossPoint(const cM3dGSph *pSph, const cM3dGTri *pTri, VEC3 *pPnt) {
    VEC3 scale, add;
    PSVECAdd(pTri->mA, pTri->mB, add);
    PSVECScale(add, scale, 0.5f);
    f32 mag = VEC3DistSq(&scale, pSph->GetC());
    if (cM3d_IsZero(mag)) {
        *pPnt = pSph->GetC();
    } else {
        cM3d_InDivPos2(&pSph->GetC(), &scale, pSph->GetR() / mag, pPnt);
    }
}
bool cM3d_Cross_SphTri(const cM3dGSph *, const cM3dGTri *, VEC3 *, f32 *, VEC3 *) {}
bool cM3d_Cross_CylCyl(const cM3dGCyl *, const cM3dGCyl *, f32 *) {}
bool cM3d_Cross_CylCyl(const cM3dGCyl *, const cM3dGCyl *, VEC3 *) {}
bool cM3d_Cross_CylTri(const cM3dGCyl *, const cM3dGTri *, VEC3 *) {}
int cM3d_Cross_CylLin(const cM3dGCyl *, const cM3dGLin *, VEC3 *, VEC3 *) {}
int cM3d_Cross_CylPntPnt(const cM3dGCyl *, const VEC3 *, const VEC3 *, VEC3 *, VEC3 *) {}
bool cM3d_Cross_CylPnt(const cM3dGCyl *, const VEC3 *) {}
bool cM3d_Cross_CpsSph_CrossPos(const cM3dGCps &, const cM3dGSph &, const VEC3 &, VEC3 *) {}
bool cM3d_Cross_CpsSph(const cM3dGCps &, const cM3dGSph &, VEC3 *) {}
bool cM3d_Cross_TriTri(const cM3dGTri &, const cM3dGTri &, VEC3 *) {}
bool cM3d_Cross_CpsTri(const cM3dGCps &, cM3dGTri, VEC3 *) {}
void cM3d_CalcVecZAngle(const VEC3 &, mAng3_c *) {}
void cM3d_PlaneCrossLineProcWork(f32, f32, f32, f32, f32, f32, f32, f32 *, f32 *) {}
int cM3d_2PlaneCrossLine(const cM3dGPla &, const cM3dGPla &, cM3dGLin *) {}
bool cM3d_3PlaneCrossPos(const cM3dGPla &, const cM3dGPla &, const cM3dGPla &, VEC3 *) {}
f32 cM3d_lineVsPosSuisenCross(const cM3dGLin *, const VEC3 *, VEC3 *) {}
f32 cM3d_lineVsPosSuisenCross(const VEC3 &, const VEC3 &, const VEC3 &, VEC3 *) {}
