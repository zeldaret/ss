#ifndef D_BG_S_ROOF_CHK_H
#define D_BG_S_ROOF_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "m/m_vec.h"

class dBgS_RoofChk : public cBgS_PolyInfo, public cBgS_Chk, public dBgS_Chk {
public:
    /* 0x6C */ mVec3_c mPos;
    /* 0x78 */ f32 mNowY;
    /* 0x7C */ int field_0x48;
    /* 0x80 */ mVec3_c mPosCopy;

    dBgS_RoofChk();

    void Init() {
        SetNowY(1e9f);
        ClearPi();
    }

    void SetPos(mVec3_c const &);

    virtual ~dBgS_RoofChk();

    void SetNowY(f32 y) {
        mNowY = y;
    }
    void i_SetPos(mVec3_c const &pos) {
        mPos = pos;
    }
    f32 GetNowY() {
        return mNowY;
    }
    mVec3_c *GetPosP() {
        return &mPos;
    }

    void CopyPos() {
        mPosCopy = mPos;
    }
};

class dBgS_LinkRoofChk : public dBgS_RoofChk {
public:
    dBgS_LinkRoofChk() {
        SetLink();
    }
    virtual ~dBgS_LinkRoofChk() {}
};

class dBgS_ObjRoofChk : public dBgS_RoofChk {
public:
    dBgS_ObjRoofChk() {
        SetObj();
    }

    virtual ~dBgS_ObjRoofChk() {}

    dBgS_ObjRoofChk &GetInstance();
    static void CtInstance();

    static dBgS_ObjRoofChk sInstance;
};

#endif
