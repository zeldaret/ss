#include "nw4r/snd/snd_StrmSound.h"
#include "nw4r/snd/snd_SoundInstanceManager.h"
#include "nw4r/snd/snd_StrmSoundHandle.h"

namespace nw4r {
namespace snd {
namespace detail {

NW4R_UT_RTTI_DEF_DERIVED(StrmSound, BasicSound);

StrmSound::StrmSound(SoundInstanceManager<StrmSound>* pManager)
    : mManager(pManager), mTempSpecialHandle(NULL) {}

bool StrmSound::Prepare(StrmBufferPool* pPool,
                        StrmPlayer::StartOffsetType offsetType, s32 offset,
                        int voices, ut::FileStream* pStream) {
    if (pPool == NULL) {
        return false;
    }

    InitParam();

    if (!mStrmPlayer.Setup(pPool)) {
        return false;
    }

    if (!mStrmPlayer.Prepare(pStream, voices, offsetType, offset)) {
        mStrmPlayer.Shutdown();
        return false;
    }

    return true;
}

void StrmSound::Shutdown() {
    BasicSound::Shutdown();
    mManager->Free(this);
}

void StrmSound::SetPlayerPriority(int priority) {
    BasicSound::SetPlayerPriority(priority);
    mManager->UpdatePriority(this, CalcCurrentPlayerPriority());
}

bool StrmSound::IsAttachedTempSpecialHandle() {
    return mTempSpecialHandle != NULL;
}

void StrmSound::DetachTempSpecialHandle() {
    mTempSpecialHandle->DetachSound();
}

} // namespace detail
} // namespace snd
} // namespace nw4r
