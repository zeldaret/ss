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

    /* 8049b390 */ void set(f32 fw, f32 fx, f32 fy, f32 fz);
    /*          */ void set(f32 f, const Vector3f &vec);
    /*          */ void setRPY(const EGG::Vector3f &rpy);
    /*          */ Vector3f calcRPY();
    /*          */ void setRPY(f32 roll, f32 pitch, f32 yaw);
    /* 8049b3b0 */ void setAxisRotation(const Vector3f &, f32);
    /* 8049b450 */ f32 norm();
    /* 8049b480 */ void normalise();
    /* 8049b500 */ Quatf conjugate();
    /*          */ Quatf inverse();
    /* 8049b550 */ Vector3f rotateVector(const Vector3f &);
    /*          */ Vector3f rotateVectorInv(const Vector3f &);
    /* 8049b800 */ void slerpTo(const Quatf &, f32, Quatf &out) const;
    /* 8049b800 */ void limitSlerpTo(const Quatf &, f32, f32, Quatf &out) const;
    /*          */ void makeVectorRotationLimit(Vector3f &, Vector3f &, f32);
    /* 8049bbb0 */ void makeVectorRotation(Vector3f &, Vector3f &);

    void multScalar(f32 s) {
        w *= s;
        v.x *= s;
        v.y *= s;
        v.z *= s;
    }
    void setUnit() {
        set(1.0f, 0.0f, 0.0f, 0.0f);
    }

    Vector3f v;
    f32 w;
};

} // namespace EGG

#endif
