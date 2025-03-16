#ifndef NW4R_G3D_BASIC_H
#define NW4R_G3D_BASIC_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_resanmtexsrt.h"

#include "nw4r/math.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {

// Forward declarations
struct ChrAnmResult;

namespace detail {
namespace dcc {

bool CalcTexMtx_Basic(math::MTX34 *pMtx, bool set, const TexSrt &rSrt, TexSrt::Flag flag);

u32 CalcWorldMtx_Basic(
    math::MTX34 *pW, math::VEC3 *pS, const math::MTX34 *pW1, const math::VEC3 *pS1, u32 attr,
    const ChrAnmResult *pResult
);

} // namespace dcc
} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif
