#ifndef NW4R_G3D_CALC_WORLD_H
#define NW4R_G3D_CALC_WORLD_H
#include "common.h"
#include "nw4r/math.h"

namespace nw4r {
namespace g3d {

class WorldMtxManip {};

void CalcWorld(math::MTX34 *, u32 *, const u8 *, const math::MTX34 *, ResMdl, AnmObjChr *, FuncObjCalcWorld *, u32);

void CalcWorld(math::MTX34 *, u32 *, const u8 *, const math::MTX34 *, ResMdl, AnmObjChr *, FuncObjCalcWorld *);

void CalcSkinning(math::MTX34 *, u32 *, ResMdl, const u8 *);
} // namespace g3d
} // namespace nw4r

#endif
