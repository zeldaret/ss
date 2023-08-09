#ifndef RVL_SDK_NAND_H
#define RVL_SDK_NAND_H
#include <lib/rvl/FS.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NAND_BANNER_TITLE_MAX 32
#define NAND_BANNER_ICON_MAX_FRAME 8

// Forward declarations
typedef struct NANDCommandBlock;

typedef enum {
    NAND_RESULT_FATAL_ERROR = -128,
    NAND_RESULT_UNKNOWN = -64,

    NAND_RESULT_MAXDEPTH = -16,
    NAND_RESULT_AUTHENTICATION,
    NAND_RESULT_OPENFD,
    NAND_RESULT_NOTEMPTY,
    NAND_RESULT_NOEXISTS,
    NAND_RESULT_MAXFILES,
    NAND_RESULT_MAXFD,
    NAND_RESULT_MAXBLOCKS,
    NAND_RESULT_INVALID,

    NAND_RESULT_EXISTS = -6,
    NAND_RESULT_ECC_CRIT,
    NAND_RESULT_CORRUPT,
    NAND_RESULT_BUSY,
    NAND_RESULT_ALLOC_FAILED,
    NAND_RESULT_ACCESS,

    NAND_RESULT_OK,
} NANDResult;

typedef enum {
    NAND_SEEK_BEG,
    NAND_SEEK_CUR,
    NAND_SEEK_END,
} NANDSeekMode;

typedef enum {
    NAND_ACCESS_NONE,
    NAND_ACCESS_READ,
    NAND_ACCESS_WRITE,
    NAND_ACCESS_RW
} NANDAccessType;

typedef enum {
    NAND_FILE_TYPE_NONE,
    NAND_FILE_TYPE_FILE,
    NAND_FILE_TYPE_DIR,
} NANDFileType;

typedef enum {
    // Read/write by owner
    NAND_PERM_RUSR = (NAND_ACCESS_READ << 4),
    NAND_PERM_WUSR = (NAND_ACCESS_WRITE << 4),
    // Read/write by group
    NAND_PERM_RGRP = (NAND_ACCESS_READ << 2),
    NAND_PERM_WGRP = (NAND_ACCESS_WRITE << 2),
    // Read/write by other
    NAND_PERM_ROTH = (NAND_ACCESS_READ << 0),
    NAND_PERM_WOTH = (NAND_ACCESS_WRITE << 0),
    // Read/write by all
    NAND_PERM_RALL = NAND_PERM_RUSR | NAND_PERM_RGRP | NAND_PERM_ROTH,
    NAND_PERM_WALL = NAND_PERM_WUSR | NAND_PERM_WGRP | NAND_PERM_WOTH,
    NAND_PERM_RWALL = NAND_PERM_RALL | NAND_PERM_WALL
} NANDPermission;

typedef void (*NANDAsyncCallback)(s32 result, struct NANDCommandBlock* block);

typedef struct NANDStatus {
    u32 ownerId; // at 0x0
    u16 groupId; // at 0x4
    u8 attr;     // at 0x6
    u8 perm;     // at 0x7
} NANDStatus;

typedef struct NANDFileInfo {
    s32 fd;                     // at 0x0
    s32 tempFd;                 // at 0x4
    char openPath[FS_MAX_PATH]; // at 0x8
    char tempPath[FS_MAX_PATH]; // at 0x48
    u8 access;                  // at 0x88
    u8 stage;                   // at 0x89
    u8 mark;                    // at 0x8A
} NANDFileInfo;

typedef struct NANDCommandBlock {
    void* userData;             // at 0x0
    NANDAsyncCallback callback; // at 0x4
    NANDFileInfo* info;         // at 0x8
    void* bytes;                // at 0xC
    void* inodes;               // at 0x10
    NANDStatus* status;         // at 0x14
    u32 ownerId;                // at 0x18
    u16 groupId;                // at 0x1C
    u8 nextStage;               // at 0x1E
    u32 attr;                   // at 0x20
    u32 ownerPerm;              // at 0x24
    u32 groupPerm;              // at 0x28
    u32 otherPerm;              // at 0x2C
    u32 dirFileCount;           // at 0x30
    char path[FS_MAX_PATH];     // at 0x34
    u32* length;                // at 0x74
    u32* position;              // at 0x78
    s32 state;                  // at 0x7C
    void* buffer;               // at 0x80
    u32 bufferSize;             // at 0x84
    u8* type;                   // at 0x88
    u32 uniqueNo;               // at 0x8C
    u32 reqBlocks;              // at 0x90
    u32 reqInodes;              // at 0x94
    u32* answer;                // at 0x98
    u32 homeBlocks;             // at 0x9C
    u32 homeInodes;             // at 0xA0
    u32 userBlocks;             // at 0xA4
    u32 userInodes;             // at 0xA8
    u32 workBlocks;             // at 0xAC
    u32 workInodes;             // at 0xB0
    const char** dir;           // at 0xB4
} NANDCommandBlock;

typedef struct NANDBanner {
    u32 magic;                                          // at 0x0
    u32 flags;                                          // at 0x4
    u16 iconSpeed;                                      // at 0x8
    u8 reserved[0x20 - 0xA];                            // at 0xA
    wchar_t title[NAND_BANNER_TITLE_MAX];               // at 0x20
    wchar_t subtitle[NAND_BANNER_TITLE_MAX];            // at 0x60
    u8 bannerTexture[0x6000];                           // at 0xA0
    u8 iconTexture[0x1200][NAND_BANNER_ICON_MAX_FRAME]; // at 0x60A0
} NANDBanner;

NANDResult NANDCreate(const char* path, u8 perm, u8 attr);
NANDResult NANDPrivateCreate(const char* path, u8 perm, u8 attr);
NANDResult NANDPrivateCreateAsync(const char* path, u8 perm, u8 attr,
                                  NANDAsyncCallback callback,
                                  NANDCommandBlock* block);

NANDResult NANDDelete(const char* path);
NANDResult NANDPrivateDelete(const char* path);
NANDResult NANDPrivateDeleteAsync(const char* path, NANDAsyncCallback callback,
                                  NANDCommandBlock* block);

NANDResult NANDRead(NANDFileInfo* info, void* buf, u32 len);
NANDResult NANDReadAsync(NANDFileInfo* info, void* buf, u32 len,
                         NANDAsyncCallback callback, NANDCommandBlock* block);

NANDResult NANDWrite(NANDFileInfo* info, const void* buf, u32 len);
NANDResult NANDWriteAsync(NANDFileInfo* info, const void* buf, u32 len,
                          NANDAsyncCallback callback, NANDCommandBlock* block);

NANDResult NANDSeek(NANDFileInfo* info, s32 offset, NANDSeekMode whence);
NANDResult NANDSeekAsync(NANDFileInfo* info, s32 offset, NANDSeekMode whence,
                         NANDAsyncCallback callback, NANDCommandBlock* block);

NANDResult NANDPrivateCreateDir(const char* path, u8 perm, u8 attr);
NANDResult NANDPrivateCreateDirAsync(const char* path, u8 perm, u8 attr,
                                     NANDAsyncCallback callback,
                                     NANDCommandBlock* block);

NANDResult NANDMove(const char* from, const char* to);

NANDResult NANDGetLength(NANDFileInfo* info, u32* length);
NANDResult NANDGetLengthAsync(NANDFileInfo* info, u32* lengthOut,
                              NANDAsyncCallback callback,
                              NANDCommandBlock* block);

NANDResult NANDGetStatus(const char* path, NANDStatus* status);
NANDResult NANDPrivateGetStatusAsync(const char* path, NANDStatus* status,
                                     NANDAsyncCallback callback,
                                     NANDCommandBlock* block);

void NANDSetUserData(NANDCommandBlock* block, void* data);
void* NANDGetUserData(NANDCommandBlock* block);

#ifdef __cplusplus
}
#endif
#endif
