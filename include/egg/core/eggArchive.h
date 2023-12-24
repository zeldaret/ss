#pragma once

#include "egg/core/eggDisposer.h"
#include "rvl/ARC.h"
#include "rvl/NAND.h"
#include <common.h>


typedef void (*FileCallbackFunc)(void *, void *, const ARCDirEntry *, const char *);

namespace EGG {
// Functions that need a proper home:
/* 80493510 */ void readNANDFile(NANDFileInfo *info, void *buf, u32 len, s32 offs);

class Archive : public Disposer {
public:
    struct FileInfo {
        u32 mArcStartOffset;
        u32 mArcLength;
    };
    enum MountType { // enum name is correct, values arent official
        NOT_LOADED,
        LOADED_CAN_FAST_READ,
        NAND_File,
    };
    /* 0x10 */ MountType mMountType;
    /* 0x14 */ u32 mRefCount;
    /* 0x18 */ ARCHandle mHandle;
    /* 0x34 */ s32 mDvdEntryNum; // gets put into first param of DVDFastOpen
    /* 0x38 */ s32 mNandFile;    // pointer to NANDFileInfo (was allocated)
    /* 0x3C */ nw4r::ut::Node mLink;

public:
    /* 804935f0 */ virtual ~Archive();

public:
    /* 80493580 */ Archive();
    /* 80493660 */ void initHandle(void *data, MountType mountType);
    /* 804936d0 */ static Archive *findArchive(void *data);
    /* 80493760 */ static Archive *findArchive(NANDFileInfo *file);
    /* 804937f0 */ static void appendList(Archive *); // prob private
    /* 80493850 */ static void removeList(Archive *);
    /* 80493860 */ static Archive *mount(void *data, Heap *heap, s32 align);
    /* 80493950 */ static Archive *mountFST(void *data, Heap *heap, s32 align);
    /* 80493a40 */ static Archive *mountNandFile(NANDFileInfo *, Heap *heap, s32 align);
    /* 80493ac0 */ static Archive *loadFST(const char *fileName, Heap *heap, s32 align);
    /* 80493b80 */ void unmount();
    /* 80493c20 */ u32 getFile(const char *name, FileInfo *out);
    /* 80493cf0 */ long convertPathToEntryID(const char *path);
    /* 80493d00 */ void *getFileFast(long entryId, FileInfo *fileinfo);
    /* 80493dc0 */ void *getFileFast(long entryId, Heap *heap, s32 align);
    /* 80493f30 */ s32 countFile();
    /* 804940c0 */ void searchInside(FileCallbackFunc, void *);
    /* 80494140 */ static void *loadFromNAND(NANDFileInfo *fileInfo, Heap *heap, s32 align);

public:
    /* 80576728 */ static bool sIsArchiveListInitialized;
    /* 80673ac8 */ static nw4r::ut::List sArchiveList;
};
/* 80493f10 */ void countFileCallbackFunc(void *, void *, const ARCDirEntry *, const char *);
/* 80493f70 */ void searchInsideWithPath(Archive *arc, ARCHandle *handle, FileCallbackFunc, void *, char *outPath, u32);
} // namespace EGG
