#ifndef RVL_SDK_OS_FAST_CAST_H
#define RVL_SDK_OS_FAST_CAST_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

static inline void OSInitFastCast(void) {
    // clang-format off
    asm {
        li r3, 4
        oris r3, r3, 4
        mtspr 0x392, r3

        li r3, 5
        oris r3, r3, 5
        mtspr 0x393, r3
        
        li r3, 6
        oris r3, r3, 6
        mtspr 0x394, r3
        
        li r3, 7
        oris r3, r3, 7
        mtspr 0x395, r3
    }
    // clang-format on
}

static inline f32 __OSu16tof32(register const u16 *arg) {
    register f32 ret;

    // clang-format off
    asm {
        psq_l ret, 0(arg), 1, 3
    }
    // clang-format on

    return ret;
}

static inline void OSu16tof32(const u16 *in, f32 *out) {
    *out = __OSu16tof32(in);
}

static inline u16 __OSf32tou16(register f32 arg) {
    f32 a;
    register f32 *ptr = &a;
    u16 r;

    // clang-format off
    asm {
        psq_st arg, 0(ptr), 1, 3
    }
    // clang-format on

    r = *(u16 *)ptr;
    return r;
}
static inline u8 __OSf32tou8(register f32 arg) {
    f32 a;
    register f32 *ptr = &a;
    register u8 r;

    // clang-format off
    asm {
        psq_st arg, 0(ptr), 1, 2
        lbz r, 0(ptr)
    }
    // clang-format on

    return r;
}
static inline void OSf32tou16(const f32 *in, volatile u16 *out) {
    *out = __OSf32tou16(*in);
}

static inline void OSf32tou8(const f32 *in, volatile u8 *out) {
    *out = __OSf32tou8(*in);
}
static inline f32 __OSs16tof32(register const s16 *arg) {
    register f32 ret;

    // clang-format off
    asm {
        psq_l ret, 0(arg), 1, 5
    }
    // clang-format on

    return ret;
}

static inline void OSs16tof32(const s16 *in, f32 *out) {
    *out = __OSs16tof32(in);
}

static inline s16 __OSf32tos16(register f32 arg) {
    f32 a;
    register f32 *ptr = &a;
    s16 r;

    // clang-format off
    asm {
        psq_st arg, 0(ptr), 1, 5
    }
    // clang-format on

    r = *(s16 *)ptr;
    return r;
}

static inline void OSf32tos16(const f32 *in, volatile s16 *out) {
    *out = __OSf32tos16(*in);
}

#ifdef __cplusplus
}
#endif
#endif
