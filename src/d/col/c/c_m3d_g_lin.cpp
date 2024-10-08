#include "d/col/c/c_m3d_g_lin.h"

void cM3dGLin::SetStartEnd(const VEC3 &start, const VEC3 &end) {
    mStart = start;
    mEnd = end;
}

void cM3dGLin::SetStartEnd(const mVec3_c *start, const mVec3_c *end) {
    SetStartEnd(*start, *end);
}

void cM3dGLin::CalcPos(mVec3_c *out, f32 scale) const {
    mVec3_c tmp;
    PSVECSubtract(mEnd, mStart, tmp);
    PSVECScale(tmp, tmp, scale);
    PSVECAdd(tmp, mStart, *out);
}

void cM3dGLin::SetStart(const mVec3_c &start) {
    // This just feels weird
    mStart = start;
}
