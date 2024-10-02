#ifndef NW4R_G3D_ANMCAMERA_H
#define NW4R_G3D_ANMCAMERA_H
#include "common.h"
#include "nw4r/math/math_types.h"
#include <rvl/GX.h>


namespace nw4r {
namespace g3d {
struct CameraAnmResult {
    UNKWORD WORD_0x0;
    GXProjectionType projType;
    f32 FLOAT_0x8;
    f32 FLOAT_0xC;
    f32 FLOAT_0x10;
    f32 FLOAT_0x14;
    f32 FLOAT_0x18;
    f32 FLOAT_0x1C;
    f32 FLOAT_0x20;
    f32 FLOAT_0x24;
    f32 FLOAT_0x28;
    f32 FLOAT_0x2C;
    f32 FLOAT_0x30;
};
} // namespace g3d
} // namespace nw4r

#endif
