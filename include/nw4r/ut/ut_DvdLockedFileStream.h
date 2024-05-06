#ifndef NW4R_UT_DVD_LOCKED_FILE_STREAM_H
#define NW4R_UT_DVD_LOCKED_FILE_STREAM_H
#include <nw4r/types_nw4r.h>
#include <nw4r/ut/ut_DvdFileStream.h>
#include <rvl/OS.h>

namespace nw4r {
namespace ut {

class DvdLockedFileStream : public DvdFileStream {
public:
    NW4R_UT_RTTI_DECL(DvdLockedFileStream);

    DvdLockedFileStream(long entrynum);
    DvdLockedFileStream(const DVDFileInfo *info, bool close);
    virtual ~DvdLockedFileStream(); // at 0xC

    virtual void Close(); // at 0x10

    virtual long Read(void *dst, unsigned long size); // at 0x14
    virtual bool ReadAsync(void *dst, unsigned long size, AsyncCallback callback, void *arg) {
        return false;
    } // at 0x18

    virtual long Peek(void *dst, unsigned long size); // at 0x5C
    virtual bool PeekAsync(void *dst, unsigned long size, AsyncCallback callback, void *arg) {
        return false;
    } // at 0x60

    virtual bool CanAsync() const {
        return false;
    } // at 0x28

    void Cancel();

private:
    static void InitMutex_();
    bool LockMutex();
    static void UnlockMutex();
    void CancelMutex();

private:
    volatile bool mCancelFlag; // 0x6f
    static bool sInitialized;
    static OSThreadQueue sThreadQueue;
    static OSMutex sMutex;
};

} // namespace ut
} // namespace nw4r
#endif
