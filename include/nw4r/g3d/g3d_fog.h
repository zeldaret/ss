#ifndef NW4R_G3D_FOG_H
#define NW4R_G3D_FOG_H
#include "common.h"
#include "nw4r/g3d/g3d_rescommon.h"
#include "nw4r/math/math_types.h"
#include "nw4r/ut/ut_Color.h"
#include <rvl/GX.h>


namespace nw4r {
namespace g3d {
struct FogData {
    GXFogType mFogType;    // at 0x0
    f32 mStartZ;           // at 0x4
    f32 mEndZ;             // at 0x8
    f32 mNear;             // at 0xC
    f32 mFar;              // at 0x10
    GXColor mColor;        // at 0x14
    u8 mFogRangeAdjEnable; // at 0x18
    u8 BYTE_0x19;
    u16 mAdjTableWidth;      // at 0x1A
    GXFogAdjTable mAdjTable; // at 0x1C
};

struct Fog {
    ResCommon<FogData> mFogData;

    inline Fog(void *vptr) : mFogData(vptr) {}

    Fog(FogData *);
    void Init();
    void *CopyTo(void *) const;
    void SetFogRangeAdjParam(u16, u16, const math::MTX44 &);
    void SetGP() const;

    bool IsValid() const {
        return mFogData.IsValid();
    }

    bool IsFogRangeAdjEnable() const {
        return (IsValid() && mFogData.ref().mFogRangeAdjEnable != 1) ? true : false;
    }

    void SetFogColor(GXColor c) {
        if (IsValid()) {
            mFogData.ref().mColor = c;
        }
    }

    void SetFogType(GXFogType fog) {
        if (IsValid()) {
            mFogData.ref().mFogType = fog;
        }
    }

    void SetNearFar(f32 near, f32 far) {
        if (IsValid()) {
            FogData &ref = mFogData.ref();
            ref.mNear = near;
            ref.mFar = far;
        }
    }

    void SetZ(f32 start, f32 end) {
        if (IsValid()) {
            FogData &ref = mFogData.ref();
            ref.mStartZ = start;
            ref.mEndZ = end;
        }
    }
};
} // namespace g3d
} // namespace nw4r

#endif
