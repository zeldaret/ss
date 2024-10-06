#ifndef C_BG_S_TIME_AREA_H
#define C_BG_S_TIME_AREA_H

/**
 * Made up for timeshift related area
 */

#include "common.h"
#include "d/col/c/c_bg_s_chk.h"
#include "d/col/c/c_m3d_g_lin.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/col/c/c_m3d_g_tri.h"
#include "m/m_vec.h"

class cBgS_TimeArea;
typedef void (*TimeDrawCallback)(cBgS_TimeArea *, const mVec3_c &, const mVec3_c &, const mVec3_c &, const cM3dGTri &);

class cBgS_TimeArea : public cBgS_Chk {
public:
    /* 0x28 */ cM3dGLin mLin;
    /* 0x40 */ cM3dGPla *mpPla;
    /* 0x44 */ TimeDrawCallback mpCallback;

    cBgS_TimeArea();
    virtual ~cBgS_TimeArea();

    void Set(const mVec3_c &, const mVec3_c &);
    void SetCallback(TimeDrawCallback);
    void SetP(cM3dGPla *);
    void Calc(cM3dGTri *, mVec3_c *, mVec3_c *, mVec3_c *);
};

#endif
