#include "egg/gfx/eggLightObject.h"

#include "egg/gfx/eggDrawGX.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtxvec.h"
#include "rvl/MTX/vec.h"

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
      mSpotFn(0),
      mDistAttnFn(0),
      field_0x40(90.0f),
      field_0x44(0.5f),
      field_0x48(0.5f),
      field_0x64(16.0f),
      field_0x68(0x661) {}

void LightObject::Reset() {
    SetPosAt(nw4r::math::VEC3(-10000.0f, 10000.0f, 10000.0f), zeroVec);
    field_0x68 = 0x661;
    mWhite = DrawGX::WHITE;
    mBlack = DrawGX::BLACK;
    field_0x30 = 1.0;
    field_0x3C = 0;
    field_0x3D = 1;
    field_0x40 = 90.0;
    field_0x44 = 0.5;
    field_0x48 = 0.5;
    mSpotFn = 0;
    mDistAttnFn = 0;
    field_0x64 = 16.0;
    field_0x58.x = 1.0;
    field_0x4C.x = 1.0;
    field_0x58.z = 0.0;
    field_0x4C.y = 0.0;
    field_0x58.y = 0.0;
    field_0x4C.z = 0.0;
}

void LightObject::Calc() {
    field_0x6C = mDir;
    field_0x78 = mWhite;
}

void LightObject::CalcView(nw4r::math::MTX34 const &viewMtx) {
    if ((field_0x68 & 1) == 0) {
        return;
    }

    if (getField0x3C() < 0 || getField0x3C() > 1) {
        switch (getField0x3C()) {
            case 3: {
                // Halp
                f32 scale = -1.0f;
                field_0x88.x = 0.0f;
                field_0x94.x = 0.0f;
                field_0x88.y = 0.0f;
                field_0x94.y = 0.0f;
                field_0x94.z = -1.0f;
                field_0x88.z = scale * mDist;
                field_0x7C.z = 0.0f;
                field_0x7C.y = 0.0f;
                field_0x7C.x = 0.0f;
                break;
            }
            case 4: {
                // Halp
                f32 scale = 1.0f;
                field_0x7C.x = 0.0f;
                field_0x94.x = 0.0f;
                field_0x7C.y = 0.0f;
                field_0x94.y = 0.0f;
                field_0x94.z = 1.0f;
                field_0x7C.z = scale * mDist;
                field_0x88.z = 0.0f;
                field_0x88.y = 0.0f;
                field_0x88.x = 0.0f;
                break;
            }
            case 2:
                field_0x94 = mDir;
                field_0x88 = mAt;
                field_0x7C = mPos;
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
        PSMTXMultVec(viewMtx, mPos, field_0x7C);
        PSMTXMultVec(viewMtx, mAt, field_0x88);
        nw4r::math::VEC3TransformNormal(&field_0x94, &viewMtx, &field_0x6C);
    }
}

void LightObject::InitGX(GXLightObj *obj) const {
    if ((field_0x68 & 1) != 0 && ((field_0x68 >> 6) & 1) != 0) {
        GXInitLightColor(obj, field_0x78);
        switch (field_0x3D) {
            case 1: {
                nw4r::math::VEC3 tmp = field_0x94 * -1e10f;
                GXInitLightPos(obj, tmp.x, tmp.y, tmp.z);
                GXInitLightDir(obj, zeroVec[0], zeroVec[1], zeroVec[2]);
                break;
            }
            case 0: {
                GXInitLightPos(obj, field_0x7C.x, field_0x7C.y, field_0x7C.z);
                GXInitLightDir(obj, zeroVec[0], zeroVec[1], zeroVec[2]);
                break;
            }
            case 2: {
                GXInitLightPos(obj, field_0x7C.x, field_0x7C.y, field_0x7C.z);
                GXInitLightDir(obj, field_0x94.x, field_0x94.y, field_0x94.z);
                break;
            }
        }

        if ((field_0x68 & 0x80) != 0) {
            GXInitLightAttnA(obj, field_0x4C.x, field_0x4C.y, field_0x4C.z);
        } else {
            GXInitLightSpot(obj, field_0x40 > 0.0f ? field_0x40 : 0.0001f, getSpotFn());
        }

        if ((field_0x68 & 0x100) != 0) {
            GXInitLightAttnK(obj, field_0x58.x, field_0x58.y, field_0x58.z);
        } else {
            GXInitLightDistAttn(obj, field_0x44 * getDistance(), field_0x48, getDistAttnFn());
        }

        if ((field_0x68 & 0x800) != 0) {
            f32 ka = field_0x64 / 2.0f;
            GXInitLightAttn(obj, 0.0f, 0.0f, 1.0f, ka, 0.0f, 1.0f - ka);
        }

    } else {
        GXInitLightColor(obj, DrawGX::BLACK);
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
