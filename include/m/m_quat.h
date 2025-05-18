#ifndef M_QUAT_H
#define M_QUAT_H

#include "egg/math/eggQuat.h"
#include "m/m_vec.h"

class mQuat_c : public EGG::Quatf {
public:
    mQuat_c() {}
    mQuat_c(f32 x, f32 y, f32 z, f32 w) : EGG::Quatf(w, x, y, z) {}
    mQuat_c(const mQuat_c &other) : EGG::Quatf(other.w, other.v) {}
    mQuat_c(const mVec3_c &v, f32 w) : EGG::Quatf(w, v) {}

    mQuat_c &operator=(const EGG::Quatf &rhs) {
        v = rhs.v;
        w = rhs.w;
        return *this;
    }

    mQuat_c &operator=(const mQuat_c &rhs) {
        v = rhs.v;
        w = rhs.w;
        return *this;
    }

    bool Set(const mVec3_c &, const mVec3_c &);
    void fn_802F2780(const mQuat_c &other);
    bool fn_802F2450(const mVec3_c &, const mVec3_c &, f32);
};

#endif
