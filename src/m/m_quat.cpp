#include "m/m_quat.h"

#include "c/c_math.h"
#include "common.h"
#include "egg/math/eggMath.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/ut/ut_algorithm.h"

bool mQuat_c::Set(const mVec3_c &from, const mVec3_c &to) {
    mVec3_c cross;
    cross.x = (from.y * to.z) - (from.z * to.y);
    cross.y = (from.z * to.x) - (from.x * to.z);
    cross.z = (from.x * to.y) - (from.y * to.x);
    f32 dot = (from.dot(to) + 1.0f) * 2.0f;

    if (dot < 0.0f) {
        dot = 0.0f;
    }
    f32 f = nw4r::math::FSqrt(dot);
    if (f <= EGG::Math<f32>::epsilon()) {
        return false;
    }

    f32 norm = (1.0f / f);
    set(f * 0.5f, cross.x * norm, cross.y * norm, cross.z * norm);
    return true;
}

bool mQuat_c::slerp(const mVec3_c &from, const mVec3_c &to, f32 interp) {
    f32 z = (from.x * to.y) - (from.y * to.x);
    f32 y = (from.z * to.x) - (from.x * to.z);
    f32 x = (from.y * to.z) - (from.z * to.y);

    mVec3_c cross(x, y, z);
    f32 mag = cross.mag();

    if (cM::isZero(mag)) {
        set(1.0f, 0.0f, 0.0f, 0.0f);
        return from.dot(to) > 0.0f;
    }
    cross *= 1.0f / mag;
    setAxisRotation(cross, interp * cM::atan2f(mag, from.dot(to)));
    return true;
}

bool mQuat_c::slerpTo(const mVec3_c &from, const mVec3_c &to, f32 maxStep) {
    f32 z = (from.x * to.y) - (from.y * to.x);
    f32 y = (from.z * to.x) - (from.x * to.z);
    f32 x = (from.y * to.z) - (from.z * to.y);

    mVec3_c cross(x, y, z);
    f32 mag = cross.mag();

    if (cM::isZero(mag)) {
        set(1.0f, 0.0f, 0.0f, 0.0f);
        return from.dot(to) > 0.0f;
    }
    cross *= 1.0f / mag;
    setAxisRotation(cross, nw4r::ut::Min(cM::atan2f(mag, from.dot(to)), maxStep));
    return true;
}
