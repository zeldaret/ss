#ifndef NW4R_G3D_DRAW_H
#define NW4R_G3D_DRAW_H
#include "common.h"
#include "nw4r/g3d/g3d_resmdl.h"

namespace nw4r {
namespace g3d {
struct DrawResMdlReplacement {};

void DrawResMdlDirectly(ResMdl, const math::MTX34 *, const math::MTX33 *, const math::MTX34 *, const u8 *, const u8 *,
        DrawResMdlReplacement *, u32);
} // namespace g3d
} // namespace nw4r

#endif
