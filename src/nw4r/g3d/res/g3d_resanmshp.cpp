#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void ResAnmShp::GetAnmResult(ShpAnmResult* pResult, u32 idx, f32 frame,
                             const ShpAnmVtxSet* pShapeArray) const {
    const ResAnmShpAnmData* pAnmData = GetShapeAnm(idx);

    const u16* pAnmIdxToVtxIdxTable = static_cast<const u16*>(
        ut::AddOffsetToPtr(pAnmData, pAnmData->toAnmIdxToVtxIdxTable));

    u32 flags = pAnmData->flags;
    u32 constFlags = pAnmData->constFlags;

    pResult->flags = flags;
    pResult->numKeyShape = pAnmData->numKeyShape;
    pResult->baseShapeVtxSet = pShapeArray[pAnmData->baseShapeVtxIdx];
    pResult->baseShapeWeight = 1.0f;

    for (int i = 0; i < pAnmData->numKeyShape; constFlags >>= 1, i++) {
        BlendVtx& rShape = pResult->keyShape[i];

        bool constant = (constFlags & 1) != 0;
        int vtxIdx = pAnmIdxToVtxIdxTable[i];

        f32 weight =
            detail::GetResAnmResult(&pAnmData->anms[i], frame, constant);

        rShape.vtxSet = pShapeArray[vtxIdx];
        rShape.weight = weight;

        pResult->baseShapeWeight -= weight;
    }
}

} // namespace g3d
} // namespace nw4r
