#ifndef RVL_SDK_VF_PF_W_CLIB_H
#define RVL_SDK_VF_PF_W_CLIB_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

size_t VFipf_w_strlen(const wchar_t* str);
wchar_t* VFipf_w_strcpy(wchar_t* dst, const wchar_t* src);
int VFipf_w_strncmp(const wchar_t* s1, const wchar_t* s2, size_t n);

#ifdef __cplusplus
}
#endif
#endif
