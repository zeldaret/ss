#ifndef EGG_GLOBAL_DRAW_STATE_H
#define EGG_GLOBAL_DRAW_STATE_H

#include "egg/gfx/eggScreen.h"
#include "nw4r/math/math_types.h"

namespace EGG {

class GlobalDrawState {
public:
    GlobalDrawState() {}
    static void setDrawSettingGX(bool, bool);
    static void beginDrawView(u16, nw4r::math::MTX34 const &, Screen const &);

private:
    static u16 sCameraId;
    static u32 sDrawFlag;
    static Screen const *spScreen;

    // unknown
    static nw4r::math::MTX34 sMtx;
};

} // namespace EGG

#endif
