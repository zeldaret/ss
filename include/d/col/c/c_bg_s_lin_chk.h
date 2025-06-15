#ifndef C_BG_S_LIN_CHK_H
#define C_BG_S_LIN_CHK_H

#include "d/col/c/c_bg_s_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/c/c_m3d_g_lin.h"

class cBgS_LinChk : public cBgS_Chk, public cBgS_PolyInfo {
public:
    /* 0x54 */ cM3dGLin mLin;
    /* 0x6C */ mVec3_c mField_0x6C;
    /* 0x78 */ bool mFrontFlag;
    /* 0x79 */ bool mBackFlag;
    /* 0x7A */ bool mHit;
    /* 0x7B */ bool mPreRoofChk;
    /* 0x7C */ bool mPreWallChk;
    /* 0x7D */ bool mPreGroundChk;

    cBgS_LinChk();
    virtual ~cBgS_LinChk();
    virtual void SetEnd(const mVec3_c *);

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
    bool ChkBackFlag() const {
        return mBackFlag;
    }
    void OnBackFlag() {
        mBackFlag = true;
    }
    bool ChkFrontFlag() const {
        return mFrontFlag;
    }
    void OnFrontFlag() {
        mFrontFlag = true;
    }
    bool GetPreWallChk() const {
        return mPreWallChk;
    }
    bool GetPreGroundChk() const {
        return mPreGroundChk;
    }
    void SetPreRoofChk(bool b) {
        mPreRoofChk = b;
    }
    bool GetPreRoofChk() const {
        return mPreRoofChk;
    }

    const mVec3_c &GetLinStart() const {
        return mLin.mStart;
    }
    const mVec3_c &GetLinEnd() const {
        return mLin.mEnd;
    }
};

#endif
