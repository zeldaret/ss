#ifndef RVL_SDK_NAND_OPEN_CLOSE_H
#define RVL_SDK_NAND_OPEN_CLOSE_H
#include <lib/rvl/NAND/nand.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

NANDResult NANDOpen(const char* path, NANDFileInfo* info, u8 mode);
NANDResult NANDPrivateOpen(const char* path, NANDFileInfo* info, u8 mode);
NANDResult NANDOpenAsync(const char* path, NANDFileInfo* info, u8 mode,
                         NANDAsyncCallback callback, NANDCommandBlock* block);
NANDResult NANDPrivateOpenAsync(const char* path, NANDFileInfo* info, u8 mode,
                                NANDAsyncCallback callback,
                                NANDCommandBlock* block);

NANDResult NANDClose(NANDFileInfo* info);
NANDResult NANDCloseAsync(NANDFileInfo* info, NANDAsyncCallback callback,
                          NANDCommandBlock* block);

NANDResult NANDPrivateSafeOpenAsync(const char* path, NANDFileInfo* info,
                                    u8 access, void* buffer, u32 bufferSize,
                                    NANDAsyncCallback callback,
                                    NANDCommandBlock* block);
NANDResult NANDSafeCloseAsync(NANDFileInfo* info, NANDAsyncCallback callback,
                              NANDCommandBlock* block);

#ifdef __cplusplus
}
#endif
#endif
