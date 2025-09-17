#ifndef D_BG_S_CAPT_POLY_H
#define D_BG_S_CAPT_POLY_H

#include "d/col/bg/d_bg_s_chk.h"

struct dBgS_CaptPoly;

typedef void (*CaptPolyCallback)(dBgS_CaptPoly &, cBgD_Vtx_t *, u16, u16, u16, cM3dGPla *);

struct dBgS_CaptPoly : public cBgS_Chk, public dBgS_Chk {
public:
    cM3dGAab *GetBndP() {
        return &mBnd;
    }

    /* 0x40 */ cM3dGAab mBnd;
    /* 0x58 */ UNKWORD field_0x58;
    /* 0x5C */ CaptPolyCallback mCallback;
};
#endif
