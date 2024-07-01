#ifndef NW4R_G3D_ANMCHR_H
#define NW4R_G3D_ANMCHR_H
#include "common.h"
#include "nw4r/math/math_types.h"

namespace nw4r {
namespace g3d {
struct ChrAnmResult {
    u32 mFlags; // at 0x0
    math::VEC3 VEC3_0x4;
    math::VEC3 VEC3_0x10;
    math::MTX34 mMtx; // at 0x1C
};

struct AnmObjChr {};

struct AnmObjChrNode {};

struct AnmObjChrBlend {};

struct AnmObjChrRes {};
} // namespace g3d
} // namespace nw4r

#endif
