#ifndef RVL_SDK_AXFX_REVERB_HI_DPL2_H
#define RVL_SDK_AXFX_REVERB_HI_DPL2_H
#include <common.h>
#include <rvl/AXFX/AXFXReverbHiExpDpl2.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_REVERBHI_DPL2 {
    AXFX_REVERBHI_EXP_DPL2 exp; // at 0x0
    f32 coloration;             // at 0x148
    f32 mix;                    // at 0x14C
    f32 time;                   // at 0x150
    f32 damping;                // at 0x154
    f32 preDelay;               // at 0x158
    f32 crosstalk;              // at 0x15C
} AXFX_REVERBHI_DPL2;

u32 AXFXReverbHiGetMemSizeDpl2(AXFX_REVERBHI_DPL2 *fx);
BOOL AXFXReverbHiInitDpl2(AXFX_REVERBHI_DPL2 *fx);
BOOL AXFXReverbHiShutdownDpl2(AXFX_REVERBHI_DPL2 *fx);
BOOL AXFXReverbHiSettingsDpl2(AXFX_REVERBHI_DPL2 *fx);
void AXFXReverbHiCallbackDpl2(void *chans, void *context);

#ifdef __cplusplus
}
#endif
#endif
