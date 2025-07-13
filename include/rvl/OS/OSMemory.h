#ifndef RVL_SDK_OS_MEMORY_H
#define RVL_SDK_OS_MEMORY_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

#define OS_MEM_MB_TO_B(mb) ((mb) * 1024 * 1024)
#define OS_MEM_B_TO_MB(mb) ((mb) / 1024 / 1024)

#define OS_MEM_IS_MEM1(addr) (((u32)(addr) & 0x30000000) == 0)
#define OS_MEM_IS_MEM2(addr) (((u32)(addr) & 0x30000000) == 0x10000000)

#define OS_PROTECT_CHAN0 0
#define OS_PROTECT_CHAN1 1
#define OS_PROTECT_CHAN2 2
#define OS_PROTECT_CHAN3 3

#define OS_PROTECT_CONTROL_NONE 0x00
#define OS_PROTECT_CONTROL_READ 0x01
#define OS_PROTECT_CONTROL_WRITE 0x02
#define OS_PROTECT_CONTROL_RDWR (OS_PROTECT_CONTROL_READ | OS_PROTECT_CONTROL_WRITE)

u32 OSGetPhysicalMem1Size(void);
u32 OSGetPhysicalMem2Size(void);
u32 OSGetConsoleSimulatedMem1Size(void);
u32 OSGetConsoleSimulatedMem2Size(void);
void __OSInitMemoryProtection(void);
void OSProtectRange(u32 chan, void* addr, u32 nBytes, u32 control);

#ifdef __cplusplus
}
#endif
#endif
