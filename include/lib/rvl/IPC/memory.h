#ifndef RVL_SDK_IPC_MEMORY_H
#define RVL_SDK_IPC_MEMORY_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

s32 iosCreateHeap(void*, u32);

void* iosAllocAligned(s32, u32, s32);
s32 iosFree(s32, void*);

#ifdef __cplusplus
}
#endif
#endif
