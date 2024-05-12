// Ported From https://github.com/kiwi515/ogws/blob/master/src/nw4r/ut/ut_DvdFileStream.cpp

#include <nw4r/ut.h>

namespace nw4r {
namespace ut {

NW4R_UT_RTTI_DEF_DERIVED(DvdFileStream, FileStream);

void DvdFileStream::DvdAsyncCallback_(long result, DVDFileInfo *info) {
    DvdFileStream *self = reinterpret_cast<AsyncContext *>(info)->stream;

    self->mIsBusy = false;
    self->mResult = result;

    if (self->mCallback != NULL) {
        self->mCallback(result, self, self->mCallbackArg);
    }
}

void DvdFileStream::DvdCBAsyncCallback_(long result, DVDCommandBlock *block) {
    DvdFileStream *self = reinterpret_cast<AsyncContext *>(block)->stream;

    self->mIsCancelling = false;

    if (self->mCancelCallback != NULL) {
        self->mCancelCallback(result, self, self->mCancelCallbackArg);
    }
}

void DvdFileStream::Initialize_() {
    mCloseOnDestroy = false;
    mAllowClose = false;
    mIsOpen = false;
    mPriority = DVD_PRIO_MEDIUM;
    mIsBusy = false;
    mCallback = NULL;
    mCallbackArg = NULL;
    mResult = DVD_RESULT_OK;
    mCancelCallback = NULL;
    mIsCancelling = false;
    mCancelCallbackArg = NULL;
    mAsyncContext.stream = this;
}

DvdFileStream::DvdFileStream(long entrynum) {
    Initialize_();
    Open(entrynum);
}

DvdFileStream::DvdFileStream(const DVDFileInfo *info, bool close) {
    Initialize_();
    Open(info, close);
}

DvdFileStream::~DvdFileStream() {
    if (mCloseOnDestroy) {
        Close();
    }
}

bool DvdFileStream::Open(s32 entrynum) {
    if (mCloseOnDestroy) {
        Close();
    }

    if (DVDFastOpen(entrynum, &mAsyncContext.info)) {
        mFilePosition.SetFileSize(mAsyncContext.info.size);
        mFilePosition.Seek(0, SEEK_BEG);

        mCloseOnDestroy = true;
        mAllowClose = true;
        mIsOpen = true;

        return true;
    }

    return false;
}

bool DvdFileStream::Open(const DVDFileInfo *info, bool close) {
    if (mCloseOnDestroy) {
        Close();
    }

    mAsyncContext.info = *info;
    mFilePosition.SetFileSize(mAsyncContext.info.size);
    mFilePosition.Seek(0, SEEK_BEG);

    mCloseOnDestroy = false;
    mAllowClose = close;
    mIsOpen = true;

    return true;
}

void DvdFileStream::Close() {
    if (mAllowClose && mIsOpen) {
        DVDClose(&mAsyncContext.info);
        mIsOpen = false;
    }
}

long DvdFileStream::Read(void *dst, unsigned long size) {
    size = AdjustReadLength_(size);

    long result = DVDReadPrio(&mAsyncContext.info, dst, size, mFilePosition.Tell(), mPriority);

    if (result > 0) {
        mFilePosition.Skip(result);
    }

    return result;
}

bool DvdFileStream::ReadAsync(void *dst, unsigned long size, AsyncCallback callback, void *arg) {
    size = AdjustReadLength_(size);

    bool success = DvdFileStream::PeekAsync(dst, size, callback, arg);

    if (success) {
        mFilePosition.Skip(size);
    } else {
        mIsBusy = false;
    }

    return success;
}

long DvdFileStream::Peek(void *dst, unsigned long size) {
    size = AdjustReadLength_(size);

    return DVDReadPrio(&mAsyncContext.info, dst, size, mFilePosition.Tell(), mPriority);
}

bool DvdFileStream::PeekAsync(void *dst, unsigned long size, AsyncCallback callback, void *arg) {
    mCallback = callback;
    mCallbackArg = arg;
    mIsBusy = true;

    size = AdjustReadLength_(size);

    return DVDReadAsyncPrio(&mAsyncContext.info, dst, size, mFilePosition.Tell(), DvdAsyncCallback_, mPriority);
}

void DvdFileStream::Seek(long offset, unsigned long origin) {
    mFilePosition.Seek(offset, origin);
}

void DvdFileStream::Cancel() {
    DVDCancel(&mAsyncContext.info.block);
}

bool DvdFileStream::CancelAsync(AsyncCallback callback, void *arg) {
    mCancelCallback = callback;
    mCancelCallbackArg = arg;

    BOOL success = DVDCancelAsync(&mAsyncContext.info.block, DvdCBAsyncCallback_);

    if (success) {
        mIsCancelling = true;
    }

    return success;
}

unsigned long DvdFileStream::AdjustReadLength_(unsigned long len) {
    u32 fileOffset = mFilePosition.Tell();
    u32 fileSize = mFilePosition.GetFileSize();

    u32 alignSize = RoundUp(fileSize, 32);
    u32 alignPos = RoundUp(fileOffset + len, 32);

    if (alignPos > alignSize) {
        len = RoundUp(fileSize - fileOffset, 32);
    }

    return len;
}

} // namespace ut
} // namespace nw4r
