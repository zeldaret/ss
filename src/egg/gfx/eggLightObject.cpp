#include "egg/gfx/eggLightObject.h"

#include "common.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggLightTexture.h"
#include "nw4r/g3d/g3d_light.h"
#include "nw4r/g3d/res/g3d_resanmlight.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtxvec.h"
#include "rvl/MTX/vec.h"

// TODO: This whole file does weird things with vectors!
// NSMBW symbol names indicate that functions use nw4r::math::VEC3
// arguments and we have some nw4r PS inlines, but
// a fair number of vector assignments happen in a weird order
// which might indicate EGG::Vector3f?

namespace EGG {

template <>
const char *IBinary<LightObject>::GetBinaryType() {
    return "LOBJ";
}

template <>
int IBinary<LightObject>::GetVersion() {
    return 2;
}

// why
static const f32 zeroVec[3] = {0.0f, 0.0f, 0.0f};

LightObject::LightObject()
    : mIndex(0),
      field_0x06(0),
      mAt(zeroVec),
      mPos(-10000.0f, 10000.0f, 10000.0f),
      mDir(0.0f, -1.0f, 0.0f),
      mWhite(DrawGX::WHITE),
      field_0x30(1.0f),
      mDist(1.0f),
      mBlack(DrawGX::BLACK),
      field_0x3C(0),
      field_0x3D(1),
      mSpotFn(GX_SP_OFF),
      mDistAttnFn(GX_DA_OFF),
      field_0x40(90.0f),
      field_0x44(0.5f),
      field_0x48(0.5f),
      mShininess(16.0f),
      mFlags(0x661) {}

void LightObject::Reset() {
    SetPosAt(nw4r::math::VEC3(-10000.0f, 10000.0f, 10000.0f), zeroVec);
    mFlags = 0x661;
    mWhite = DrawGX::WHITE;
    mBlack = DrawGX::BLACK;
    field_0x30 = 1.0;
    field_0x3C = 0;
    field_0x3D = 1;
    field_0x40 = 90.0;
    field_0x44 = 0.5;
    field_0x48 = 0.5;
    mSpotFn = GX_SP_OFF;
    mDistAttnFn = GX_DA_OFF;
    mShininess = 16.0;
    field_0x58.x = 1.0;
    field_0x4C.x = 1.0;
    field_0x58.z = 0.0;
    field_0x4C.z = 0.0;
    field_0x58.y = 0.0;
    field_0x4C.y = 0.0;
}

void LightObject::Calc() {
    field_0x6C = mDir;
    mLightColor = mWhite;
}

void LightObject::CalcView(nw4r::math::MTX34 const &viewMtx) {
    if ((mFlags & 1) == 0) {
        return;
    }

    if (getField0x3C() < 0 || getField0x3C() > 1) {
        switch (getField0x3C()) {
            case 3: {
                field_0x94.x = mViewAt.x = 0.0f;
                field_0x94.y = mViewAt.y = 0.0f;
                field_0x94.z = mViewAt.z = -1.0f;
                mViewAt.z *= mDist;
                mViewPos.x = mViewPos.y = mViewPos.z = 0.0f;
                break;
            }
            case 4: {
                field_0x94.x = mViewPos.x = 0.0f;
                field_0x94.y = mViewPos.y = 0.0f;
                field_0x94.z = mViewPos.z = 1.0f;
                mViewPos.z *= mDist;
                mViewAt.z = 0.0f;
                mViewAt.y = 0.0f;
                mViewAt.x = 0.0f;
                break;
            }
            case 2:
                field_0x94 = mDir;
                mViewAt = mAt;
                mViewPos = mPos;
                break;
        }

    } else {
        if (getField0x3C() == 1) {
            nw4r::math::VEC3 tmp(-viewMtx._20, -viewMtx._21, -viewMtx._22);
            if (PSVECMag(tmp) > 0.0f) {
                nw4r::math::VEC3 angle;
                C_VECHalfAngle(mDir, tmp, angle);
                nw4r::math::VEC3Scale(&field_0x6C, &angle, -1.0f);
                PSVECNormalize(field_0x6C, field_0x6C);
            } else {
                field_0x6C.y = 0.0f;
                field_0x6C.x = 0.0f;
                field_0x6C.z = 1.0f;
            }
        }
        PSMTXMultVec(viewMtx, mPos, mViewPos);
        PSMTXMultVec(viewMtx, mAt, mViewAt);
        nw4r::math::VEC3TransformNormal(&field_0x94, &viewMtx, &field_0x6C);
    }
}

void LightObject::fn_804A9C30(const LightTexture *tex, nw4r::math::VEC3 *pVec, GXColor *color) const {
    *pVec = field_0x94;
    *color = mLightColor;
    f32 f = tex->getFloat(mIndex);
    if (f < 1.0f) {
        color->r = color->r * f;
        color->g = color->g * f;
        color->b = color->b * f;
        color->a = color->a * f;
    } else {
        // Yes
        *color = *color;
    }
}

void LightObject::InitGX(GXLightObj *obj) const {
    if ((mFlags & 1) != 0 && ((mFlags >> 6) & 1) != 0) {
        GXInitLightColor(obj, mLightColor);
        switch (field_0x3D) {
            case 1: {
                nw4r::math::VEC3 tmp = field_0x94 * -1e10f;
                GXInitLightPos(obj, tmp.x, tmp.y, tmp.z);
                GXInitLightDir(obj, zeroVec[0], zeroVec[1], zeroVec[2]);
                break;
            }
            case 0: {
                GXInitLightPos(obj, mViewPos.x, mViewPos.y, mViewPos.z);
                GXInitLightDir(obj, zeroVec[0], zeroVec[1], zeroVec[2]);
                break;
            }
            case 2: {
                GXInitLightPos(obj, mViewPos.x, mViewPos.y, mViewPos.z);
                GXInitLightDir(obj, field_0x94.x, field_0x94.y, field_0x94.z);
                break;
            }
        }

        if ((mFlags & 0x80) != 0) {
            GXInitLightAttnA(obj, field_0x4C.x, field_0x4C.y, field_0x4C.z);
        } else {
            GXInitLightSpot(obj, field_0x40 > 0.0f ? field_0x40 : 0.0001f, getSpotFn());
        }

        if ((mFlags & 0x100) != 0) {
            GXInitLightAttnK(obj, field_0x58.x, field_0x58.y, field_0x58.z);
        } else {
            GXInitLightDistAttn(obj, field_0x44 * getDistance(), field_0x48, getDistAttnFn());
        }

        if ((mFlags & 0x800) != 0) {
            f32 ka = mShininess / 2.0f;
            GXInitLightAttn(obj, 0.0f, 0.0f, 1.0f, ka, 0.0f, 1.0f - ka);
        }

    } else {
        GXInitLightColor(obj, DrawGX::BLACK);
    }
}

void LightObject::CopyToG3D_World(nw4r::g3d::LightObj &g3dObj) const {
    if ((mFlags & 1) != 0 && (mFlags & 0x40) != 0) {
        g3dObj.Clear();
        if ((mFlags & 0x200) == 0) {
            g3dObj.DisableColor();
        }
        if ((mFlags & 0x400) == 0) {
            g3dObj.DisableAlpha();
        }

        g3dObj.Enable();
        g3dObj.InitLightColor(mLightColor);

        if ((mFlags & 0x80) != 0) {
            g3dObj.InitLightAttnA(field_0x4C.x, field_0x4C.y, field_0x4C.z);
        } else {
            g3dObj.InitLightSpot(field_0x40 > 0.0f ? field_0x40 : 0.0001f, getSpotFn());
        }

        if ((mFlags & 0x100) != 0) {
            g3dObj.InitLightAttnK(field_0x58.x, field_0x58.y, field_0x58.z);
        } else {
            g3dObj.InitLightDistAttn(field_0x44 * getDistance(), field_0x48, getDistAttnFn());
        }

        if ((mFlags & 0x800) != 0) {
            g3dObj.InitLightShininess(mShininess);
        }
    } else {
        g3dObj.Disable();
    }
}

void LightObject::CopyToG3D_View(nw4r::g3d::LightObj &g3dObj, const nw4r::math::MTX34 &viewMtx) const {
    if (!g3dObj.IsEnable()) {
        return;
    }

    // TODO
    if (!((mFlags >> 11) & 1)) {
        nw4r::math::VEC3 dir;
        nw4r::math::VEC3 pos;
        switch (field_0x3D) {
            case 1: {
                nw4r::math::VEC3TransformNormal(&dir, &viewMtx, &field_0x94);
                nw4r::math::VEC3Scale(&pos, &dir, -1e10f);
                dir.z = 0.0f;
                dir.y = 0.0f;
                dir.x = 0.0f;
                break;
            }
            case 0: {
                PSMTXMultVec(viewMtx, mViewPos, pos);
                dir.z = 0.0f;
                dir.y = 0.0f;
                dir.x = 0.0f;
                break;
            }
            case 2: {
                PSMTXMultVec(viewMtx, mViewPos, pos);
                nw4r::math::VEC3TransformNormal(&dir, &viewMtx, &field_0x94);
                break;
            }
        }
        g3dObj.InitLightPos(pos.x, pos.y, pos.z);
        g3dObj.InitLightDir(dir.x, dir.y, dir.z);
    } else {
        g3dObj.InitSpecularDir(mDir.x, mDir.y, mDir.z);
    }
}

void LightObject::CopyFromG3D(
    const nw4r::g3d::ResAnmLight &g3dLight, f32 frame, EGG::LightObject *optObj, bool skipDoingSomething
) {
    nw4r::g3d::LightAnmResult result;
    g3dLight.GetAnmResult(&result, frame);
    if (!skipDoingSomething || (result.flags & nw4r::g3d::LightAnmResult::FLAG_LIGHT_ENABLE) != 0) {
        if ((result.flags & nw4r::g3d::LightAnmResult::FLAG_LIGHT_ENABLE) != 0) {
            ApplyAnmResultA(result);
            if (optObj != nullptr && optObj->ApplyAnmResultB(result)) {
                optObj->field_0x06 = mIndex;
                optObj->mFlags |= 2;
            }
        } else {
            // TODO
            ClearFlag2();
            if (optObj != nullptr) {
                optObj->ClearFlag2();
            }
        }
    }
}

void LightObject::ApplyAnmResultInner(const nw4r::g3d::LightAnmResult &res) {
    // TODO
    mFlags = mFlags & 0xF07E;
    field_0x40 = 90.0f;
    field_0x44 = 0.5f;
    field_0x48 = 0.5f;
    mSpotFn = GX_SP_OFF;
    mDistAttnFn = GX_DA_OFF;
    mShininess = 16.0f;
    // ...
    field_0x58.x = 1.0f;
    field_0x4C.x = 1.0f;
    field_0x58.z = 0.0f;
    field_0x4C.z = 0.0f;
    field_0x58.y = 0.0f;
    field_0x4C.y = 0.0f;
    if ((res.flags & nw4r::g3d::LightAnmResult::FLAG_COLOR_ENABLE) != 0) {
        mFlags |= 0x200;
    }
    if ((res.flags & nw4r::g3d::LightAnmResult::FLAG_ALPHA_ENABLE) != 0) {
        mFlags |= 0x400;
    }
    if ((res.flags & nw4r::g3d::LightAnmResult::FLAG_LIGHT_ENABLE) != 0) {
        mFlags |= 1;
    }
}

void LightObject::ApplyAnmResultA(const nw4r::g3d::LightAnmResult &res) {
    ApplyAnmResultInner(res);
    switch ((int)(res.flags & nw4r::g3d::LightAnmResult::FLAG_LIGHT_TYPE_MASK)) {
        case 0: {
            field_0x3D = 0;
            mWhite = res.color;
            SetPosAt(res.pos, res.pos);
            field_0x40 = 0.0f;
            mSpotFn = GX_SP_OFF;
            field_0x44 = res.refDistance;
            field_0x48 = res.refBrightness;
            mDistAttnFn = res.distFunc;
            break;
        }
        case 1: {
            field_0x3D = 1;
            mWhite = res.color;
            SetPosAt(res.pos, res.aim);
            mSpotFn = GX_SP_OFF;
            mDistAttnFn = GX_AF_SPEC;
            break;
        }
        case 2: {
            field_0x3D = 2;
            mWhite = res.color;
            SetPosAt(res.pos, res.aim);
            mSpotFn = res.spotFunc;
            field_0x40 = res.cutoff;
            mDistAttnFn = res.distFunc;
            field_0x44 = res.refDistance;
            field_0x48 = res.refBrightness;
            break;
        }
    }
}

bool LightObject::ApplyAnmResultB(const nw4r::g3d::LightAnmResult &res) {
    if ((res.flags & nw4r::g3d::LightAnmResult::FLAG_SPECULAR_ENABLE) != 0) {
        ApplyAnmResultInner(res);
        field_0x3D = 1;
        mWhite = res.specColor;
        SetPosAt(res.pos, res.aim);
        mFlags |= 0x800;
        mShininess = res.shininess;
        return true;
    } else {
        ClearFlag2();
        return false;
    }
}

void LightObject::CalcDirDist() {
    mDir.x = mAt.x - mPos.x;
    mDir.y = mAt.y - mPos.y;
    mDir.z = mAt.z - mPos.z;
    mDist = nw4r::math::VEC3LenSq(&mDir);
    if (mDist > 0.0f) {
        nw4r::math::VEC3Scale(&mDir, &mDir, 1.0f / nw4r::math::FSqrt(mDist));
    } else {
        mDir.z = 0.0f;
        mDir.y = 0.0f;
        mDir.x = 0.0f;
    }
}

void LightObject::CalcAt() {
    mAt.x = mPos.x + mDir.x * mDist;
    mAt.y = mPos.y + mDir.y * mDist;
    mAt.z = mPos.z + mDir.z * mDist;
}

void LightObject::SetPos(const nw4r::math::VEC3 &pos) {
    mPos = pos;
    CalcDirDist();
}

void LightObject::SetAt(const nw4r::math::VEC3 &at) {
    mAt = at;
    CalcDirDist();
}

void LightObject::SetDist(f32 dist) {
    mDist = dist;
    CalcAt();
}

void LightObject::SetPosAt(nw4r::math::VEC3 const &pos, nw4r::math::VEC3 const &at) {
    mPos = pos;
    mAt = at;
    CalcDirDist();
}

void LightObject::SetBinaryInner(const Bin &bin) {}

void LightObject::GetBinaryInner(Bin *bin) const {}

void LightObject::SetBinaryInner(const Bin &bin1, const Bin &bin2, f32 blend) {}

} // namespace EGG
