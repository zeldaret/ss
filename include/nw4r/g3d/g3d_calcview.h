#ifndef NW4R_G3D_CALC_VIEW_H
#define NW4R_G3D_CALC_VIEW_H
#include "common.h"
#include "nw4r/math.h"

namespace nw4r {
namespace g3d {
void CalcView(math::MTX34 *, math::MTX33 *, const math::MTX34 *, const u32 *, u32, const math::MTX34 *, ResMdl,
        math::MTX34 *);
}
} // namespace nw4r

#endif
