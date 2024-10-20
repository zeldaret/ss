#ifndef M_ANGLE_H
#define M_ANGLE_H

#include "common.h"
#include "nw4r/math/math_triangular.h"

struct mAng {
    mAng() {}
    mAng(s16 s) : mVal(s) {}
    mAng(const mAng &other) : mVal(other.mVal) {}

    operator s16() const {
        return mVal;
    }

    mAng operator-() {
        return mAng(-mVal);
    }

    mAng operator+(const mAng &other) const {
        return mAng(mVal + other.mVal);
    }

    mAng &operator+=(const mAng &other) {
        mVal += other.mVal;
        return *this;
    }

    s32 step(s16 target, s32 steps, s16 max, s16 min);

    f32 sin() const {
        return nw4r::math::SinIdx(*this);
    }

    f32 cos() const {
        return nw4r::math::CosIdx(*this);
    }

    s16 mVal;

    static mAng fromDeg(f32 deg) {
        return deg * sDegToAng;
    }

private:
    static const f32 sHalfCircleDeg;
    static const f32 sAngToDeg;
    static const f32 sAngToRad;
    static const f32 NotSure;
    static const f32 sDegToRad;
    static const f32 sDegToAng;
    static const f32 sRadToAng;
    static const f32 sRadToDeg;
};

class mAng3_c {
public:
    mAng3_c() {}

    mAng3_c(s16 fx, s16 fy, s16 fz) : x(fx), y(fy), z(fz) {}

    mAng3_c(const mAng3_c &r) {
        *this = r;
    }

    mAng3_c &operator=(const mAng3_c &r) {
        x = r.x;
        y = r.y;
        z = r.z;
        return *this;
    }

    void set(s16 fx, s16 fy, s16 fz) {
        x = fx;
        y = fy;
        z = fz;
    }

    mAng x, y, z;

    static mAng3_c Zero;
};

#endif
