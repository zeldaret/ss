#ifndef NW4R_G3D_TRANSFORM_H
#define NW4R_G3D_TRANSFORM_H

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {
namespace detail {

bool CalcViewNrmMtx(nw4r::math::MTX33 *, const nw4r::math::MTX34 *);
bool CalcViewTexMtx(nw4r::math::MTX33 *, const nw4r::math::MTX34 *);
bool CalcInvWorldMtx(nw4r::math::MTX33 *, const nw4r::math::MTX34 *);

} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif
