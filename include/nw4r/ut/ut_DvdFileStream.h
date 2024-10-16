#ifndef NW4R_UT_DVD_FILE_STREAM_H
#define NW4R_UT_DVD_FILE_STREAM_H

#include "nw4r/types_nw4r.h"
#include "nw4r/ut/ut_FileStream.h"
#include "rvl/DVD.h" // IWYU pragma: export

namespace nw4r {
namespace ut {

class DvdFileStream : public FileStream {
public:
    NW4R_UT_RTTI_DECL(DvdFileStream);

    /**
     * DVD callbacks need a reference to the NW4R stream. This structure is
     * used to up-cast the DVDFileInfo given to the async callback.
     *
     * This structure's DVDFileInfo is always used in calls to async DVD
     * functions, so the up-cast is always safe.
     */
    struct AsyncContext {
        DVDFileInfo info;      // at 0x0
        DvdFileStream *stream; // at 0x3C
    };

public:
    DvdFileStream(s32 entrynum);
    DvdFileStream(const DVDFileInfo *info, bool close);
    virtual ~DvdFileStream(); // at 0xC

    virtual bool IsBusy() const {
        return mIsBusy;
    } // at 0x24

    virtual u32 Tell() const {
        return mFilePosition.Tell();
    } // at 0x58
    virtual u32 GetSize() const {
        return mFilePosition.GetFileSize();
    } // at 0x40

    virtual bool CanAsync() const {
        return true;
    } // at 0x28
    virtual bool CanSeek() const {
        return true;
    } // at 0x50
    virtual bool CanRead() const {
        return true;
    } // at 0x2C
    virtual bool CanWrite() const {
        return false;
    } // at 0x30
    virtual bool CanCancel() const {
        return true;
    } // at 0x54

    virtual u32 GetOffsetAlign() const {
        return 4;
    } // at 0x34
    virtual u32 GetSizeAlign() const {
        return 32;
    } // at 0x38
    virtual u32 GetBufferAlign() const {
        return 32;
    } // at 0x3C

    virtual void Close(); // at 0x10

    virtual s32 Read(void *dst, u32 size); // at 0x14
    virtual bool ReadAsync(void *dst, u32 size, AsyncCallback callback,
                           void *arg); // at 0x18

    virtual s32 Peek(void *dst, u32 size); // at 0x5C
    virtual bool PeekAsync(void *dst, u32 size, AsyncCallback callback,
                           void *arg); // at 0x60

    virtual void Seek(s32 offset, u32 origin); // at 0x44

    virtual void Cancel();                                       // at 0x48
    virtual bool CancelAsync(AsyncCallback callback, void *arg); // at 0x4C

    void SetPriority(s32 priority) {
        mPriority = priority;
    }

    bool Open(s32 entrynum);
    bool Open(const DVDFileInfo *info, bool close);

private:
    static void DvdAsyncCallback_(s32 result, DVDFileInfo *info);
    static void DvdCBAsyncCallback_(s32 result, DVDCommandBlock *block);

    void Initialize_();
    u32 AdjustReadLength_(u32 len);

private:
    FilePosition mFilePosition;    // at 0x14
    AsyncCallback mCancelCallback; // at 0x1C
    void *mCancelCallbackArg;      // at 0x20
    volatile bool mIsCancelling;   // at 0x24
    AsyncContext mAsyncContext;    // at 0x28
    s32 mPriority;                 // at 0x68
    volatile bool mIsBusy;         // at 0x6C
    bool mCloseOnDestroy;          // at 0x6D
    bool mAllowClose;              // at 0x6E
};

} // namespace ut
} // namespace nw4r
#endif
