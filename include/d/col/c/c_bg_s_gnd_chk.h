#ifndef C_BG_S_GND_CHK_H
#define C_BG_S_GND_CHK_H

#include "d/col/c/c_bg_s_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "m/m_vec.h"

class cBgS_GndChk : public cBgS_Chk, public cBgS_PolyInfo {
public:
    /* 0x54 */ mVec3_c mPos;
    /* 0x60 */ f32 mNowY;
    /* 0x64 */ u8 unk0x64[0x68 - 0x64];
    /* 0x68 */ mVec3_c mField_0x68;
    /* 0x74 */ u8 mField_0x74;

    cBgS_GndChk();
    virtual ~cBgS_GndChk();
    void SetPos(mVec3_c const &);
    void SetPos(mVec3_c const *);

    f32 GetNowY() const {
        return mNowY;
    }
    void SetNowY(f32 y) {
        mNowY = y;
    }
    const mVec3_c &GetPointP() {
        return mPos;
    }
    void SetField_0x68(const mVec3_c &other) {
        mField_0x68 = other;
    }
    // u32 GetWallPrecheck() const {
    //     return mWallPrecheck;
    // }
    // void OffWall() {
    //     mFlags &= ~2;
    // }
};

#endif
