#ifndef NW4R_MATH_TYPES_H
#define NW4R_MATH_TYPES_H
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_triangular.h"
#include "nw4r/types_nw4r.h"
#include "rvl/MTX.h" // IWYU pragma: export

namespace nw4r {
namespace math {

/******************************************************************************
 *
 * VEC2 structure
 *
 ******************************************************************************/
// Optimization: Forces copy by lwz/stw
struct _VEC2 {
    f32 x, y;
};

struct VEC2 : _VEC2 {
    VEC2() {}
    VEC2(f32 fx, f32 fy) {
        x = fx;
        y = fy;
    }

    operator f32 *() {
        return reinterpret_cast<f32 *>(this);
    }
    operator const f32 *() const {
        return reinterpret_cast<const f32 *>(this);
    }

    VEC2 operator+(const VEC2 &rhs) const {
        return VEC2(x + rhs.x, y + rhs.y);
    }
    VEC2 operator-(const VEC2 &rhs) const {
        return VEC2(x - rhs.x, y - rhs.y);
    }

    VEC2 &operator+=(const VEC2 &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    VEC2 &operator-=(const VEC2 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    bool operator==(const VEC2 &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const VEC2 &rhs) const {
        return x != rhs.x || y != rhs.y;
    }
};

/******************************************************************************
 *
 * VEC3 structure
 *
 ******************************************************************************/
// Forward declarations
VEC3 *VEC3Add(VEC3 *out, const VEC3 *a, const VEC3 *b);
VEC3 *VEC3Sub(VEC3 *out, const VEC3 *a, const VEC3 *b);
VEC3 *VEC3Scale(VEC3 *out, const VEC3 *in, f32 scale);

// Optimization: Forces copy by lwz/stw
struct _VEC3 {
    f32 x, y, z;
};

struct VEC3 : _VEC3 {
    VEC3() {}
    VEC3(f32 fx, f32 fy, f32 fz) {
        x = fx;
        y = fy;
        z = fz;
    }
    VEC3(const Vec &vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }
    VEC3(const f32 *p) {
        x = p[0];
        y = p[1];
        z = p[2];
    }

    operator Vec *() {
        return reinterpret_cast<Vec *>(this);
    }
    operator const Vec *() const {
        return reinterpret_cast<const Vec *>(this);
    }

    f32 LenSq() const {
        return x * x + y * y + z * z;
    }

    VEC3 operator-() const {
        return VEC3(-x, -y, -z);
    }

    VEC3 operator+(const VEC3 &rhs) const {
        VEC3 out;
        VEC3Add(&out, this, &rhs);
        return out;
    }
    VEC3 operator-(const VEC3 &rhs) const {
        VEC3 out;
        VEC3Sub(&out, this, &rhs);
        return out;
    }
    VEC3 operator*(f32 x) const {
        VEC3 out;
        VEC3Scale(&out, this, x);
        return out;
    }
    VEC3 operator/(f32 x) const {
        f32 r = 1 / x;
        return *this * r;
    }

    VEC3 &operator+=(const VEC3 &rhs) {
        VEC3Add(this, this, &rhs);
        return *this;
    }
    VEC3 &operator-=(const VEC3 &rhs) {
        VEC3Sub(this, this, &rhs);
        return *this;
    }
    VEC3 &operator*=(f32 x) {
        VEC3Scale(this, this, x);
        return *this;
    }
    VEC3 &operator/=(f32 x) {
        return *this *= (1 / x);
    }

    bool operator==(const VEC3 &rhs) const {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    bool operator!=(const VEC3 &rhs) const {
        return x != rhs.x || y != rhs.y || z != rhs.z;
    }
};

/******************************************************************************
 *
 * MTX33 structure
 *
 ******************************************************************************/
// Optimization: Forces copy by lwz/stw
struct _MTX33 {
    union {
        struct {
            f32 _00, _01, _02;
            f32 _10, _11, _12;
            f32 _20, _21, _22;
        };

        f32 m[3][3];
        f32 a[3 * 3];
    };
};

struct MTX33 : _MTX33 {
    MTX33() {}
};

/******************************************************************************
 *
 * MTX34 structure
 *
 ******************************************************************************/
// Optimization: Forces copy by lwz/stw
struct _MTX34 {
    union {
        struct {
            f32 _00, _01, _02, _03;
            f32 _10, _11, _12, _13;
            f32 _20, _21, _22, _23;
        };

        f32 m[3][4];
        f32 a[3 * 4];
        Mtx mtx;
    };
};

struct MTX34 : _MTX34 {
    typedef f32 (*MtxRef)[4];
    typedef const f32 (*MtxRefConst)[4];

    MTX34() {}

    // clang-format off
    MTX34(f32 f00, f32 f01, f32 f02, f32 f03,
          f32 f10, f32 f11, f32 f12, f32 f13,
          f32 f20, f32 f21, f32 f22, f32 f23) {
        _00 = f00; _01 = f01; _02 = f02; _03 = f03;
        _10 = f10; _11 = f11; _12 = f12; _13 = f13;
        _20 = f20; _21 = f21; _22 = f22; _23 = f23;
    }
    // clang-format on

    inline operator MtxRef() {
        return mtx;
    }
    inline operator MtxRefConst() const {
        return mtx;
    }
};

/******************************************************************************
 *
 * MTX44 structure
 *
 ******************************************************************************/
// Optimization: Forces copy by lwz/stw
struct _MTX44 {
    union {
        struct {
            f32 _00, _01, _02, _03;
            f32 _10, _11, _12, _13;
            f32 _20, _21, _22, _23;
            f32 _30, _31, _32, _33;
        };

        f32 m[4][4];
        f32 a[4 * 4];
        Mtx44 mtx;
    };
};

struct MTX44 : _MTX44 {
    typedef f32 (*Mtx44Ref)[4];
    typedef const f32 (*Mtx44RefConst)[4];

    MTX44() {}

    operator Mtx44Ref() {
        return mtx;
    }
    operator Mtx44RefConst() const {
        return mtx;
    }
};

/******************************************************************************
 *
 * QUAT structure
 *
 ******************************************************************************/
// Optimization: Forces copy by lwz/stw
struct _QUAT {
    f32 x, y, z, w;
};

struct QUAT : _QUAT {
    QUAT() {}
    QUAT(f32 fx, f32 fy, f32 fz, f32 fw) {
        x = fx;
        y = fy;
        z = fz;
        w = fw;
    }

    // These are not real AFAIK. Do they really manually cast the QUAT?
    operator Quaternion *() {
        return reinterpret_cast<Quaternion *>(this);
    }
    operator const Quaternion *() const {
        return reinterpret_cast<const Quaternion *>(this);
    }
};

/******************************************************************************
 *
 * VEC2 functions
 *
 ******************************************************************************/
inline f32 VEC2Len(const VEC2 *vec) {
    return FSqrt(vec->x * vec->x + vec->y * vec->y);
}

/******************************************************************************
 *
 * VEC3 functions
 *
 ******************************************************************************/
VEC3 *VEC3Maximize(VEC3 *out, const VEC3 *a, const VEC3 *b);
VEC3 *VEC3Minimize(VEC3 *out, const VEC3 *a, const VEC3 *b);
VEC3 *VEC3TransformNormal(VEC3 *out, const MTX34 *mtx, const VEC3 *vec);

inline VEC3 *VEC3Add(register VEC3 *out, register const VEC3 *a, register const VEC3 *b) {
    register f32 work0, work1, work2;

    // clang-format off
    asm {
        // Add XY
        psq_l  work0, VEC3.x(a),   0, 0
        psq_l  work1, VEC3.x(b),   0, 0
        ps_add work2, work0, work1
        psq_st work2, VEC3.x(out), 0, 0
        
        // Add Z
        psq_l  work0, VEC3.z(a),   1, 0
        psq_l  work1, VEC3.z(b),   1, 0
        ps_add work2, work0, work1
        psq_st work2, VEC3.z(out), 1, 0
    }
    // clang-format on

    return out;
}

inline f32 VEC3Dot(register const VEC3 *a, register const VEC3 *b) {
    register f32 dot;
    register f32 work0, work1, work2, work3;

    // clang-format off
    asm {
        // YZ product
        psq_l  work0, VEC3.y(a), 0, 0
        psq_l  work1, VEC3.y(b), 0, 0
        ps_mul work0, work0, work1
        
        // X product + YZ product
        psq_l   work3, VEC3.x(a), 1, 0
        psq_l   work2, VEC3.x(b), 1, 0
        ps_madd work1, work3, work2, work0
        
        // Dot product
        ps_sum0 dot, work1, work0, work0
    }
    // clang-format on

    return dot;
}

inline f32 VEC3LenSq(register const VEC3 *vec) {
    register f32 work0, work1, work2;

    // clang-format off
    asm {
        // Square XY
        psq_l  work0, VEC3.x(vec), 0, 0
        ps_mul work0, work0, work0

        // Square Z
        lfs     work1, VEC3.z(vec)
        ps_madd work2, work1, work1, work0

        // Sum products
        ps_sum0 work2, work2, work0, work0
    }
    // clang-format on

    return work2;
}

inline VEC3 *VEC3Lerp(register VEC3 *out, register const VEC3 *vec1, register const VEC3 *vec2, register f32 t) {
    register f32 work0, work1, work2;

    // clang-format off
    asm {
        // X/Y delta
        psq_l  work0, VEC3.x(vec1), 0, 0
        psq_l  work1, VEC3.x(vec2), 0, 0
        ps_sub work2, work1, work0
        // Scale with time and add to v0
        ps_madds0 work2, work2, t, work0
        psq_st    work2, VEC3.x(out), 0, 0
        
        // Z delta
        psq_l  work0, VEC3.z(vec1), 1, 0
        psq_l  work1, VEC3.z(vec2), 1, 0
        ps_sub work2, work1, work0
        // Scale with time and add to v0
        ps_madds0 work2, work2, t, work0
        psq_st    work2, VEC3.z(out), 1, 0
    }
    // clang-format on

    return out;
}

inline VEC3 *VEC3Scale(register VEC3 *out, register const VEC3 *in, register f32 scale) {
    register f32 work0, work1;

    // clang-format off
    asm {
        // Scale XY
        psq_l    work0, VEC3.x(in),  0, 0
        ps_muls0 work1, work0, scale
        psq_st   work1, VEC3.x(out), 0, 0

        // Scale Z
        psq_l    work0, VEC3.z(in),  1, 0
        ps_muls0 work1, work0, scale
        psq_st   work1, VEC3.z(out), 1, 0
    }
    // clang-format on

    return out;
}

inline VEC3 *VEC3Sub(register VEC3 *out, register const VEC3 *a, register const VEC3 *b) {
    register f32 work0, work1, work2;

    // clang-format off
    asm {
        // Sub XY
        psq_l  work0, VEC3.x(a),   0, 0
        psq_l  work1, VEC3.x(b),   0, 0
        ps_sub work2, work0, work1
        psq_st work2, VEC3.x(out), 0, 0
        
        // Sub Z
        psq_l  work0, VEC3.z(a),   1, 0
        psq_l  work1, VEC3.z(b),   1, 0
        ps_sub work2, work0, work1
        psq_st work2, VEC3.z(out), 1, 0
    }
    // clang-format on

    return out;
}

inline VEC3 *VEC3Cross(VEC3 *out, const VEC3 *a, const VEC3 *b) {
    PSVECCrossProduct(*a, *b, *out);
    return out;
}

inline f32 VEC3DistSq(const VEC3 *a, const VEC3 *b) {
    return PSVECSquareDistance(*a, *b);
}

inline f32 VEC3Len(const VEC3 *vec) {
    return PSVECMag(*vec);
}

inline VEC3 *VEC3Normalize(VEC3 *out, const VEC3 *in) {
    PSVECNormalize(*in, *out);
    return out;
}

inline VEC3 *VEC3Transform(VEC3 *out, const MTX34 *mtx, const VEC3 *vec) {
    PSMTXMultVec(*mtx, *vec, *out);
    return out;
}

inline VEC3 *VEC3TransformCoord(VEC3 *out, const MTX34 *mtx, const VEC3 *vec) {
    PSMTXMultVec(*mtx, *vec, *out);
    return out;
}

/******************************************************************************
 *
 * MTX33 functions
 *
 ******************************************************************************/
MTX33 *MTX33Identity(MTX33 *mtx);

/******************************************************************************
 *
 * MTX34 functions
 *
 ******************************************************************************/
MTX33 *MTX34ToMTX33(MTX33 *out, const MTX34 *in);
bool MTX34InvTranspose(MTX33 *out, const MTX34 *in);
MTX34 *MTX34Zero(MTX34 *mtx);
MTX34 *MTX34Scale(MTX34 *out, const MTX34 *in, const VEC3 *scale);
MTX34 *MTX34Trans(MTX34 *out, const MTX34 *in, const VEC3 *trans);
MTX34 *MTX34RotAxisFIdx(MTX34 *mtx, const VEC3 *axis, f32 fidx);
MTX34 *MTX34RotXYZFIdx(MTX34 *mtx, f32 fx, f32 fy, f32 fz);

inline MTX34 *MTX34Copy(MTX34 *out, const MTX34 *in) {
    PSMTXCopy(*in, *out);
    return out;
}

inline MTX34 *MTX34Identity(MTX34 *mtx) {
    PSMTXIdentity(*mtx);
    return mtx;
}

inline MTX34 *MTX34Inv(MTX34 *out, const MTX34 *in) {
    PSMTXInverse(*in, *out);
    return out;
}

inline MTX34 *MTX34LookAt(MTX34 *mtx, const VEC3 *pos, const VEC3 *up, const VEC3 *target) {
    C_MTXLookAt(*mtx, *pos, *up, *target);
    return mtx;
}

inline MTX34 *MTX34Mult(MTX34 *out, const MTX34 *a, const MTX34 *b) {
    PSMTXConcat(*a, *b, *out);
    return out;
}

inline MTX34 *MTX34MultArray(MTX34 *out, const MTX34 *p1, const MTX34 *src, u32 len) {
    PSMTXConcatArray(*p1, *src, *out, len);
    return out;
}

inline MTX34 *MTX34RotAxisRad(MTX34 *out, const VEC3 *axis, f32 frad) {
    return MTX34RotAxisFIdx(out, axis, NW4R_MATH_RAD_TO_FIDX(frad));
}

inline MTX34 *MTX34RotXYZDeg(MTX34 *mtx, f32 dx, f32 dy, f32 dz) {
    return MTX34RotXYZFIdx(mtx, NW4R_MATH_DEG_TO_FIDX(dx), NW4R_MATH_DEG_TO_FIDX(dy), NW4R_MATH_DEG_TO_FIDX(dz));
}

inline MTX34 *MTX34RotXYZRad(MTX34 *mtx, f32 rx, f32 ry, f32 rz) {
    return MTX34RotXYZFIdx(mtx, NW4R_MATH_RAD_TO_FIDX(rx), NW4R_MATH_RAD_TO_FIDX(ry), NW4R_MATH_RAD_TO_FIDX(rz));
}

inline MTX34 *MTX34Scale(MTX34 *out, const VEC3 *scale, const MTX34 *in) {
    PSMTXScaleApply(*in, *out, scale->x, scale->y, scale->z);
    return out;
}

inline QUAT *MTX34ToQUAT(QUAT *quat, const MTX34 *mtx) {
    C_QUATMtx(*quat, *mtx);
    return quat;
}

inline MTX34 *MTX34Trans(MTX34 *out, const VEC3 *trans, const MTX34 *in) {
    PSMTXTransApply(*in, *out, trans->x, trans->y, trans->z);
    return out;
}

/******************************************************************************
 *
 * MTX44 functions
 *
 ******************************************************************************/
MTX44 *MTX44Identity(MTX44 *mtx);
MTX44 *MTX44Copy(MTX44 *dst, const MTX44 *src);

/******************************************************************************
 *
 * QUAT functions
 *
 ******************************************************************************/
inline MTX34 *QUATToMTX34(MTX34 *mtx, const QUAT *quat) {
    PSMTXQuat(*mtx, *quat);
    return mtx;
}

inline QUAT *C_QUATSlerp(QUAT *out, const QUAT *q1, const QUAT *q2, f32 t) {
    ::C_QUATSlerp(*q1, *q2, *out, t);
    return out;
}

} // namespace math
} // namespace nw4r

#endif
