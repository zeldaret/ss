#ifndef C_BG_S_LIN_CHK_H
#define C_BG_S_LIN_CHK_H

#include "d/col/c/c_bg_s_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/c/c_m3d_g_lin.h"

class cBgS_LinChk : public cBgS_Chk, public cBgS_PolyInfo {
public:
    /* 0x54 */ cM3dGLin mLin;
    /* 0x6C */ mVec3_c mField_0x6C;
    /* 0x78 */ bool mField_0x78;
    /* 0x79 */ bool mField_0x79;
    /* 0x7A */ bool mHit;
    /* 0x7B */ bool mField_0x7B;
    /* 0x7C */ bool mField_0x7C;
    /* 0x7D */ bool mField_0x7D;

    cBgS_LinChk();
    virtual ~cBgS_LinChk();
    virtual void vt_0xC(mVec3_c *);

    void Ct();
    void Set2(const mVec3_c &, const mVec3_c &, u32);

    bool ChkHit() const {
        return mHit;
    }
    void ClrHit() {
        mHit = false;
    }
    void SetHit() {
        mHit = true;
    }

    const mVec3_c &GetLinEnd() const {
        return mLin.mEnd;
    }
};

#endif
