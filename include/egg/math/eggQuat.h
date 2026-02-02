#ifndef EGG_QUAT_H
#define EGG_QUAT_H

#include "common.h"
#include "egg/math/eggVector.h"

namespace EGG {

struct Quatf {
    Quatf() {}
    Quatf(f32 f, const Vector3f &v) : w(f), v(v) {}
    Quatf(f32 f, f32 x, f32 y, f32 z) : w(f), v(Vector3f(x, y, z)) {}
    ~Quatf() {}

    friend Quatf operator*(const Quatf &q, const Vector3f &vec) {
        Vector3f crossed = q.v.cross(vec);
        Vector3f scaled = vec * q.w;
        Quatf ret = Quatf(-q.v.dot(vec), crossed + scaled);
        return ret;
    }

    friend Quatf operator*(const Quatf &lhs, const Quatf &rhs) {
        Vector3f cross = lhs.v.cross(rhs.v);

        Vector3f scaledRhs = lhs.w * rhs.v;
        Vector3f tmp0 = cross + scaledRhs;

        Vector3f scaledLhs = rhs.w * lhs.v;
        Vector3f tmp1 = tmp0 + scaledLhs;

        return Quatf(lhs.w * rhs.w - lhs.v.dot(rhs.v), tmp1);
    }

    void set(f32 fw, f32 fx, f32 fy, f32 fz);
    void set(f32 f, const Vector3f &vec);      // not in SS
    void setRPY(const EGG::Vector3f &rpy);     // not in SS
    Vector3f calcRPY();                        // not in SS
    void setRPY(f32 roll, f32 pitch, f32 yaw); // not in SS
    void setAxisRotation(const Vector3f &, f32);
    f32 norm();
    void normalise();
    Quatf conjugate() const;
    Quatf inverse(); // not in SS
    Vector3f rotateVector(const Vector3f &);
    Vector3f rotateVectorInv(const Vector3f &); // not in SS
    void slerpTo(const Quatf &, f32, Quatf &out) const;
    void slerpTo2(f32 fparam, const Quatf &param1, Quatf &out) const {
        const Quatf &tmp = param1;
        slerpTo(tmp, fparam, out);
    }
    void limitSlerpTo(const Quatf &, f32, f32, Quatf &out) const;
    void makeVectorRotationLimit(Vector3f &, Vector3f &, f32); // not in SS
    void makeVectorRotation(const Vector3f &, const Vector3f &);

    // not sure about the name
    f32 dot(const Quatf &other) const {
        f32 ret = v.dot(other.v);
        ret += w * other.w;
        return ret;
    }

    void set(const Quatf &other) {
        v.set(other.v);
        w = other.w;
    }

    void multScalar(f32 s) {
        w *= s;
        v.x *= s;
        v.y *= s;
        v.z *= s;
    }
    void setUnit() {
        set(1.0f, 0.0f, 0.0f, 0.0f);
    }

    void makeWPositive() {
        if (w < 0.f) {
            multScalar(-1.f);
        }
    }

    Vector3f v;
    f32 w;
};

} // namespace EGG

#endif
