#ifndef M_ANGLE_H
#define M_ANGLE_H

#include "c/c_math.h"
#include "common.h"
#include "m/m_vec.h"
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

    s16 mVal;

    static mAng fromDeg(f32 deg) {
        return deg * sDegToAng;
    }

    static mAng fromRad(f32 rad) {
        return rad * sRadToAng;
    }

    static f32 Radian_to_Degree(f32 rad) {
        return rad * 57.2957763671875f;
    }
    static f32 Degree_to_Radian(f32 deg) {
        return deg * 0.017453292f;
    }
    static s16 Degree_to_SAngle(f32 deg) {
        return deg * 182.04444885253906f;
    }
    static f32 SAngle_to_Degree(s16 angle) {
        return (360.0f / 65536.0f) * angle;
    }
    static f32 SAngle_to_Radian(s16 angle) {
        return 9.58738E-5f * angle;
    }
    static f32 SAngle_to_Normal(s16 angle) {
        return 3.0517578E-5f * angle;
    }
    static s16 Radian_to_SAngle(f32 rad) {
        return rad * 10430.378f;
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
