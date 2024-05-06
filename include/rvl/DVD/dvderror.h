#ifndef RVL_SDK_DVD_ERROR_H
#define RVL_SDK_DVD_ERROR_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*DVDErrorCallback)(s32 result, s32 arg1);

#define DVD_ERROR_CMD_MAX 5

typedef struct DVDErrorInfo {
    char game[4]; // at 0x0
    u8 disk;      // at 0x4
    u8 version;   // at 0x5
    u32 error;    // at 0x8
    s32 sec;      // at 0xC
    u32 disr;     // at 0x10
    u32 dicr;     // at 0x14
    u32 next;     // at 0x18

    struct {
        u32 command; // at 0x1C
        u32 param1;  // at 0x20
        u32 param2;  // at 0x24
        u32 intType; // at 0x28
        u32 tick;    // at 0x2C
    } info[DVD_ERROR_CMD_MAX];
} DVDErrorInfo;

extern DVDErrorInfo __ErrorInfo;

void __DVDStoreErrorCode(u32 error, DVDErrorCallback callback);

#ifdef __cplusplus
}
#endif
#endif
