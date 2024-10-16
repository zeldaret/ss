
#include "d/col/c/c_bg_s_poly_info.h"

#include "d/col/bg/d_bg_s.h"

bool cBgS_PolyInfo::ChkSetInfo() const {
    if (mPolyIndex == 0xFFFF || mBgIndex == BG_ID_MAX) {
        return false;
    }
    return true;
}

void cBgS_PolyInfo::SetPolyInfo(const cBgS_PolyInfo &other) {
    *this = other;
}

bool cBgS_PolyInfo::ChkSafe(const void *pBg, s32 id) const {
    if (unk_0x04 == pBg && mActorPId == id) {
        return true;
    }
    return false;
}

bool cBgS_PolyInfo::ChkBgIndex() const {
    return mBgIndex != BG_ID_MAX;
}

mAng cBgS_PolyInfo::GetAngle(mAng angle) const {
    cM3dGPla pla;
    if (!dBgS::GetInstance()->GetTriPla(*this, &pla)) {
        return mAng(0);
    } else {
        return pla.GetAngle(angle);
    }
}
