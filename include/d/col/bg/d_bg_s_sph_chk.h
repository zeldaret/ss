#ifndef D_BG_S_SPH_CHK_H
#define D_BG_S_SPH_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/c/c_m3d_g_sph.h"

struct cBgD_Vtx_t;

class dBgS_SphChk;
typedef void *(*SphChk_Callback)(dBgS_SphChk *, cBgD_Vtx_t *, int, int, int, cM3dGPla *, void *);

class dBgS_SphChk : public cM3dGSph, public cBgS_PolyInfo, public cBgS_Chk, public dBgS_Chk {
public:
    /* 0x80 */ f32 mField_0x80;
    /* 0x84 */ f32 mField_0x84;
    /* 0x88 */ f32 mField_0x88;
    /* 0x8C */ f32 mField_0x8C;
    /* 0x90 */ cM3dGAab mAabb;
    /* 0xA8 */ u32 mField_0xA8;
    /* 0xAC */ void *mField_0xAC;
    /* 0xB0 */ u32 mField_0xB0;
    /* 0xB4 */ SphChk_Callback mCallback;

    void SetInfo(int i, void *pV, int a) {
        mField_0xA8 = i;
        mField_0xAC = pV;
        mField_0xB0 = a;
    }

    dBgS_SphChk();
    virtual ~dBgS_SphChk();
};

class dBgS_CamSphChk : public dBgS_SphChk {
public:
    dBgS_CamSphChk();
    virtual ~dBgS_CamSphChk();
};

#endif
