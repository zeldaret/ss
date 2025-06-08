#ifndef NW4R_MATH_ARITHMETIC_H
#define NW4R_MATH_ARITHMETIC_H

#include "math.h"
#include "nw4r/types_nw4r.h" // IWYU pragma: export
#include "rvl/OS.h"          // IWYU pragma: export

#include <cmath>

#define NW4R_MATH_QNAN (-(0.0f / 0.0f))
#define NW4R_MATH_FLT_MAX 3.402823466e+38f

namespace nw4r {
namespace math {
namespace detail {

f32 FExp(f32 x);
f32 FLog(f32 x);

} // namespace detail

f32 FrSqrt(f32 x);

inline f32 FAbs(register f32 x) {
    // clang-format off
    asm {
        fabs x, x
    }
    // clang-format on

    return x;
}

inline f32 FCeil(f32 x) {
    return std::ceilf(x);
}

inline f32 FExp(f32 x) {
    return detail::FExp(x);
}

inline f32 FFloor(f32 x) {
    return std::floorf(x);
}

inline f32 FInv(register f32 x) {
    register f32 work0, work1, work2, work3;

    // clang-format off
    asm {
        fmr  work1, x     // x
        fres work0, work1 // 1/x

        // Refine estimate
        ps_add   work2, work0, work0        // 2/x
        ps_mul   work3, work0, work0        // 1/x^2
        ps_nmsub work0, work1, work3, work2 // -(x * 1/x^2 - 2/x)
    }
    // clang-format on

    return work0;
}

inline f32 FMod(f32 x, f32 y) {
    return fmodf(x, y);
}

inline f32 FModf(f32 x, f32 *y) {
    return modff(x, y);
}

inline f32 FSqrt(f32 x) {
    return (x <= 0.0f) ? 0.0f : x * FrSqrt(x);
}

inline f32 FLog(f32 x) {
    if (x > 0.0f) {
        return detail::FLog(x);
    }

    return NW4R_MATH_QNAN;
}

inline f32 FSelect(register f32 value, register f32 ge_zero, register f32 lt_zero) {
    register f32 ret;

    // clang-format off
    asm {
        fsel ret, value, ge_zero, lt_zero
    }
    // clang-format on

    return ret;
}

inline f32 U16ToF32(u16 arg) {
    f32 ret;
    OSu16tof32(&arg, &ret);
    return ret;
}

inline u16 F32ToU16(f32 arg) {
    u16 ret;
    OSf32tou16(&arg, &ret);
    return ret;
}

inline f32 S16ToF32(s16 arg) {
    f32 ret;
    OSs16tof32(&arg, &ret);
    return ret;
}

inline s16 F32ToS16(f32 arg) {
    s16 ret;
    OSf32tos16(&arg, &ret);
    return ret;
}

inline u32 F32AsU32(f32 arg) {
    return *reinterpret_cast<u32 *>(&arg);
}

inline f32 U32AsF32(u32 arg) {
    return *reinterpret_cast<f32 *>(&arg);
}

inline s32 FGetExpPart(f32 x) {
    s32 s = F32AsU32(x);
    return ((s >> 23) & 0xFF) - 0x7F;
}

inline f32 FGetMantPart(f32 x) {
    u32 u = F32AsU32(x);
    return U32AsF32((u & 0x807FFFFF) | 0x3F800000);
}

} // namespace math
} // namespace nw4r

#endif
