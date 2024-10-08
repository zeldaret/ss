#include "d/col/c/c_m3d_g_unk.h"

#include "d/col/c/c_m3d.h"
#include "d/col/c/c_m3d_g_aab.h"

cM3dGUnk::cM3dGUnk() {
    mMin.set(0.0f, 0.0f, 0.0f);
    mMax.set(0.0f, 0.0f, 0.0f);
    mField_0x78.set(0.0f, 0.0f, 0.0f);
    mField_0x84.set(0.0f, 0.0f, 0.0f);
    mField_0x90.set(0.0f, 0.0f, 0.0f);
    mField_0x9C.set(0.0f, 0.0f, 0.0f);
    mField_0xA8.set(0.0f, 0.0f, 0.0f);
    mField_0xB4 = 0;
    mVirtCenter.set(0.0f, 0.0f, 0.0f);
    mField_0xC4 = 0.0f;
    PSMTXIdentity(mMtx);
    PSMTXIdentity(mInvMtx);
}

void cM3dGUnk::Set(const mVec3_c &vA, const mVec3_c &vB) {
    mMin = vA;
    mMax = vB;

    mField_0x84 = (mMin + mMax) * 0.5f;
    mField_0x90 = mMin - mField_0x84;
    mField_0x9C = mMax - mField_0x84;
    mField_0xA8 = (mMax - mMin) * 0.5f;

    Update();
}

void cM3dGUnk::Update() {
    mVec3_c a, b;
    PSMTXMultVec(mMtx, mMin, a);
    PSMTXMultVec(mMtx, mMax, b);
    mField_0x78.x = (a.x + b.x) * 0.5f;
    mField_0x78.y = (a.y + b.y) * 0.5f;
    mField_0x78.z = (a.z + b.z) * 0.5f;
    if (!PSMTXInverse(mMtx, mInvMtx)) {
        PSMTXIdentity(mInvMtx);
    }
}

void cM3dGUnk::GetStartEnd(mVec3_c &start, mVec3_c &end) {
    start = mMin;
    end = mMax;
}

void cM3dGUnk::fn_80338c30(cM3dGCps *pCps, mVec3_c *pVec) {
    fn_80336d90(pCps, this, pVec);
}

bool cM3dGUnk::fn_80338c40(cM3dGUnk *pUnk, mVec3_c *pVec) {
    nw4r::math::VEC3 tmp;
    if (fn_803354e0(this, pUnk, &tmp)) {
        *pVec = tmp;
        return true;
    }
    return false;
}

bool cM3dGUnk::fn_80338ca0() {
    return false;
}

bool cM3dGUnk::fn_80338cb0(cM3dGCyl *pCyl, mVec3_c *pVec) {
    return fn_80336110(pCyl, this, pVec);
}

bool cM3dGUnk::fn_80338cc0(cM3dGCyl *pCyl, f32 *pF) {
    return fn_803364e0(pCyl, this, pF);
}

bool cM3dGUnk::fn_80338cd0(cM3dGSph *pSph, mVec3_c *pVec) {
    return fn_80337690(this, pSph, pVec);
}

bool cM3dGUnk::fn_80338ce0(cM3dGSph *pSph, f32 *pF) {
    return fn_80337780(this, pSph, pF);
}

void cM3dGUnk::Set(const mMtx_c &mtx) {
    mMtx.set(mtx);
    Update();
}

void cM3dGUnk::Set(const mVec3_c &vec, const mAng &ang) {
    mMtx_c mtx;
    PSMTXIdentity(mtx);
    mtx.YrotS(ang);
    mtx.SetTranslation(vec);
    Set(mtx);
}

bool cM3dGUnk::Cross(const mVec3_c &vec) {
    mVec3_c tmp;
    PSMTXMultVec(mInvMtx, vec, tmp);
    if ((tmp.x < mMin.x)) {
        return false;
    }
    if (tmp.x > mMax.x) {
        return false;
    }
    if (tmp.y < mMin.y) {
        return false;
    }
    if (tmp.y > mMax.y) {
        return false;
    }
    if (tmp.z < mMin.z) {
        return false;
    }
    if (tmp.z > mMax.z) {
        return false;
    }
    return true;
}

void cM3dGUnk::Clamp(const mVec3_c &in, mVec3_c &out) {
    mVec3_c tmp1, tmp2;
    PSMTXMultVec(mInvMtx, in, tmp1);
    cM3dGAab aab;
    aab.Set(mMin, mMax);
    aab.Clamp(tmp1, tmp2);
    PSMTXMultVec(mMtx, tmp2, out);
}

void cM3dGUnk::fn_80338f30(f32 f0, f32 f1) {
    mMtx.xw += f0;
    mMtx.zw += f1;
    Update();
}
