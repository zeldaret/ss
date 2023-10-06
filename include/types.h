#pragma once

#include "rvl/macros.h"

#ifndef TYPES_H
#define TYPES_H
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

#endif

#ifdef __CWCC__
    typedef unsigned long size_t;
#endif

typedef u8  undefined1;
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
  inline NonCopyable(const NonCopyable&) {}
};
#endif
