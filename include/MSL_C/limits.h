#ifndef MSL_LIMITS_H
#define MSL_LIMITS_H
#ifdef __cplusplus
extern "C" {
#endif

#define CHAR_BIT 8

#define SCHAR_MIN -127
#define SCHAR_MAX 127
#define UCHAR_MAX 255

#define CHAR_MIN 0
#define CHAR_MAX SCHAR_MAX

#define SHRT_MIN -32767
#define SHRT_MAX 32767
#define USHRT_MAX 65535

#define INT_MIN -2147483647
#define INT_MAX 2147483647

#define LONG_MIN INT_MIN
#define LONG_MAX INT_MAX
#define ULONG_MAX 4294967295

#ifdef __cplusplus
}
#endif
#endif
