#ifndef M_VEC_H
#define M_VEC_H

#include "c/c_math.h"
#include "common.h"
#include "egg/math/eggMath.h"
#include "egg/math/eggVector.h"
#include "nw4r/math/math_types.h"
#include "nw4r/types_nw4r.h"
#include "rvl/MTX/vec.h"

class mAng;

class mVec3_c : public EGG::Vector3f {
public:
    /// @brief Constructs an empty vector.
    mVec3_c() {}
    ~mVec3_c() {}

    /// @brief Constructs a vector from a float array.
    mVec3_c(const f32 *p) {
        x = p[0];
        y = p[1];
        z = p[2];
    }

    mVec3_c(const mVec3_c &other) {
        set(other.x, other.y, other.z);
    }

    mVec3_c(f32 fx, f32 fy, f32 fz) {
        set(fx, fy, fz);
    }

    // ?? Sometimes Works
    void copyFrom(const Vec *pV) {
        set(pV->x, pV->y, pV->z);
    }

    /// @brief Assignment operator
    mVec3_c &operator=(const mVec3_c &r) {
        set(r.x, r.y, r.z);
        return *this;
    }

    /// @brief Assignment operator
    mVec3_c &operator=(const EGG::Vector3f &r) {
        x = r.x;
        y = r.y;
        z = r.z;
        return *this;
    }

    /// @brief Assignment operator
    mVec3_c &operator=(const nw4r::math::VEC3 &r) {
        x = r.x;
        y = r.y;
        z = r.z;
        return *this;
    }

    /// @brief Constructs a new vector from an existing vector from the MTX library.
    mVec3_c(const Vec &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    /// @brief Constructs a new vector from an existing vector from the nw4r::math library.
    mVec3_c(const nw4r::math::VEC3 &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    /// @brief Constructs a new vector from an existing vector from the EGG library.
    mVec3_c(const EGG::Vector3f &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    /// @brief Float cast operator.
    operator f32 *() {
        return &x;
    }

    /// @brief Const float cast operator.
    operator const f32 *() const {
        return &x;
    }

    /// @brief Vec cast operator.
    operator Vec *() {
        return (Vec *)&x;
    }

    /// @brief Vec cast operator.
    operator Vec() const {
        return *(Vec *)&x;
    }

    /// @brief Vector3f cast operator.
    operator EGG::Vector3f *() {
        return (EGG::Vector3f *)&x;
    }

    /// @brief Const Vec cast operator.
    operator const Vec *() const {
        return (const Vec *)&x;
    }

    /// @brief nw4r::math::VEC3 cast operator.
    operator nw4r::math::VEC3 *() {
        return (nw4r::math::VEC3 *)&x;
    }

    /// @brief Const nw4r::math::VEC3 cast operator.
    operator const nw4r::math::VEC3 *() const {
        return (const nw4r::math::VEC3 *)&x;
    }

    /// @brief Augmented subtraction operator.
    mVec3_c &operator+=(const mVec3_c &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    /// @brief Augmented subtraction operator.
    mVec3_c &operator-=(const mVec3_c &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    /// @brief Augmented scalar product operator.
    mVec3_c &operator*=(f32 f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    /// @brief Augmented scalar division operator.
    mVec3_c &operator/=(f32 f) {
        return operator*=(1.0f / f);
    }

    /// @brief Positive operator.
    mVec3_c operator+() const {
        return *this;
    }

    mVec3_c operator-(f32 f) const {
        return mVec3_c(x - f, y - f, z - f);
    }
    mVec3_c operator+(f32 f) const {
        return mVec3_c(x + f, y + f, z + f);
    }
    mVec3_c &operator-=(f32 f) {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    mVec3_c &operator+=(f32 f) {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    /// @brief Negative operator.
    mVec3_c operator-() const {
        return mVec3_c(-x, -y, -z);
    }

    /// @brief Addition operator.
    mVec3_c operator+(const mVec3_c &v) const {
        return mVec3_c(x + v.x, y + v.y, z + v.z);
    }

    /// @brief Subtraction operator.
    mVec3_c operator-(const mVec3_c &v) const {
        return mVec3_c(x - v.x, y - v.y, z - v.z);
    }

    /// @brief Scalar product operator.
    mVec3_c operator*(f32 f) const {
        return mVec3_c(x * f, y * f, z * f);
    }

    /// @brief Scalar division operator.
    mVec3_c operator/(f32 f) const {
        f32 r = 1.0f / f;
        return operator*(r);
    }

    /// @brief Equality operator.
    bool operator==(const mVec3_c &v) const {
        return x == v.x && y == v.y && z == v.z;
    }

    /// @brief Inequality operator.
    bool operator!=(const mVec3_c &v) const {
        return x != v.x || y != v.y || z != v.z;
    }

    void normalize();
    bool normalizeRS();

    inline f32 mag() const {
        return PSVECMag(*this);
    }

    f32 distance(const mVec3_c &to) const {
        return EGG::Math<f32>::sqrt(PSVECSquareDistance(*this, to));
    }
    f32 squareDistance(const mVec3_c &to) const {
        return VEC3DistSq(*this, to);
    }

    f32 squareMagXZ() const {
        return x * x + z * z;
    }
    void rotX(const mAng &angle);
    void rotY(const mAng &angle);
    void rotZ(const mAng &angle);

    void CopyTo(nw4r::math::VEC3 *p) {
        p->x = x;
        p->y = y;
        p->z = z;
    }

    s16 ang() const {
        return cM::atan2s(x * x, z * z);
    }

    f32 inprodXZ(const mVec3_c &other) const {
        return x * other.x + z * other.z;
    }

    static mVec3_c Zero;
    static mVec3_c Ex;
    static mVec3_c Ey;
    static mVec3_c Ez;
};

#endif
