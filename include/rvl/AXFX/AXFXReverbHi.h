#ifndef RVL_SDK_AXFX_REVERB_HI_H
#define RVL_SDK_AXFX_REVERB_HI_H
#include <common.h>
#include <rvl/AXFX/AXFXReverbHiExp.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_REVERBHI {
    AXFX_REVERBHI_EXP exp; // at 0x0
    f32 coloration;        // at 0x148
    f32 mix;               // at 0x14C
    f32 time;              // at 0x150
    f32 damping;           // at 0x154
    f32 preDelay;          // at 0x158
    f32 crosstalk;         // at 0x15C
} AXFX_REVERBHI;

u32 AXFXReverbHiGetMemSize(AXFX_REVERBHI *fx);
BOOL AXFXReverbHiInit(AXFX_REVERBHI *fx);
BOOL AXFXReverbHiShutdown(AXFX_REVERBHI *fx);
BOOL AXFXReverbHiSettings(AXFX_REVERBHI *fx);
void AXFXReverbHiCallback(void *chans, void *context);

#ifdef __cplusplus
}
#endif
#endif
