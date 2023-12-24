#ifndef RVL_SDK_AX_OUT_H
#define RVL_SDK_AX_OUT_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

#define AX_STREAM_SIZE_RMT 40
#define AX_RMT_MAX 4

typedef void (*AXOutCallback)(void);
typedef void (*AXExceedCallback)(u32 cycles);

u32 __AXOutNewFrame(void);
void __AXOutAiCallback(void);
void __AXOutInitDSP(void);
void __AXOutInit(u32 mode);
AXOutCallback AXRegisterCallback(AXOutCallback callback);
s32 AXRmtGetSamplesLeft(void);
s32 AXRmtGetSamples(s32 chan, s16 *out, s32 num);
s32 AXRmtAdvancePtr(s32 num);

#ifdef __cplusplus
}
#endif
#endif
