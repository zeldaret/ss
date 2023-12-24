#ifndef RVL_SDK_AXFX_CHORUS_H
#define RVL_SDK_AXFX_CHORUS_H
#include <common.h>
#include <rvl/AXFX/AXFXChorusExp.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_CHORUS {
    AXFX_CHORUS_EXP exp; // at 0x0
    u32 baseDelay;       // at 0xA0
    u32 variation;       // at 0xA4
    u32 period;          // at 0xA8
} AXFX_CHORUS;

u32 AXFXChorusGetMemSize(const AXFX_CHORUS *fx);
BOOL AXFXChorusInit(AXFX_CHORUS *fx);
BOOL AXFXChorusShutdown(AXFX_CHORUS *fx);
BOOL AXFXChorusSettings(AXFX_CHORUS *fx);
void AXFXChorusCallback(void *chans, void *context);

#ifdef __cplusplus
}
#endif
#endif
