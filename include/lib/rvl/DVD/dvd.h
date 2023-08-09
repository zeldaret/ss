#ifndef RVL_SDK_DVD_H
#define RVL_SDK_DVD_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSAlarm;

typedef struct DVDDriveBlock {
    char UNK_0x0[0xC];
    UNKWORD WORD_0xC;
    char UNK_0x10[0x30 - 0x10];
} DVDDriveBlock;

typedef struct DVDDriveInfo {
    u16 revision;    // at 0x0
    u16 deviceCode;  // at 0x2
    u32 releaseDate; // at 0x4
    char padding[32 - 0x8];
} DVDDriveInfo;

typedef void (*DVDInquiryCallback)(s32, DVDDriveBlock*);

void DVDInit(void);

BOOL DVDInquiryAsync(DVDDriveBlock*, DVDDriveInfo*, DVDInquiryCallback);

u32 __DVDGetCoverStatus(void);

void __DVDPrepareReset(void);
BOOL __DVDTestAlarm(struct OSAlarm*);

#ifdef __cplusplus
}
#endif
#endif
