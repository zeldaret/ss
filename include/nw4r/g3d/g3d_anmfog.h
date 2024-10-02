#ifndef NW4R_G3D_ANMFOG_H
#define NW4R_G3D_ANMFOG_H
#include "common.h"
#include <nw4r/ut/ut_Color.h>
#include <rvl/GX.h>

namespace nw4r {
namespace g3d {
struct FogAnmResult {
    GXFogType mFogType; // at 0x18
    f32 mStartZ;        // at 0x14
    f32 mEndZ;          // at 0x10
    ut::Color mColor;   // at 0xC
};
} // namespace g3d
} // namespace nw4r

#endif
