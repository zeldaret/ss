#ifndef NW4R_UT_NAND_FILE_STREAM_H
#define NW4R_UT_NAND_FILE_STREAM_H
#include <nw4r/types_nw4r.h>
#include <nw4r/ut/ut_FileStream.h>
#include <rvl/NAND.h>

namespace nw4r {
namespace ut {

class NandFileStream : public FileStream {
public:
    NW4R_UT_RTTI_DECL(NandFileStream);

    /**
     * NAND callbacks need a reference to the NW4R stream. This structure is
     * used to up-cast the NANDCommandBlock given to the async callback.
     *
     * This structure's NANDCommandBlock is always used in calls to async NAND
     * functions, so the up-cast is always safe.
     */
    struct AsyncContext {
        NANDCommandBlock block; // at 0x0
        NANDFileInfo info;      // at 0xB8
        NandFileStream *stream; // at 0x144
    };

public:
    NandFileStream(const char *path, u32 access);
    NandFileStream(const NANDFileInfo *info, u32 access, bool close);
    virtual ~NandFileStream(); // at 0xC

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
        return mCanRead;
    } // at 0x2C
    virtual bool CanWrite() const {
        return mCanWrite;
    } // at 0x30
    virtual bool CanCancel() const {
        return false;
    } // at 0x54

    virtual u32 GetOffsetAlign() const {
        return 1;
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

    virtual s32 Write(const void *src, u32 size); // at 0x1C
    virtual bool WriteAsync(const void *src, u32 size, AsyncCallback callback,
            void *arg); // at 0x20

    virtual void Seek(s32 offset, u32 origin); // at 0x44

    bool Open(const char *path, u32 access);
    bool Open(const NANDFileInfo *info, u32 access, bool close) DECOMP_DONT_INLINE;

private:
    static void NandAsyncCallback_(s32 result, NANDCommandBlock *block);

    void Initialize_();

private:
    FilePosition mFilePosition; // at 0x14
    AsyncContext mAsyncContext; // at 0x1C
    bool mCanRead;              // at 0x168
    bool mCanWrite;             // at 0x169
    volatile bool mIsBusy;      // at 0x16A
    bool mCloseOnDestroy;       // at 0x16B
    bool mAllowClose;           // at 0x16C
};

} // namespace ut
} // namespace nw4r

#endif
