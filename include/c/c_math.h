#ifndef C_LIB_CMATH
#define C_LIB_CMATH

#include "common.h"

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
T calcTimer(T *value) {
    if (*(T *)value != 0) {
        *value = *value - 1;
    }
    return *value;
}

} // namespace cM

#endif
