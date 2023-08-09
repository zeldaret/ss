#ifndef RVL_SDK_OS_IPC_H
#define RVL_SDK_OS_IPC_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void* __OSGetIPCBufferHi(void);
void* __OSGetIPCBufferLo(void);
void __OSInitIPCBuffer(void);

#ifdef __cplusplus
}
#endif
#endif
