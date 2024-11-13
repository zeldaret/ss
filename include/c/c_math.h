#ifndef C_LIB_CMATH
#define C_LIB_CMATH

#include "common.h"
#include "egg/math/eggMath.h"

#include <cmath.h>

namespace cM {
s16 atan2s(f32, f32);
void initRnd(s32);
f32 rnd();
int rndInt(int max);
f32 rndF(f32 max);
f32 rndFX(f32 amp);

template <typename T>
T rndRange(T min, T max) {
    f32 r = cM::rndF(max - min);
    f32 m = min;
    m += r;
    return m;
}

template <typename T>
inline T minMaxLimit(T val, T min, T max) {
    return (T)((T)val < (T)min ? (T)min : ((T)val > (T)max ? (T)max : (T)val));
}

inline bool isZero(f32 val) {
    return std::fabsf(val) <= EGG::Math<f32>::epsilon();
}

// When possivle, use the `isZero` func above.
// There are small cases where the result of fabs needs to be used again
inline bool isLessThanZero(f32 val) {
    return val <= EGG::Math<f32>::epsilon();
}

} // namespace cM

#endif
