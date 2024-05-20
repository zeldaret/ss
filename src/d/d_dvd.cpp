#include <d/d_dvd.h>
#include <m/m_dvd.h>

namespace dDvd {

/** 800520f0 */
void create(s32 priority, EGG::Heap *archiveHeap, EGG::Heap *commandHeap, EGG::Heap *threadHeap) {
    mDvd::create(priority, archiveHeap, commandHeap, threadHeap);
}

/** 80052100 */
loader_c::loader_c() {
    mpCommand = nullptr;
    mSize = -1;
    mpHeap = nullptr;
    mpBuffer = nullptr;
}

/** 80052130 */
loader_c::~loader_c() {}

// sprintf2
extern "C" void fn_8003D650(char *out, const char *fmt, ...);

/** 80052170 */
void *loader_c::request(const char *path, u8 mountDirection, EGG::Heap *heap) {
    char buf[128];

    if (mpBuffer != nullptr) {
        return mpBuffer;
    }
    if (mpCommand == 0) {
        mSize = -1;
        mpHeap = heap != nullptr ? heap : mDvd::getArchiveHeap();
        buf[0] = '\0';
        fn_8003D650(buf, "US%s", path);
        if (!mDvd::IsExistPath(buf)) {
            fn_8003D650(buf, "%s", path);
        }
        mpCommand = mDvd_toMainRam_normal_c::create(buf, mountDirection, heap);
    }
    if (mpCommand != nullptr && mpCommand->mStatus != 0) {
        mpBuffer = mpCommand->mDataPtr;
        mSize = mpCommand->mAmountRead;
        mpCommand->mDataPtr = nullptr;
        fn_802EF480(mpCommand);
        mpCommand = nullptr;
        return mpBuffer;
    } else {
        return nullptr;
    }
}

/** 800522a0 */
void loader_c::remove() {
    if (mpHeap != nullptr && mpBuffer != nullptr) {
        mpHeap->free(mpBuffer);
        mpBuffer = nullptr;
    }
}

} // namespace dDvd
