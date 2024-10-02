#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

void LMS_SetMemFuncs(void *(*alloc)(size_t size), void (*free)(void *ptr));

// internal
void *LMSi_Malloc(size_t size);
void LMSi_Free(void *ptr);
int LMSi_MemCmp(const char *p1, const char *p2, int n);
void LMSi_MemCopy(char *p1, const char *p2, int n);

#ifdef __cplusplus
}
#endif
