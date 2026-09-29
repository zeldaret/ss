#include "m/m_sphere.h"

#include "c/c_math.h"
#include "egg/math/eggVector.h"
#include "m/m_vec.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_types.h"

mSphere_c &mSphere_c::merge(const mSphere_c &other) {
    mVec3_c cDiff;

    mVec3_c sub;
    nw4r::math::VEC3Sub(&sub, &mCenter, &other.mCenter);
    cDiff = sub;

    f32 dot = cDiff.getSquareMag();

    f32 rDiff = (other.mRadius - mRadius);

    if (rDiff * rDiff >= dot) {
        if (other.mRadius >= mRadius) {
            mCenter = other.mCenter;
            mRadius = other.mRadius;
        }
    } else {
        f32 norm = nw4r::math::FSqrt(dot);

        mRadius = norm + mRadius;
        mRadius = (mRadius + other.mRadius) * 0.5f;

        if (!cM::isZero(norm)) {
            mVec3_c shift = cDiff * ((mRadius - other.mRadius) / norm);
            mCenter = other.mCenter + shift;
        }
    }

    return *this;
}
