#ifndef EGG_MATH_H
#define EGG_MATH_H

#include "common.h"
#include "math.h"

namespace EGG {

template <typename T>
class Math {
public:
    static T maxNumber() {
        // TODO: Generalize to other classes
        // This is low priority since it will always be a float
        return 3.402823466e+38f;
    }

    static T pi() {
        return 3.14159265f;
    }

    static T pi_half() {
        return pi() / 2.0f;
    }

    static T epsilon() {
        return 1.192092896e-07f;
    }

    static T inv(T t) {
        return 1 / t;
    }

    static T abs(T t) {
        return t > (T)0 ? t : -t;
    }

    static T zero() {
        return (T)0;
    }

    static T clamp(T x, T low, T high) {
        return (x < low ? low : (x > high ? high : x));
    }

    static T sqrt(T);
    static T sin(T);
    static T cos(T);
    static T tan(T);
    static T asin(T);
    static T acos(T);
    static T atan2(T, T);
    static T log10(T);

    static T gcd(T, T);
    static T lcm(T, T);
};

// There is
// Math<f32>::zero
// Math<f32>::pi_half
// Math<f32>::neg(f32)
// Math<f32>::abs(f32)

// f32 impls
// /* 8049ab60 */ Math<f32>::sqrt(f32);
// /* 8049abb0 */ Math<f32>::sin(f32);
// /* 8049abe0 */ Math<f32>::cos(f32);
// /* 8049ac10 */ Math<f32>::acos(f32);
// /* 8049ac40 */ Math<f32>::atan2(f32, f32);

} // namespace EGG

#endif
