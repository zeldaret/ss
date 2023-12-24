#ifndef RVL_SDK_DVD_FS_H
#define RVL_SDK_DVD_FS_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

extern BOOL __DVDLongFileNameFlag;

typedef void (*DVDCommandCallback)(s32, struct DVDCommandBlock *);
typedef void (*DVDFileCallback)(s32, struct DVDFileInfo *);

typedef struct DVDCommandBlock {
    /* 0x00 */ struct DVDCommandBlock *next;
    /* 0x04 */ struct DVDCommandBlock *prev;
    /* 0x08 */ u32 command;
    /* 0x0c */ s32 state;
    /* 0x10 */ u32 offset;
    /* 0x14 */ u32 length;
    /* 0x18 */ void *addr;
    /* 0x1c */ u32 currTransferSize;
    /* 0x20 */ u32 transferredSize;
    /* 0x24 */ DVDDiskID *id;
    /* 0x28 */ DVDCommandCallback callback;
    /* 0x2c */ void *userData;
} DVDCommandBlock;

typedef struct DVDFileInfo {
    /* 0x00 */ DVDCommandBlock cb;
    /* 0x30 */ u32 startAddr;
    /* 0x34 */ u32 length;
    /* 0x38 */ DVDFileCallback *callback;
} DVDFileInfo;

#ifdef __cplusplus
}
#endif
#endif
