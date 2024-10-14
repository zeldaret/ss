#ifndef D_BG_S_ROOF_CHK_H
#define D_BG_S_ROOF_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "m/m_vec.h"

class dBgS_RoofChk : public cBgS_PolyInfo, public cBgS_Chk, public dBgS_Chk {
public:
    /* 0x6C */ mVec3_c mPos;
    /* 0x78 */ f32 mNowY;
    /* 0x7C */ u8 mField_0x7C;
    /* 0x80 */ mVec3_c mPosCopy;

    dBgS_RoofChk();

    virtual ~dBgS_RoofChk();

    void Init() {
        SetNowY(1000000000.0f);
        ClearPi();
    }

    void SetPos(mVec3_c const *);

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

class dBgS_ObjRoofChk : public dBgS_RoofChk {
public:
    dBgS_ObjRoofChk();

    virtual ~dBgS_ObjRoofChk();

    static dBgS_ObjRoofChk &GetInstance() {
        return sInstance;
    }
    static f32 GetRoofHeight() {
        return sRoofHeight;
    }
    static void ClearInstance();
    static bool CheckPos(const mVec3_c *);

private:
    static dBgS_ObjRoofChk sInstance;
    static f32 sRoofHeight;
};

class dBgS_LinkRoofChk : public dBgS_RoofChk {
public:
    dBgS_LinkRoofChk();

    virtual ~dBgS_LinkRoofChk();
};
#endif
