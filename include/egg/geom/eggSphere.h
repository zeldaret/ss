#ifndef EGG_SPHERE_H
#define EGG_SPHERE_H

#include "egg/math/eggVector.h"

namespace EGG {

class Sphere3f {
public:
    Sphere3f() {}
    ~Sphere3f() {}

    Sphere3f(const Vector3f &center, f32 radius) : mCenter(center), mRadius(radius) {}

    Vector3f mCenter;
    f32 mRadius;
};

} // namespace EGG

#endif
