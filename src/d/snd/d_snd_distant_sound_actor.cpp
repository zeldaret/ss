

#include "d/snd/d_snd_distant_sound_actor.h"

#include "common.h"
#include "d/snd/d_snd_3d_actor.h"
#include "d/snd/d_snd_distant_sound_actor_pool.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_enums.h"

dSndDistantSoundActor_c::dSndDistantSoundActor_c()
    : dSnd3DActor_c(&dSndDistantSoundActorPool_c::GetInstance()->getSourceParam(), SND_SOURCE_DISTANT),
      field_0x0F4(0),
      mpSoundSource(nullptr),
      mpSoundHandle(&mSoundHandle),
      field_0x104(0),
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

bool dSndDistantSoundActor_c::startSound(
    u32 soundId, const nw4r::math::VEC3 &position, nw4r::snd::SoundHandle *pHandle
) {
    if (a_field_0x7E) {
        return false;
    }

    if (pHandle == nullptr) {
        pHandle = &mSoundHandle;
    } else {
        mpSoundHandle = pHandle;
    }

    SetPosition(position);
    return StartSound(pHandle, soundId);
}

bool dSndDistantSoundActor_c::holdSound(
    u32 soundId, const nw4r::math::VEC3 &position, nw4r::snd::SoundHandle *pHandle
) {
    if (a_field_0x7E) {
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
    field_0x0F4 = 0;
}

void dSndDistantSoundActor_c::setPause(bool flag, int fadeFrames) {
    if (mIsPaused && !flag) {
        PauseAllSound(flag, fadeFrames);
        mIsPaused = 0;
    } else if (!mIsPaused && flag) {
        PauseAllSound(flag, fadeFrames);
        mIsPaused = 1;
    }
}
