#include "egg/gfx/eggFrustum.h"

#include "egg/gfx/eggGXUtility.h"
#include "egg/gfx/eggStateGX.h"
#include "nw4r/math/math_arithmetic.h"

void eggFrustum_float_ordering() {
    1.0f;
}

namespace EGG {
using namespace nw4r;

Frustum::Frustum(ProjectionType proj, const math::VEC2 &size, f32 nearZ, f32 farZ, CanvasMode canvas)
    : mProjType(proj),
      mCanvasMode(canvas),
      mSize(size),
      mFovY(45.0f),
      mTanFovY(2.4142134f),
      mNearZ(nearZ),
      mFarZ(farZ),
      mOffset(0.0f, 0.0f),
      mScale(1.0f, 1.0f, 1.0f),
      mFlags(FLAG_DIRTY) {}

Frustum::Frustum(const Frustum &other) {
    CopyFromAnother(other);
}

void Frustum::ResetOrthographic(f32 t, f32 b, f32 l, f32 r, f32 near, f32 far) {
    0.5f;
    2.0f;

    mProjType = PROJ_ORTHO;
    mScale.z = 1.0f;
    mScale.y = 1.0f;
    mScale.x = 1.0f;
    mSize.x = r - l;
    if (mSize.x < 0.0f) {
        mScale.x = -1.0f;
        mSize.x *= -1.0f;
    }

    mSize.y = t - b;
    if (mSize.y >= 0.0f) {
        SetCanvasMode(CANVASMODE_0);
        mOffset = math::VEC2(0.5f * (r + l), 0.5f * (t + b));
    } else {
        SetCanvasMode(CANVASMODE_1);
        mOffset = math::VEC2(0.0f, t);
        mSize.y *= -1.0f;
    }
    mNearZ = near;
    mFarZ = far;
    SetFlag(1);
}

void Frustum::CopyFromAnother(const Frustum &other) {
    mProjType = other.mProjType;
    mCanvasMode = other.mCanvasMode;
    mSize = other.mSize;
    mFovY = other.mFovY;
    mTanFovY = other.mTanFovY;
    mNearZ = other.mNearZ;
    mFarZ = other.mFarZ;
    mOffset = other.mOffset;
    mScale = other.mScale;
    mFlags = other.mFlags;
}

void Frustum::SetFovy(f32 fov) {
    if (mFovY != fov) {
        mFovY = fov;
        f32 sin, cos;

        math::SinCosDeg(&sin, &cos, (f32)(f64)fov / 2.0f);
        mTanFovY = cos / sin;
    }
}

void Frustum::ConvertToCanvasLU(f32 ix, f32 iy, f32 *ox, f32 *oy) const {
    if (mCanvasMode == CANVASMODE_1) {
        *ox = ix;
        *oy = iy;
    } else if (mCanvasMode == CANVASMODE_0) {
        *ox = ix + (mSize.x * 0.5f);
        *oy = -(iy - (mSize.y * 0.5f));
    }
}

f32 Frustum::GetScreenSizeToViewSize(f32 p1, f32 p2) const {
    if (mProjType != PROJ_PERSP) {
        return p1;
    }
    return p1 * math::FAbs(p2) / (mSize.y * 0.5f * mTanFovY);
}

void Frustum::SetProjectionGX() const {
    if (mProjType == PROJ_ORTHO) {
        SetProjectionOrthographicGX_();
    } else {
        SetProjectionPerspectiveGX_();
    }
}

void Frustum::CopyToG3D(g3d::Camera cam) const {
    if (mProjType == PROJ_ORTHO) {
        CopyToG3D_Orthographic_(cam);
    } else {
        CopyToG3D_Perspective_(cam);
    }
}

void Frustum::SetGlobalScaleOffset(f32 sx, f32 sy, f32 ox, f32 oy) {
    sGlobalScale.x = sx;
    sGlobalScale.y = sy;
    sGlobalOffset.x = ox;
    sGlobalOffset.y = oy;
}

void Frustum::GetGlobalScaleOffset(f32 *sx, f32 *sy, f32 *ox, f32 *oy) {
    *sx = sGlobalScale.x;
    *sy = sGlobalScale.y;
    *ox = sGlobalOffset.x;
    *oy = sGlobalOffset.y;
}

void Frustum::SetProjectionPerspectiveGX_() const {
    f32 params[7];
    GetPerspectiveParam_(params);
    StateGX::GXSetProjectionv_(params);
}

void Frustum::SetProjectionOrthographicGX_() const {
    f32 v[8];
    GetOrthographicParam_(v);
    StateGX::GXSetProjectionv_(v);
}

void Frustum::CopyToG3D_Perspective_(g3d::Camera cam) const {
    math::MTX44 mtx;
    f32 ratio = mSize.x / mSize.y;
    cam.SetPerspective(mFovY, ratio, mNearZ, mFarZ);
    CalcMtxPerspective_(&mtx);
    cam.SetProjectionMtxDirectly(&mtx);
}

void Frustum::CopyToG3D_Orthographic_(g3d::Camera cam) const {
    f32 t, b, l, r;
    GetOrthographicParam_(&t, &b, &l, &r);
    cam.SetOrtho(t, b, l, r, mNearZ, mFarZ);
}

void Frustum::CalcMtxPerspective_(math::MTX44 *out) const {
    f32 params[7];
    GetPerspectiveParam_(params);

    out->m[0][3] = 0.0f;
    out->m[0][1] = 0.0f;
    out->m[0][0] = params[1];
    out->m[0][2] = params[2];

    out->m[1][3] = 0.0f;
    out->m[1][0] = 0.0f;
    out->m[1][1] = params[3];
    out->m[1][2] = params[4];

    out->m[2][1] = 0.0f;
    out->m[2][0] = 0.0f;
    out->m[2][2] = params[5];
    out->m[2][3] = params[6];

    out->m[3][3] = 0.0f;
    out->m[3][1] = 0.0f;
    out->m[3][0] = 0.0f;
    out->m[3][2] = -1.0f;
}

void Frustum::GetPerspectiveParam_(f32 *p) const {
    // NONMATCHING - FPR regswaps
    f32 f1 = 1.0f / (mSize.x * 0.5f);
    f32 f2 = 1.0f / (mSize.y * 0.5f);
    p[0] = 0.0f;
    p[1] = (mTanFovY / (mScale.x * (mSize.x / mSize.y)));
    p[2] = mOffset.x * f1;
    p[3] = mTanFovY / mScale.y;
    p[4] = mOffset.y * f2;

    f32 z = -mNearZ / (mFarZ - mNearZ);
    p[5] = z;
    p[6] = mFarZ * z;

    GXUtility::setScaleOffsetPerspective(p, sGlobalScale.x, sGlobalScale.y, sGlobalOffset.x * f1, sGlobalOffset.y * f2);
}

void Frustum::GetOrthographicParam_(f32 *p) const {
    f32 t, b, l, r;
    GetOrthographicParam_(&t, &b, &l, &r);

    f32 invHor = 1.0f / (r - l);
    f32 invVert = 1.0f / (t - b);
    f32 invZ = 1.0f / (mFarZ - mNearZ);

    p[0] = 1.0f;
    p[1] = invHor * 2.0f;
    p[2] = invHor * -(r + l);
    p[3] = invVert * 2.0f;

    p[4] = invVert * -(t + b);
    p[5] = -1.0f * invZ;
    p[6] = -mFarZ * invZ;
}

void Frustum::GetOrthographicParam_(f32 *pT, f32 *pB, f32 *pL, f32 *pR) const {
    f32 adjOffsetX = mOffset.x + sGlobalOffset.x;
    f32 adjOffsetY = mOffset.y + sGlobalOffset.y;

    if (mCanvasMode == CANVASMODE_0) {
        const math::VEC2 adjScale(mScale.x * sGlobalScale.x, mScale.y * sGlobalScale.y);

        *pT = adjScale.y * ((0.5f * mSize.y) + adjOffsetY);
        *pB = adjScale.y * ((-0.5f * mSize.y) + adjOffsetY);
        *pL = adjScale.x * ((-0.5f * mSize.x) + adjOffsetX);
        *pR = adjScale.x * ((0.5f * mSize.x) + adjOffsetX);
    } else if (mCanvasMode == CANVASMODE_1) {
        f32 adjScaleX = -(sGlobalScale.x - 1.0f) * mSize.x / 2.0f;
        f32 adjScaleY = -(sGlobalScale.y - 1.0f) * mSize.y / 2.0f;

        *pT = adjScaleY + -sGlobalScale.y * adjOffsetY;
        *pB = adjScaleY + -sGlobalScale.y * (adjOffsetY + mSize.y * -1.0f * mScale.y);
        *pL = adjScaleX + sGlobalScale.x * adjOffsetX;
        *pR = adjScaleX + sGlobalScale.x * (adjOffsetX + mSize.x * mScale.x);
    }
}

math::VEC2 Frustum::sGlobalScale(1.0f, 1.0f);
math::VEC2 Frustum::sGlobalOffset(0.0f, 0.0f);

} // namespace EGG
