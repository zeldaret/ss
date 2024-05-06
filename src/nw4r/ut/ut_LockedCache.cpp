#include <nw4r/ut.h>

namespace nw4r {
namespace ut {
namespace {

class LCImpl {
public:
    class Lock_ {
    public:
        Lock_(LCImpl &lc) : mMutex(lc.mMutex) {
            OSLockMutex(&mMutex);
        }
        ~Lock_() {
            OSUnlockMutex(&mMutex);
        }

    private:
        OSMutex &mMutex; // at 0x0
    };

public:
    LCImpl() : mIsEnabled(false) {
        OSInitMutex(&mMutex);
    }

    void Enable() {
        Lock_ lock(*this);

        if (!mIsEnabled) {
            LCEnable();
            mIsEnabled = true;
        }
    }

    void Disable() {
        Lock_ lock(*this);

        if (mIsEnabled) {
            LC::QueueWaitEx(0);
            LCDisable();
            mIsEnabled = false;
        }
    }

    bool Lock() {
        OSLockMutex(&mMutex);

        if (mIsEnabled) {
            LC::QueueWaitEx(0);
            return true;
        }

        OSUnlockMutex(&mMutex);
        return false;
    }

    void Unlock() {
        LC::QueueWaitEx(0);
        OSUnlockMutex(&mMutex);
    }

private:
    bool mIsEnabled; // at 0x0
    OSMutex mMutex;  // at 0x4
};

static LCImpl sLCImpl;

} // namespace

namespace LC {

void Enable() {
    sLCImpl.Enable();
}

void Disable() {
    sLCImpl.Disable();
}

bool Lock() {
    return sLCImpl.Lock();
}

void Unlock() {
    sLCImpl.Unlock();
}

void LoadBlocks(void *dst, void *src, unsigned long size) {
    LCLoadBlocks(dst, src, size);
}

void StoreBlocks(void *dst, void *src, unsigned long size) {
    LCStoreBlocks(dst, src, size);
}

void StoreData(void *dst, void *src, unsigned long size) {
    LCStoreData(dst, src, size);
}

} // namespace LC
} // namespace ut
} // namespace nw4r
