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

    mAng &operator=(const s32 &val) {
        mVal = val;
        return *this;
    }

    static mAng atan2s(f32 a, f32 b) {
        return mAng(cM::atan2s(a, b));
    }
    static mAng fromVec(const mVec3_c &other) {
        return mAng(cM::atan2s(other.x, other.z));
    }

    operator s16() const {
        return mVal;
    }

    void set(s16 val) {
        mVal = val;
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
    mAng &operator*=(const s32 &other) {
        mVal *= other;
        return *this;
    }
    mAng &operator*=(const f32 &other) {
        mVal *= other;
        return *this;
    }

    static s32 abs(const mAng b) {
        return labs(b);
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

    static s16 fromDeg(f32 deg) {
        return deg * sDegToAng;
    }
    f32 degree2() const {
        return mVal * sAngToDeg;
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

    // TODO - This is the only way I could get the regswap to be fixed..
    // Found with the pattern mAng3_c.y += cM::rndFX()
    void addY(f32 val) {
        y.mVal += (s16)val;
    }

    void addX(const s32 &fx) {
        x += fx;
    }

    mAng x, y, z;

    static mAng3_c Zero;
};

#endif
