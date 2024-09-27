#ifndef RVL_SDK_PUBLIC_KPA_H
#define RVL_SDK_PUBLIC_KPA_H

#include <common.h>

extern "C" {

typedef void (*KPADConnectCallback)(s32, s32);
// TODO: Create Proper Headers
void KPADInitEx(void * /* UNKTYPE */, u32);
void KPADSetConnectCallback(u32, KPADConnectCallback);
}

#endif
