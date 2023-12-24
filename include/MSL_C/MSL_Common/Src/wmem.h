#ifndef MSL_WMEM_H
#define MSL_WMEM_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

void *wmemcpy(void *, const void *, size_t);
void *wmemchr(const void *, int, size_t);

#ifdef __cplusplus
}
#endif
#endif
