#ifndef RVL_SDK_WENC_H
#define RVL_SDK_WENC_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WENC_FLAG_USER_INFO = (1 << 0),
} WENCFlag;

typedef struct WENCInfo {
    s32 xn;        // at 0x0
    s32 dl;        // at 0x4
    s32 qn;        // at 0x8
    s32 dn;        // at 0xC
    s32 dlh;       // at 0x10
    s32 dlq;       // at 0x14
    u8 padding[8]; // at 0x18
} WENCInfo;

s32 WENCGetEncodeData(WENCInfo* info, u32 flag, const s16* pcmData, s32 samples,
                      u8* adpcmData);

#ifdef __cplusplus
}
#endif
#endif
