#ifndef EGG_ARCHIVE_H
#define EGG_ARCHIVE_H

#include "egg/core/eggDisposer.h"
#include "rvl/ARC.h"
#include "rvl/NAND.h"
#include <common.h>

typedef void (*FileCallbackFunc)(void *, void *, const ARCDirEntry *, const char *);

namespace EGG {

class Archive : public Disposer {
public:
    struct FileInfo {
        u32 mArcStartOffset;
        u32 mArcLength;
    };
    enum MountType { // enum name is correct, values arent official
        MOUNT_NONE,
        MOUNT_MEM,
        MOUNT_NAND,
    };
    /* 0x10 */ MountType mMountType;
    /* 0x14 */ u32 mRefCount;
    /* 0x18 */ ARCHandle mHandle;
    /* 0x34 */ s32 mDvdEntryNum;        // gets put into first param of DVDFastOpen
    /* 0x38 */ NANDFileInfo *mNandFile; // pointer to NANDFileInfo (was allocated)
    /* 0x3C */ nw4r::ut::Node mLink;

public:
    /* 804935f0 */ virtual ~Archive();

public:
    /* 80493580 */ Archive();
    /* 80493660 */ bool initHandle(void *data, MountType mountType);
    /* 804936d0 */ static Archive *findArchive(void *data);
    /* 80493760 */ static Archive *findArchive(NANDFileInfo *file);
    /* 80493860 */ static Archive *mount(void *data, Heap *pHeap, int align);
    /* 80493950 */ static Archive *mountFST(void *data, Heap *pHeap, int align);          // name assumed
    /* 80493a40 */ static Archive *mountNAND(NANDFileInfo *, Heap *pHeap, int align);     // name assumed
    /* 80493ac0 */ static Archive *loadFST(const char *fileName, Heap *pHeap, int align); // name assumed
    /* 80493b80 */ void unmount();
    /* 80493c20 */ void *getFile(const char *name, FileInfo *out);
    /* 80493cf0 */ s32 convertPathToEntryID(const char *path);
    /* 80493d00 */ void *getFileFast(s32 entryId, FileInfo *fileinfo);
    /* 80493dc0 */ void *getFileFast(s32 entryId, Heap *pHeap, int align);
    /* 80493f30 */ s32 countFile();
    /* 804940c0 */ void searchInside(FileCallbackFunc, void *);

    /* 80494140 */ static void *loadNAND(NANDFileInfo *fileInfo, Heap *pHeap, int align);

    /* 80673ac8 */ static nw4r::ut::List sArchiveList;

private:
    static bool sIsArchiveListInitialized;
    static void appendList(Archive *);
    static void removeList(Archive *);
};
} // namespace EGG

#endif
