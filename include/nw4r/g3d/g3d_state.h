#ifndef NW4R_G3D_STATE_H
#define NW4R_G3D_STATE_H
#include "common.h"
#include "nw4r/math.h"
#include <rvl/GX.h>

namespace nw4r {
namespace g3d {
namespace G3DState {
struct IndMtxOp {};

void SetViewPosNrmMtxArray(const math::MTX34 *, const math::MTX33 *, const math::MTX34 *);

void SetRenderModeObj(const GXRenderModeObj &);
GXRenderModeObj &GetRenderModeObj();
void LoadLightSet(int idx_lightset, u32 *mask_diff_color /* r1+0xC */, u32 *mask_diff_alpha, u32 *mask_spec_color,
        u32 *mask_spec_alpha, AmbLightObj *amb);

void Invalidate(u32);
} // namespace G3DState
} // namespace g3d
} // namespace nw4r

#endif
