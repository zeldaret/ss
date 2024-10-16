#ifndef C_BG_S_SHDW_DRAW_H
#define C_BG_S_SHDW_DRAW_H

#include "common.h"
#include "d/col/c/c_bg_s_chk.h"
#include "d/col/c/c_m3d_g_lin.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/col/c/c_m3d_g_tri.h"
#include "m/m_vec.h"

class cBgS_ShdwDraw;
typedef void (*cBgS_ShdwDraw_Callback)(cBgS_ShdwDraw *, const mVec3_c &, const mVec3_c &, const mVec3_c &, const cM3dGTri &);

class cBgS_ShdwDraw : public cBgS_Chk {
public:
    /* 0x28 */ cM3dGAab mBnd;
    /* 0x40 */ cM3dGPla *mpPla;
    /* 0x44 */ cBgS_ShdwDraw_Callback mpCallback;

    cBgS_ShdwDraw();
    virtual ~cBgS_ShdwDraw();

    void Set(const mVec3_c &, const mVec3_c &);
    void SetCallback(cBgS_ShdwDraw_Callback);
    void SetP(cM3dGPla *);
    void Calc(cM3dGTri *, mVec3_c *, mVec3_c *, mVec3_c *);

    cM3dGAab *GetBndP() {
        return &mBnd;
    }
};

#endif
