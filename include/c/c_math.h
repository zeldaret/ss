#ifndef C_LIB_CMATH
#define C_LIB_CMATH

#include <common.h>

namespace cM {
static s16 atan2s(f32, f32);
void initRnd(s32);
f32 rnd();
int rndInt(int max);
f32 rndF(f32 max);
f32 rndFX(f32 amp);
} // namespace cM

#endif
