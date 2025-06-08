#ifndef RVL_SDK_AXFX_REVERB_STD_H
#define RVL_SDK_AXFX_REVERB_STD_H

#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_REVERBSTD_EXP {
    u8 _0x00[0xB4];
    u32 field_0xB4;
    f32 field_0xB8;
    f32 field_0xBC;
    u32 field_0xC0;
    f32 field_0xC4;
    f32 field_0xC8;
    f32 field_0xCC;
    f32 field_0xD0;
    f32 field_0xD4;
    u32 field_0xD8;
    u32 field_0xDC;
    f32 field_0xE0;
    f32 field_0xE4;
} AXFX_REVERBSTD_EXP;

typedef struct AXFX_REVERBSTD_EXP_DPL2 {
    u8 _0x00[0xD0];
    u32 field_0xD0;
    f32 field_0xD4;
    f32 field_0xD8;
    u32 field_0xDC;
    f32 field_0xE0;
    f32 field_0xE4;
    f32 field_0xE8;
    f32 field_0xEC;
    f32 field_0xF0;
    u32 field_0xF4;
    u32 field_0xF8;
    f32 field_0xFC;
    f32 field_0x100;
} AXFX_REVERBSTD_EXP_DPL2;

u32 AXFXReverbStdExpGetMemSize(const AXFX_REVERBSTD_EXP *fx);
BOOL AXFXReverbStdExpInit(AXFX_REVERBSTD_EXP *fx);
void AXFXReverbStdExpShutdown(AXFX_REVERBSTD_EXP *fx);
BOOL AXFXReverbStdExpSettings(AXFX_REVERBSTD_EXP *fx);
BOOL AXFXReverbStdExpSettingsUpdate(AXFX_REVERBSTD_EXP *fx);
void AXFXReverbStdExpCallback(void* chans, AXFX_REVERBSTD_EXP* context);

u32 AXFXReverbStdExpGetMemSizeDpl2(const AXFX_REVERBSTD_EXP_DPL2 *fx);
BOOL AXFXReverbStdExpInitDpl2(AXFX_REVERBSTD_EXP_DPL2 *fx);
void AXFXReverbStdExpShutdownDpl2(AXFX_REVERBSTD_EXP_DPL2 *fx);
BOOL AXFXReverbStdExpSettingsDpl2(AXFX_REVERBSTD_EXP_DPL2 *fx);
BOOL AXFXReverbStdExpSettingsUpdateDpl2(AXFX_REVERBSTD_EXP_DPL2 *fx);

void AXFXReverbStdExpCallbackDpl2(void* chans, AXFX_REVERBSTD_EXP_DPL2* context);

#ifdef __cplusplus
}
#endif
#endif
