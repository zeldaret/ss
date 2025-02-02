#ifndef NW4R_G3D_CALC_VIEW_H
#define NW4R_G3D_CALC_VIEW_H
#include <nw4r/types_nw4r.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {
namespace detail {

/**
 * Locked cache memory layout
 */
struct MtxCacheMap {
    u8 currDmaArray[32 * sizeof(math::MTX44)];  // at 0x0
    u8 currViewArray[32 * sizeof(math::MTX44)]; // at 0x800
    u8 currNrmArray[32 * sizeof(math::MTX44)];  // at 0x1000
    u8 currTexArray[32 * sizeof(math::MTX44)];  // at 0x1800

    u8 prevDmaArray[32 * sizeof(math::MTX44)];  // at 0x2000
    u8 prevViewArray[32 * sizeof(math::MTX44)]; // at 0x2800
    u8 prevNrmArray[32 * sizeof(math::MTX44)];  // at 0x3000
    u8 prevTexArray[32 * sizeof(math::MTX44)];  // at 0x3800
};

} // namespace detail

void CalcView(
    math::MTX34 *pViewPosArray, math::MTX33 *pViewNrmArray, const math::MTX34 *pModelMtxArray,
    const u32 *pModelMtxAttribArray, u32 numMtx, const math::MTX34 *pViewMtx, const ResMdl mdl,
    math::MTX34 *pViewTexMtxArray
);

void CalcView_LC(
    math::MTX34 *pViewPosArray, math::MTX33 *pViewNrmArray, const math::MTX34 *pModelMtxArray,
    const u32 *pModelMtxAttribArray, u32 numMtx, const math::MTX34 *pViewMtx, const ResMdl mdl,
    math::MTX34 *pViewTexMtxArray
);

void CalcView_LC_DMA_ModelMtx(
    math::MTX34 *pViewPosArray, math::MTX33 *pViewNrmArray, const math::MTX34 *pModelMtxArray,
    const u32 *pModelMtxAttribArray, u32 numMtx, const math::MTX34 *pViewMtx, const ResMdl mdl,
    math::MTX34 *pViewTexMtxArray
);

} // namespace g3d
} // namespace nw4r

#endif
