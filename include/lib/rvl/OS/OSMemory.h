#ifndef RVL_SDK_OS_MEMORY_H
#define RVL_SDK_OS_MEMORY_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define OS_MEM_MB_TO_B(mb) ((mb)*1024 * 1024)
#define OS_MEM_B_TO_MB(mb) ((mb) / 1024 / 1024)

#define OS_MEM_IS_MEM1(addr) (((u32)(addr)&0x30000000) == 0)
#define OS_MEM_IS_MEM2(addr) (((u32)(addr)&0x30000000) == 0x10000000)

u32 OSGetPhysicalMem1Size(void);
u32 OSGetPhysicalMem2Size(void);
u32 OSGetConsoleSimulatedMem1Size(void);
u32 OSGetConsoleSimulatedMem2Size(void);
void __OSInitMemoryProtection(void);

#ifdef __cplusplus
}
#endif
#endif
