
// Ported From https://github.com/kiwi515/ogws/blob/master/src/nw4r/ut/ut_NandFileStream.cpp

#include "nw4r/ut.h"

namespace nw4r {
namespace ut {

NW4R_UT_RTTI_DEF_DERIVED(NandFileStream, FileStream);

void NandFileStream::NandAsyncCallback_(s32 result, NANDCommandBlock *block) {
    NandFileStream *self = reinterpret_cast<AsyncContext *>(block)->stream;

    self->mIsBusy = false;
    self->mResult = result;

    if (self->mCallback != NULL) {
        self->mCallback(result, self, self->mCallbackArg);
    }
}

void NandFileStream::Initialize_() {
    mCanRead = false;
    mCanWrite = false;
    mCloseOnDestroy = false;
    mAllowClose = false;
    mIsOpen = false;
    mIsBusy = false;
    mCallback = NULL;
    mCallbackArg = NULL;
    mResult = NAND_RESULT_OK;
    mAsyncContext.stream = this;
}

NandFileStream::NandFileStream(const char *path, u32 access) {
    Initialize_();
    Open(path, access);
}

NandFileStream::NandFileStream(const NANDFileInfo *info, u32 access, bool close) {
    Initialize_();
    Open(info, access, close);
}

NandFileStream::~NandFileStream() {
    if (mCloseOnDestroy) {
        Close();
    }
}

bool NandFileStream::Open(const char *path, u32 access) {
    if (mCloseOnDestroy) {
        Close();
    }

    mCanRead = access & NAND_ACCESS_READ;
    mCanWrite = access & NAND_ACCESS_WRITE;

    if (NANDOpen(path, &mAsyncContext.info, access) != NAND_RESULT_OK) {
        return false;
    }

    if (mCanRead) {
        u32 fileSize;

        if (NANDGetLength(&mAsyncContext.info, &fileSize) != NAND_RESULT_OK) {
            NANDClose(&mAsyncContext.info);
            return false;
        }

        mFilePosition.SetFileSize(fileSize);
    }

    mFilePosition.Seek(0, SEEKORG_BEG);

    mCloseOnDestroy = true;
    mAllowClose = true;
    mIsOpen = true;

    return true;
}

bool NandFileStream::Open(const NANDFileInfo *info, u32 access, bool close) {
    if (mCloseOnDestroy) {
        Close();
    }

    mCanRead = access & NAND_ACCESS_READ;
    mCanWrite = access & NAND_ACCESS_WRITE;

    mAsyncContext.info = *info;

    u32 fileSize;
    if (mCanRead) {
        if (NANDGetLength(&mAsyncContext.info, &fileSize) != NAND_RESULT_OK) {
            if (close) {
                NANDClose(&mAsyncContext.info);
            }
            return false;
        }
        mFilePosition.SetFileSize(fileSize);
    }
    mFilePosition.Seek(0, SEEKORG_BEG);

    mCloseOnDestroy = false;
    mAllowClose = close;
    mIsOpen = true;
    return true;
}

void NandFileStream::Close() {
    if (mAllowClose && mIsOpen) {
        NANDClose(&mAsyncContext.info);
        mIsOpen = false;
    }
}

s32 NandFileStream::Read(void *dst, u32 size) {
    NANDSeek(&mAsyncContext.info, mFilePosition.Tell(), NAND_SEEK_BEG);

    s32 result = NANDRead(&mAsyncContext.info, dst, size);
    if (result > 0) {
        mFilePosition.Skip(result);
    }

    return result;
}

bool NandFileStream::ReadAsync(void *dst, u32 size, AsyncCallback callback, void *arg) {
    mCallback = callback;
    mCallbackArg = arg;
    mIsBusy = true;

    NANDSeek(&mAsyncContext.info, mFilePosition.Tell(), NAND_SEEK_BEG);

    bool success =
        NANDReadAsync(&mAsyncContext.info, dst, size, NandAsyncCallback_, &mAsyncContext.block) == NAND_RESULT_OK;

    if (success) {
        mFilePosition.Skip(size);
    } else {
        mIsBusy = false;
    }

    return success;
}

s32 NandFileStream::Write(const void *src, u32 size) {
    NANDSeek(&mAsyncContext.info, mFilePosition.Tell(), NAND_SEEK_BEG);
    s32 result = NANDWrite(&mAsyncContext.info, src, size);
    if (result > 0) {
        mFilePosition.Append(result);
    }
    return result;
}

bool NandFileStream::WriteAsync(const void *src, u32 size, AsyncCallback callback, void *arg) {
    mCallback = callback;
    mCallbackArg = arg;
    mIsBusy = true;

    NANDSeek(&mAsyncContext.info, mFilePosition.Tell(), NAND_SEEK_BEG);

    s32 result = NANDWriteAsync(&mAsyncContext.info, src, size, NandAsyncCallback_, &mAsyncContext.block);

    if (result == NAND_RESULT_OK) {
        mFilePosition.Append(size);
    } else {
        mIsBusy = false;
    }

    return result == NAND_RESULT_OK;
}

void NandFileStream::Seek(s32 offset, u32 origin) {
    mFilePosition.Seek(offset, origin);
}

} // namespace ut
} // namespace nw4r
