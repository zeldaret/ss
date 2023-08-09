#ifndef RVL_SDK_NDEV_EXI2_AD_EXI2_H
#define RVL_SDK_NDEV_EXI2_AD_EXI2_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

BOOL __EXI2Imm(void* mem, s32 size, u32 type);
void __DBEXIInit(void);
BOOL __DBEXIReadReg(u32 cmd, void* mem, s32 size);
BOOL __DBEXIWriteReg(u32 cmd, const void* mem, s32 size);
BOOL __DBEXIReadRam(u32 cmd, void* mem, s32 size);
BOOL __DBEXIWriteRam(u32 cmd, const void* mem, s32 size);

#ifdef __cplusplus
}
#endif
#endif
