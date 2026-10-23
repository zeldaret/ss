#ifndef EGG_MATRIX_H
#define EGG_MATRIX_H

#include "common.h"
#include "egg/math/eggQuat.h"
#include "egg/math/eggVector.h"
#include "nw4r/math/math_types.h"

namespace EGG {

struct Matrix34f {
    Matrix34f() {}
    Matrix34f(f32 xx, f32 xy, f32 xz, f32 xw, f32 yx, f32 yy, f32 yz, f32 yw, f32 zx, f32 zy, f32 zz, f32 zw);

    f32 operator()(int i, int j) const {
        return m[i][j];
    }
    f32 &operator()(int i, int j) {
        return m[i][j];
    }
    f32 operator()(int i) const {
        return arr[i];
    }
    f32 &operator()(int i) {
        return arr[i];
    }

    Matrix34f &operator=(const Matrix34f &other) {
        copyFrom(other);
        return *this;
    }

    void rotateBaseX(Vector3f &, Matrix34f &);
    void rotateVectorChange(Vector3f &, Vector3f &, Matrix34f &);
    void inverseTo(Matrix34f &to) const;
    void inverseTransposeTo(Matrix34f &to) const;
    void makeIdentity();
    void makeSRT(const Vector3f &s, const Vector3f &r, const Vector3f &t) {
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
    void makeRT(const Vector3f &r, const Vector3f &t) {
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
    void makeR(const Vector3f &r) {
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
    void makeST(const Vector3f &s, const Vector3f &t) {
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
    void makeSQT(const Vector3f &s, const Quatf &q, const Vector3f &t) {
        f32 yy = 2.0f * q.v.y * q.v.y;
        f32 zz = 2.0f * q.v.z * q.v.z;
        f32 xx = 2.0f * q.v.x * q.v.x;
        f32 xy = 2.0f * q.v.x * q.v.y;
        f32 xz = 2.0f * q.v.x * q.v.z;
        f32 yz = 2.0f * q.v.y * q.v.z;
        f32 wz = 2.0f * q.w * q.v.z;
        f32 wx = 2.0f * q.w * q.v.x;
        f32 wy = 2.0f * q.w * q.v.y;
    
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
    void makeSQ(const Vector3f &, const Quatf &);
    void makeQT(const Quatf &, const Vector3f &);
    void makeQ(const Quatf &);
    void makeS(const Vector3f &s) {
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
    void makeT(const Vector3f &t) {
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
    void fromQuat(const Quatf &q);
    void toQuat(Quatf &q) const;
    void slerpTo(const Matrix34f &, Matrix34f &, f32) const;
    void copyTo16(f32 *pf) const;
    void setAxisRotation(const Vector3f &, f32);
    Vector3f multVector(const Vector3f &vec) const {
        Vector3f ret;
        multVectorTo(vec, ret);
        return ret;
    }
    void loadPosMtx(u32);
    void loadNrmMtx(u32 nrmMtxId);
    void multiplyTo(const Matrix34f &m2, Matrix34f &to) const;
    void dump() {}

    operator nw4r::math::MTX34 *() {
        return (nw4r::math::MTX34 *)this;
    }

    operator const nw4r::math::MTX34 *() const {
        return (nw4r::math::MTX34 *)this;
    }

public:
    void concat(const Matrix34f &, Matrix34f &) const;
    void rotate(const Vector3f &rpy) const;
    void multVectorTo(const Vector3f &from, Vector3f &to) const;
    void transposeTo(Matrix34f &to) const;
    void copyFrom(const Matrix34f &other) {
        for (int i = 0; i < 12; i++) {
            (*this)(i) = other(i);
        }
    }
    void setBase(int idx, const Vector3f &b) {
        (*this)(0, idx) = b.x;
        (*this)(1, idx) = b.y;
        (*this)(2, idx) = b.z;
    }

    void getBase(int idx, Vector3f &b) const {
        b(0) = (*this)(0, idx);
        b(1) = (*this)(1, idx);
        b(2) = (*this)(2, idx);
    }

    void setTranslation(const Vector3f &t) {
        setBase(3, t);
    }

    void getTranslation(Vector3f &t) const {
        getBase(3, t);
    }

    void makeZero() {
        for (int i = 0; i < 12; i++) {
            arr[i] = 0.0f;
        }
    }

public:
    union {
        f32 m[3][4];
        f32 arr[12];
    };

public:
    static Matrix34f ident;
};

} // namespace EGG

#endif
