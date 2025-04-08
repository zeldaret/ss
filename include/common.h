#ifndef DECOMP_COMMON_TYPES_H
#define DECOMP_COMMON_TYPES_H

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define CLAMP(low, high, x) ((x) > (high) ? (high) : ((x) < (low) ? (low) : (x)))

#define ROUND_UP(x, align) (((x) + (align) - 1) & (-(align)))
#define ROUND_UP_PTR(x, align) ((void *)((((u32)(x)) + (align) - 1) & (~((align) - 1))))

#define ROUND_DOWN(x, align) ((x) & (-(align)))
#define ROUND_DOWN_PTR(x, align) ((void *)(((u32)(x)) & (~((align) - 1))))

#define ARRAY_LENGTH(x) (sizeof((x)) / sizeof((x)[0]))

#define CLEAR_PATH(x) __memclr((x), sizeof((x)))

#define READU32_BE(ptr, offset)                                                                                        \
    (((u32)ptr[offset] << 24) | ((u32)ptr[offset + 1] << 16) | ((u32)ptr[offset + 2] << 8) | (u32)ptr[offset + 3]);

#define ALIGN_DECL(ALIGNMENT) __attribute__((aligned(ALIGNMENT)))

// static_assert from TP
#ifdef __MWERKS__
    #define GLUE(a, b) a##b
    #define GLUE2(a, b) GLUE(a, b)
    #define STATIC_ASSERT(cond) typedef char GLUE2(static_assertion_failed, __LINE__)[(cond) ? 1 : -1]
#else
    #define STATIC_ASSERT(...)
#endif

// To remove some warnings
#ifdef __MWERKS__
#define DECL_SECTION(x) __declspec(section x)
#define DECL_WEAK __declspec(weak)
#define DONT_INLINE __attribute__((never_inline))
#define FORCE_INLINE inline
#define AT_ADDRESS(xyz) : (xyz)
#else
#define DECL_SECTION(x)
#define AT_ADDRESS(xyz)
#define DECL_WEAK
#define DONT_INLINE
#define FORCE_INLINE
#define asm
#define nofralloc
// Quick Fixup
#define __alloca(x) ((void *)x)
#endif

#ifdef NON_MATCHING
#define DECOMP_FORCEACTIVE(module, ...)
#define DECOMP_FORCELITERAL(module, x)
#define DECOMP_FORCEDTOR(module, cls)
#define DECOMP_INLINE
#define DECOMP_DONT_INLINE
#else
// Force reference specific data
#define __CONCAT(x, y) x##y
#define CONCAT(x, y) __CONCAT(x, y)

#define DECOMP_FORCEACTIVE(module, ...)                                                                                \
    void fake_function(...);                                                                                           \
    void CONCAT(FORCEACTIVE##module, __LINE__)(void);                                                                  \
    void CONCAT(FORCEACTIVE##module, __LINE__)(void) {                                                                 \
        fake_function(__VA_ARGS__);                                                                                    \
    }

// Force literal ordering, such as floats in sdata2
#define DECOMP_FORCELITERAL(module, ...)                                                                               \
    void CONCAT(FORCELITERAL##module, __LINE__)(void);                                                                 \
    void CONCAT(FORCELITERAL##module, __LINE__)(void) {                                                                \
        (__VA_ARGS__);                                                                                                 \
    }

// Force reference destructor
#define DECOMP_FORCEDTOR(module, cls)                                                                                  \
    void CONCAT(FORCEDTOR##module##cls, __LINE__)(void) {                                                              \
        cls dummy;                                                                                                     \
        dummy.~cls();                                                                                                  \
    }
#define DECOMP_INLINE inline
#define DECOMP_DONT_INLINE __attribute__((never_inline))
#endif

#include "stddef.h"

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

typedef int UNKWORD;
typedef void UNKTYPE;

enum {
    FALSE,
    TRUE
};
typedef int BOOL;

typedef unsigned char byte_t;

#ifdef __CWCC__
typedef unsigned long size_t;
#endif

typedef u8 undefined1;
typedef u16 undefined2;
typedef u32 undefined4;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef volatile f32 vf32;
typedef volatile f64 vf64;

// Necesary for CW
#if __MWERKS__
#define override
#define noexcept
#define nullptr NULL
#endif

#ifndef NULL
#define NULL 0
#endif
#define nullptr NULL

// taken from mkw
#ifdef __cplusplus
#define restrict

class NonCopyable {
public:
    inline NonCopyable() {}

private:
    inline NonCopyable(const NonCopyable &) {}
};
#endif

#define INT32_MAX (0x7fffffff)
#define UINT32_MAX (0xffffffff)

#define FLOAT_MIN (1.175494351e-38f)
#define FLOAT_MAX (3.40282346638528860e+38f)

#ifdef __MWERKS__
#define AT_ADDRESS(xyz) : (xyz)
#else
#define AT_ADDRESS(xyz)
#endif

#endif
