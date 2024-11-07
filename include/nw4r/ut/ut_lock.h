#ifndef NW4R_UT_LOCK_H
#define NW4R_UT_LOCK_H
#include "nw4r/types_nw4r.h"
#include "nw4r/ut/ut_NonCopyable.h"
#include "rvl/OS.h" // IWYU pragma: export

namespace nw4r {
namespace ut {
namespace detail {

inline void Lock(OSMutex &mutex) {
    OSLockMutex(&mutex);
}
inline void Unlock(OSMutex &mutex) {
    OSUnlockMutex(&mutex);
}

template <typename T>
class AutoLock : private NonCopyable {
public:
    AutoLock(T &obj) : mMutex(obj) {
        Lock(mMutex);
    }
    ~AutoLock() {
        Unlock(mMutex);
    }

private:
    T &mMutex; // at 0x0
};

} // namespace detail

class AutoInterruptLock : private NonCopyable {
public:
    AutoInterruptLock() : mOldState(OSDisableInterrupts()) {}
    ~AutoInterruptLock() {
        OSRestoreInterrupts(mOldState);
    }

private:
    BOOL mOldState; // at 0x0
};

} // namespace ut
} // namespace nw4r

#endif
