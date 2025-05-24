#ifndef C_M3D_H
#define C_M3D_H

#include "common.h"
#include "math.h"
#include "nw4r/types_nw4r.h"

#include "nw4r/math.h" // IWYU pragma: export

class cM3dGAab;
class cM3dGCps;
class cM3dGCyl;
class cM3dGLin;
class cM3dGPla;
class cM3dGSph;
class cM3dGTri;
class cM3dGUnk;
class mAng3_c;
struct cM3d_Range;

extern const f32 G_CM3D_F_ABS_MIN;

bool cM3d_Len2dSqPntAndSegLine(f32, f32, f32, f32, f32, f32, f32 *, f32 *, f32 *);
bool cM3d_Len3dSqPntAndSegLine(const cM3dGLin &, const nw4r::math::VEC3 &, nw4r::math::VEC3 *, f32 *, f32 *);
f32 cM3d_SignedLenPlaAndPos(const cM3dGPla *, const nw4r::math::VEC3 *);
void cM3d_CalcPla(
    const nw4r::math::VEC3 *, const nw4r::math::VEC3 *, const nw4r::math::VEC3 *, nw4r::math::VEC3 *, f32 *
);
int cM3d_Check_LinLin(const cM3dGLin &, const cM3dGLin *, f32 *, f32 *);
bool cM3d_Cross_LinPla(const cM3dGLin &, const cM3dGPla *, nw4r::math::VEC3 *, bool, bool);
bool cM3d_Cross_MinMaxBoxLine(
    const nw4r::math::VEC3 *, const nw4r::math::VEC3 *, const nw4r::math::VEC3 *, const nw4r::math::VEC3 *
);
bool cM3d_InclusionCheckPosIn3PosBox3d(
    const nw4r::math::VEC3 *, const nw4r::math::VEC3 *, const nw4r::math::VEC3 *, const nw4r::math::VEC3 *, f32
);
bool cM3d_CrossX_Tri(const cM3dGTri &, const nw4r::math::VEC3 *, f32);
bool cM3d_CrossX_Tri(const cM3dGTri &, const nw4r::math::VEC3 *);
bool cM3d_CrossY_Tri(const cM3dGTri &, const nw4r::math::VEC3 *);
bool cM3d_CrossY_Tri_Front(
    const nw4r::math::VEC3 &, const nw4r::math::VEC3 &, const nw4r::math::VEC3 &, const nw4r::math::VEC3 *
);
bool cM3d_CrossY_Tri(const cM3dGTri &, const nw4r::math::VEC3 *, f32 *);
bool cM3d_CrossY_Tri(const cM3dGTri &, const nw4r::math::VEC3 *, f32);
bool cM3d_CrossY_Tri(const cM3dGTri &, const nw4r::math::VEC3 *, const cM3d_Range *, f32 *);
bool cM3d_CrossZ_Tri(const cM3dGTri &, const nw4r::math::VEC3 *, f32);
bool cM3d_CrossZ_Tri(const cM3dGTri &, const nw4r::math::VEC3 *);
bool cM3d_CrossZ_Tri(const cM3dGTri &, const nw4r::math::VEC3 *, f32 *);
bool cM3d_Cross_LinTri(const cM3dGLin &, const cM3dGTri &, nw4r::math::VEC3 *, bool, bool);
bool cM3d_Cross_LinTri(
    const cM3dGLin &, const nw4r::math::VEC3 &, const nw4r::math::VEC3 &, const nw4r::math::VEC3 &, const cM3dGPla *,
    nw4r::math::VEC3 *, bool, bool
);
bool cM3d_Cross_LinTri_Easy(const cM3dGTri *, const nw4r::math::VEC3 *);
bool cM3d_Cross_SphPnt(const cM3dGSph &, const nw4r::math::VEC3 *);
bool cM3d_Cross_LinSph(const cM3dGLin &, const cM3dGSph &, nw4r::math::VEC3 *);
bool cM3d_Cross_LinSph_CrossPos(const cM3dGSph &, const cM3dGLin &, nw4r::math::VEC3 *, nw4r::math::VEC3 *);
bool cM3d_Cross_CylSph(const cM3dGCyl &, const cM3dGSph &, f32 *);
bool cM3d_Cross_CylSph(const cM3dGCyl &, const cM3dGSph &, nw4r::math::VEC3 *, f32 *);
bool cM3d_Cross_SphSph(const cM3dGSph &, const cM3dGSph &, f32 *);
bool cM3d_Cross_SphSph(const cM3dGSph &, const cM3dGSph &, f32 *, f32 *);
bool cM3d_Cross_SphSph(const cM3dGSph &, const cM3dGSph &, nw4r::math::VEC3 *);
void cM3d_CalcSphVsTriCrossPoint(const cM3dGSph &, const cM3dGTri &, nw4r::math::VEC3 *);
bool cM3d_Cross_SphTri(const cM3dGSph &, const cM3dGTri &, nw4r::math::VEC3 *, f32 *, nw4r::math::VEC3 *);
bool cM3d_Cross_CylCyl(const cM3dGCyl &, const cM3dGCyl &, f32 *);
bool cM3d_Cross_CylCyl(const cM3dGCyl &, const cM3dGCyl &, nw4r::math::VEC3 *);
bool cM3d_Cross_CylTri(const cM3dGCyl &, const cM3dGTri &, nw4r::math::VEC3 *);
int cM3d_Cross_CylLin(const cM3dGCyl &, const cM3dGLin &, nw4r::math::VEC3 *, nw4r::math::VEC3 *);
int cM3d_Cross_CylPntPnt(
    const cM3dGCyl &, const nw4r::math::VEC3 &, const nw4r::math::VEC3 &, nw4r::math::VEC3 *, nw4r::math::VEC3 *
);
bool cM3d_Cross_CylPnt(const cM3dGCyl &, const nw4r::math::VEC3 &);
bool cM3d_Cross_CpsCps(const cM3dGCps &, const cM3dGCps &, nw4r::math::VEC3 *, f32 *, f32 *);
bool cM3d_Cross_CpsCyl(const cM3dGCps &, const cM3dGCyl &, nw4r::math::VEC3 *);
bool cM3d_Cross_CpsSph_CrossPos(const cM3dGCps &, const cM3dGSph &, const nw4r::math::VEC3 &, nw4r::math::VEC3 *);
bool cM3d_Cross_CpsSph(const cM3dGCps &, const cM3dGSph &, nw4r::math::VEC3 *);
bool cM3d_Cross_TriTri(const cM3dGTri &, const cM3dGTri &, nw4r::math::VEC3 *);
bool cM3d_Cross_CpsTri(const cM3dGCps &, const cM3dGTri &, nw4r::math::VEC3 *);
void cM3d_CalcVecZAngle(const nw4r::math::VEC3 &, mAng3_c *);
void cM3d_PlaneCrossLineProcWork(f32, f32, f32, f32, f32, f32, f32, f32 *, f32 *);
int cM3d_2PlaneCrossLine(const cM3dGPla &, const cM3dGPla &, cM3dGLin *);
bool cM3d_3PlaneCrossPos(const cM3dGPla &, const cM3dGPla &, const cM3dGPla &, nw4r::math::VEC3 *);
f32 cM3d_lineVsPosSuisenCross(const cM3dGLin &, const nw4r::math::VEC3 &, nw4r::math::VEC3 *);
f32 cM3d_lineVsPosSuisenCross(
    const nw4r::math::VEC3 &, const nw4r::math::VEC3 &, const nw4r::math::VEC3 &, nw4r::math::VEC3 *
);

bool cM3d_Cross_UnkTri(const cM3dGUnk &, cM3dGTri &, nw4r::math::VEC3 *);
bool cM3d_Cross_CylUnk(const cM3dGCyl &, cM3dGUnk &, nw4r::math::VEC3 *);
bool cM3d_Cross_CylUnk(const cM3dGCyl &, cM3dGUnk &, f32 *);
bool cM3d_Cross_CpsUnk(const cM3dGCps &, cM3dGUnk &, nw4r::math::VEC3 *);
bool cM3d_Cross_UnkSph(const cM3dGUnk &, cM3dGSph &, nw4r::math::VEC3 *);
bool cM3d_Cross_UnkSph(const cM3dGUnk &, cM3dGSph &, f32 *);

bool cM3d_Normalize(nw4r::math::VEC3 *);
void cM3d_Normalize_Ex(nw4r::math::VEC3 *);

inline bool cM3d_IsZero(f32 f) {
    return fabsf(f) < G_CM3D_F_ABS_MIN;
}
// this is the inline cM3d_IsZero but inverted. Sometimes this will get a match where the regular cM3d_IsZero inline
// won't
inline bool cM3d_IsZero_inverted(f32 param_0) {
    return !(fabsf(param_0) < G_CM3D_F_ABS_MIN);
}

inline void
cM3d_InDivPos1(const nw4r::math::VEC3 *pVecA, const nw4r::math::VEC3 *pVecB, f32 pF, nw4r::math::VEC3 *pOut) {
    nw4r::math::VEC3 tmp2;
    VEC3Scale(&tmp2, pVecB, pF);
    VEC3Add(pOut, &tmp2, pVecA);
}
inline void
cM3d_InDivPos2(const nw4r::math::VEC3 *pVecA, const nw4r::math::VEC3 *pVecB, f32 pF, nw4r::math::VEC3 *pOut) {
    nw4r::math::VEC3 tmp;
    VEC3Sub(&tmp, pVecB, pVecA);
    cM3d_InDivPos1(pVecA, &tmp, pF, pOut);
}

#endif
