#ifndef EGG_MATRIX_H
#define EGG_MATRIX_H

#include "egg/math/eggQuat.h"
#include "egg/math/eggVector.h"
#include <common.h>

namespace EGG {

struct Matrix34f {
    Matrix34f() {}
    /* 8049acd0 */
    Matrix34f(f32 xx, f32 xy, f32 xz, f32 xw, f32 yx, f32 yy, f32 yz, f32 yw, f32 zx, f32 zy, f32 zz, f32 zw);

    //////////////////////////////////////////////////////////////////////////////
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

    /*          */ void rotateBaseX(Vector3f &, Matrix34f &);
    /*          */ void rotateVectorChange(Vector3f &, Vector3f &, Matrix34f &);
    /* 8049ac70 */ void inverseTo(Matrix34f &to) const;
    /* 8049ac80 */ void inverseTransposeTo(Matrix34f &to) const;
    /* 8049ac90 */ void makeIdentity();
    /*          */ void makeSRT(const Vector3f &s, const Vector3f &r, const Vector3f &t);
    /*          */ void makeRT(const Vector3f &r, const Vector3f &t);
    /*          */ void makeR(const Vector3f &r);
    /*          */ void makeST(const Vector3f &s, const Vector3f &t);
    /*          */ void makeSQT(const Vector3f &s, const Quatf &q, const Vector3f &t);
    /*          */ void makeSQ(const Vector3f &, const Quatf &);
    /* 8049ad20 */ void makeQT(const Quatf &, const Vector3f &);
    /* 8049ade0 */ void makeQ(const Quatf &);
    /*          */ void makeS(const Vector3f &s);
    /*          */ void makeT(const Vector3f &t);
    /* 8049ae90 */ void fromQuat(const Quatf &q);
    /* 8049af40 */ void toQuat(Quatf &q) const;
    /* 8049b250 */ void slerpTo(const Matrix34f &, Matrix34f &, f32) const;
    /*          */ void copyTo16(f32 *pf) const;
    /* 8049b2d0 */ void setAxisRotation(const Vector3f &, f32);
    /*          */ Vector3f multVector(const Vector3f &vec) const;
    /* 8049b310 */ void loadPosMtx(u32);
    /*          */ void loadNrmMtx(u32 nrmMtxId);
    /* 8049b320 */ void multiplyTo(const Matrix34f &m2, Matrix34f &to) const;
    /*          */ void dump();

    //////////////////////////////////////////////////////////////////////////////
public:
    void concat(const Matrix34f &, Matrix34f &) const;
    void copyFrom(const Matrix34f &other);
    void rotate(const Vector3f &rpy) const;
    void multVectorTo(const Vector3f &from, Vector3f &to) const;
    void transposeTo(Matrix34f &to) const;
    void setBase(int idx, const Vector3f &b) {
        m[0][idx] = b.x;
        m[1][idx] = b.y;
        m[2][idx] = b.z;
    }
    void getBase(int idx, Vector3f &b) const {
        b.x = m[0][idx];
        b.y = m[1][idx];
        b.z = m[2][idx];
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

    //////////////////////////////////////////////////////////////////////////////
public:
    union {
        f32 m[3][4];
        f32 arr[12];
    };

    //////////////////////////////////////////////////////////////////////////////
public:
    /* 80674c00 */ static Matrix34f ident;
};

} // namespace EGG

#endif
