#ifndef NW4R_UT_IO_STREAM_H
#define NW4R_UT_IO_STREAM_H
#include <nw4r/types_nw4r.h>
#include <nw4r/ut/ut_RuntimeTypeInfo.h>

namespace nw4r {
namespace ut {

class IOStream {
public:
    NW4R_UT_RTTI_DECL(IOStream);

    typedef void (*AsyncCallback)(s32 result, IOStream* stream, void* arg);

    IOStream() : mIsOpen(false), mCallback(NULL), mCallbackArg(NULL) {}
    virtual ~IOStream() {} // at 0xC

    virtual void Close() = 0;                  // at 0x10
    virtual s32 Read(void* dst, u32 size) = 0; // at 0x14
    virtual bool ReadAsync(void* dst, u32 size, AsyncCallback callback,
                           void* arg);             // at 0x18
    virtual void Write(const void* src, u32 size); // at 0x1C
    virtual bool WriteAsync(const void* src, u32 size, AsyncCallback callback,
                            void* arg);              // at 0x20
    virtual bool IsBusy() const;                     // at 0x24
    virtual bool CanAsync() const = 0;               // at 0x28
    virtual bool CanRead() const = 0;                // at 0x2C
    virtual bool CanWrite() const = 0;               // at 0x30
    virtual u32 GetOffsetAlign() const { return 1; } // at 0x34
    virtual u32 GetSizeAlign() const { return 1; }   // at 0x38
    virtual u32 GetBufferAlign() const { return 1; } // at 0x3C

    bool IsAvailable() const { return mIsOpen; }

protected:
    bool mIsOpen;            // at 0x4
    s32 mResult;             // at 0x8
    AsyncCallback mCallback; // at 0xC
    void* mCallbackArg;      // at 0x10
};

} // namespace ut
} // namespace nw4r

#endif
