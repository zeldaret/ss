#include <nw4r/g3d.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(AnmObj);

f32 FrameCtrl::smBaseUpdateRate = 1.0f;

f32 PlayPolicy_Onetime(f32 start, f32 end, f32 frame) {
#pragma unused(start)
#pragma unused(end)

    return frame;
}

f32 PlayPolicy_Loop(f32 start, f32 end, f32 frame) {
    f32 length = end - start;

    if (frame >= 0.0f) {
        return math::FMod(frame, length);
    }

    f32 offset = math::FMod(frame + length, length);
    return offset + math::FSelect(offset, 0.0f, length);
}

void AnmObj::SetAnmFlag(AnmFlag flag, bool value) {
    if (value) {
        mFlags |= flag;
    } else {
        mFlags &= ~flag;
    }
}

bool AnmObj::TestAnmFlag(AnmFlag flag) const {
    return mFlags & flag;
}

} // namespace g3d
} // namespace nw4r
