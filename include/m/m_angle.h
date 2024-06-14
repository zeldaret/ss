#ifndef M_ANGLE_H
#define M_ANGLE_H

#include <common.h>

struct mAng {
    mAng() {}
    mAng(s16 s) : mVal(s) {}

    s32 step(s16 target, s32 steps, s16 max, s16 min);

    s16 mVal;
};

class mAng3_c {
public:
    mAng3_c() {}

    mAng3_c(s16 fx, s16 fy, s16 fz) : x(fx), y(fy), z(fz) {}

    void set(s16 fx, s16 fy, s16 fz) {
        x = fx;
        y = fy;
        z = fz;
    }

    mAng x, y, z;

    static mAng3_c Zero;
};

#endif
