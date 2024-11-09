#include "d/col/c/c_m3d_g_cyl.h"

#include "d/col/c/c_m3d.h"
#include "m/m_vec.h"

void cM3dGCyl::SetC(const mVec3_c &c, f32 radius, f32 height) {
    SetC(c);
    SetR(radius);
    SetH(height);
}

void cM3dGCyl::SetC(const mVec3_c &c) {
    mCenter = c;
}

void cM3dGCyl::SetH(f32 h) {
    if (h < 0.0f) {
        h = 0.0f;
    }
    mHeight = h;
}

void cM3dGCyl::SetR(f32 r) {
    if (r < 0.0f) {
        r = 0.0f;
    }
    mRadius = r;
}

bool cM3dGCyl::Cross(cM3dGUnk &unk, mVec3_c *vec) {
    return cM3d_Cross_CylUnk(*this, unk, vec);
}

bool cM3dGCyl::Cross(cM3dGUnk &unk, f32 *vec) {
    return cM3d_Cross_CylUnk(*this, unk, vec);
}

bool cM3dGCyl::Cross(const mVec3_c &pnt) const {
    if ((mCenter.y > pnt.y) || (mCenter.y + mHeight < pnt.y)) {
        return false;
    }
    f32 x_dist = mCenter.x - pnt.x;
    f32 z_dist = mCenter.z - pnt.z;
    return x_dist * x_dist + z_dist * z_dist > mRadius * mRadius ? false : true;
}

void cM3dGCyl::Clamp(const mVec3_c &in, mVec3_c &out) const {
    f32 floor = mCenter.y;
    mVec3_c diff = in - mCenter;
    f32 ceil = floor + mHeight;
    f32 dist_sq = diff.x * diff.x + diff.z * diff.z;
    f32 dist = EGG::Math<f32>::sqrt(dist_sq);

    if (ceil < in.y) {
        if (dist < mRadius) {
            out.x = in.x;
            out.y = ceil;
            out.z = in.z;
        } else {
            f32 scale = mRadius / dist;
            out.x = mCenter.x + scale * (in.x - mCenter.x);
            out.y = ceil;
            out.z = mCenter.z + scale * (in.z - mCenter.z);
        }
    } else if (floor > in.y) {
        if (dist < mRadius) {
            out.x = in.x;
            out.y = floor;
            out.z = in.z;
        } else {
            f32 scale = mRadius / dist;
            out.y = floor;
            out.x = mCenter.x + scale * (in.x - mCenter.x);
            out.z = mCenter.z + scale * (in.z - mCenter.z);
        }
    } else {
        if (dist < mRadius) {
            out = in;
        } else {
            f32 scale = mRadius / dist;
            out.y = in.y;
            out.x = mCenter.x + scale * (in.x - mCenter.x);
            out.z = mCenter.z + scale * (in.z - mCenter.z);
        }
    }
}
