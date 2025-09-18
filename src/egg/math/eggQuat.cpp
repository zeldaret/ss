#include "egg/math/eggQuat.h"

namespace EGG {

void Quatf::set(f32 fw, f32 fx, f32 fy, f32 fz) {
    w = fw;
    v.x = fx;
    v.y = fy;
    v.z = fz;
}

void Quatf::set(f32 fw, const Vector3f &vec) {
    w = fw;
    (Vector3f &)*this = vec;
}

/* NOT IN SS */
void Quatf::setRPY(const EGG::Vector3f &rpy) {
    const f32 cy = Math<f32>::cos(rpy.z * 0.5f);
    const f32 cp = Math<f32>::cos(rpy.y * 0.5f);
    const f32 cr = Math<f32>::cos(rpy.x * 0.5f);
    const f32 sy = Math<f32>::sin(rpy.z * 0.5f);
    const f32 sp = Math<f32>::sin(rpy.y * 0.5f);
    const f32 sr = Math<f32>::sin(rpy.x * 0.5f);

    const f32 cy_cp = cy * cp;
    const f32 sy_sp = sy * sp;
    const f32 cy_sp = cy * sp;
    const f32 sy_cp = sy * cp;

    w = (cy_cp * cr) + (sy_sp * sr);
    v.x = (cy_cp * sr) - (sy_sp * cr);
    v.y = (cy_sp * cr) + (sy_cp * sr);
    v.z = (sy_cp * cr) - (cy_sp * sr);
}

/* NOT IN SS */
// Vector3f Quatf::calcRPY() {
//     Vector3f rpy;

// const f32 ww = w * w;
// const f32 yy = v.y * v.y;
// const f32 xx = v.x * v.x;
// const f32 zz = v.z * v.z;

// const f32 a0 = (ww + xx) - yy - zz;
// const f32 a1 = (v.x * v.y + w * v.z) * 2.0f;
// const f32 a2 = (v.x * v.z - w * v.y) * 2.0f;
// const f32 a3 = (v.y * v.z + w * v.x) * 2.0f;
// const f32 a4 = (ww - xx) - yy + zz;

// const f32 a5 = Math<f32>::abs(a2);

// if (a5 > 0.999999f) {
//     f32 t0 = (v.x * v.y - w * v.z) * 2.0f;
//     f32 t1 = (v.x * v.z + w * v.y) * 2.0f;

// rpy.x = 0.0f;
// rpy.y = a2 / a5 * -Math<f32>::pi_half();
// rpy.z = Math<f32>::atan2(-t0, -a2 * t1);
// } else {
// rpy.x = Math<f32>::atan2(a3, a4);
// rpy.y = Math<f32>::asin(-a2);
// rpy.z = Math<f32>::atan2(a1, a0);
// }

// return rpy;
// }

/* NOT IN SS */
void Quatf::setRPY(f32 roll, f32 pitch, f32 yaw) {
    setRPY(Vector3f(roll, pitch, yaw));
}

void Quatf::setAxisRotation(const Vector3f &axis, f32 rot) {
    2.0f; // force sdata2 ordering
    const f32 half_angle = rot * 0.5f;
    const f32 cos = Math<f32>::cos(half_angle);
    const f32 sin = Math<f32>::sin(half_angle);
    set(cos, sin * axis.x, sin * axis.y, sin * axis.z);
}

f32 Quatf::norm() {
    return w * w + v.dot(v);
}

void Quatf::normalise() {
    f32 mag = Math<f32>::sqrt(norm());
    if (mag > 0.0f) {
        multScalar(Math<f32>::inv(mag));
    }
}

Quatf Quatf::conjugate() const {
    Quatf q;
    q.w = w;
    q.v = -1.0f * v;
    return q;
}

Vector3f Quatf::rotateVector(const Vector3f &vec) {
    Quatf conj, mult;
    conj = conjugate();
    mult = *this * vec;
    mult = mult * conj;
    return (mult.v);
}

// /* NOT IN SS */
// Quatf operator*(const Quatf &q, const Vector3f &v) {
//     Vector3f crossed = q.v.cross(v);
//     Vector3f scaled = q.w * v;
//     Quatf ret = Quatf(-q.v.dot(v), crossed + scaled);
//     return ret;
// }

void Quatf::slerpTo(const Quatf &q2, f32 t, Quatf &out) const {
    f32 dot = v.x * q2.v.x + v.y * q2.v.y + v.z * q2.v.z + w * q2.w;

    if (dot > 1.0f) {
        dot = 1.0f;
    } else if (dot < -1.0f) {
        dot = -1.0f;
    }

    bool lt0;
    if (dot < 0.0) {
        dot = -dot;
        lt0 = true;
    } else {
        lt0 = false;
    }

    const f32 theta_0 = Math<f32>::acos(dot);
    const f32 sin_theta_0 = Math<f32>::sin(theta_0);

    f32 a, b;
    if (Math<f32>::abs(sin_theta_0) < 1e-5f) {
        a = 1.0f - t;
        b = t;
    } else {
        const f32 sin_theta_0_inv = 1.0f / sin_theta_0;
        const f32 theta = t * theta_0;
        a = sin_theta_0_inv * Math<f32>::sin(theta_0 - theta);
        b = sin_theta_0_inv * Math<f32>::sin(theta);
    }

    if (lt0) {
        b = -b;
    }

    out.v.x = a * v.x + b * q2.v.x;
    out.v.y = a * v.y + b * q2.v.y;
    out.v.z = a * v.z + b * q2.v.z;
    out.w = a * w + b * q2.w;
}

void Quatf::limitSlerpTo(const Quatf &q2, f32 t, f32 t2, Quatf &out) const {
    t2 *= 0.5f;

    f32 dot = v.x * q2.v.x + v.y * q2.v.y + v.z * q2.v.z + w * q2.w;

    if (dot > 1.0f) {
        dot = 1.0f;
    } else if (dot < -1.0f) {
        dot = -1.0f;
    }

    bool lt0;
    if (dot < 0.0) {
        dot = -dot;
        lt0 = true;
    } else {
        lt0 = false;
    }

    const f32 theta_0 = Math<f32>::acos(dot);
    if (theta_0 > t2) {
        t = t2 / theta_0;
    }
    const f32 sin_theta_0 = Math<f32>::sin(theta_0);

    f32 a, b;

    if (Math<f32>::abs(sin_theta_0) < 1e-5f) {
        a = 1.0f - t;
        b = t;
    } else {
        const f32 sin_theta_0_inv = 1.0f / sin_theta_0;
        const f32 theta = t * theta_0;
        a = sin_theta_0_inv * Math<f32>::sin(theta_0 - theta);
        b = sin_theta_0_inv * Math<f32>::sin(theta);
    }

    if (lt0) {
        b = -b;
    }

    out.v.x = a * v.x + b * q2.v.x;
    out.v.y = a * v.y + b * q2.v.y;
    out.v.z = a * v.z + b * q2.v.z;
    out.w = a * w + b * q2.w;
}

void Quatf::makeVectorRotation(const Vector3f &from, const Vector3f &to) {
    Vector3f cross = from.cross(to);
    f32 t0 = (from.dot(to) + 1) * 2.0f;

    if (t0 < 0.0f) {
        t0 = 0.0f;
    }
    const f32 s = Math<f32>::sqrt(t0);

    if (s <= Math<f32>::epsilon()) {
        setUnit();
    } else {
        const f32 inv = 1.0f / s;
        set(s * 0.5f, cross.x * inv, cross.y * inv, cross.z * inv);
    }
}

} // namespace EGG
