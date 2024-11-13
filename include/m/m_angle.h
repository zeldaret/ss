#ifndef M_ANGLE_H
#define M_ANGLE_H

#include "c/c_math.h"
#include "common.h"
#include "m/m_vec.h"
#include "math.h"
#include "nw4r/math/math_triangular.h"

struct mAng {
    mAng() {}
    mAng(s16 s) : mVal(s) {}
    mAng(const mAng &other) : mVal(other.mVal) {}

    static mAng atan2s(f32 a, f32 b) {
        return mAng(cM::atan2s(a, b));
    }
    static mAng fromVec(const mVec3_c &other) {
        return mAng(cM::atan2s(other.x, other.z));
    }

    operator s16() const {
        return mVal;
    }

    s16 *ref() {
        return &mVal;
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
    mAng &operator-=(const mAng &other) {
        mVal -= other.mVal;
        return *this;
    }

    s32 step(s16 target, s32 steps, s16 max, s16 min);

    f32 sin() const {
        return nw4r::math::SinIdx(*this);
    }

    f32 cos() const {
        return nw4r::math::CosIdx(*this);
    }

    f32 degree() const {
        return (360.0f / 65536.0f) * mVal;
    }

    static mAng fromDeg(f32 deg) {
        return deg * sDegToAng;
    }
    f32 degree2() const {
        return mVal * sAngToDeg;
    }

    static s16 angle(const mVec3_c &a, const mVec3_c &b) {
        f32 rads = a.angle(b);
        return fromRad(rads);
    }

    f32 radian() const {
        return ((2.f * M_PI) / 65536.0f) * mVal;
    }

    f32 radian2() const {
        return mVal * sAngToRad;
    }

    static s16 fromRad(f32 rad) {
        return rad * sRadToAng;
    }

    static f32 rad2deg(f32 rad) {
        return rad * (360.f / (2.f * M_PI));
    }
    static f32 deg2rad(f32 deg) {
        return deg * ((2.f * M_PI) / 360.f);
    }
    static s16 deg2short(f32 deg) {
        return deg * (65536.0f / 360.0f);
    }
    static f32 short2deg(s16 angle) {
        return (360.0f / 65536.0f) * angle;
    }
    static f32 short2rad(s16 angle) {
        return ((2.f * M_PI) / 65536.0f) * angle;
    }
    static f32 short2norm(s16 angle) {
        return 3.0517578E-5f * angle;
    }
    static s16 rad2short(f32 rad) {
        return rad * (65536.0f / (2.f * M_PI));
    }

    s16 mVal;

private:
    static const f32 sHalfCircleDeg;
    static const f32 sAngToDeg;
    static const f32 sAngToRad;
    static const f32 sAngToNorm;
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
