#ifndef NW4R_G3D_MAYA_H
#define NW4R_G3D_MAYA_H
#include "common.h"
#include "nw4r/g3d/g3d_anmtexsrt.h"
#include "nw4r/math.h"

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {
bool CalcTexMtx_Maya(math::MTX34 *, bool, const TexSrt &, TexSrt::Flag);
}
} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif
