#ifndef RVL_SDK_FS_H
#define RVL_SDK_FS_H
#include <lib/rvl/IPC.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define FS_MAX_PATH 64

typedef void (*FSAsyncCallback)(s32 result, void* arg);

typedef struct FSStats {
    char UNK_0x0[0x1C];
} FSStats;

// Could be more fields, but not larger than 32B
typedef struct FSFileStats {
    u32 length;   // at 0x0
    u32 position; // at 0x4
} FSFileStats ALIGN(32);

s32 ISFS_OpenLib(void);
s32 ISFS_CreateDir(const char* path, u32 attr, u32 ownerPerm, u32 groupPerm,
                   u32 otherPerm);
s32 ISFS_CreateDirAsync(const char* path, u32 attr, u32 ownerPerm,
                        u32 groupPerm, u32 otherPerm, FSAsyncCallback callback,
                        void* callbackArg);
s32 ISFS_ReadDir(const char* path, char* filesOut, u32* fileCountOut);
s32 ISFS_ReadDirAsync(const char* path, char* filesOut, u32* fileCountOut,
                      FSAsyncCallback callback, void* callbackArg);
s32 ISFS_GetAttr(const char* path, u32* ownerIdOut, u16* groupIdOut,
                 u32* attrOut, u32* ownerPermOut, u32* groupPermOut,
                 u32* otherPermOut);
s32 ISFS_GetAttrAsync(const char* path, u32* ownerIdOut, u16* groupIdOut,
                      u32* attrOut, u32* ownerPermOut, u32* groupPermOut,
                      u32* otherPermOut, FSAsyncCallback callback,
                      void* callbackArg);
s32 ISFS_Delete(const char* path);
s32 ISFS_DeleteAsync(const char* path, FSAsyncCallback callback,
                     void* callbackArg);
s32 ISFS_Rename(const char* from, const char* to);
s32 ISFS_RenameAsync(const char* from, const char* to, FSAsyncCallback callback,
                     void* callbackArg);
s32 ISFS_GetUsage(const char* path, s32* blockCountOut, s32* fileCountOut);
s32 ISFS_CreateFile(const char* path, u32 attr, u32 ownerPerm, u32 groupPerm,
                    u32 otherPerm);
s32 ISFS_CreateFileAsync(const char* path, u32 attr, u32 ownerPerm,
                         u32 groupPerm, u32 otherPerm, FSAsyncCallback callback,
                         void* callbackArg);
s32 ISFS_Open(const char* path, IPCOpenMode mode);
s32 ISFS_OpenAsync(const char* path, IPCOpenMode mode, FSAsyncCallback callback,
                   void* callbackArg);
s32 ISFS_GetFileStats(s32 fd, FSFileStats* statsOut);
s32 ISFS_GetFileStatsAsync(s32 fd, FSFileStats* statsOut,
                           FSAsyncCallback callback, void* callbackArg);
s32 ISFS_Seek(s32 fd, s32 offset, IPCSeekMode mode);
s32 ISFS_SeekAsync(s32 fd, s32 offset, IPCSeekMode mode,
                   FSAsyncCallback callback, void* callbackArg);
s32 ISFS_Read(s32 fd, void* dst, s32 len);
s32 ISFS_ReadAsync(s32 fd, void* dst, s32 len, FSAsyncCallback callback,
                   void* callbackArg);
s32 ISFS_Write(s32 fd, const void* src, s32 len);
s32 ISFS_WriteAsync(s32 fd, const void* src, s32 len, FSAsyncCallback callback,
                    void* callbackArg);
s32 ISFS_Close(s32 fd);
s32 ISFS_CloseAsync(s32 fd, FSAsyncCallback callback, void* callbackArg);
s32 ISFS_ShutdownAsync(FSAsyncCallback callback, void* callbackArg);

#ifdef __cplusplus
}
#endif
#endif
