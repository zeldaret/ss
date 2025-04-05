#ifndef D_LYT_VEC2F_H
#define D_LYT_VEC2F_H

#include "common.h"

// This better not be mVec2_c or something stupid
struct LytVec2f {
    LytVec2f() {}
    ~LytVec2f() {}

    LytVec2f(const LytVec2f &other) {
        set(other.x, other.y);
    }
    void set(f32 fx, f32 fy) {
        x = fx;
        y = fy;
    }

    f32 x;
    f32 y;
};

#endif
