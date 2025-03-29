#include "egg/gfx/eggFog.h"

#include "egg/egg_types.h"
#include "egg/gfx/eggDrawGX.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

template <>
const char *IBinary<Fog>::GetBinaryType() {
    return "FOGD";
}

template <>
int IBinary<Fog>::GetVersion() {
    return 0;
}

Fog::Fog() {
    Reset();
}

void Fog::Reset() {
    // TODO
    mColor = DrawGX::WHITE;
    mFarZ = 0.0f;
    mNearZ = 0.0f;
    mEndZ = 0.0f;
    mStartZ = 0.0f;
    mFogType = GX_FOG_NONE;
    mFlags = 0;
}

void Fog::CopyToG3D(nw4r::g3d::Fog fog) const {
    fog.SetFogType((mFlags & 1) ? mFogType : GX_FOG_NONE);
    fog.SetZ(mStartZ, mEndZ);
    fog.SetFogColor(mColor);
    fog.SetFogRangeAdjEnable(mFlags & 2);
}

void Fog::SetBinaryInner(const Bin &bin) {
    if (bin.mHeader.mVersion != 0) {
        return;
    }
    mStartZ = bin.mData.mStartZ;
    mEndZ = bin.mData.mEndZ;
    mNearZ = bin.mData.mNearZ;
    mFarZ = bin.mData.mFarZ;
    mColor = bin.mData.mColor;
    mFogType = (GXFogType)bin.mData.field_0x24;
    mFlags = bin.mData.mFlag2;
}

void Fog::GetBinaryInner(Bin *pBin) const {
    pBin->mData.mStartZ = mStartZ;
    pBin->mData.mEndZ = mEndZ;
    pBin->mData.mNearZ = mNearZ;
    pBin->mData.mFarZ = mFarZ;
    pBin->mData.mColor = mColor;
    pBin->mData.field_0x24 = mFogType;
    pBin->mData.mFlag2 = mFlags;
}

void Fog::SetBinaryInner(const Bin &, const Bin &, f32) {
    // TODO
}

Fog::~Fog() {}

} // namespace EGG
