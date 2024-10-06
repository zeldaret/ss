#ifndef C_BG_S_POLY_INFO_H
#define C_BG_S_POLY_INFO_H

#include "common.h"
#include "d/col/c/c_bg_w.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "m/m_vec.h"

class cBgS_PolyInfo {
private:
    /* 0x00 */ int mPolyIndex;
    /* 0x04 */ int mBgIndex;
    /* 0x08 */ void *unk_0x04; // Collision Pointer?
    /* 0x0C */ int mActorPId;  // Matched the Unique Actor Id
    /* 0x10 */ cM3dGPla mPla;
    /* 0x20 */ mVec3_c mField_0x20;

public:
    cBgS_PolyInfo() {
        ClearPi();
    }
    bool ChkSetInfo() const;
    void SetPolyInfo(const cBgS_PolyInfo &);
    void SetActorInfo(int, void *, s32);
    bool ChkSafe(const void *, s32) const;
    void SetPolyIndex(int);
    bool ChkBgIndex() const;
    mAng GetAngle(mAng) const;

    void ClearPi() {
        mPolyIndex = 0xFFFF;
        mBgIndex = BG_ID_MAX;
        unk_0x04 = nullptr;
        mActorPId = 0;
    }

    u32 GetPolyIndex() const {
        return mPolyIndex;
    }
    u32 GetBgIndex() const {
        return mBgIndex;
    }
};

#endif
