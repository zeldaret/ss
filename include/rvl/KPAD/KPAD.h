#ifndef REVOSDK_KPAD_H
#define REVOSDK_KPAD_H
#ifdef __cplusplus
#include <common.h>
extern "C" {
#endif

typedef struct {
    // TODO
} KPADStatus;

typedef void (*KPADConnectCallback)(s32, s32);
// TODO: Create Proper Headers
void KPADInitEx(void * /* UNKTYPE */, u32);

s32 KPADRead(s32 chan, UNKTYPE *status, s32);
s32 KPADReadEx(s32 chan, UNKTYPE *status, s32, s32 *);

void KPADSetConnectCallback(u32, KPADConnectCallback);

void KPADSetPosParam(u32, f32, f32);
void KPADSetHoriParam(u32, f32, f32);
void KPADSetDistParam(u32, f32, f32);
void KPADSetAccParam(u32, f32, f32);

#ifdef __cplusplus
}
#endif
#endif
