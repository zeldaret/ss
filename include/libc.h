#ifndef DECOMP_LIB_C
#define DECOMP_LIB_C

#include "common.h"

extern "C" {

// void *memset(void *dest, s32 value, u32 count);
// void *memcpy(void *dest, const void *src, u32 count);
void *checkedMemcpy(void *dest, u16 destLen, const void *src, u16 count);
}

#endif
