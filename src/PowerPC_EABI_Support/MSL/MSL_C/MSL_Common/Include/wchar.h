#ifndef MSL_WCHAR_H
#define MSL_WCHAR_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

#define WCHAR_MIN SHRT_MIN
#define WCHAR_MAX USHRT_MAX

#ifdef __cplusplus
}
#endif
#endif
