#ifndef EGG_VECTOR_H
#define EGG_VECTOR_H

#include "common.h"
#include "egg/math/eggMath.h"
#include "nw4r/math/math_triangular.h"

#include "nw4r/math.h"

namespace EGG {

struct Vector3f : public nw4r::math::VEC3 {
    Vector3f() : nw4r::math::VEC3() {}
    Vector3f(f32 fx, f32 fy, f32 fz) : VEC3(fx, fy, fz) {}

    f32 &operator()(int i) {
        return ((f32 *)this)[i];
    }

    Vector3f operator*(f32 f) const {
        return Vector3f(x * f, y * f, z * f);
    }

    friend Vector3f operator*(f32 f, const Vector3f &v) {
        return Vector3f(v.x * f, v.y * f, v.z * f);
    }

    Vector3f operator+(const Vector3f &v) const {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }

    Vector3f &operator+=(const Vector3f &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    void mul(const Vector3f &v, const Vector3f &w) {
        set(v.x * w.x, v.y * w.y, v.z * w.z);
    }

    Vector3f &operator*=(const Vector3f &v) {
        set(x * v.x, y * v.y, z * v.z);
        return *this;
    }

    Vector3f operator-() const {
        f32 z = this->z;
        f32 y = this->y;
        f32 x = this->x;
        return Vector3f(-x, -y, -z);
    }

    Vector3f operator-(const Vector3f &v) const {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }

    Vector3f &operator-=(const Vector3f &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3f &operator*=(f32 f) {
        multScalar(f);
        return *this;
    }

    Vector3f operator/(f32 f) const {
        return Vector3f(x / f, y / f, z / f);
    }

    Vector3f &operator/=(f32 f) {
        divScalar(f);
        return *this;
    }

    bool operator!=(const Vector3f &v) {
        return x != v.x || y != v.y || z != v.z;
    }

    void multScalar(f32 f) {
        x *= f;
        y *= f;
        z *= f;
    }
    void divScalar(f32 f) {
        multScalar(1.0f / f);
    }

    f32 angle(const Vector3f &v) const {
        f32 a = dot(v);
        f32 b = cross(v).length();
        return EGG::Math<f32>::abs(EGG::Math<f32>::atan2(b, a));
    }

    f32 dot(const Vector3f &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3f cross(const Vector3f &b) const {
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

    Vector3f negated() {
        return Vector3f(-x, -y, -z);
    }

    void set(const EGG::Vector3f &other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    bool isZero() const {
        return squaredLength() <= Math<f32>::epsilon();
    }

    void setZero() {
        x = y = z = 0.0f;
    };

    f32 normalise();

    Vector3f normalize() {
        Vector3f other(x, y, z);
        other.normalise();
        return other;
    }

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

    void set(f32 fx, f32 fy) {
        x = fx;
        y = fy;
    }

    Vector2f operator-(const Vector2f &v) {
        return Vector2f(x - v.x, y - v.y);
    }
    f32 length() const {
        return Math<f32>::sqrt(squaredLength());
    }
    f32 squaredLength() const {
        return (x * x + y * y);
    }
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
