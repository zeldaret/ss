#ifndef EGG_VECTOR_H
#define EGG_VECTOR_H

#include "MSL_C/float.h"
#include "egg/math/eggMath.h"
#include <common.h>
#include <nw4r/math.h>

namespace EGG {

struct Vector3f : public nw4r::math::VEC3 {
    Vector3f() {}                                          // __ct__Q23EGG8Vector3fFv
    Vector3f(f32 fx, f32 fy, f32 fz) : VEC3(fx, fy, fz) {} // __ct__Q23EGG8Vector3fFfff

    // ~Vector3f() {}

    f32 &operator()(int i) { // __cl__Q23EGG8Vector3fFi
        return ((f32 *)this)[i];
    }
    Vector3f operator*(f32 f) const { // __ml__Q23EGG8Vector3fCFf
        return Vector3f(x * f, y * f, z * f);
    }
    friend Vector3f operator*(f32 f, const Vector3f &v) {
        return v.operator*(f);
    }

    Vector3f operator+(const Vector3f &v) { // __pl__Q23EGG8Vector3fCFRCQ23EGG8Vector3f
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }

    Vector3f &operator+=(const Vector3f &v) { // __apl__Q23EGG8Vector3fFRCQ23EGG8Vector3f
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3f operator-() const { // __mi__Q23EGG8Vector3fCFv
        return Vector3f(-1.0f * x, -1.0f * y, -1.0f * z);
    }

    Vector3f operator-(const Vector3f &v) { // __mi__Q23EGG8Vector3fCFRCQ23EGG8Vector3f
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }

    Vector3f &operator-=(const Vector3f &v) { // __ami__Q23EGG8Vector3fFRCQ23EGG8Vector3f
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3f &operator*=(f32 f) { // __amu__Q23EGG8Vector3fFf
        multScalar(f);
        return *this;
    }

    Vector3f operator/(f32 f) const { // __dv__Q23EGG8Vector3fCFf
        return Vector3f(x / f, y / f, z / f);
    }

    Vector3f &operator/=(f32 f) { // __adv__Q23EGG8Vector3fCFf // assumed
        divScalar(f);
        return *this;
    }

    bool operator!=(const Vector3f &v) { // __ne__Q23EGG8Vector3fCFRCQ23EGG8Vector3f
        return x != v.x || y != v.y || z != v.z;
    }

    /* 8049bcc0 */ f32 normalise();
    /* 8049bd50 */ f32 setLength(const Vector3f &src, f32 len);
    /* 8049be10 */ f32 setLength(f32 len);

    void multScalar(f32 f) {
        x *= f;
        y *= f;
        z *= f;
    }
    void divScalar(f32 f) {
        multScalar(1.0f / f);
    }
    f32 dot(const Vector3f &v) const { // dot__Q23EGG8Vector3fCFRCQ23EGG8Vector3f
        return x * v.x + y * v.y + z * v.z;
    }
    Vector3f cross(const Vector3f &b) const { // cross__Q23EGG8Vector3fCFRCQ23EGG8Vector3f

        f32 _x = (y * b.z) - (z * b.y);
        f32 _y = (z * b.x) - (x * b.z);
        f32 _z = (x * b.y) - (y * b.x);
        return Vector3f(_x, _y, _z);
    }
    f32 squaredLength() const {
        return (x * x + y * y + z * z);
    }
    f32 length() const {
        return Math<f32>::sqrt(squaredLength());
    }
    void set(f32 fx, f32 fy, f32 fz) {
        x = fx;
        y = fy;
        z = fz;
    }

    /* 80674c30 */ static const Vector3f zero;
    /* 80674c3c */ static const Vector3f ex;
    /* 80674c48 */ static const Vector3f ey;
    /* 80674c54 */ static const Vector3f ez;
};

struct Vector2f : nw4r::math::VEC2 {
    inline Vector2f() {}
    Vector2f(f32 fx, f32 fy) : VEC2(fx, fy) {}
    inline ~Vector2f() {}

public:
    /* 805767c0 */ static const Vector2f zero;
    /* 805767c8 */ static const Vector2f ex;
    /* 805767d0 */ static const Vector2f ey;

    Vector2f operator-(const Vector2f &v) {
        return Vector2f(x - v.x, y - v.y);
    }
};

struct Vector3s {
    s16 x, y, z;

public:
    Vector3s(s16 sx, s16 sy, s16 sz) {
        x = sx;
        y = sy;
        z = sz;
    }
    /* 805767d8 */ static const Vector3s zero;
    /* 805767e0 */ static const Vector3s ex;
    /* 805767e8 */ static const Vector3s ey;
    /* 805767f0 */ static const Vector3s ez;
};

} // namespace EGG

#endif
