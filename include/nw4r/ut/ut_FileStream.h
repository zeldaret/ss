#ifndef NW4R_UT_FILE_STREAM_H
#define NW4R_UT_FILE_STREAM_H
#include <nw4r/types_nw4r.h>
#include <nw4r/ut/ut_IOStream.h>

namespace nw4r {
namespace ut {

class FileStream : public IOStream {
public:
    NW4R_UT_RTTI_DECL(FileStream);

    enum SeekOrigin { SEEK_BEG, SEEK_CUR, SEEK_END };

    class FilePosition {
    public:
        FilePosition() : mFileSize(0), mFileOffset(0) {}

        u32 GetFileSize() const { return mFileSize; }
        void SetFileSize(u32 fileSize) { mFileSize = fileSize; }

        u32 Tell() const { return mFileOffset; }

        u32 Skip(s32 offset);
        u32 Append(s32 offset);
        void Seek(s32 offset, u32 origin);

    private:
        u32 mFileSize;   // at 0x0
        u32 mFileOffset; // at 0x4
    };

public:
    FileStream() {}
    virtual ~FileStream() {} // at 0xC

    virtual u32 GetSize() const = 0;                             // at 0x40
    virtual void Seek(s32 offset, u32 origin);                   // at 0x44
    virtual void Cancel();                                       // at 0x48
    virtual bool CancelAsync(AsyncCallback callback, void* arg); // at 0x4C
    virtual bool CanSeek() const = 0;                            // at 0x50
    virtual bool CanCancel() const = 0;                          // at 0x54
    virtual u32 Tell() const = 0;                                // at 0x58
};

} // namespace ut
} // namespace nw4r

#endif
