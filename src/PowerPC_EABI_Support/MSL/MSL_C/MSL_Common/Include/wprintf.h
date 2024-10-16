#ifndef MSL_WPRINTF_H
#define MSL_WPRINTF_H

#include "__va_arg.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

int vswprintf(wchar_t *, size_t, const wchar_t *, va_list);
int swprintf(wchar_t *, size_t, const wchar_t *, ...);

#ifdef __cplusplus
}
#endif
#endif
