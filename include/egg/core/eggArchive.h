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
    virtual ~Archive();

public:
    Archive();
    bool initHandle(void *data, MountType mountType);
    static Archive *findArchive(void *data);
    static Archive *findArchive(NANDFileInfo *file);
    static Archive *mount(void *data, Heap *pHeap, int align);
    static Archive *mountFST(void *data, Heap *pHeap, int align);          // name assumed
    static Archive *mountNAND(NANDFileInfo *, Heap *pHeap, int align);     // name assumed
    static Archive *loadFST(const char *fileName, Heap *pHeap, int align); // name assumed
    void unmount();
    void *getFile(const char *name, FileInfo *out);
    s32 convertPathToEntryID(const char *path);
    void *getFileFast(s32 entryId, FileInfo *fileinfo);
    void *getFileFast(s32 entryId, Heap *pHeap, int align);
    s32 countFile();
    void searchInside(FileCallbackFunc, void *);

    static void *loadNAND(NANDFileInfo *fileInfo, Heap *pHeap, int align);

    static nw4r::ut::List sArchiveList;

private:
    static bool sIsArchiveListInitialized;
    static void appendList(Archive *);
    static void removeList(Archive *);
};
} // namespace EGG

#endif
