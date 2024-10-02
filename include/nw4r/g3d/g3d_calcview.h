#ifndef NW4R_G3D_CALC_VIEW_H
#define NW4R_G3D_CALC_VIEW_H
#include "common.h"
#include "nw4r/math.h"

namespace nw4r {
namespace g3d {
void CalcView(math::MTX34 *pViewPosArray, math::MTX33 *pViewNrmArray, const math::MTX34 *pModelMtxArray,
        const u32 *pModelMtxAttribArray, u32 numMtx, const math::MTX34 *pView, ResMdl resMdl,
        math::MTX34 *pVewTexMtxArray);
}
} // namespace nw4r

#endif
