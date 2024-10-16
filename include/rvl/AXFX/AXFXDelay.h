#ifndef RVL_SDK_AXFX_DELAY_H
#define RVL_SDK_AXFX_DELAY_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_DELAY {
    s32 *line[3];        // at 0x0
    u32 curPos[3];       // at 0xC
    u32 length[3];       // at 0x18
    s32 feedbackGain[3]; // at 0x24
    s32 outGain[3];      // at 0x30
    u32 active;          // at 0x3C
    u32 delay[3];        // at 0x40
    u32 feedback[3];     // at 0x4C
    u32 output[3];       // at 0x58
} AXFX_DELAY;

u32 AXFXDelayGetMemSize(const AXFX_DELAY *fx);
BOOL AXFXDelayInit(AXFX_DELAY *fx);
BOOL AXFXDelaySettings(AXFX_DELAY *fx);
void AXFXDelayShutdown(AXFX_DELAY *fx);
void AXFXDelayCallback(void *chans, void *context);

#ifdef __cplusplus
}
#endif
#endif
