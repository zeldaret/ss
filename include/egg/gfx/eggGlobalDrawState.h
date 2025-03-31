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

    static const Screen &getScreen() {
        return *spScreen;
    }
    static void setScreen(const Screen &screen) {
        spScreen = &screen;
    }
    static u32 getDrawFlag() {
        return sDrawFlag;
    }
    static void setDrawFlag(u32 flag) {
        sDrawFlag = flag;
    }
    static void onDrawFlag(u32 flag) {
        sDrawFlag |= flag;
    }
    static void offDrawFlag(u32 flag) {
        sDrawFlag &= ~flag;
    }

    static Screen const *spScreen;
private:
    static u16 sCameraId;
    static u32 sDrawFlag;

    // unknown
    static nw4r::math::MTX34 sMtx;
};

} // namespace EGG

#endif
