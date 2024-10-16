#include "egg/core/eggDvdFile.h"

namespace EGG {

bool DvdFile::sIsInitialized;
nw4r::ut::List DvdFile::sDvdList;

void DvdFile::initialize() {
    if (!sIsInitialized) {
        nw4r::ut::List_Init(&sDvdList, offsetof(DvdFile, mNode));
        sIsInitialized = true;
    }
}

DvdFile::DvdFile() {
    initiate();
}

DvdFile::~DvdFile() {
    close();
}

void DvdFile::initiate() {
    mFileInfo.dvdFile = this;
    OSInitMutex(&mMutex);
    OSInitMutex(&mMutexUnused);
    OSInitMessageQueue(&mMsgQueue, &mMsg, 1);
    OSInitMessageQueue(&mMsgQueueUnsued, &mMsgUnused, 1);
    mThread = nullptr;
    field_0x38 = 0;
}

bool DvdFile::open(s32 entryId) {
    if (!mIsOpen && entryId != -1) {
        s32 res = DVDFastOpen(entryId, &mFileInfo);
        mIsOpen = res;
        if (mIsOpen) {
            nw4r::ut::List_Append(&sDvdList, this);
            s32 _status = DVDGetCommandBlockStatus(&mFileInfo.block);
        }
    }
    return mIsOpen;
}

bool DvdFile::open(const char *path) {
    return open(DVDConvertPathToEntrynum(path));
}

bool DvdFile::open(const char *path, void *) {
    return open(path);
}

void DvdFile::close() {
    if (mIsOpen && DVDClose(&mFileInfo)) {
        mIsOpen = false;
        nw4r::ut::List_Remove(&sDvdList, this);
    }
}

s32 DvdFile::readData(void *buffer, s32 length, s32 offset) {
    OSLockMutex(&mMutex);
    if (mThread) {
        OSUnlockMutex(&mMutex);
        return -1;
    }

    mThread = OSGetCurrentThread();
    s32 res = -1;

    if (DVDReadAsyncPrio(&mFileInfo, buffer, length, offset, doneProcess, DVD_PRIO_MEDIUM)) {
        res = sync();
    }

    mThread = nullptr;
    OSUnlockMutex(&mMutex);

    return res;
}

s32 DvdFile::writeData(const void *buffer, s32 length, s32 offset) {
    return -1;
}

s32 DvdFile::sync() {
    OSMessage msg;
    OSLockMutex(&mMutex);
    OSReceiveMessage(&mMsgQueue, &msg, OS_MSG_PERSISTENT);
    mThread = nullptr;
    OSUnlockMutex(&mMutex);
    return (s32)(msg);
}

void DvdFile::doneProcess(s32 result, DVDFileInfo *pFileInfo) {
    OSSendMessage(&((FileInfo *)pFileInfo)->dvdFile->mMsgQueue, (OSMessage)result, 0);
}

u32 DvdFile::getFileSize() const {
    return mFileInfo.size;
}

} // namespace EGG
