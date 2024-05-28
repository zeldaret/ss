#ifndef REVOSDK_WPAD_H
#define REVOSDK_WPAD_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*WPADCallback)(s32 chan, s32 result);

typedef enum { WPAD_CHAN_0, WPAD_CHAN_1, WPAD_CHAN_2, WPAD_CHAN_3, WPAD_MAX_CONTROLLERS } WPADChan;

typedef enum {
    WPAD_RESULT_ERR_3 = -3,
    WPAD_RESULT_ERR_2,
    WPAD_RESULT_ERR_1,

    WPAD_RESULT_SUCCESS = 0,
} WPADResult;


typedef struct {
    u32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    bool field_0x14;
    bool field_0x15;
    bool field_0x16;
    bool field_0x17;
} WPADInfo;

UNKTYPE WPADControlSpeaker(s32, UNKWORD, WPADCallback);
BOOL WPADCanSendStreamData(s32);
UNKWORD WPADSendStreamData(s32, UNKTYPE *, UNKWORD);

s32 WPADProbe(s32 chan, u32 *type);
s32 WPADReadFaceData(s32 chan, void *dst, u32 size, u32 src, WPADCallback cb);
s32 WPADGetInfoAsync(s32 chan, WPADInfo *dst, WPADCallback cb);

#ifdef __cplusplus
}
#endif
#endif
