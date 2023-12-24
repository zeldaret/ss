#ifndef NW4R_G3D_DRAW_H
#define NW4R_G3D_DRAW_H
#include "types_nw4r.h"
#include "g3d_resmdl.h"

namespace nw4r
{
    namespace g3d
    {
        struct DrawResMdlReplacement
        {
        };

        void DrawResMdlDirectly(ResMdl, const math::MTX34 *, const math::MTX33 *,
            const math::MTX34 *, const u8 *, const u8 *, DrawResMdlReplacement *, u32);
    }
}

#endif