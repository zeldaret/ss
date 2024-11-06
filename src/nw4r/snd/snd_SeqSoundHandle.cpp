#include "nw4r/snd/snd_SeqSoundHandle.h"

#include "nw4r/snd/snd_SoundHandle.h"

namespace nw4r {
namespace snd {

SeqSoundHandle::SeqSoundHandle(SoundHandle* pHandle) : mSound(NULL) {
    if (pHandle == NULL) {
        return;
    }

    if (!pHandle->IsAttachedSound()) {
        return;
    }

    detail::SeqSound* pSound =
        ut::DynamicCast<detail::SeqSound*>(pHandle->detail_GetAttachedSound());

    if (pSound != NULL) {
        mSound = pSound;

        if (mSound->IsAttachedTempSpecialHandle()) {
            mSound->DetachTempSpecialHandle();
        }

        mSound->mTempSpecialHandle = this;
    }
}

void SeqSoundHandle::DetachSound() {
    if (IsAttachedSound()) {
        if (mSound->mTempSpecialHandle == this) {
            mSound->mTempSpecialHandle = NULL;
        }
    }

    if (mSound != NULL) {
        mSound = NULL;
    }
}

} // namespace snd
} // namespace nw4r
