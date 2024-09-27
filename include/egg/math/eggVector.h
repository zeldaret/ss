#ifndef EGG_VECTOR_H
#define EGG_VECTOR_H

#include "MSL_C/float.h"
#include "egg/math/eggMath.h"

#include <common.h>
#include <nw4r/math.h>

namespace EGG {

struct Vector3f : public nw4r::math::VEC3 {
    // __ct__Q23EGG8Vector3fFv
    Vector3f() : nw4r::math::VEC3() {}

    // __ct__Q23EGG8Vector3fFfff
    Vector3f(f32 fx, f32 fy, f32 fz) : VEC3(fx, fy, fz) {}

    // ~Vector3f() {}

    // __cl__Q23EGG8Vector3fFi
    f32 &operator()(int i) { return ((f32 *)this)[i]; }

    // __ml__Q23EGG8Vector3fCFf
    Vector3f operator*(f32 f) const { return Vector3f(x * f, y * f, z * f); }

    friend Vector3f operator*(f32 f, const Vector3f &v) { return v.operator*(f); }

    // __pl__Q23EGG8Vector3fCFRCQ23EGG8Vector3f
    Vector3f operator+(const Vector3f &v) { return Vector3f(x + v.x, y + v.y, z + v.z); }

    // __apl__Q23EGG8Vector3fFRCQ23EGG8Vector3f
    Vector3f &operator+=(const Vector3f &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    // __mi__Q23EGG8Vector3fCFv
    Vector3f operator-() const {
        f32 z = this->z;
        f32 y = this->y;
        f32 x = this->x;
        return Vector3f(-x, -y, -z);
    }

    // __mi__Q23EGG8Vector3fCFRCQ23EGG8Vector3f
    Vector3f operator-(const Vector3f &v) { return Vector3f(x - v.x, y - v.y, z - v.z); }

    // __ami__Q23EGG8Vector3fFRCQ23EGG8Vector3f
    Vector3f &operator-=(const Vector3f &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    // __amu__Q23EGG8Vector3fFf
    Vector3f &operator*=(f32 f) {
        multScalar(f);
        return *this;
    }

    // __dv__Q23EGG8Vector3fCFf
    Vector3f operator/(f32 f) const { return Vector3f(x / f, y / f, z / f); }

    // __adv__Q23EGG8Vector3fCFf // assumed
    Vector3f &operator/=(f32 f) {
        divScalar(f);
        return *this;
    }

    // __ne__Q23EGG8Vector3fCFRCQ23EGG8Vector3f
    bool operator!=(const Vector3f &v) { return x != v.x || y != v.y || z != v.z; }

    void multScalar(f32 f) {
        x *= f;
        y *= f;
        z *= f;
    }
    void divScalar(f32 f) { multScalar(1.0f / f); }

    // dot__Q23EGG8Vector3fCFRCQ23EGG8Vector3f
    f32 dot(const Vector3f &v) const { return x * v.x + y * v.y + z * v.z; }

    // cross__Q23EGG8Vector3fCFRCQ23EGG8Vector3f
    Vector3f cross(const Vector3f &b) const {
        f32 _x = (y * b.z) - (z * b.y);
        f32 _y = (z * b.x) - (x * b.z);
        f32 _z = (x * b.y) - (y * b.x);
        return Vector3f(_x, _y, _z);
    }
    f32 squaredLength() const { return (x * x + y * y + z * z); }
    f32 length() const { return Math<f32>::sqrt(squaredLength()); }
    void set(f32 fx, f32 fy, f32 fz) {
        x = fx;
        y = fy;
        z = fz;
    }

    f32 normalise();
    f32 setLength(const Vector3f &src, f32 len);
    f32 setLength(f32 len);

    static const Vector3f zero;
    static const Vector3f ex;
    static const Vector3f ey;
    static const Vector3f ez;
};

struct Vector2f : nw4r::math::VEC2 {
    Vector2f() {}
    Vector2f(f32 fx, f32 fy) : VEC2(fx, fy) {}
    ~Vector2f() {}

public:
    static const Vector2f zero;
    static const Vector2f ex;
    static const Vector2f ey;

    Vector2f operator-(const Vector2f &v) { return Vector2f(x - v.x, y - v.y); }
    f32 squaredLength() const { return (x * x + y * y); }
};

struct Vector3s {
    s16 x, y, z;

public:
    Vector3s() {}
    Vector3s(s16 sx, s16 sy, s16 sz) {
        x = sx;
        y = sy;
        z = sz;
    }
    static const Vector3s zero;
    static const Vector3s ex;
    static const Vector3s ey;
    static const Vector3s ez;
};

} // namespace EGG

#endif
