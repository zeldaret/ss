#ifndef D_BG_S_ROOF_CHK_H
#define D_BG_S_ROOF_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "m/m_vec.h"

class dBgS_RoofChk : public cBgS_PolyInfo, public cBgS_Chk, public dBgS_Chk {
public:
    dBgS_RoofChk();
    void Init();
    void SetPos(mVec3_c const &);

    virtual ~dBgS_RoofChk();

    void SetNowY(f32 y) {
        mNowY = y;
    }
    void i_SetPos(mVec3_c const &pos) {
        m_pos = pos;
    }
    f32 GetNowY() {
        return mNowY;
    }
    mVec3_c *GetPosP() {
        return &m_pos;
    }

private:
    mVec3_c m_pos;
    int field_0x48;
    f32 mNowY;
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
