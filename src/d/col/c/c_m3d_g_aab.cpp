#include "d/col/c/c_m3d_g_aab.h"

#include "rvl/MTX.h"

void cM3dGAab::SetMin(const mVec3_c &min) {
    if (mMin.x > min.x) {
        mMin.x = min.x;
    }
    if (mMin.y > min.y) {
        mMin.y = min.y;
    }
    if (mMin.z > min.z) {
        mMin.z = min.z;
    }
}

void cM3dGAab::SetMax(const mVec3_c &max) {
    if (mMax.x < max.x) {
        mMax.x = max.x;
    }
    if (mMax.y < max.y) {
        mMax.y = max.y;
    }
    if (mMax.z < max.z) {
        mMax.z = max.z;
    }
}

void cM3dGAab::CalcCenter(mVec3_c &center) const {
    PSVECAdd(mMin, mMax, center);
    PSVECScale(center, center, 0.5f);
}

void cM3dGAab::PlusR(f32 r) {
    mMin -= r;
    mMax += r;
}

void cM3dGAab::Clamp(const mVec3_c &in, mVec3_c &out) const {
    f32 x = in.x;
    if (x < mMin.x) {
        x = mMin.x;
    }
    if (x > mMax.x) {
        x = mMax.x;
    }
    out.x = x;

    f32 y = in.y;
    if (y < mMin.y) {
        y = mMin.y;
    }
    if (y > mMax.y) {
        y = mMax.y;
    }
    out.y = y;

    f32 z = in.z;
    if (z < mMin.z) {
        z = mMin.z;
    }
    if (z > mMax.z) {
        z = mMax.z;
    }
    out.z = z;
}

void cM3dGAab::Set(const cM3dGSph &sph) {
    mVec3_c min, max;
    min = max = sph.GetC();

    f32 r = sph.GetR();

    f32 ratio = sph.mRatio;
    if (ratio < 1.0f) {
        ratio = 1.0f;
    }
    f32 h = r * ratio;

    min.x -= r;
    min.y -= h;
    min.z -= r;

    max.x += r;
    max.y += h;
    max.z += r;

    Set(min, max);
}

bool cM3dGAab::Cross(const mVec3_c &pos) const {
    if ((mMin.x < pos.x && pos.x < mMax.x) && (mMin.y < pos.y && pos.y < mMax.y) &&
        (mMin.z < pos.z && pos.z < mMax.z)) {
        return true;
    }
    return false;
}

void cM3dGAab::SetBounds(const mVec3_c &pntA, const mVec3_c &pntB) {
    if (pntA.x < pntB.x) {
        mMin.x = pntA.x;
        mMax.x = pntB.x;
    } else {
        mMin.x = pntB.x;
        mMax.x = pntA.x;
    }
    if (pntA.y < pntB.y) {
        mMin.y = pntA.y;
        mMax.y = pntB.y;
    } else {
        mMin.y = pntB.y;
        mMax.y = pntA.y;
    }
    if (pntA.z < pntB.z) {
        mMin.z = pntA.z;
        mMax.z = pntB.z;
    } else {
        mMin.z = pntB.z;
        mMax.z = pntA.z;
    }
    PlusR(1.0f);
}
