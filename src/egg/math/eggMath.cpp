#include "egg/math/eggMath.h"

#include "nw4r/math/math_arithmetic.h"

#include <cmath>

namespace EGG {

template <>
f32 Math<f32>::sqrt(f32 x) {
    if (x <= 0.0f) {
        return 0.0f;
    }
    return x * nw4r::math::FrSqrt(x);
}

template <>
f32 Math<f32>::sin(f32 x) {
    return ::sin(x);
}

template <>
f32 Math<f32>::cos(f32 x) {
    return ::cos(x);
}

template <>
f32 Math<f32>::acos(f32 x) {
    return ::acos(x);
}

template <>
f32 Math<f32>::atan2(f32 y, f32 x) {
    return ::atan2(y, x);
}

} // namespace EGG
