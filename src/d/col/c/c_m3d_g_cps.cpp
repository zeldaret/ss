#include "d/col/c/c_m3d_g_cps.h"

#include "d/col/c/c_m3d.h"
#include "nw4r/math.h" // IWYU pragma: export

cM3dGCps::cM3dGCps() {
    mField_0x1C = 0.0f;
    mField_0x20 = 0;
    mFlags = 0;
}

void cM3dGCps::SetR(f32 r) {
    if (r < 0.0f) {
        r = 0.0f;
    }
    mRadius = r;
    mField_0x1C = (double)r; // huh
}

void cM3dGCps::Set_0x1C(f32 f) {
    mField_0x1C = f;
}

void cM3dGCps::Set(const mVec3_c &start, const mVec3_c &end, f32 r) {
    cM3dGLin::Set(start, end);
    SetR(r);
}

bool cM3dGCps::Cross(cM3dGUnk &unk, mVec3_c *vec) {
    return cM3d_Cross_CpsUnk(*this, unk, vec);
}

bool cM3dGCps::fn_80337f40(const mVec3_c &v) const {
    f32 r2 = mRadius * mRadius;
    if (!(mFlags & 1) && PSVECSquareDistance(mStart, v) <= r2) {
        return true;
    }
    if (!(mFlags & 2) && PSVECSquareDistance(mEnd, v) <= r2) {
        return true;
    }
    f32 tmp;
    mVec3_c tmpv;
    if (cM3d_Len3dSqPntAndSegLine(*this, v, tmpv, &tmp, nullptr) && tmp <= r2) {
        return true;
    }
    return false;
}
