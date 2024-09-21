#ifndef EGG_DVD_FILE_H
#define EGG_DVD_FILE_H

#include "egg/core/eggFile.h"
#include "nw4r/ut.h"
#include "rvl/DVD.h"
#include "rvl/OS.h"
#include <common.h>

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
    /* vt 0x08 | 804942f0 */ virtual ~DvdFile();
    /* vt 0x0C | 80494450 */ virtual bool open(const char *path);
    /* vt 0x10 | 804944b0 */ virtual void close();
    /* vt 0x14 | 80494510 */ virtual s32 readData(void *buffer, s32 length, s32 offset);
    /* vt 0x18 | 804945e0 */ virtual s32 writeData(const void *buffer, s32 length, s32 offset);
    /* vt 0x1C | 80494670 */ virtual u32 getFileSize() const;
    /* vt 0x20 | 804943d0 */ virtual bool open(s32 entryNum);
    /* vt 0x24 | 804944a0 */ virtual bool open(const char *path, void *);

public:
    /* 80494260 */ void initialize();
    /* 804942a0 */ DvdFile();
    /* 80494360 */ void initiate();
    /* 804945f0 */ s32 sync();
    /* 80494650 */ static void doneProcess(s32 result, DVDFileInfo *fileInfo);

private:
    /* 80576730 */ static bool sIsInitialized;
    /* 80673ad8 */ static nw4r::ut::List sDvdList;
};

} // namespace EGG

#endif
