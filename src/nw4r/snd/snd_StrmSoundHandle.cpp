#include "nw4r/snd/snd_StrmSoundHandle.h"

#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_StrmSound.h"
#include "nw4r/ut/ut_RuntimeTypeInfo.h"

namespace nw4r {
namespace snd {

StrmSoundHandle::StrmSoundHandle(SoundHandle *pHandle) : mSound(NULL) {
    if (pHandle == NULL) {
        return;
    }

    if (!pHandle->IsAttachedSound()) {
        return;
    }

    detail::StrmSound *s = ut::DynamicCast<detail::StrmSound *>(pHandle->detail_GetAttachedSound());
    if (s != NULL) {
        mSound = s;
        if (mSound->IsAttachedTempSpecialHandle()) {
            mSound->DetachTempSpecialHandle();
        }
        mSound->mTempSpecialHandle = this;
    }
}

void StrmSoundHandle::DetachSound() {
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
