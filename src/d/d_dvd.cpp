#include "d/d_dvd.h"

#include "m/m_dvd.h"

// clang-format off
#include "sized_string.h"
// clang-format on
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

/** 80052170 */
void *loader_c::request(const char *path, u8 mountDirection, EGG::Heap *heap) {
    if (mpBuffer != nullptr) {
        return mpBuffer;
    }
    if (mpCommand == 0) {
        mSize = -1;
        mpHeap = heap != nullptr ? heap : mDvd::getArchiveHeap();
        SizedString<128> buf;
        buf.sprintf("US%s", path);
        if (!mDvd::IsExistPath(buf)) {
            buf.sprintf("%s", path);
        }
        mpCommand = mDvd_toMainRam_normal_c::create(buf, mountDirection, heap);
    }
    if (mpCommand != nullptr && mpCommand->mStatus != 0) {
        mpBuffer = mpCommand->mDataPtr;
        mSize = mpCommand->mAmountRead;
        mpCommand->mDataPtr = nullptr;
        mpCommand->do_delete();
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
