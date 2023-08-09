#pragma once
#define TYPES_H
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

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

typedef float f32;
typedef double f64;
typedef volatile f32 vf32;
typedef volatile f64 vf64;

#define nullptr 0

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
