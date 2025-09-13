#ifndef TOBESORTED_DEG_ANGLE_UTIL_H
#define TOBESORTED_DEG_ANGLE_UTIL_H

#include "common.h"
#include "m/m_vec.h"

struct dDegree {
public:
    /** Wraps a degree angle so that value is within [-180.0f; 180.0f] */
    static f32 adjust(f32 value);
    /** Canonicalizes a degree angle so that value is within [-180.0f; 180.0f) */
    static f32 canonicalize(f32 value);

    f32 rotate180F() const;
    void rotate180();
    f32 abs() const;

    f32 sin() const;
    f32 cos() const;
    f32 tan() const;
    f32 tan2() const;
    f32 toRad() const;
    s16 toIdx(); // can't be const...

    void Set(f32 v) {
        value = adjust(v);
    }

    f32 operator-(const dDegree &other) const {
        return value - other.value;
    }

    f32 operator+(const dDegree &other) const {
        return value + other.value;
    }

    dDegree() : value(0.0f) {}
    dDegree(f32 value) : value(adjust(value)) {}
    dDegree(const dDegree &other) : value(other.value) {}

    /* 0x0 */ f32 value;
};

struct dPolar {
public:
    dPolar(f32 r, f32 u, f32 v);
    dPolar(const mVec3_c &v);

    dPolar *canonicalize();

    void Set(f32 r, f32 u, f32 v);

    void setCartesian(const mVec3_c &v);
    mVec3_c toCartesian() const;

    f32 SetR(f32 r);
    f32 SetU(f32 u);
    f32 SetV(f32 v);

    /* 0x0 */ f32 R;
    /* 0x4 */ dDegree U;
    /* 0x8 */ dDegree V;
};

#endif
