#ifndef NW4R_G3D_DRAW_H
#define NW4R_G3D_DRAW_H
#include "common.h"
#include "nw4r/g3d/g3d_resmdl.h"

namespace nw4r {
namespace g3d {
struct DrawResMdlReplacement {};

void DrawResMdlDirectly(ResMdl mdl, const math::MTX34 *pViewPos, const math::MTX33 *pViewNrm,
        const math::MTX34 *pViewEnv, const u8 *pOpa, const u8 *pXlu, DrawResMdlReplacement *pRep, u32 resMdlDrawMode);
} // namespace g3d
} // namespace nw4r

#endif
