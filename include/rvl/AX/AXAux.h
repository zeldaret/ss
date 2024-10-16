#ifndef RVL_SDK_AX_AUX_H
#define RVL_SDK_AX_AUX_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Stereo: Left, Right, Surround
 * DPL2:   Left, Right, Left Surround, Right Surround
 */
#define AX_STEREO_CHAN_MAX 3
#define AX_DPL2_CHAN_MAX 4

#define AX_SAMPLES_PER_FRAME 96
#define AX_SAMPLE_DEPTH_BYTES sizeof(u32)
#define AX_SAMPLES_PER_FRAME_RMT 18
#define AX_FRAME_SIZE (AX_SAMPLES_PER_FRAME * AX_SAMPLE_DEPTH_BYTES)

typedef void (*AXAuxCallback)(void *chans, void *context);

void __AXAuxInit(void);
void __AXGetAuxAInput(void **out);
void __AXGetAuxAOutput(void **out);
void __AXGetAuxAInputDpl2(void **out);
void __AXGetAuxAOutputDpl2R(void **out);
void __AXGetAuxAOutputDpl2Ls(void **out);
void __AXGetAuxAOutputDpl2Rs(void **out);
void __AXGetAuxBInput(void **out);
void __AXGetAuxBOutput(void **out);
void __AXGetAuxBInputDpl2(void **out);
void __AXGetAuxBOutputDpl2R(void **out);
void __AXGetAuxBOutputDpl2Ls(void **out);
void __AXGetAuxBOutputDpl2Rs(void **out);
void __AXGetAuxCInput(void **out);
void __AXGetAuxCOutput(void **out);
void __AXProcessAux(void);
void AXRegisterAuxACallback(AXAuxCallback callback, void *context);
void AXRegisterAuxBCallback(AXAuxCallback callback, void *context);
void AXRegisterAuxCCallback(AXAuxCallback callback, void *context);
void AXGetAuxACallback(AXAuxCallback *callback, void **context);
void AXGetAuxBCallback(AXAuxCallback *callback, void **context);
void AXGetAuxCCallback(AXAuxCallback *callback, void **context);

#ifdef __cplusplus
}
#endif
#endif
