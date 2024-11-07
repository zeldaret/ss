#include "nw4r/snd/snd_WaveSoundHandle.h"

#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_WaveSound.h"
#include "nw4r/ut/ut_RuntimeTypeInfo.h"

namespace nw4r {
namespace snd {

WaveSoundHandle::WaveSoundHandle(SoundHandle *pHandle) : mSound(NULL) {
    if (pHandle == NULL) {
        return;
    } 
    
    if (!pHandle->IsAttachedSound()) {
        return;
    }

    detail::WaveSound *pSound = ut::DynamicCast<detail::WaveSound *>(pHandle->detail_GetAttachedSound());
    if (pSound != NULL) {
        mSound = pSound;
        if (mSound->IsAttachedTempSpecialHandle()) {
            mSound->DetachTempSpecialHandle();
        }
        mSound->mTempSpecialHandle = this;
    }
}

void WaveSoundHandle::DetachSound() {
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
