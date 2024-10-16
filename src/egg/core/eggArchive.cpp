#include "egg/core/eggArchive.h"

#include "egg/core/eggHeap.h"
#include "string.h"


namespace EGG {

bool Archive::sIsArchiveListInitialized;
nw4r::ut::List Archive::sArchiveList;

s32 readNANDFile(NANDFileInfo *info, void *buf, u32 len, s32 offs) {
    s32 seek = NANDSeek(info, offs, NAND_SEEK_BEG);
    if (seek == offs) {
        return NANDRead(info, buf, len);
    }
    return seek;
}

Archive::Archive() {
    mDvdEntryNum = -1;
    mNandFile = nullptr;
    mRefCount = 1;
    mMountType = MOUNT_NONE;
    memset(&mHandle, 0, sizeof(ARCHandle));
    appendList(this);
}

Archive::~Archive() {
    removeList(this);
}

bool Archive::initHandle(void *data, MountType mount) {
    bool success = false;

    if (data) {
        success = ARCInitHandle(data, &mHandle);
    }

    if (success) {
        mMountType = mount;
    } else {
        mMountType = MOUNT_NONE;
    }

    return success;
}

Archive *Archive::findArchive(void *data) {
    Archive *arc = nullptr;

    if (sIsArchiveListInitialized && data) {
        Archive *node = nullptr;
        while (node = (Archive *)nw4r::ut::List_GetNext(&sArchiveList, node)) {
            if (node->mHandle.header == data) {
                arc = node;
                break;
            }
        }
    }

    return arc;
}

Archive *Archive::findArchive(NANDFileInfo *data) {
    Archive *arc = nullptr;

    if (sIsArchiveListInitialized && data) {
        Archive *node = nullptr;
        while (node = (Archive *)nw4r::ut::List_GetNext(&sArchiveList, node)) {
            if (node->mNandFile == data) {
                arc = node;
                break;
            }
        }
    }

    return arc;
}

void Archive::appendList(Archive *arc) {
    if (!sIsArchiveListInitialized) {
        nw4r::ut::List_Init(&sArchiveList, offsetof(Archive, mLink));
        sIsArchiveListInitialized = true;
    }

    nw4r::ut::List_Append(&sArchiveList, arc);
}

void Archive::removeList(Archive *arc) {
    nw4r::ut::List_Remove(&sArchiveList, arc);
}

Archive *Archive::mount(void *data, Heap *pHeap, int align) {
    if (data == nullptr) {
        return nullptr;
    }

    Archive *arc = findArchive(data);
    if (!arc) {
        arc = new (pHeap, align) Archive();

        if (!arc || !arc->initHandle(data, MOUNT_MEM)) {
            delete arc;
            arc = nullptr;
        }
    } else {
        arc->mRefCount++;
    }

    return arc;
}

Archive *Archive::mountFST(void *data, Heap *pHeap, int align) {
    if (data == nullptr) {
        return nullptr;
    }

    Archive *arc = findArchive(data);
    if (!arc) {
        arc = new (pHeap, align) Archive();

        if (!arc || !arc->initHandle(data, MOUNT_NAND)) {
            delete arc;
            arc = nullptr;
        }
    } else {
        arc->mRefCount++;
    }

    return arc;
}

Archive *Archive::mountNAND(NANDFileInfo *nandFile, Heap *pHeap, int align) {
    Archive *arc = findArchive(nandFile);
    if (!arc) {
        void *arc_bin = loadNAND(nandFile, pHeap, align);
        arc = mountFST(arc_bin, pHeap, align);
        arc->mNandFile = nandFile;
    } else {
        arc->mRefCount++;
    }
    return arc;
}

Archive *Archive::loadFST(const char *fileName, Heap *pHeap, int align) {
    NANDFileInfo *nandFile = (NANDFileInfo *)pHeap->alloc(sizeof(NANDFileInfo), align > 0 ? -0x20 : 0x20);

    if (NANDOpen(fileName, nandFile, NAND_ACCESS_READ) != NAND_RESULT_OK) {
        pHeap->free(nandFile);
        return nullptr;
    }

    return mountNAND(nandFile, pHeap, align);
}

void Archive::unmount() {
    if (mRefCount == 0) {
        return;
    }
    if (--mRefCount) {
        return;
    }

    if (mNandFile) {
        NANDClose(mNandFile);
        Heap::free(mNandFile, nullptr);
        mNandFile = nullptr;
    }
    mDvdEntryNum = -1;
    mMountType = MOUNT_NONE;
    delete this;
}

void *Archive::getFile(const char *path, FileInfo *fileInfo) {
    ARCFileInfo arcFileInfo;
    void *file = nullptr;
    bool opened = false;
    int entryNum = ARCConvertPathToEntrynum(&mHandle, path);

    if (entryNum != -1) {
        opened = ARCFastOpen(&mHandle, entryNum, &arcFileInfo);
    }

    if (opened) {
        if (mMountType == MOUNT_MEM) {
            file = ARCGetStartAddrInMem(&arcFileInfo);
        }
        if (fileInfo) {
            fileInfo->mArcStartOffset = ARCGetStartOffset(&arcFileInfo);
            fileInfo->mArcLength = ARCGetLength(&arcFileInfo);
        }
    }

    ARCClose(&arcFileInfo);
    return file;
}

s32 Archive::convertPathToEntryID(const char *path) {
    return ARCConvertPathToEntrynum(&mHandle, path);
}

void *Archive::getFileFast(s32 entryId, FileInfo *fileInfo) {
    if (entryId == -1) {
        if (fileInfo) {
            fileInfo->mArcStartOffset = 0;
            fileInfo->mArcLength = 0;
        }
        return nullptr;
    }

    void *file = nullptr;
    if (mMountType == MOUNT_MEM) {
        ARCFileInfo arcFileInfo;
        bool opened = ARCFastOpen(&mHandle, entryId, &arcFileInfo);
        if (opened) {
            file = ARCGetStartAddrInMem(&arcFileInfo);
            if (fileInfo) {
                fileInfo->mArcStartOffset = ARCGetStartOffset(&arcFileInfo);
                fileInfo->mArcLength = ARCGetLength(&arcFileInfo);
            }
        }
        ARCClose(&arcFileInfo);
    }

    return file;
}

void *Archive::getFileFast(s32 entryId, Heap *pHeap, int align) {
    if (entryId == -1) {
        return nullptr;
    }

    u32 file_len;
    void *file = nullptr;
    ARCFileInfo arcFileInfo;
    bool opened = ARCFastOpen(&mHandle, entryId, &arcFileInfo);

    if (opened) {
        file_len = ROUND_UP(ARCGetLength(&arcFileInfo), 0x20);
        file = pHeap->alloc(file_len, align);
        if (file) {
            if (mMountType == MOUNT_MEM) {
                void *startAddr = ARCGetStartAddrInMem(&arcFileInfo);
                memcpy(file, startAddr, file_len);
            } else {
                u32 offs = ARCGetStartOffset(&arcFileInfo);
                if (mNandFile) {
                    readNANDFile(mNandFile, file, file_len, offs);
                } else {
                    if (mDvdEntryNum != -1) {
                        DVDFileInfo dvdFileInfo;
                        DVDFastOpen(mDvdEntryNum, &dvdFileInfo);
                        DVDReadPrio(&dvdFileInfo, file, file_len, offs, DVD_PRIO_MEDIUM);
                        DVDClose(&dvdFileInfo);
                    }
                }
            }
        }
    }
    ARCClose(&arcFileInfo);
    return file;
}

void countFileCallbackFunc(void *arg1, void *arg2, const ARCDirEntry *, const char *) {
    if (!arg2) {
        return;
    }
    (*(u32 *)arg1)++;
    return;
}

s32 Archive::countFile() {
    u32 count = 0;
    searchInside(countFileCallbackFunc, &count);
    return count;
}

void searchInsideWithPath(
    Archive *arc, ARCHandle *handle, FileCallbackFunc cb, void *cbArg, char *outPath, u32 max_len
) {
    ARCDir dir;
    if (ARCOpenDir(handle, outPath, &dir)) {
        ARCDirEntry dirEntry;
        while (ARCReadDir(&dir, &dirEntry)) {
            size_t path_len = strlen(outPath);
            if (dirEntry.isDir) {
                size_t dir_len = strlen(dirEntry.name) + 1;
                strcat(outPath, dirEntry.name);
                cb(cbArg, nullptr, &dirEntry, outPath);
                strcat(outPath, "/");
                searchInsideWithPath(arc, handle, cb, cbArg, outPath, max_len - dir_len);
            } else {
                size_t _unused = strlen(dirEntry.name);
                strcat(outPath, dirEntry.name);
                cb(cbArg, arc->getFile(outPath, nullptr), &dirEntry, outPath);
            }
            outPath[path_len] = '\0';
        }
        ARCCloseDir(&dir);
    }
}

void Archive::searchInside(FileCallbackFunc cb, void *cbArg) {
    char buffer[256];
    strcpy(buffer, "/");
    size_t bufSize = sizeof(buffer);
    bufSize -= 1 + strlen(buffer);
    searchInsideWithPath(this, &mHandle, cb, cbArg, buffer, bufSize);
}

void *Archive::loadNAND(NANDFileInfo *fileInfo, Heap *pHeap, int align) {
    int file_align = align >= 0 ? 0x20 : -0x20;
    void *arc_file = nullptr;
    ARCHeader *header = (ARCHeader *)pHeap->alloc(sizeof(ARCHeader), -file_align);
    if (header) {
        s32 readres = readNANDFile(fileInfo, header, sizeof(ARCHeader), 0);
        if (readres >= sizeof(ARCHeader)) {
            u32 file_len = ROUND_UP(header->nodes.size + sizeof(ARCHeader), 0x20);
            arc_file = pHeap->alloc(file_len, file_align);
            if (arc_file) {
                readres = readNANDFile(fileInfo, arc_file, file_len, 0);
                if (readres != file_len) {
                    pHeap->free(arc_file);
                    arc_file = nullptr;
                }
            }
        }
        pHeap->free(header);
    }
    return arc_file;
}

} // namespace EGG
