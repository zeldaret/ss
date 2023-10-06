#pragma once

#include "types.h"
#include "egg/math/eggMath.h"
namespace EGG
{

class Vector3f {
public:
    f32 x,y,z;
public:
    // may need change
    Vector3f(const Vector3f& other) {x = other.x, y = other.y, z = other.z;}
    // may need change
    f32 squaredLength() { return x*x + y*y + z*z; }
    f32 length() { return Math<f32>::frsqrt(squaredLength()); }
    /* 8049bcc0 */ void normalise();
    /* 8049bd50 */ void setLength(Vector3f& src, f32 length);
    /* 8049be10 */ void setLength(f32 length);

public:
    /* 80674c30 */ static Vector3f zero;
    /* 80674c3c */ static Vector3f ex;
    /* 80674c48 */ static Vector3f ey;
    /* 80674c54 */ static Vector3f ez;
};

struct Vector2f {
    f32 x,y;

public:
    /* 805767c0 */ static Vector2f zero;
    /* 805767c8 */ static Vector2f ex;
    /* 805767d0 */ static Vector2f ey;
};

struct Vector3s {
    s16 x,y,z;
public:
    /* 805767d8 */ static Vector3s zero;
    /* 805767e0 */ static Vector3s ex;
    /* 805767e8 */ static Vector3s ey;
    /* 805767f0 */ static Vector3s ez;
};

} // namespace EGG
