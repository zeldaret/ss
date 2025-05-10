#ifndef NW4R_G3D_TRANSFORM_H
#define NW4R_G3D_TRANSFORM_H

#include "nw4r/math.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {
namespace detail {

bool CalcViewNrmMtx(nw4r::math::MTX33 *pOut, const nw4r::math::MTX34 *pW);
bool CalcViewTexMtx(nw4r::math::MTX34 *pOut, const nw4r::math::MTX34 *pW);
bool CalcInvWorldMtx(nw4r::math::MTX34 *pOut, const nw4r::math::MTX34 *pW);

} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif
