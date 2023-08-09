#ifndef RVL_SDK_VF_PF_SERVICE_H
#define RVL_SDK_VF_PF_SERVICE_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

u16 VFiPF_GET_LE_U16(const u8* buf);
void VFiPF_LE16_TO_U16_STR(u8* str, u32 n);

#ifdef __cplusplus
}
#endif
#endif
