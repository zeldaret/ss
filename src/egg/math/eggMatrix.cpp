#include "egg/math/eggMatrix.h"

#include "rvl/GX.h"  // IWYU pragma: export
#include "rvl/MTX.h" // IWYU pragma: export

namespace EGG {

void Matrix34f::inverseTo(Matrix34f &to) const {
    MTXInverse(m, to.m);
}

void Matrix34f::inverseTransposeTo(Matrix34f &to) const {
    MTXInvXpose(m, to.m);
}

void Matrix34f::makeIdentity() {
    makeZero();
    m[2][2] = 1.0f;
    m[1][1] = 1.0f;
    m[0][0] = 1.0f;
} // namespace EGG

Matrix34f::Matrix34f(f32 xx, f32 xy, f32 xz, f32 xw, f32 yx, f32 yy, f32 yz, f32 yw, f32 zx, f32 zy, f32 zz, f32 zw) {
    m[0][0] = xx;
    m[0][1] = xy;
    m[0][2] = xz;
    m[0][3] = xw;
    m[1][0] = yx;
    m[1][1] = yy;
    m[1][2] = yz;
    m[1][3] = yw;
    m[2][0] = zx;
    m[2][1] = zy;
    m[2][2] = zz;
    m[2][3] = zw;
}

void Matrix34f::makeQT(const Quatf &q, const Vector3f &t) {
    f32 yy = 2.0f * q.v.y * q.v.y;
    f32 zz = 2.0f * q.v.z * q.v.z;
    f32 xx = 2.0f * q.v.x * q.v.x;
    f32 xy = 2.0f * q.v.x * q.v.y;
    f32 xz = 2.0f * q.v.x * q.v.z;
    f32 yz = 2.0f * q.v.y * q.v.z;
    f32 wz = 2.0f * q.w * q.v.z;
    f32 wx = 2.0f * q.w * q.v.x;
    f32 wy = 2.0f * q.w * q.v.y;

    m[0][0] = 1.0f - yy - zz;
    m[0][1] = xy - wz;
    m[0][2] = xz + wy;

    m[1][0] = xy + wz;
    m[1][1] = 1.0f - xx - zz;
    m[1][2] = yz - wx;

    m[2][0] = xz - wy;
    m[2][1] = yz + wx;
    m[2][2] = 1 - xx - yy;

    m[0][3] = t.x;
    m[1][3] = t.y;
    m[2][3] = t.z;
}

void Matrix34f::makeQ(const Quatf &q) {
    f32 yy = 2.0f * q.v.y * q.v.y;
    f32 zz = 2.0f * q.v.z * q.v.z;
    f32 xx = 2.0f * q.v.x * q.v.x;
    f32 xy = 2.0f * q.v.x * q.v.y;
    f32 xz = 2.0f * q.v.x * q.v.z;
    f32 yz = 2.0f * q.v.y * q.v.z;
    f32 wz = 2.0f * q.w * q.v.z;
    f32 wx = 2.0f * q.w * q.v.x;
    f32 wy = 2.0f * q.w * q.v.y;

    m[0][0] = 1.0f - yy - zz;
    m[0][1] = xy - wz;
    m[0][2] = xz + wy;

    m[1][0] = xy + wz;
    m[1][1] = 1.0f - xx - zz;
    m[1][2] = yz - wx;

    m[2][0] = xz - wy;
    m[2][1] = yz + wx;
    m[2][2] = 1.0f - xx - yy;

    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
}

static inline f32 getQuatW(const Quatf &q) { return q.w; }
static inline f32 getQuatX(const Quatf &q) { return q.v.x; }
static inline f32 getQuatY(const Quatf &q) { return q.v.y; }
static inline f32 getQuatZ(const Quatf &q) { return q.v.z; }

void Matrix34f::fromQuat(const Quatf &q) {
    m[0][0] = 1.0f - (2 * q.v.y * q.v.y) - (2.0f * q.v.z * q.v.z);
    m[0][1] = (2.0f * q.v.x * q.v.y) - (2.0f * getQuatW(q) * getQuatZ(q));
    m[0][2] = (2.0f * q.v.x * q.v.z) + (2.0f * getQuatW(q) * getQuatY(q));

    m[1][0] = (2.0f * q.v.x * q.v.y) + (2.0f * getQuatW(q) * getQuatZ(q));
    m[1][1] = 1.0f - (2.0f * q.v.x * q.v.x) - (2.0f * q.v.z * q.v.z);
    m[1][2] = (2.0f * q.v.y * q.v.z) - (2.0f * getQuatW(q) * getQuatX(q));

    m[2][0] = (2.0f * q.v.x * q.v.z) - (2.0f * getQuatW(q) * getQuatY(q));
    m[2][1] = (2.0f * q.v.y * q.v.z) + (2.0f * getQuatW(q) * getQuatX(q));
    m[2][2] = 1.0f - (2.0f * q.v.x * q.v.x) - (2.0f * q.v.y * q.v.y);

    m[2][3] = 0.0f;
    m[1][3] = 0.0f;
    m[0][3] = 0.0f;
}

void Matrix34f::toQuat(Quatf &q) const {
    const f32 temp0 = (m[0][0] + m[1][1] + m[2][2] + 1.0f) * 0.25f;
    const f32 temp1 = temp0 - (m[1][1] + m[2][2]) * 0.5f;
    const f32 temp2 = temp0 - (m[2][2] + m[0][0]) * 0.5f;
    const f32 temp3 = temp0 - (m[0][0] + m[1][1]) * 0.5f;

    // The fun method
    // int tempMax = temp0 > temp1 ?
    //         (temp0 > temp2 ? (temp0 > temp3 ? 0 : 3) : (temp2 > temp3 ? 2 : 3)) :
    //         (temp1 > temp2 ? temp1 > temp3 ? 1 : 3 : (temp2 > temp3 ? 2 : 3));

    int tempMax;
    if (temp0 > temp1) {
        if (temp0 > temp2) {
            if (temp0 > temp3) {
                tempMax = 0;
            } else {
                tempMax = 3;
            }
        } else {
            if (temp2 > temp3) {
                tempMax = 2;
            } else {
                tempMax = 3;
            }
        }
    } else if (temp1 > temp2) {
        if (temp1 > temp3) {
            tempMax = 1;
        } else {
            tempMax = 3;
        }
    } else if (temp2 > temp3) {
        tempMax = 2;
    } else {
        tempMax = 3;
    }

    switch (tempMax) {
        case 0:
            q.w = Math<f32>::sqrt(temp0);
            q.v.x = (0.25f / q.w) * (m[2][1] - m[1][2]);
            q.v.y = (0.25f / q.w) * (m[0][2] - m[2][0]);
            q.v.z = (0.25f / q.w) * (m[1][0] - m[0][1]);
            break;
        case 1:
            q.v.x = Math<f32>::sqrt(temp1);
            q.w = (0.25f / q.v.x) * (m[2][1] - m[1][2]);
            q.v.y = (0.25f / q.v.x) * (m[0][1] + m[1][0]);
            q.v.z = (0.25f / q.v.x) * (m[0][2] + m[2][0]);
            break;
        case 2:
            q.v.y = Math<f32>::sqrt(temp2);
            q.w = (0.25f / q.v.y) * (m[0][2] - m[2][0]);
            q.v.z = (0.25f / q.v.y) * (m[1][2] + m[2][1]);
            q.v.x = (0.25f / q.v.y) * (m[1][0] + m[0][1]);
            break;
        case 3:
            q.v.z = Math<f32>::sqrt(temp3);
            q.w = (0.25f / q.v.z) * (m[1][0] - m[0][1]);
            q.v.x = (0.25f / q.v.z) * (m[2][0] + m[0][2]);
            q.v.y = (0.25f / q.v.z) * (m[2][1] + m[1][2]);
            break;
        default: break;
    }

    if (q.w < 0.0f) {
        q.w = -q.w;
        q.v.x = -q.v.x;
        q.v.y = -q.v.y;
        q.v.z = -q.v.z;
    }
    q.multScalar(Math<f32>::inv(Math<f32>::sqrt(q.w * q.w + q.v.x * q.v.x + q.v.y * q.v.y + q.v.z * q.v.z)));
}

void Matrix34f::slerpTo(const Matrix34f &m2, Matrix34f &out, f32 t) const {
    Quatf q1, q2, q3;
    m2.toQuat(q1);
    toQuat(q2);
    q2.slerpTo(q1, t, q3);
    out.makeQ(q3);
}

void Matrix34f::setAxisRotation(const Vector3f &axis, f32 rot) {
    Quatf q;
    q.setAxisRotation(axis, rot);
    makeQ(q);
}

void Matrix34f::loadPosMtx(u32 posMtxId) {
    GXLoadPosMtxImm(m, posMtxId);
}

void Matrix34f::multiplyTo(const Matrix34f &m2, Matrix34f &to) const {
    MTXConcat(m, m2.m, to.m);
}

Matrix34f Matrix34f::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

} // namespace EGG
