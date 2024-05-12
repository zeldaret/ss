#include <egg/math/eggMatrix.h>
#include <rvl/GX.h>
#include <rvl/MTX.h>

namespace EGG {

void Matrix34f::inverseTo(Matrix34f &to) const {
    PSMTXInverse(m, to.m);
}

void Matrix34f::inverseTransposeTo(Matrix34f &to) const {
    PSMTXInvXpose(m, to.m);
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

void Matrix34f::makeSRT(const Vector3f &s, const Vector3f &r, const Vector3f &t) {
    const f32 sin[3] = {Math<f32>::sin(r.x), Math<f32>::sin(r.y), Math<f32>::sin(r.z)};
    const f32 cos[3] = {Math<f32>::cos(r.x), Math<f32>::cos(r.y), Math<f32>::cos(r.z)};

    const f32 c0_c2 = cos[0] * cos[2];
    const f32 s0_s1 = sin[0] * sin[1];
    const f32 c0_s2 = cos[0] * sin[2];

    m[0][0] = s.x * (cos[1] * cos[2]);
    m[1][0] = s.x * (cos[1] * sin[2]);
    m[2][0] = s.x * (-sin[1]);

    m[0][1] = s.y * ((s0_s1 * cos[2]) - c0_s2);
    m[1][1] = s.y * ((s0_s1 * sin[2]) + c0_c2);
    m[2][1] = s.y * (sin[0] * cos[1]);

    m[0][2] = s.z * ((c0_c2 * sin[1]) + (sin[0] * sin[2]));
    m[1][2] = s.z * ((c0_s2 * sin[1]) - (sin[0] * cos[2]));
    m[2][2] = s.z * (cos[0] * cos[1]);

    m[0][3] = t.x;
    m[1][3] = t.y;
    m[2][3] = t.z;
}

void Matrix34f::makeRT(const Vector3f &r, const Vector3f &t) {
    const f32 sin[3] = {Math<f32>::sin(r.x), Math<f32>::sin(r.y), Math<f32>::sin(r.z)};
    const f32 cos[3] = {Math<f32>::cos(r.x), Math<f32>::cos(r.y), Math<f32>::cos(r.z)};

    const f32 c0_c2 = cos[0] * cos[2];
    const f32 s0_s1 = sin[0] * sin[1];
    const f32 c0_s2 = cos[0] * sin[2];

    m[0][0] = (cos[1] * cos[2]);
    m[1][0] = (cos[1] * sin[2]);
    m[2][0] = (-sin[1]);

    m[0][1] = (s0_s1 * cos[2]) - c0_s2;
    m[1][1] = (s0_s1 * sin[2]) + c0_c2;
    m[2][1] = (sin[0] * cos[1]);

    m[0][2] = (c0_c2 * sin[1]) + (sin[0] * sin[2]);
    m[1][2] = (c0_s2 * sin[1]) - (sin[0] * cos[2]);
    m[2][2] = (cos[0] * cos[1]);

    m[0][3] = t.x;
    m[1][3] = t.y;
    m[2][3] = t.z;
}

void Matrix34f::makeR(const Vector3f &r) {
    const f32 sin[3] = {Math<f32>::sin(r.x), Math<f32>::sin(r.y), Math<f32>::sin(r.z)};
    const f32 cos[3] = {Math<f32>::cos(r.x), Math<f32>::cos(r.y), Math<f32>::cos(r.z)};

    const f32 c0_c2 = cos[0] * cos[2];
    const f32 s0_s1 = sin[0] * sin[1];
    const f32 c0_s2 = cos[0] * sin[2];

    m[0][0] = (cos[1] * cos[2]);
    m[1][0] = (cos[1] * sin[2]);
    m[2][0] = (-sin[1]);

    m[0][1] = (s0_s1 * cos[2]) - c0_s2;
    m[1][1] = (s0_s1 * sin[2]) + c0_c2;
    m[2][1] = (sin[0] * cos[1]);

    m[0][2] = (c0_c2 * sin[1]) + (sin[0] * sin[2]);
    m[1][2] = (c0_s2 * sin[1]) - (sin[0] * cos[2]);
    m[2][2] = (cos[0] * cos[1]);

    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
}

void Matrix34f::makeST(const Vector3f &s, const Vector3f &t) {
    m[0][0] = s.x;
    m[1][0] = 0.0f;
    m[2][0] = 0.0f;
    m[0][1] = 0.0f;
    m[1][1] = s.y;
    m[2][1] = 0.0f;
    m[0][2] = 0.0f;
    m[1][2] = 0.0f;
    m[2][2] = s.z;
    m[0][3] = t.x;
    m[1][3] = t.y;
    m[2][3] = t.z;
}

void Matrix34f::makeSQT(const Vector3f &s, const Quatf &q, const Vector3f &t) {
    f32 yy = 2.0f * q.y * q.y;
    f32 zz = 2.0f * q.z * q.z;
    f32 xx = 2.0f * q.x * q.x;
    f32 xy = 2.0f * q.x * q.y;
    f32 xz = 2.0f * q.x * q.z;
    f32 yz = 2.0f * q.y * q.z;
    f32 wz = 2.0f * q.w * q.z;
    f32 wx = 2.0f * q.w * q.x;
    f32 wy = 2.0f * q.w * q.y;

    m[0][0] = s.x * (1.0f - yy - zz);
    m[0][1] = s.y * (xy - wz);
    m[0][2] = s.z * (xz + wy);

    m[1][0] = s.x * (xy + wz);
    m[1][1] = s.y * (1.0f - xx - zz);
    m[1][2] = s.z * (yz - wx);

    m[2][0] = s.x * (xz - wy);
    m[2][1] = s.y * (yz + wx);
    m[2][2] = s.z * (1.0f - xx - yy);

    m[0][3] = t.x;
    m[1][3] = t.y;
    m[2][3] = t.z;
}

void Matrix34f::makeQT(const Quatf &q, const Vector3f &t) {
    f32 yy = 2.0f * q.y * q.y;
    f32 zz = 2.0f * q.z * q.z;
    f32 xx = 2.0f * q.x * q.x;
    f32 xy = 2.0f * q.x * q.y;
    f32 xz = 2.0f * q.x * q.z;
    f32 yz = 2.0f * q.y * q.z;
    f32 wz = 2.0f * q.w * q.z;
    f32 wx = 2.0f * q.w * q.x;
    f32 wy = 2.0f * q.w * q.y;

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
    f32 yy = 2.0f * q.y * q.y;
    f32 zz = 2.0f * q.z * q.z;
    f32 xx = 2.0f * q.x * q.x;
    f32 xy = 2.0f * q.x * q.y;
    f32 xz = 2.0f * q.x * q.z;
    f32 yz = 2.0f * q.y * q.z;
    f32 wz = 2.0f * q.w * q.z;
    f32 wx = 2.0f * q.w * q.x;
    f32 wy = 2.0f * q.w * q.y;

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

void Matrix34f::makeS(const Vector3f &s) {
    m[0][0] = s.x;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = s.y;
    m[1][2] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = s.z;
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
}

void Matrix34f::makeT(const Vector3f &t) {
    m[0][0] = 1.0f;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[0][3] = t.x;
    m[1][3] = t.y;
    m[2][3] = t.z;
}

void Matrix34f::fromQuat(const Quatf &q) {
    m[0][0] = 1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z);
    m[0][1] = (2.0f * q.x * q.y) - (2.0f * q.w * q.z);
    m[0][2] = (2.0f * q.x * q.z) + (2.0f * q.w * q.y);

    m[1][0] = (2.0f * q.x * q.y) + (2.0f * q.w * q.z);
    m[1][1] = 1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z);
    m[1][2] = (2.0f * q.y * q.z) - (2.0f * q.w * q.x);

    m[2][0] = (2.0f * q.x * q.z) - (2.0f * q.w * q.y);
    m[2][1] = (2.0f * q.y * q.z) + (2.0f * q.w * q.x);
    m[2][2] = 1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y);

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
        q.x = (0.25f / q.w) * (m[2][1] - m[1][2]);
        q.y = (0.25f / q.w) * (m[0][2] - m[2][0]);
        q.z = (0.25f / q.w) * (m[1][0] - m[0][1]);
        break;
    case 1:
        q.x = Math<f32>::sqrt(temp1);
        q.w = (0.25f / q.x) * (m[2][1] - m[1][2]);
        q.y = (0.25f / q.x) * (m[0][1] + m[1][0]);
        q.z = (0.25f / q.x) * (m[0][2] + m[2][0]);
        break;
    case 2:
        q.y = Math<f32>::sqrt(temp2);
        q.w = (0.25f / q.y) * (m[0][2] - m[2][0]);
        q.z = (0.25f / q.y) * (m[1][2] + m[2][1]);
        q.x = (0.25f / q.y) * (m[1][0] + m[0][1]);
        break;
    case 3:
        q.z = Math<f32>::sqrt(temp3);
        q.w = (0.25f / q.z) * (m[1][0] - m[0][1]);
        q.x = (0.25f / q.z) * (m[2][0] + m[0][2]);
        q.y = (0.25f / q.z) * (m[2][1] + m[1][2]);
        break;
    default:
        break;
    }

    if (q.w < 0.0f) {
        q.w = -q.w;
        q.x = -q.x;
        q.y = -q.y;
        q.z = -q.z;
    }
    q.multScalar(Math<f32>::inv(q.length()));
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

Vector3f Matrix34f::multVector(const Vector3f &vec) const {
    Vector3f ret;
    multVectorTo(vec, ret);
    return ret;
}

void Matrix34f::loadPosMtx(u32 posMtxId) {
    GXLoadPosMtxImm(m, posMtxId);
}

void Matrix34f::multiplyTo(const Matrix34f &m2, Matrix34f &to) const {
    PSMTXConcat(m, m2.m, to.m);
}

void Matrix34f::dump() {}

Matrix34f Matrix34f::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

} // namespace EGG
