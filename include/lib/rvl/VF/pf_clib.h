#ifndef RVL_SDK_VF_PF_CLIB_H
#define RVL_SDK_VF_PF_CLIB_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

int VFipf_toupper(int c);
void* VFipf_memcpy(void* dst, const void* src, size_t n);
void* VFipf_memset(void* dst, int ch, size_t n);
size_t VFipf_strlen(const char* str);
char* VFipf_strcpy(char* dst, const char* src);
int VFipf_strcmp(const char* s1, const char* s2);
int VFipf_strncmp(const char* s1, const char* s2, size_t n);

#ifdef __cplusplus
}
#endif
#endif
