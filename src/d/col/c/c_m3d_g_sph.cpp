#include "d/col/c/c_m3d_g_sph.h"

#include "m/m_vec.h"

cM3dGSph::cM3dGSph() {
    mRatio = 1.0f;
}

void cM3dGSph::SetC(const mVec3_c &c) {
    mCenter = c;
}

void cM3dGSph::Set(const mVec3_c &c, f32 r) {
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
    SetC(mVec3_c(x, y, z));
}
