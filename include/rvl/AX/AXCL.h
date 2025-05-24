#ifndef RVL_SDK_AX_CL_H
#define RVL_SDK_AX_CL_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

#define AX_MAX_VOLUME 32768
// Command list can hold up to 64 commands
#define AX_CL_MAX_CMD 64
// Each command takes up two bytes
#define AX_CL_SIZE (AX_CL_MAX_CMD * sizeof(u16))

typedef u32 AXCLMode;
enum AXCLMode_et
{
	AX_CL_MODE_STEREO,
	AX_CL_MODE_SURROUND,
	AX_CL_MODE_DPL2,
};

extern u32 __AXClMode;

u32 __AXGetCommandListCycles(void);
void *__AXGetCommandListAddress(void);
void __AXWriteToCommandList(u16 cmd);
void __AXNextFrame(void *surround, void *lr, void *rmt);
void __AXClInit(void);
void AXSetMode(u32 mode);
u32 AXGetMode(void);
u16 AXGetAuxAReturnVolume(void);
u16 AXGetAuxBReturnVolume(void);
u16 AXGetAuxCReturnVolume(void);
void AXSetMasterVolume(u16 volume);
void AXSetAuxAReturnVolume(u16 volume);
void AXSetAuxBReturnVolume(u16 volume);
void AXSetAuxCReturnVolume(u16 volume);

#ifdef __cplusplus
}
#endif
#endif
