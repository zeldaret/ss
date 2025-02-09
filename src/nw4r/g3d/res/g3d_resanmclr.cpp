#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void ResAnmClr::GetAnmResult(ClrAnmResult* pResult, u32 idx, f32 frame) const {
    const ResAnmClrMatData* pMatData = GetMatAnm(idx);
    const ResAnmClrAnmData* pAnmData = pMatData->anms;
    const ResAnmClrInfoData& rInfoData = ref().info;

    u32 flags = pMatData->flags;
    pResult->bRgbaExist = 0;

    if (flags == 0) {
        return;
    }

    f32 clippedFrame = detail::ClipFrame(rInfoData, frame);

    for (int i = 0; i < ClrAnmResult::CLA_MAX;
         flags >>= ResAnmClrMatData::NUM_OF_FLAGS, i++) {

        if (!(flags & ResAnmClrMatData::FLAG_ANM_EXISTS)) {
            continue;
        }

        bool constant = flags & ResAnmClrMatData::FLAG_ANM_CONSTANT;

        pResult->bRgbaExist |= 1 << i;
        pResult->rgbaMask[i] = pAnmData->mask;
        pResult->rgba[i] = detail::GetResColorAnmResult(&pAnmData->color,
                                                        clippedFrame, constant);

        pAnmData++;
    }
}

} // namespace g3d
} // namespace nw4r
