#ifndef EGG_DVD_FILE_H
#define EGG_DVD_FILE_H

#include "common.h"
#include "egg/core/eggFile.h"
#include "nw4r/ut.h" // IWYU pragma: export
#include "rvl/DVD.h" // IWYU pragma: export
#include "rvl/OS.h"  // IWYU pragma: export


namespace EGG {

class DvdFile : public File {
public:
    struct FileInfo : public DVDFileInfo {
        DvdFile *dvdFile;
    };

public:
    /* 0x08 */ OSMutex mMutex;
    /* 0x20 */ OSMutex mMutexUnused;
    /* 0x38 */ u32 field_0x38;
    /* 0x3c */ FileInfo mFileInfo;
    /* 0x7c */ OSMessageQueue mMsgQueueUnsued;
    /* 0x9c */ OSMessage mMsgUnused;
    /* 0xa0 */ OSMessageQueue mMsgQueue;
    /* 0xc0 */ OSMessage mMsg;
    /* 0xc4 */ OSThread *mThread;
    /* 0xc8 */ nw4r::ut::Node mNode;

public:
    /* vt 0x08 */ virtual ~DvdFile();
    /* vt 0x0C */ virtual bool open(const char *path);
    /* vt 0x10 */ virtual void close();
    /* vt 0x14 */ virtual s32 readData(void *buffer, s32 length, s32 offset);
    /* vt 0x18 */ virtual s32 writeData(const void *buffer, s32 length, s32 offset);
    /* vt 0x1C */ virtual u32 getFileSize() const;
    /* vt 0x20 */ virtual bool open(s32 entryNum);
    /* vt 0x24 */ virtual bool open(const char *path, void *);

public:
    void initialize();
    DvdFile();
    void initiate();
    s32 sync();
    static void doneProcess(s32 result, DVDFileInfo *fileInfo);

private:
    static bool sIsInitialized;
    static nw4r::ut::List sDvdList;
};

} // namespace EGG

#endif
