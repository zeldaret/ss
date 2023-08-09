#ifndef RVL_SDK_CNT_H
#define RVL_SDK_CNT_H
#include <lib/rvl/ARC.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CNTHandle {
    ARCHandle arcHandle; // at 0x0
    s32 fd;              // at 0x1C
} CNTHandle;

typedef struct CNTFileInfo {
    CNTHandle* handle; // at 0x0
    u32 offset;        // at 0x4
    u32 length;        // at 0x8
    u32 position;      // at 0xC
} CNTFileInfo;

s32 contentFastOpenNAND(CNTHandle* handle, s32 entrynum, CNTFileInfo* info);
s32 contentConvertPathToEntrynumNAND(CNTHandle* handle, const char* path);
u32 contentGetLengthNAND(CNTFileInfo* info);
s32 contentReadNAND(CNTFileInfo* info, void* dst, s32 len, s32 offset);
s32 contentCloseNAND(CNTFileInfo* info);

#ifdef __cplusplus
}
#endif
#endif
