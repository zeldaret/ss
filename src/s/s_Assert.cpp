#include "s/s_Assert.h"

#include "common.h"

namespace sAssert {

typedef void (*AssertCallback)(void *);

AssertCallback sAssertCallback;

void setAssertCallback(AssertCallback cb) {
    sAssertCallback = cb;
}

void assert() {
    if (sAssertCallback != nullptr) {
        (sAssertCallback)(nullptr);
    }
    while (true) {}
}

} // namespace sAssert
