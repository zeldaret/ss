#include "toBeSorted/deg_angle_util.h"

#include "c/c_math.h"
#include "common.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "math.h"

#include <cmath>

f32 dDegree::adjust(f32 value) {
    while (value < -180.0f) {
        value += 360.0f;
    }

    while (value > 180.0f) {
        value -= 360.0f;
    }

    return value;
}

f32 dDegree::canonicalize(f32 value) {
    while (value < -180.0f) {
        value += 360.0f;
    }

    while (value >= 180.0f) {
        value -= 360.0f;
    }

    return value;
}

f32 dDegree::rotate180F() const {
    return adjust(value + 180.0f);
}

void dDegree::rotate180() {
    value = adjust(value + 180.0f);
}

f32 dDegree::abs() const {
    f32 v = value;
    if (v < 0.0f) {
        v = -v;
    }
    return v;
}

f32 dDegree::sin() const {
    return std::sinf(toRad());
}

f32 dDegree::cos() const {
    return std::cosf(toRad());
}

f32 dDegree::tan() const {
    f32 c = cos();
    f32 s = sin();
    if (cM::isZero(c)) {
        return s > 0.0f ? INFINITY : -INFINITY;
    }
    return s / c;
}

f32 dDegree::tan2() const {
    return tan();
}

f32 dDegree::toRad() const {
    return mAng::deg2rad_c(value);
}

s16 dDegree::toIdx() {
    return mAng::fromDeg(value);
}

dPolar::dPolar(f32 r, f32 u, f32 v) {
    Set(r, u, v);
}

dPolar::dPolar(const mVec3_c &v) {
    setCartesian(v);
}

dPolar *dPolar::canonicalize() {
    // NONMATCHING - redundant stack copies
    if (R < 0.0f) {
        R = -R;
        U.Set(-U.value);
        V.Set(V.rotate180F());
    }

    if (U.value < -90.0f) {
        U.Set(U.value + 180.0f);
        V.Set(V.rotate180F());
    } else if (U.value > 90.0f) {
        U.Set(U - 180.0f);
        V.Set(V.rotate180F());
    }
    return this;
}

void dPolar::Set(f32 r, f32 u, f32 v) {
    R = r;
    U.Set(u);
    V.Set(v);
    canonicalize();
}

// TODO - how to prevent the inline from being used?
extern "C" double sqrt_help(double);

void dPolar::setCartesian(const mVec3_c &v) {
    // NONMATCHING - too many frsp
    f32 x = v.x;
    f32 y = v.y;
    f32 z = v.z;
    f64 magXZSq = (f64)(z * z) + (f64)(x * x);
    f64 magSq = magXZSq + (f64)(y * y);

    f32 magXZ, mag;
    if (magXZSq > 0.0) {
        magXZ = sqrt_help(magXZSq);
    } else {
        magXZ = 0.0;
    }

    if (magSq > 0.0) {
        mag = sqrt_help(magSq);
    } else {
        mag = 0.0;
    }

    R = mag;
    f32 atan = cM::atan2f(magXZ, y);
    U = mAng::rad2deg_c(M_PI / 2 - atan);
    V = mAng::rad2deg_c(cM::atan2f(x, z));
    canonicalize();
}

mVec3_c dPolar::toCartesian() const {
    // NONMATCHING - regswap
    f32 cosU = U.cos();
    f32 sinU = U.sin();
    f32 cosV = V.cos();
    f32 sinV = V.sin();
    f32 tmp = R * cosU;
    return mVec3_c(sinV * tmp, R * sinU, cosV * tmp);
}

f32 dPolar::SetR(f32 r) {
    R = r;
    canonicalize();
    return R;
}

f32 dPolar::SetU(f32 u) {
    U.Set(u);
    canonicalize();
    return U.value;
}

f32 dPolar::SetV(f32 v) {
    V.Set(v);
    return V.value;
}
