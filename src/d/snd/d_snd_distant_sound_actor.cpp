

#include "d/snd/d_snd_distant_sound_actor.h"

#include "common.h"
#include "d/snd/d_snd_3d_actor.h"
#include "d/snd/d_snd_distant_sound_actor_pool.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_state_mgr.h"

dSndDistantSoundActor_c::dSndDistantSoundActor_c()
    : dSnd3DActor_c(&dSndDistantSoundActorPool_c::GetInstance()->getSourceParam(), SND_SOURCE_DISTANT),
      mpHoldSoundHandle(nullptr),
      mpSoundSource(nullptr),
      mpSoundHandle(&mSoundHandle),
      mIsActive(false),
      mUseSourcePosition(false) {}

void dSndDistantSoundActor_c::initSource(dSoundSource_c *pSource) {
    resetCachedRelativePositions();
    loadDefaultParam();
    setSourceDirectly(pSource);
    mUseSourcePosition = false;
}

void dSndDistantSoundActor_c::setSourceDirectly(dSoundSource_c *pSource) {
    mpSoundSource = pSource;
    if (mpSoundSource != nullptr) {
        setSourceParam(pSource->getSourceParam());
    }
}

void dSndDistantSoundActor_c::updatePosition() {
    if (mpSoundSource != nullptr && mUseSourcePosition) {
        dSnd3DActor_c::setPosition(mpSoundSource->GetPosition());
    } else {
        dSnd3DActor_c::setPosition(GetPosition());
    }
}

void dSndDistantSoundActor_c::postStartSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id) {
    if (mpSoundSource != nullptr) {
        mpSoundSource->postStartSound(handle, pSound, id);
    }
}

void dSndDistantSoundActor_c::postHoldSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id, UNKWORD arg) {
    if (mpSoundSource != nullptr) {
        mpSoundSource->postHoldSound(handle, pSound, id, arg);
    }
}

bool dSndDistantSoundActor_c::startSound(
    u32 soundId, const nw4r::math::VEC3 &position, nw4r::snd::SoundHandle *pHandle
) {
    if (mIsDisabled) {
        return false;
    }

    if (pHandle == nullptr) {
        pHandle = &mSoundHandle;
    } else {
        mpSoundHandle = pHandle;
    }

    SetPosition(position);
    updateSome3DField();
    return StartSound(pHandle, soundId);
}

bool dSndDistantSoundActor_c::holdSound(
    u32 soundId, const nw4r::math::VEC3 &position, nw4r::snd::SoundHandle *pHandle
) {
    if (mIsDisabled) {
        return false;
    }

    if (pHandle == nullptr) {
        pHandle = &mSoundHandle;
    } else {
        mpSoundHandle = pHandle;
    }

    SetPosition(position);
    return HoldSound(pHandle, soundId);
}

void dSndDistantSoundActor_c::loadDefaultParam() {
    setSourceParam(&dSndDistantSoundActorPool_c::GetInstance()->getSourceParam());
}

void dSndDistantSoundActor_c::detachFromSource() {
    if (mpSoundSource != nullptr) {
        mpSoundSource->detachDistantSound(this);
    }
    mpSoundSource = nullptr;
    mpHoldSoundHandle = nullptr;
}

void dSndDistantSoundActor_c::setPause(bool flag, int fadeFrames) {
    if (mIsPaused && !flag) {
        PauseAllSound(flag, fadeFrames);
        mIsPaused = false;
    } else if (!mIsPaused && flag) {
        PauseAllSound(flag, fadeFrames);
        mIsPaused = true;
    }
}

void dSndDistantSoundActor_c::updateSome3DField() {
    a_field_0xE0 = dSndStateMgr_c::GetInstance()->getField_0x49C();
}

UNKWORD dSndDistantSoundActor_c::d_vt_0x3C() {
    if (mpSoundSource != nullptr) {
        return mpSoundSource->d_vt_0x3C();
    }
    return 0;
}
