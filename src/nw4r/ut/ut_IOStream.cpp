// Ported from https://github.com/kiwi515/ogws/blob/master/src/nw4r/ut/ut_IOStream.cpp

#include <nw4r/ut.h>

namespace nw4r {
namespace ut {

NW4R_UT_RTTI_DEF_BASE(IOStream);

bool IOStream::ReadAsync(void *dst, unsigned long size, AsyncCallback callback, void *arg) {
    return false;
}

long IOStream::Write(const void *src, unsigned long size) {
    return false;
}

bool IOStream::WriteAsync(const void *src, unsigned long size, AsyncCallback callback, void *arg) {
    return false;
}

bool IOStream::IsBusy() const {
    return false;
}

} // namespace ut
} // namespace nw4r
