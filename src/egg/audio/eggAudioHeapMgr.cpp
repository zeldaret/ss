#include "egg/audio/eggAudioHeapMgr.h"

namespace EGG {

void SoundHeapMgr::createSoundHeap(Allocator *allocator, u32 size) {
    if (!mSoundHeap.IsValid()) {
        void *ptr = allocator->alloc(size);
        if (ptr != nullptr) {
            mSoundHeap.Create(ptr, size);
        }
    }
}

void SoundHeapMgr::destroySoundHeap() {
    mSoundHeap.Destroy();
}

} // namespace EGG
