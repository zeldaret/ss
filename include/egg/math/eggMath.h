#pragma once

#include "types.h"

namespace EGG
{

template <typename T>
class Math {
public:
    static T sqrt(T);
    static T sin(T);
    static T cos(T);
    static T acos(T);
    static T atan2(T);
};

// f32 impls
// /* 8049ab60 */ Math<f32>::sqrt(f32);
// /* 8049abb0 */ Math<f32>::sin(f32);
// /* 8049abe0 */ Math<f32>::cos(f32);
// /* 8049ac10 */ Math<f32>::acos(f32);
// /* 8049ac40 */ Math<f32>::atan2(f32, f32);

} // namespace EGG
