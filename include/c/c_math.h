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
inline T minMaxLimit(T val, T min, T max) {
    return (T)((T)val < (T)min ? (T)min : ((T)val > (T)max ? (T)max : (T)val));
}

} // namespace cM

#endif
