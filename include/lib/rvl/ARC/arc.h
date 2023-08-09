#ifndef RVL_SDK_ARC_H
#define RVL_SDK_ARC_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Modified from decompilation by riidefi in WiiCore
 * https://github.com/riidefi/WiiCore/tree/master/source/rx
 */

typedef enum { ARC_ENTRY_FILE, ARC_ENTRY_FOLDER } ARCEntryType;

typedef struct ARCNode {
    union {
        struct {
            u32 is_folder : 8;
            u32 name : 24;
        };
        u32 packed_type_name;
    }; // at 0x0

    union {
        struct {
            u32 offset;
            u32 size;
        } file;

        struct {
            u32 parent;
            u32 sibling_next;
        } folder;
    }; // at 0x4
} ARCNode;

typedef struct ARCHeader {
    u32 magic; // at 0x0

    struct {
        s32 offset; // at 0x4
        s32 size;   // at 0x8
    } nodes;

    struct {
        s32 offset; // at 0xC
    } files;

    char UNK_0x10[0x10];
} ARCHeader;

typedef struct ARCHandle {
    ARCHeader* header;   // at 0x0
    ARCNode* nodes;      // at 0x4
    u8* file;            // at 0x8
    u32 count;           // at 0xC
    const char* strings; // at 0x10
    u32 fstSize;         // at 0x14
    s32 entrynum;        // at 0x18
} ARCHandle;

typedef struct ARCFileInfo {
    ARCHandle* handle; // at 0x0
    u32 offset;        // at 0x4
    u32 size;          // at 0x8
} ARCFileInfo;

typedef struct ARCEntry {
    ARCHandle* handle; // at 0x0
    u32 path;          // at 0x4
    ARCEntryType type; // at 0x8
    const char* name;  // at 0xC
} ARCEntry;

typedef struct ARCDir {
    ARCHandle* handle; // at 0x0
    u32 path_begin;    // at 0x4
    u32 path_it;       // at 0x8
    u32 path_end;      // at 0xC
} ARCDir;

BOOL ARCGetCurrentDir(ARCHandle* handle, char* string, u32 maxlen);
BOOL ARCInitHandle(void* bin, ARCHandle* handle);
BOOL ARCOpen(ARCHandle* handle, const char* path, ARCFileInfo* info);
BOOL ARCFastOpen(ARCHandle* handle, s32 entrynum, ARCFileInfo* info);
s32 ARCConvertPathToEntrynum(ARCHandle* handle, const char* path);
void* ARCGetStartAddrInMem(ARCFileInfo* info);
s32 ARCGetStartOffset(ARCFileInfo* info);
u32 ARCGetLength(ARCFileInfo* info);
BOOL ARCClose(ARCFileInfo* info);
BOOL ARCChangeDir(ARCHandle* info, const char* path);
BOOL ARCOpenDir(ARCHandle* info, const char* path, ARCDir* dir);
BOOL ARCReadDir(ARCDir* dir, ARCEntry* entry);
BOOL ARCCloseDir(ARCDir* dir);

#ifdef __cplusplus
}
#endif
#endif
