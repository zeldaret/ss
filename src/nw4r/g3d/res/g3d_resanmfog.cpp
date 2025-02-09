#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void ResAnmFog::GetAnmResult(FogAnmResult* pResult, f32 frame) const {
    const ResAnmFogData& r = ref();
    u32 flags = r.flags;

    const ResAnmScnInfoData& rInfoData =
        ofs_to_ptr<ResAnmScnData>(r.toResAnmScnData)->info;

    f32 clippedFrame = detail::ClipFrame(rInfoData, frame);

    bool startZConstant = (flags & ResAnmFogData::FLAG_START_CONST) != 0;
    bool endZConstant = (flags & ResAnmFogData::FLAG_END_CONST) != 0;
    bool colorConstant = (flags & ResAnmFogData::FLAG_COLOR_CONST) != 0;

    pResult->type = r.type;
    pResult->startz = detail::GetResAnmResult(&r.startz, frame, startZConstant);
    pResult->endz = detail::GetResAnmResult(&r.endz, frame, endZConstant);
    pResult->color =
        detail::GetResColorAnmResult(&r.color, clippedFrame, colorConstant);
}

} // namespace g3d
} // namespace nw4r
