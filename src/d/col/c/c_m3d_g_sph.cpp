#include "d/col/c/c_m3d_g_sph.h"

#include "m/m_vec.h"

cM3dGSph::cM3dGSph() {
    field_0x10 = 1.0f;
}

void cM3dGSph::setC(const mVec3_c &c) {
    mCenter = c;
}

void cM3dGSph::set(const mVec3_c &c, f32 r) {
    setC(c);
    setR(r);
}

void cM3dGSph::setR(f32 r) {
    if (r < 0.0f) {
        r = 0.0f;
    }
    mRadius = r;
}

void cM3dGSph::setC(f32 x, f32 y, f32 z) {
    setC(mVec3_c(x, y, z));
}
