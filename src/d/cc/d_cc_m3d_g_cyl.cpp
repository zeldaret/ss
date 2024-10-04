#include <d/cc/d_cc_m3d_g_cyl.h>
#include <m/m_vec.h>


void cM3dGCyl::setC(const mVec3_c &c, f32 radius, f32 height) {
    setC(c);
    setR(radius);
    setH(height);
}

void cM3dGCyl::setC(const mVec3_c &c) {
    mCenter = c;
}

void cM3dGCyl::setH(f32 h) {
    if (h < 0.0f) {
        h = 0.0f;
    }
    mHeight = h;
}

void cM3dGCyl::setR(f32 r) {
    if (r < 0.0f) {
        r = 0.0f;
    }
    mRadius = r;
}
