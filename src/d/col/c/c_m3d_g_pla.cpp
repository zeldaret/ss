
#include "d/col/c/c_m3d_g_pla.h"

#include "d/col/c/c_m3d.h"

bool cM3dGPla::CrossInfLin(const mVec3_c &start, const mVec3_c &end, mVec3_c &out) const {
    f32 tmp1 = getPlaneFunc(start);
    f32 tmp2 = getPlaneFunc(end);

    if (cM3d_IsZero(tmp1 - tmp2)) {
        out = end;
        return false;
    }

    f32 pF = tmp1 / (tmp1 - tmp2);
    cM3d_InDivPos2(&start, &end, pF, &out);
    return true;
}

bool cM3dGPla::getCrossYLessD(const mVec3_c &point, f32 *out) const {
    if (cM3d_IsZero(mNormal.y)) {
        return false;
    } else {
        *out = (-mNormal.x * point.x - mNormal.z * point.z) / mNormal.y;
        return true;
    }
}

mAng cM3dGPla::GetAngle(mAng ang) const {
    // reg swap
    mAng tmp1 = GetXZAngle();
    f32 cos = nw4r::math::CosIdx(tmp1 - ang);
    f32 sq = EGG::Math<f32>::sqrt(mNormal.x * mNormal.x + mNormal.z * mNormal.z);

    return cM::atan2s(sq * cos, mNormal.y);
}

mAng cM3dGPla::GetNegativeAngle(mAng ang) const {
    return -GetAngle(ang);
}
