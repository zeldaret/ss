#include <RVL/OS.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace ut {

NW4R_UT_RTTI_DEF_DERIVED(DvdLockedFileStream, DvdFileStream);

OSThreadQueue DvdLockedFileStream::sThreadQueue;
OSMutex DvdLockedFileStream::sMutex;
bool DvdLockedFileStream::sInitialized = false;

void DvdLockedFileStream::InitMutex_() {
    BOOL enabled = OSDisableInterrupts();

    if (!sInitialized) {
        OSInitMutex(&sMutex);
        OSInitThreadQueue(&sThreadQueue);
        sInitialized = true;
    }

    OSRestoreInterrupts(enabled);
}

DvdLockedFileStream::DvdLockedFileStream(long entrynum) : DvdFileStream(entrynum) {
    mCancelFlag = false;
    InitMutex_();
}

DvdLockedFileStream::DvdLockedFileStream(const DVDFileInfo *info, bool close) : DvdFileStream(info, close) {
    mCancelFlag = false;
    InitMutex_();
}

DvdLockedFileStream::~DvdLockedFileStream() {}

void DvdLockedFileStream::Close() {
    DvdFileStream::Close();
    mCancelFlag = false;
}

long DvdLockedFileStream::Read(void *dst, unsigned long size) {
    if (!LockMutex()) {
        return DVD_RESULT_CANCELED;
    }
    long ret = DvdFileStream::Read(dst, size);
    UnlockMutex();
    return ret;
}

long DvdLockedFileStream::Peek(void *dst, unsigned long size) {
    if (!LockMutex()) {
        return DVD_RESULT_CANCELED;
    }
    long ret = DvdFileStream::Peek(dst, size);
    UnlockMutex();
    return ret;
}

void DvdLockedFileStream::Cancel() {
    CancelMutex();
    DvdFileStream::Cancel();
}

bool DvdLockedFileStream::LockMutex() {
    BOOL enabled = OSDisableInterrupts();
    while (!OSTryLockMutex(&sMutex)) {
        OSSleepThread(&sThreadQueue);
        if (mCancelFlag) {
            OSRestoreInterrupts(enabled);
            return false;
        }
    };
    OSRestoreInterrupts(enabled);
    return true;
}

void DvdLockedFileStream::UnlockMutex() {
    BOOL enabled = OSDisableInterrupts();
    OSUnlockMutex(&sMutex);
    OSWakeupThread(&sThreadQueue);
    OSRestoreInterrupts(enabled);
}

void DvdLockedFileStream::CancelMutex() {
    BOOL enabled = OSDisableInterrupts();
    mCancelFlag = true;
    OSWakeupThread(&sThreadQueue);
    OSRestoreInterrupts(enabled);
}

} // namespace ut
} // namespace nw4r
