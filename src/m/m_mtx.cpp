#include "m/m_mtx.h"

#include "c/c_math.h"
#include "m/m_math.h"
#include "m/m_vec.h"

void mMtx_c::toRot(mAng3_c &out) const {
    f32 t1 = 0.0f;
    f32 t0 = m[0][2] + m[2][2];
    if (t0 > 0.0f) {
        t1 = t0 * nw4r::math::FrSqrt(t0);
    }
    s16 anglex = cM::atan2s(-m[1][2], t1);
}

void mMtx_c::fn_802F1C40(s32 i, s32 j) {
    f32 *pJ = &arr[j];
    f32 *pI = &arr[i];

    mVec3_c v0(pI[0], pI[4], pI[8]);
    mVec3_c v1(pJ[0], pJ[4], pJ[8]);

    mVec3_c v2;
    v0.normalize();

    f32 z, y, x;
    if (j - i == 1 || j - i == -2) {
        z = (v0.x * v1.y) - (v0.y * v1.x);
        y = (v0.z * v1.x) - (v0.x * v1.z);
        x = (v0.y * v1.z) - (v0.z * v1.y);
        v2 = mVec3_c(x, y, z);

        z = (v2.x * v0.y) - (v2.y * v0.x);
        y = (v2.z * v0.x) - (v2.x * v0.z);
        x = (v2.y * v0.z) - (v2.z * v0.y);
        v1 = mVec3_c(x, y, z);
    } else {
        z = (v1.x * v0.y) - (v1.y * v0.x);
        y = (v1.z * v0.x) - (v1.x * v0.z);
        x = (v1.y * v0.z) - (v1.z * v0.y);
        v2 = mVec3_c(x, y, z);

        z = (v0.x * v2.y) - (v0.y * v2.x);
        y = (v0.z * v2.x) - (v0.x * v2.z);
        x = (v0.y * v2.z) - (v0.z * v2.y);
        v1 = mVec3_c(x, y, z);
    }
    v2.normalize();
    v1.normalize();

    f32 *pK = &arr[((3 - i) - j)];

    // clang-format off
    pI[0] = v0.x; pI[4] = v0.y; pI[8] = v0.z;
    pJ[0] = v1.x; pJ[4] = v1.y; pJ[8] = v1.z;
    pK[0] = v2.x; pK[4] = v2.y; pK[8] = v2.z;
    // clang-format on
}
