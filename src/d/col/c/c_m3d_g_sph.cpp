#include "d/col/c/c_m3d_g_sph.h"

#include "m/m_vec.h"

cM3dGSph::cM3dGSph() {
    mRatio = 1.0f;
}

void cM3dGSph::SetC(const mVec3_c *c) {
    mCenter = *c;
}

void cM3dGSph::Set(const mVec3_c *c, f32 r) {
    SetC(c);
    SetR(r);
}

void cM3dGSph::SetR(f32 r) {
    if (r < 0.0f) {
        r = 0.0f;
    }
    mRadius = r;
}

void cM3dGSph::SetC(f32 x, f32 y, f32 z) {
    SetC(&mVec3_c(x, y, z));
}

bool cM3dGSph::fn_80338750(cM3dGUnk *pUnk, mVec3_c *pOut) {
    return fn_80337690(pUnk, this, pOut);
}

bool cM3dGSph::fn_80338760(cM3dGUnk *pUnk, f32 *pOut) {
    return fn_80337780(pUnk, this, pOut);
}

f32 cM3dGSph::GetYDist(f32 r) const {
    f32 b = mRadius * mRadius - (r - mCenter.y) * (r - mCenter.y);
    if (b < 0.0f) {
        return 0.0f;
    }
    return nw4r::math::FSqrt(b);
}

bool cM3dGSph::Cross(const cM3dGTri *pTri, f32 *p2, mVec3_c *p3) {
    cM3d_Cross_SphTri(this, pTri, nullptr, p2, p3);
}

void cM3dGSph::Clamp(const mVec3_c &in, mVec3_c &out) const {
    f32 dist = EGG::Math<f32>::sqrt(VEC3DistSq(mCenter, in));

    if (dist < mRadius) {
        out = in;
    } else {
        f32 a = mRadius / dist;
        out = (in - mCenter) * a + mCenter;
    }
}

void cM3dGSph::SetRatio(f32 ratio) {
    mRatio = ratio;
}
