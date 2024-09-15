#ifndef DECOMP_COMMON_TYPES_H
#define DECOMP_COMMON_TYPES_H

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define CLAMP(low, high, x) ((x) > (high) ? (high) : ((x) < (low) ? (low) : (x)))

#define ROUND_UP(x, align) (((x) + (align)-1) & (-(align)))
#define ROUND_UP_PTR(x, align) ((void *)((((u32)(x)) + (align)-1) & (~((align)-1))))

#define ROUND_DOWN(x, align) ((x) & (-(align)))
#define ROUND_DOWN_PTR(x, align) ((void *)(((u32)(x)) & (~((align)-1))))

#define ARRAY_LENGTH(x) (sizeof((x)) / sizeof((x)[0]))

#define CLEAR_PATH(x) __memclr((x), sizeof((x)))

#define ALIGN(x) __attribute__((aligned(x)))
#define DECL_SECTION(x) __declspec(section x)
#define DECL_WEAK __declspec(weak)
#define DONT_INLINE __attribute__((never_inline))

// TODO - Fix MSL (stddef.h)
#define offsetof(ST, M) ((size_t) & (((ST*)0)->M))

// Codewarrior tricks for matching decomp
// (Functions are given prototypes for -requireprotos)
#ifdef __MWERKS__
// Force BSS order
#define CW_FORCE_BSS(module, ...) \
    void fake_function(...); \
    void FORCE_BSS##module##x(void); \
    void FORCE_BSS##module##x(void) { \
        fake_function(__VA_ARGS__); \
    }
// Force strings into pool
#define CW_FORCE_STRINGS(module, ...) \
    void fake_function(...); \
    void FORCE_STRINGS##module(void); \
    void FORCE_STRINGS##module(void) { \
        fake_function(__VA_ARGS__); \
    }
#else
#define CW_FORCE_BSS(module, ...)
#define CW_FORCE_STRINGS(module, ...)
#endif

#include <decomp.h>

// For VSCode
#ifdef __INTELLISENSE__
#define asm
#define __attribute__(x)
#define __declspec(x)
#endif

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

enum { FALSE, TRUE };
typedef int BOOL;

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
#if __cplusplus < 201103L && !defined(_WIN32)
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

#endif
