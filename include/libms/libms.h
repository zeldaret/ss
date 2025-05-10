#ifndef LIBMS_LIBMS_H
#define LIBMS_LIBMS_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// made up
enum LMS_Error {
    LMS_NOT_FOUND = -1,
    LMS_MISSING_LBL1_DATA = -2,
    LMS_MISSING_FLW3_DATA = -4,
    LMS_MISSING_FEN1_DATA = -10,
};

void LMS_SetMemFuncs(void *(*alloc)(size_t size), void (*free)(void *ptr));

// internal
void *LMSi_Malloc(size_t size);
void LMSi_Free(void *ptr);
int LMSi_MemCmp(const char *p1, const char *p2, int n);
void LMSi_MemCopy(char *p1, const char *p2, int n);

#ifdef __cplusplus
}
#endif

#endif
