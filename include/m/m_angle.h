#ifndef M_ANGLE_H
#define M_ANGLE_H

#include "c/c_math.h"
#include "common.h"
#include "m/m_vec.h"
#include "math.h"
#include "nw4r/math/math_triangular.h"
#include "s/s_Math.h"

class mAng {
public:
    mAng() {}
    mAng(s16 s) : mVal(s) {}
    mAng(const mAng &other) : mVal(other.mVal) {}

public:
    void set(s16 val) {
        mVal = val;
    }

public:
    s16 *ref() {
        return &mVal;
    }

    operator s16() const {
        return mVal;
    }

    mAng operator-() {
        return mAng(-mVal);
    }

    template <typename T>
    mAng &operator+=(const T &other) {
        mVal += s16(other);
        return *this;
    }
    template <typename T>
    mAng &operator-=(const T &other) {
        mVal -= s16(other);
        return *this;
    }
    template <typename T>
    mAng &operator*=(const T &other) {
        mVal *= other;
        return *this;
    }
    template <typename T>
    mAng &operator=(const T &other) {
        mVal = other;
        return *this;
    }

public:
    static s32 abs(const mAng b) {
        return labs(b);
    }
    s32 abs() const {
        return abs(*this);
    }
    s32 absDiff(const mAng &other) const {
        return sLib::absDiff(mVal, other.mVal);
    }
    f32 sin() const {
        return nw4r::math::SinIdx(*this);
    }
    f32 cos() const {
        return nw4r::math::CosIdx(*this);
    }
    static mAng atan2s(f32 a, f32 b) {
        return mAng(cM::atan2s(a, b));
    }

public:
    s32 step(s16 target, s32 steps, s16 max, s16 min);

public:
    static mAng fromVec(const mVec3_c &other) {
        return mAng(cM::atan2s(other.x, other.z));
    }
    static s16 fromDeg(f32 deg) {
        return deg * sDegToAng;
    }
    static mAng fromRad(f32 rad) {
        return rad * sRadToAng;
    }

    f32 degree() const {
        return (360.0f / 65536.0f) * mVal;
    }
    f32 radian() const {
        return ((2.f * M_PI) / 65536.0f) * mVal;
    }

    f32 degree_c() const {
        return mVal * sAngToDeg;
    }
    f32 radian_c() const {
        return mVal * sAngToRad;
    }

public:
    static s16 d2s(f32 deg) {
        return deg * (65536.0f / 360.0f);
    }
    static f32 s2d(s16 angle) {
        return (360.0f / 65536.0f) * angle;
    }
    static f32 s2d_c(f32 ang) {
        return ang * sAngToDeg;
    }
    static f32 d2s_c(f32 ang) {
        return ang * sDegToAng;
    }

    static f32 s2r(s16 angle) {
        return ((2.f * M_PI) / 65536.0f) * angle;
    }
    static s16 r2s(f32 rad) {
        return rad * (65536.0f / (2.f * M_PI));
    }
    static f32 s2r_c(f32 ang) {
        return ang * sAngToRad;
    }
    static f32 s2n_c(f32 ang) {
        return ang * sAngToNorm;
    }
    static f32 r2s_c(f32 ang) {
        return ang * sRadToAng;
    }

    static f32 d2r(f32 deg) {
        return deg * ((2.f * M_PI) / 360.f);
    }
    static f32 r2d(f32 rad) {
        return rad * (360.f / (2.f * M_PI));
    }
    static f32 d2r_c(f32 deg) {
        return deg * sDegToRad;
    }
    static f32 r2d_c(f32 rad) {
        return rad * sRadToDeg;
    }

    static f32 s2n(s16 angle) {
        return 3.0517578E-5f * angle;
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
    mAng3_c &operator=(const s32 &val) {
        set(val);
        return *this;
    }

    bool operator==(const mAng3_c &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    void set(const mAng3_c &other) {
        set(other.x, other.y, other.z);
    }

    void set(const s32 &f) {
        x = y = z = f;
    }

    void set(const s32 &fx, const s32 &fy, const s32 &fz) {
        x = fx;
        y = fy;
        z = fz;
    }

    void clear() {
        set(0, 0, 0);
    }

    mAng x, y, z;

    static mAng3_c Zero;
};

#endif
