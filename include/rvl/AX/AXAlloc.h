#ifndef RVL_SDK_AX_ALLOC_H
#define RVL_SDK_AX_ALLOC_H
#include <common.h>
#include <rvl/AX/AXVPB.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AX_PRIORITY_FREE 0
#define AX_PRIORITY_MIN 1
#define AX_PRIORITY_MAX 31

AXVPB *__AXGetStackHead(u32 prio);
void __AXServiceCallbackStack(void);
void __AXInitVoiceStacks(void);
void __AXAllocInit(void);
void __AXPushFreeStack(AXVPB *vpb);
AXVPB *__AXPopFreeStack(void);
void __AXPushCallbackStack(AXVPB *vpb);
AXVPB *__AXPopCallbackStack(void);
void __AXRemoveFromStack(AXVPB *vpb);
void __AXPushStackHead(AXVPB *vpb, u32 prio);
AXVPB *__AXPopStackFromBottom(u32 prio);
void AXFreeVoice(AXVPB *vpb);
AXVPB *AXAcquireVoice(u32 prio, AXVoiceCallback callback, u32 userContext);
void AXSetVoicePriority(AXVPB *vpb, u32 prio);

#ifdef __cplusplus
}
#endif
#endif
