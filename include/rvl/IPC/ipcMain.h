#ifndef RVL_SDK_IPC_MAIN_H
#define RVL_SDK_IPC_MAIN_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

// Random number for arg typing
u32 IPC_HW_REGS[10] AT_ADDRESS(0xCD000000);

static inline u32 ACRReadReg(u32 reg) {
    return *(u32 *)((char *)IPC_HW_REGS + (reg & ~0x3));
}

static inline void ACRWriteReg(u32 reg, u32 val) {
    *(u32 *)((char *)IPC_HW_REGS + (reg & ~0x3)) = val;
}

void IPCInit(void);
u32 IPCReadReg(s32 index);
void IPCWriteReg(s32 index, u32 value);
void *IPCGetBufferHi(void);
void *IPCGetBufferLo(void);
void IPCSetBufferLo(void *lo);

#ifdef __cplusplus
}
#endif
#endif
