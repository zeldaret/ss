#ifndef M_ANGLE_H
#define M_ANGLE_H

#include <common.h>

struct mAng {
    mAng() {}
    mAng(s16 s) : val(s) {}
    s16 val;
};

class mAng3_c {
public:
    s16 x, y, z;
    void set(s16 fx, s16 fy, s16 fz) {
        x = fx;
        y = fy;
        z = fz;
    }
};

#endif
