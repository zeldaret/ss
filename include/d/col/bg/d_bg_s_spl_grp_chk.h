#ifndef D_BG_S_SPL_GRP_CHK_H
#define D_BG_S_SPL_GRP_CHK_H

#include "d/col/bg/d_bg_s_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "m/m_vec.h"

class dBgS_SplGrpChk : public cBgS_PolyInfo, public cBgS_Chk, public dBgS_Chk {
public:
    /* 0x6C */ mVec3_c mGnd;
    /* 0x78 */ f32 mRoof;
    /* 0x7C */ f32 mHeight;
    /* 0x80 */ f32 mGroundH;
    /* 0x84 */ u32 mFlags;
    /* 0x88 */ mVec3_c mGroundCopy;

    enum {
        /* 0x1 */ FIND = 1,
        /* 0x2 */ IN = 2,
        /* 0x4 */ MOVE_BG_ONLY = 4
    };

    void Set(mVec3_c &, f32);
    dBgS_SplGrpChk();

    void Init() {
        OffIn();
        OffFind();

        mGroundH = -1000000000.0f;
        SetHeight(mGnd.y);
        ClearPi();
    }

    virtual ~dBgS_SplGrpChk();

    void OffFind() {
        mFlags &= ~FIND;
    }
    void OffIn() {
        mFlags &= ~IN;
    }
    void OffMoveBGOnly() {
        mFlags &= ~MOVE_BG_ONLY;
    }
    void OnMoveBGOnly() {
        mFlags |= MOVE_BG_ONLY;
    }
    void OnFind() {
        mFlags |= FIND;
    }
    bool ChkMoveBGOnly() {
        return mFlags & MOVE_BG_ONLY;
    }
    void SetHeight(f32 height) {
        mHeight = height;
    }
    f32 GetHeight() {
        return mHeight;
    }
    mVec3_c &GetPosP() {
        return mGnd;
    }
    f32 GetRoof() const {
        return mRoof;
    }
    void CopyGnd() {
        mGroundCopy = mGnd;
    }

    f32 GetGroundH() const {
        return mGroundH;
    }
};

#endif
