#pragma once

#include "types.h"

struct mAng {
    public:
    s16 val;
};

class mAng3_c {
    public:
    s16 x,y,z;
    void set(s16 fx, s16 fy, s16 fz) {
        x = fx;
        y = fy;
        z = fz;
    }
};