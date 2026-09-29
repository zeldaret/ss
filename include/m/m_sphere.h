#ifndef M_SPHERE_H
#define M_SPHERE_H

#include "egg/geom/eggSphere.h"
#include "egg/math/eggMath.h"
#include "egg/math/eggVector.h"
#include "m/m_vec.h"

class mSphere_c : public EGG::Sphere3f {
public:
    mSphere_c() {}
    mSphere_c(const mVec3_c &center, f32 radius) : EGG::Sphere3f(center, radius) {}
    mSphere_c(const mSphere_c &other) : EGG::Sphere3f(other.mCenter, other.mRadius) {}

    mSphere_c &operator=(const mSphere_c &r) {
        mCenter.x = r.mCenter.x;
        mCenter.y = r.mCenter.y;
        mCenter.z = r.mCenter.z;
        mRadius = r.mRadius;
        return *this;
    }

    void set(const mVec3_c &center, f32 radius) {
        mCenter = center;
        mRadius = radius;
    }

    bool isZero() {
        return std::fabs(mRadius) <= EGG::Math<f32>::epsilon();
    }

    const mVec3_c &getCenter() const {
        return static_cast<const mVec3_c &>(mCenter);
    }
    f32 getRadius() const {
        return mRadius;
    }

    mSphere_c &merge(const mSphere_c &);
};

#endif
