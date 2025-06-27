#include "d/snd/d_snd_source.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/snd/d_snd_3d_actor.h"
#include "d/snd/d_snd_3d_manager.h"
#include "d/snd/d_snd_anim_sound.h"
#include "d/snd/d_snd_checkers.h"
#include "d/snd/d_snd_distant_sound_actor.h"
#include "d/snd/d_snd_distant_sound_actor_pool.h"
#include "d/snd/d_snd_id_mappers.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_se_sound.h"
#include "d/snd/d_snd_se_sound_1.h"
#include "d/snd/d_snd_se_sound_2.h"
#include "d/snd/d_snd_se_sound_pool.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_source_group.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_stage_data.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/audio/eggAudioRmtSpeakerMgr.h"
#include "m/m_vec.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/ut/ut_list.h"
#include "sized_string.h"
#include "toBeSorted/time_area_mgr.h"

static dSndSoundCtxParam sCtxParams;
bool dSoundSource_c::sIsStartingBaseSound;

dSoundSourceIf_c *dSoundSourceIf_c::create(u32 id, dAcBase_c *actor, const char *name, u8 subtype) {
    return dSndSourceMgr_c::createSource(id, actor, name, subtype);
}

dSoundSource_c::dSoundSource_c(u8 sourceType, dAcBase_c *actor, const char *name, dSndSourceGroup_c *pOwnerGroup)
    : dSnd3DActor_c(pOwnerGroup->getAmbientParam(), sourceType),
      mpName(name),
      field_0x0F4(0),
      mpActor(actor),
      mSourceType(sourceType),
      mSubtype(0),
      mIsSetup(false),
      field_0x100(0),
      field_0x101(0),
      field_0x102(0),
      mFlagsPersistent(0),
      mFlagsHold(0),
      mpCtxParam(nullptr),
      field_0x11C(0),
      mpOwnerGroup(pOwnerGroup),
      field_0x154(0),
      mPolyAttr0(-1),
      mPolyAttr1(-1) {
    mSourceCategory = dSndSourceMgr_c::getSourceCategoryForSourceType(sourceType, name);
    // TODO: Offsetof
    nw4r::ut::List_Init(&mDistantSoundList, 0xEC);
    nw4r::ut::List_Init(&mHandleType1List, 0x04);
    nw4r::ut::List_Init(&mHandleType2List, 0x04);
    pOwnerGroup->registerSource(this);
}

dSoundSource_c::~dSoundSource_c() {
    SetUserParam(0);
    shutdown();
    removeAllSeHandles();
    detachAllDistantSounds();
    dSndSourceMgr_c::GetInstance()->unregisterSource(this);
    mpOwnerGroup->unregisterSource(this);
}

void dSoundSource_c::setup() {
    if (mIsSetup) {
        return;
    }
    SetPosition(mpActor->position);
    resetCachedRelativePositions();
    initVolumeFade();
    if (mSourceCategory == SND_SOURCE_CATEGORY_PLAYER) {
        mPositionRelativeToPlayer.x = 0.0f;
        mPositionRelativeToPlayer.y = 0.0f;
        mPositionRelativeToPlayer.z = 0.0f;
    }
    mIsPaused = false;
    field_0x102 = 0;
    switch (mSourceCategory) {
        case SND_SOURCE_CATEGORY_ENEMY:
        case SND_SOURCE_CATEGORY_OBJECT:
        case SND_SOURCE_CATEGORY_NPC:
        case SND_SOURCE_CATEGORY_TG_SOUND:
        case SND_SOURCE_CATEGORY_HARP_RELATED:
            if (dSndSourceMgr_c::GetInstance()->getField_0x0013() != 0 && mSourceType != SND_SOURCE_SHUTTER) {
                field_0x101 = 1;
            } else {
                field_0x101 = 0;
            }
    }

    postSetup();
    dSndSourceMgr_c::GetInstance()->registerSource(this);
    s32 id = dSndStateMgr_c::GetInstance()->getStageId_0x040();
    if (SND_STAGE_F000_L26 <= id && id <= SND_STAGE_F000_L27) {
        setVolumeFade(0.0f, 0);
    }
    field_0x100 = dSndSourceMgr_c::isSwOrEOc(mpName);
    mIsSetup = true;
}

void dSoundSource_c::shutdown() {
    if (!mIsDisabled) {
        if (getAnimSound() != nullptr) {
            getAnimSound()->shutdown();
        }
        if (mIsPaused) {
            stopAllSound(1);
        }
        dSndSourceMgr_c::GetInstance()->onShutdownSource(this);
        mIsDisabled = true;
    }
}

void dSoundSource_c::stopAllPlaySounds(s32 fadeFrames) {
    dSndSeSound1_c *sound = getHandleType1First();
    while (sound != nullptr) {
        sound->getHandle()->Stop(fadeFrames);
        sound = getHandleType1Next(sound);
    }
}

const nw4r::math::VEC3 &dSoundSource_c::getListenerPosition() const {
    return dSnd3DManager_c::GetInstance()->getSndListenerPos();
}

void dSoundSource_c::calc(const nw4r::math::VEC3 &pos) {
    preCalc();
    setPosition(pos);
    calcHandles();
    calcVolumeFade();
    postCalc();
    mFlagsHold = 0;
}

void dSoundSource_c::preCalc() {
    if (field_0x100 != 0) {
        return;
    }

    if (dSndSourceMgr_c::GetInstance()->getField_0x0012() != 0) {
        return;
    }

    if (!mIsPaused && !mpActor->isBasePropertyFlag(dBase_c::BASE_PROP_0x4)) {
        setPause(true, 3);
    } else if (mIsPaused && mpActor->isBasePropertyFlag(dBase_c::BASE_PROP_0x4)) {
        setPause(false, 3);
    }
}

void dSoundSource_c::postCalc() {
    // noop
}

bool dSoundSource_c::hasPlayingSounds() const {
    return dSnd3DActor_c::hasPlayingSounds();
}

bool dSoundSource_c::isPlayingSound(u32 soundId) {
    return dSnd3DActor_c::isPlayingSound(soundId);
}

bool dSoundSource_c::isPlayingSound(const char *soundId) {
    return isPlayingSound(soundLabelToSoundId(soundId));
}

void dSoundSource_c::initVolumeFade() {
    mVolumeFadeTarget = 1.0f;
    mVolumeFadeStepSize = 0.0f;
}

void dSoundSource_c::setVolumeFade(f32 volume, u32 fadeFrames) {
    // @bug not actually clamped
    (void)nw4r::ut::Clamp(volume, 0.0f, 2.0f);
    if (fadeFrames == 0) {
        SoundActor::SetVolume(volume);
        mVolumeFadeTarget = volume;
        mVolumeFadeStepSize = 0.0f;
        return;
    }
    f32 currVolume = SoundActor::GetVolume();
    if (currVolume == volume) {
        mVolumeFadeTarget = volume;
        mVolumeFadeStepSize = 0.0f;
    } else {
        mVolumeFadeTarget = volume;
        mVolumeFadeStepSize = (volume - currVolume) / fadeFrames;
    }
}

void dSoundSource_c::calcVolumeFade() {
    if (mIsPaused) {
        return;
    }

    if (mVolumeFadeStepSize == 0.0f) {
        return;
    }

    f32 nextVolume = SoundActor::GetVolume();
    nextVolume += mVolumeFadeStepSize;
    if (mVolumeFadeStepSize > 0.0f) {
        if (nextVolume >= mVolumeFadeTarget) {
            nextVolume = mVolumeFadeTarget;
            mVolumeFadeStepSize = 0.0f;
        }
    } else {
        if (nextVolume <= mVolumeFadeTarget) {
            nextVolume = mVolumeFadeTarget;
            mVolumeFadeStepSize = 0.0f;
        }
    }

    SoundActor::SetVolume(nextVolume);
}

void dSoundSource_c::setCtxInt(s32 value) {
    sCtxParams.intParam = value;
    setCtxParam(&sCtxParams);
}

void dSoundSource_c::setCtxFloat(f32 value) {
    sCtxParams.floatParam = value;
    setCtxParam(&sCtxParams);
}

bool dSoundSource_c::isInaudibleInternal() {
    if (field_0x102 != 0) {
        return true;
    }

    return dSnd3DActor_c::getDistanceToCameraTarget() >= mpSourceParam->getField0x10();
}

bool dSoundSource_c::d_s_vt_0x190() const {
    if (field_0x101 != 0) {
        return true;
    }
    if (mIsDisabled) {
        return true;
    }
    return dSndStateMgr_c::GetInstance()->getField_0x14() > 0;
}

void dSoundSource_c::setPause(bool flag, int fadeFrames) {
    if (!flag) {
        PauseAllSound(flag, fadeFrames);
        pauseAllDistantSounds(flag, fadeFrames);
        mIsPaused = false;
    } else if (!mIsDisabled) {
        PauseAllSound(flag, fadeFrames);
        pauseAllDistantSounds(flag, fadeFrames);
        mIsPaused = true;
    }
}

bool dSoundSource_c::startSound(u32 soundId) {
    return startSound(soundId, nullptr);
}

bool dSoundSource_c::startSound(const char *label) {
    return startSound(soundLabelToSoundId(label));
}

bool dSoundSource_c::startSoundWithIntParam(u32 soundId, s32 value) {
    setCtxInt(value);
    return startSound(soundId, nullptr) != nullptr;
}

bool dSoundSource_c::startSoundWithIntParam(const char *label, s32 value) {
    u32 soundId = soundLabelToSoundId(label);
    return startSoundWithIntParam(soundId, value);
}

bool dSoundSource_c::startSoundWithFloatParam(u32 soundId, f32 value) {
    setCtxFloat(value);
    return startSound(soundId, nullptr) != nullptr;
}

bool dSoundSource_c::startSoundWithFloatParam(const char *label, f32 value) {
    u32 soundId = soundLabelToSoundId(label);
    return startSoundWithFloatParam(soundId, value);
}

bool dSoundSource_c::startSoundWithParams(u32 soundId, f32 fValue, s32 value) {
    setCtxInt(value);
    setCtxFloat(fValue);
    return startSound(soundId, nullptr) != nullptr;
}

bool dSoundSource_c::startSoundWithParams(const char *label, f32 fValue, s32 value) {
    u32 soundId = soundLabelToSoundId(label);
    return startSoundWithParams(soundId, fValue, value);
}

bool dSoundSource_c::maybeUnusedEnemyDeath() {
    if (mSourceCategory == SND_SOURCE_CATEGORY_ENEMY) {
        dSndSourceMgr_c::GetInstance()->onShutdownSource(this);
        if (getAnimSound() != nullptr) {
            getAnimSound()->shutdown();
        }
        return startSound(SE_E_DISAPPEAR);
    }
    return false;
}

bool dSoundSource_c::startRemoConSound(u32 soundId) {
    nw4r::snd::SoundHandle *outHandle = nullptr;
    StartResult result = startSound(soundId, nullptr, &outHandle);
    if (result == START_SUCCESS && EGG::AudioRmtSpeakerMgr::getWpadVolume() != 0) {
        outHandle->SetOutputLineFlag(nw4r::snd::OUTPUT_LINE_REMOTE_1);
    }
    return result == START_SUCCESS;
}

nw4r::snd::SoundHandle *dSoundSource_c::startSound(u32 soundId, nw4r::snd::SoundHandle *handle) {
    nw4r::snd::SoundHandle *outHandle = nullptr;
    StartResult result = startSound(soundId, handle, &outHandle);
    if (result == START_SUCCESS) {
        return outHandle;
    }
    return nullptr;
}

nw4r::snd::SoundStartable::StartResult
dSoundSource_c::startSound(u32 soundId, nw4r::snd::SoundHandle *handle, nw4r::snd::SoundHandle **pOutHandle) {
    StartResult result;

    dSndSeSound1_c *seHandle = dSndSeSoundPool_c::GetInstance()->acquireSoundType1(this, soundId);
    if (seHandle == nullptr) {
        resetTempParams();
        return START_ERR_USER;
    }

    seHandle->shiftParam(mpCtxParam);
    if (handle != nullptr) {
        seHandle->setHandle(handle);
    } else {
        handle = seHandle->getHandle();
    }

    field_0x11C |= 0x80000000;
    result = StartSoundReturnStatus(handle, soundId, nullptr);
    if (result == START_SUCCESS) {
        addSeHandleType1(seHandle);
        postStartSound(*handle, seHandle, soundId);
        postSetupSound(handle->GetId(), soundId, seHandle);
        seHandle->onStart(this);
        seHandle->onCalc(this);
    } else {
        dSndSeSoundPool_c::GetInstance()->releaseSoundType1(seHandle, true);
        handle = nullptr;
    }
    resetTempParams();
    if (pOutHandle != nullptr) {
        *pOutHandle = handle;
    }
    return result;
}

bool dSoundSource_c::startSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position) {
    if (!d_s_vt_0x1AC(soundId)) {
        return false;
    }

    u32 actualSoundId = soundId;
    if (!sIsStartingBaseSound) {
        actualSoundId = overrideStartSoundId(soundId);
        if (actualSoundId == -1) {
            resetTempParams();
            return false;
        }
    }
    dSndDistantSoundActor_c *ac = startSoundAtPosition(actualSoundId, position, nullptr);

    if (ac != nullptr) {
        if (ac->getHandle() != nullptr) {
            actualSoundId = ac->getHandle()->GetId();
        }
        f32 volume = getBaseSoundVolume(actualSoundId, soundId);
        startBaseSoundAtPosition(soundId, position, volume);
    }
    return ac != nullptr;
}

bool dSoundSource_c::startSoundAtPosition(const char *label, const nw4r::math::VEC3 *position) {
    return startSoundAtPosition(soundLabelToSoundId(label), position);
}

bool dSoundSource_c::startObjHitSound(u32 baseSoundId, dSoundSourceIf_c *source, const nw4r::math::VEC3 *position) {
    if (source == nullptr) {
        return startSoundAtPosition(baseSoundId, position);
    }

    if (!d_s_vt_0x1AC(baseSoundId)) {
        return false;
    }

    u32 soundId = baseSoundId;
    if (!sIsStartingBaseSound) {
        u32 tmp = overrideStartSoundId(soundId);
        if (tmp == -1) {
            resetTempParams();
            return false;
        }
        soundId = overrideHitObjSoundId(tmp, static_cast<dSoundSource_c *>(source));
        if (soundId == -1) {
            resetTempParams();
            return false;
        }
    }
    dSndDistantSoundActor_c *ac = startSoundAtPosition(soundId, position, nullptr);
    if (ac != nullptr) {
        if (ac->getHandle() != nullptr) {
            soundId = ac->getHandle()->GetId();
        }
        f32 volume = getBaseSoundVolume(soundId, baseSoundId);
        startBaseSoundAtPosition(baseSoundId, position, volume);
    }
    return ac != nullptr;
}

bool dSoundSource_c::startObjHitSound(const char *label, dSoundSourceIf_c *source, const nw4r::math::VEC3 *position) {
    return startObjHitSound(soundLabelToSoundId(label), source, position);
}

bool dSoundSource_c::startBgHitSound(u32 baseSoundId, u32 polyAttr0, u32 polyAttr1, const nw4r::math::VEC3 *position) {
    if (!d_s_vt_0x1AC(baseSoundId)) {
        return false;
    }

    u32 soundId = baseSoundId;
    if (!sIsStartingBaseSound) {
        u32 tmp = overrideStartSoundId(soundId);
        if (tmp == -1) {
            resetTempParams();
            return false;
        }
        soundId = specializeBgHitSoundId(tmp, polyAttr0, polyAttr1);
        if (soundId == -1) {
            resetTempParams();
            return false;
        }
    }
    dSndDistantSoundActor_c *ac = startSoundAtPosition(soundId, position, nullptr);
    if (ac != nullptr) {
        if (ac->getHandle() != nullptr) {
            soundId = ac->getHandle()->GetId();
        }
        f32 volume = getBaseSoundVolume(soundId, baseSoundId);
        startBaseSoundAtPosition(baseSoundId, position, volume);
    }
    return ac != nullptr;
}

bool dSoundSource_c::startBgHitSound(
    const char *label, u32 polyAttr0, u32 polyAttr1, const nw4r::math::VEC3 *position
) {
    return startBgHitSound(soundLabelToSoundId(label), polyAttr0, polyAttr1, position);
}

// exact copy of startSoundAtPosition - used only for when the hookshot
// hits the water level
bool dSoundSource_c::startSoundAtPosition2(u32 soundId, const nw4r::math::VEC3 *position) {
    if (!d_s_vt_0x1AC(soundId)) {
        return false;
    }

    u32 actualSoundId = soundId;
    if (!sIsStartingBaseSound) {
        actualSoundId = overrideStartSoundId(soundId);
        if (actualSoundId == -1) {
            resetTempParams();
            return false;
        }
    }
    dSndDistantSoundActor_c *ac = startSoundAtPosition(actualSoundId, position, nullptr);

    if (ac != nullptr) {
        if (ac->getHandle() != nullptr) {
            actualSoundId = ac->getHandle()->GetId();
        }
        f32 volume = getBaseSoundVolume(actualSoundId, soundId);
        startBaseSoundAtPosition(soundId, position, volume);
    }
    return ac != nullptr;
}

bool dSoundSource_c::startSoundAtPosition2(const char *label, const nw4r::math::VEC3 *position) {
    return startSoundAtPosition2(soundLabelToSoundId(label), position);
}

dSndDistantSoundActor_c *
dSoundSource_c::startSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position, nw4r::snd::SoundHandle *handle) {
    if (position == nullptr) {
        position = &GetPosition();
    }
    dSndSeSound1_c *seHandle = nullptr;
    if (handle != nullptr) {
        seHandle = getHandleType1ForSoundHandle(handle);
    }
    if (seHandle == nullptr) {
        seHandle = dSndSeSoundPool_c::GetInstance()->acquireSoundType1(this, soundId);
        if (seHandle == nullptr) {
            resetTempParams();
            return nullptr;
        }
    }

    dSndDistantSoundActor_c *ac = dSndDistantSoundActorPool_c::GetInstance()->acquireActor(soundId, position, this);
    if (ac == nullptr) {
        resetTempParams();
        return nullptr;
    }

    seHandle->setAcquiredMaybe(true);
    seHandle->shiftParam(mpCtxParam);
    if (handle != nullptr) {
        seHandle->setHandle(handle);
    } else {
        handle = seHandle->getHandle();
    }

    if (ac->startSound(soundId, *position, handle)) {
        addSeHandleType1(seHandle);
        seHandle->setInUseMaybe(true);
        ac->postStartSound(*handle, seHandle, soundId);
        postSetupSound(handle->GetId(), soundId, seHandle);
        seHandle->onStart(this);
        seHandle->onCalc(this);
    } else {
        dSndSeSoundPool_c::GetInstance()->releaseSoundType1(seHandle, true);
        ac = nullptr;
    }
    resetTempParams();
    return ac;
}

u32 dSoundSource_c::overrideHitObjSoundId(u32 soundId, dSoundSource_c *source) {
    if (source == nullptr) {
        return soundId;
    }
    if (checkUserParamMask(soundId, 0x200)) {
        return modifySoundId(soundId, source);
    }
    return soundId;
}

void dSoundSource_c::handleRemoCon(nw4r::snd::SoundHandle *pHandle, u32 baseId) {
    u32 userParam = getSomeUserParam(baseId);
    if ((userParam & 0x8) != 0) {
        if (EGG::AudioRmtSpeakerMgr::getWpadVolume() != 0) {
            pHandle->SetOutputLineFlag(nw4r::snd::OUTPUT_LINE_REMOTE_1);
        }
    } else if ((userParam & 0x4) != 0) {
        pHandle->SetOutputLineFlag(nw4r::snd::OUTPUT_LINE_MAIN | nw4r::snd::OUTPUT_LINE_REMOTE_1);
    }

    if ((userParam & 0x10) != 0) {
        if (EGG::AudioRmtSpeakerMgr::getWpadVolume() != 0) {
            u32 variant = getRemoConSoundVariant(baseId);
            if (variant != -1) {
                startRemoConSound(variant);
            }
        }
    }
}

bool dSoundSource_c::startVoiceLine(const char *label) {
    return startVoiceLine(soundLabelToSoundId(label));
}

s32 dSoundSource_c::getRoomId() const {
    return mpActor->roomid;
}

void dSoundSource_c::stopSounds(u32 soundId, s32 fadeFrames) {
    SoundStopper handler(soundId, fadeFrames);
    ForEachSound(handler, false);
}

void dSoundSource_c::stopSounds(const char *label, s32 fadeFrames) {
    stopSounds(soundLabelToSoundId(label), fadeFrames);
}

bool dSoundSource_c::holdSound(u32 soundId) {
    return holdSound(soundId, nullptr);
}

bool dSoundSource_c::holdSound(const char *label) {
    return holdSound(soundLabelToSoundId(label));
}

bool dSoundSource_c::holdSoundWithIntParam(u32 soundId, s32 value) {
    setCtxInt(value);
    return holdSound(soundId, nullptr) != nullptr;
}

bool dSoundSource_c::holdSoundWithIntParam(const char *label, s32 value) {
    u32 soundId = soundLabelToSoundId(label);
    return holdSoundWithIntParam(soundId, value);
}

bool dSoundSource_c::holdSoundWithFloatParam(u32 soundId, f32 value) {
    setCtxFloat(value);
    return holdSound(soundId, nullptr) != nullptr;
}

bool dSoundSource_c::holdSoundWithFloatParam(const char *label, f32 value) {
    u32 soundId = soundLabelToSoundId(label);
    return holdSoundWithFloatParam(soundId, value);
}

bool dSoundSource_c::holdSoundWithParams(u32 soundId, f32 fValue, s32 value) {
    setCtxInt(value);
    setCtxFloat(fValue);
    return holdSound(soundId, nullptr) != nullptr;
}

bool dSoundSource_c::holdSoundWithParams(const char *label, f32 fValue, s32 value) {
    u32 soundId = soundLabelToSoundId(label);
    return holdSoundWithParams(soundId, fValue, value);
}

nw4r::snd::SoundHandle *dSoundSource_c::holdSound(u32 soundId, nw4r::snd::SoundHandle *handle) {
    if (soundId == -1) {
        return nullptr;
    }

    dSndSeSound2_c *seHandle = getHandleType2ForSoundId(soundId);
    if (seHandle == nullptr) {
        seHandle = dSndSeSoundPool_c::GetInstance()->acquireSoundType2(this, soundId);
        if (seHandle == nullptr) {
            resetTempParams();
            return nullptr;
        }

        seHandle->shiftParam(mpCtxParam);

        if (handle != nullptr) {
            seHandle->setHandle(handle);
        } else {
            handle = seHandle->getHandle();
        }

        field_0x11C |= 0x80000000;
        if (HoldSound(handle, soundId)) {
            addSeHandleType2(seHandle);
            postHoldSound(*handle, seHandle, soundId, 0);
            postSetupSound(handle->GetId(), soundId, seHandle);
            seHandle->onStart(this);
            seHandle->onCalc(this);
        } else {
            dSndSeSoundPool_c::GetInstance()->releaseSoundType2(seHandle);
            handle = nullptr;
        }
    } else {
        handle = continueHoldingSound(seHandle, soundId, nullptr, nullptr);
    }
    resetTempParams();

    return handle;
}

nw4r::snd::SoundHandle *dSoundSource_c::continueHoldingSound(
    dSndSeSound2_c *seHandle, u32 soundId, dSndDistantSoundActor_c *distant, const nw4r::math::VEC3 *position
) {
    if (!d_s_vt_0x1AC(soundId)) {
        resetTempParams();
        return nullptr;
    }

    u32 actualSoundId = soundId;
    if (!sIsStartingBaseSound) {
        actualSoundId = overrideHoldSoundId(soundId, false);
        if (actualSoundId == -1) {
            resetTempParams();
            return nullptr;
        }
        actualSoundId = d_s_vt_0x1E8(actualSoundId);
        if (actualSoundId == -1) {
            resetTempParams();
            return nullptr;
        }
    }
    nw4r::snd::SoundHandle *handle = seHandle->getHandle();
    seHandle->shiftParam(mpCtxParam);
    field_0x11C |= 0x80000000;
    bool ok;
    if (distant != nullptr && position != nullptr) {
        // not sure why we're using the original ID here
        ok = distant->holdSound(soundId, *position, handle);
    } else {
        ok = HoldSound(handle, actualSoundId);
    }
    field_0x11C = 0;
    field_0x154 = 0;
    if (ok) {
        seHandle->setField0x130(1);
        postHoldSound(*handle, seHandle, soundId, 0);
    } else {
        handle = nullptr;
    }
    resetTempParams();
    return handle;
}

bool dSoundSource_c::holdSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position) {
    dSndSeSound2_c *seHandle;
    nw4r::snd::SoundHandle *handle;

    if (position != nullptr) {
        dSndDistantSoundActor_c *actor = dSndDistantSoundActorPool_c::GetInstance()->findActiveActor(soundId, this);
        if (actor != nullptr) {
            seHandle = actor->getHoldSoundHandle();
            seHandle->setAcquiredMaybe(1);
            handle = continueHoldingSound(seHandle, soundId, actor, position);
        } else {
            if (d_s_vt_0x190()) {
                resetTempParams();
                return false;
            }
            if (isInaudibleInternal()) {
                resetTempParams();
                return false;
            }
            actor = dSndDistantSoundActorPool_c::GetInstance()->acquireActor(soundId, position, this);
            if (actor == nullptr) {
                resetTempParams();
                return false;
            }
            seHandle = dSndSeSoundPool_c::GetInstance()->acquireSoundType2(this, soundId);
            if (seHandle == nullptr) {
                resetTempParams();
                return false;
            }
            seHandle->shiftParam(mpCtxParam);
            handle = seHandle->getHandle();
            if (actor->holdSound(soundId, *position, handle)) {
                addSeHandleType2(seHandle);
                postHoldSound(*handle, seHandle, soundId, 0);
                postSetupSound(handle->GetId(), soundId, seHandle);
                seHandle->onStart(this);
                seHandle->onCalc(this);
                actor->setHoldSoundHandle(seHandle);
            } else {
                dSndSeSoundPool_c::GetInstance()->releaseSoundType2(seHandle);
                handle = nullptr;
            }
        }

    } else {
        handle = holdSound(soundId, nullptr);
    }
    resetTempParams();
    return handle != nullptr;
}

bool dSoundSource_c::holdSoundAtPosition(const char *label, const nw4r::math::VEC3 *position) {
    return holdSoundAtPosition(soundLabelToSoundId(label), position);
}

nw4r::snd::SoundStartable::StartResult
dSoundSource_c::SetupSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *arg) {
    bool holdFlag = *(bool *)arg;
    if (!d_s_vt_0x1AC(soundId)) {
        return onSetupError();
    } else if ((field_0x11C & 0x80000000) != 0) {
        return setupSound(pHandle, soundId, pStartInfo, arg, holdFlag);
    } else {
        return onSetupError();
    }
}

bool dSoundSource_c::d_s_vt_0x1AC(u32 soundId) {
    if (soundId == -1) {
        return false;
    }

    if (d_s_vt_0x190()) {
        return false;
    }

    if (isInaudibleInternal()) {
        return false;
    }

    if (checkUserParamMask(soundId, 0x80000000)) {
        return false;
    }

    return true;
}

nw4r::snd::SoundStartable::StartResult dSoundSource_c::setupSound(
    nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *arg, bool holdFlag
) {
    u32 userParam = getSomeUserParam(soundId);
    if (userParam & 0x2) {
        // maybe a flag for not allowing duplicates
        if (getHandleType1ForSoundId(soundId) != nullptr) {
            return onSetupError();
        }
    } else if (userParam & 0x1) {
        // maybe a flag for stopping other copies
        stopSoundHandles(soundId, 2);
    }

    u32 variantSoundId = soundId;
    if (!sIsStartingBaseSound) {
        u32 anotherSoundId;
        if (holdFlag) {
            anotherSoundId = overrideHoldSoundId(soundId, true);
        } else {
            anotherSoundId = overrideStartSoundId(soundId);
        }
        if (anotherSoundId == -1) {
            return onSetupError();
        }

        if (userParam & 0x800) {
            anotherSoundId = modifySoundId(anotherSoundId);
            if (anotherSoundId == -1) {
                return onSetupError();
            }
        }
        variantSoundId = d_s_vt_0x1E8(anotherSoundId);
        if (variantSoundId == -1) {
            return onSetupError();
        }
    }

    if (!holdFlag && (userParam & 0x1C000000) != 0) {
        dSndDistantSoundActor_c *ac = startSoundAtPosition(variantSoundId, &GetPosition(), nullptr);
        f32 volume = 0.0f;
        if (ac != nullptr) {
            if ((userParam & 0x10000000) != 0 && (userParam & 0xC000000) == 0) {
                ac->setUseSourcePosition(true);
            }
            // unnecessary double check
            if (!holdFlag) {
                volume = getBaseSoundVolume(variantSoundId, soundId);
            }
        }
        startBaseSoundAtPosition(soundId, &GetPosition(), volume);
        resetTempParams();
        return onSetupError();
    } else {
        StartResult result = dSnd3DActor_c::SetupSound(pHandle, variantSoundId, pStartInfo, arg);
        if (result == START_SUCCESS && !holdFlag) {
            f32 volume = getBaseSoundVolume(variantSoundId, soundId);
            startBaseSound(soundId, volume);
        }
        resetTempParams();
        return result;
    }
}

f32 dSoundSource_c::getBaseSoundVolume(u32 variantSoundId, u32 baseSoundId) {
    if (variantSoundId == baseSoundId) {
        return 0.0f;
    } else if (sIsStartingBaseSound) {
        return 0.0f;
    }

    u32 volume = (dSndPlayerMgr_c::GetInstance()->getSomeUserParam(variantSoundId) >> 16) & 0xFF;
    if (volume > 100) {
        volume = 100;
    }
    return volume / 100.0f;
}

nw4r::snd::SoundHandle *dSoundSource_c::startBaseSound(u32 baseSoundId, f32 volume) {
    if (volume <= 0.0f) {
        return nullptr;
    }

    sIsStartingBaseSound = true;
    nw4r::snd::SoundHandle *handle = startSound(baseSoundId, nullptr);
    sIsStartingBaseSound = false;
    if (handle != nullptr) {
        handle->SetVolume(volume, 0);
    }
    return handle;
}

dSndDistantSoundActor_c *
dSoundSource_c::startBaseSoundAtPosition(u32 baseSoundId, const nw4r::math::VEC3 *position, f32 volume) {
    if (volume <= 0.0f) {
        return nullptr;
    }

    sIsStartingBaseSound = true;
    dSndDistantSoundActor_c *handle = startSoundAtPosition(baseSoundId, position, nullptr);
    sIsStartingBaseSound = false;
    if (handle != nullptr) {
        handle->getHandle()->SetVolume(volume, 0);
    }
    return handle;
}

void dSoundSource_c::resetTempParams() {
    mpCtxParam = nullptr;
    sCtxParams.floatParam = 0.0f;
    sCtxParams.intParam = 0;
    field_0x11C = 0;
    field_0x154 = 0;
}

nw4r::snd::SoundStartable::StartResult dSoundSource_c::onSetupError() {
    resetTempParams();
    return START_ERR_USER;
}

u32 dSoundSource_c::overrideStartSoundId(u32 soundId) {
    return soundId;
}

void dSoundSource_c::postStartSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id) {
    // noop
}

u32 dSoundSource_c::overrideHoldSoundId(u32 soundId, bool initial) {
    return soundId;
}

void dSoundSource_c::postHoldSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id, UNKWORD) {
    // noop
}

void dSoundSource_c::attachDistantSound(dSndDistantSoundActor_c *sound) {
    if (sound != nullptr) {
        sound->setSourceDirectly(this);
        appendDistantSound(sound);
    }
}

void dSoundSource_c::detachDistantSound(dSndDistantSoundActor_c *sound) {
    if (sound != nullptr && sound->isAttachedSource(this)) {
        sound->setSourceDirectly(nullptr);
        removeDistantSound(sound);
    }
}

void dSoundSource_c::detachAllDistantSounds() {
    dSndDistantSoundActor_c *it = getDistantSoundFirst();
    while (it != nullptr) {
        it->setSourceDirectly(nullptr);
        removeDistantSound(it);
        it = getDistantSoundFirst();
    }
}

void dSoundSource_c::pauseAllDistantSounds(bool flag, int fadeFrames) {
    for (dSndDistantSoundActor_c *it = getDistantSoundFirst(); it != nullptr; it = getDistantSoundNext(it)) {
        it->PauseAllSound(flag, fadeFrames);
    }
}

void dSoundSource_c::addSeHandleType1(dSndSeSound1_c *handle) {
    if (handle == nullptr) {
        return;
    }
    if (handle->isAttachedSource()) {
        return;
    }
    handle->attachSource(this);
    appendHandleType1(handle);
}

void dSoundSource_c::addSeHandleType2(dSndSeSound2_c *handle) {
    if (handle == nullptr) {
        return;
    }
    if (handle->isAttachedSource()) {
        return;
    }
    handle->attachSource(this);
    appendHandleType2(handle);
}

void dSoundSource_c::removeSeHandleType1(dSndSeSound1_c *handle, bool stop) {
    if (handle == nullptr) {
        return;
    }
    if (!handle->isAttachedSource()) {
        return;
    }
    removeHandleType1(handle);
    dSndSeSoundPool_c::GetInstance()->releaseSoundType1(handle, stop);
}

void dSoundSource_c::removeSeHandleType2(dSndSeSound2_c *handle) {
    if (handle == nullptr) {
        return;
    }
    if (!handle->isAttachedSource()) {
        return;
    }
    removeHandleType2(handle);
    dSndSeSoundPool_c::GetInstance()->releaseSoundType2(handle);
}

void dSoundSource_c::removeAllSeHandles() {
    removeAllSeHandlesType1();
    removeAllSeHandlesType2();
}

void dSoundSource_c::removeAllSeHandlesType1() {
    dSndSeSound1_c *sound = getHandleType1First();
    while (sound != nullptr) {
        removeSeHandleType1(sound, sound->unkRemovalCheck());
        sound = getHandleType1First();
    }
}

void dSoundSource_c::removeAllSeHandlesType2() {
    dSndSeSound2_c *sound = getHandleType2First();
    while (sound != nullptr) {
        removeSeHandleType2(sound);
        sound = getHandleType2First();
    }
}

void dSoundSource_c::calcHandles() {
    calcHandlesType1();
    calcHandlesType2();
}

void dSoundSource_c::calcHandlesType1() {
    dSndSeSound1_c *next, *it;
    it = getHandleType1First();
    while (it != nullptr) {
        next = getHandleType1Next(it);
        if (it->isAttachedSound()) {
            it->calc(this);
        } else {
            removeSeHandleType1(it, true);
        }
        it = next;
    }
}

void dSoundSource_c::calcHandlesType2() {
    dSndSeSound2_c *next, *it;
    it = getHandleType2First();
    while (it != nullptr) {
        next = getHandleType2Next(it);
        if (it->isAttachedSound()) {
            if (it->calcTimer()) {
                continueHoldingSound(it, it->getHandle()->GetId(), nullptr, nullptr);
            }
            it->calc(this);
        } else {
            removeSeHandleType2(it);
        }
        it = next;
    }
}

dSndSeSound1_c *dSoundSource_c::getHandleType1ForSoundId(u32 soundId) {
    dSndSeSound1_c *sound = getHandleType1First();
    while (sound != nullptr) {
        if (sound->isSoundId(soundId) || sound->getHandle()->GetId() == soundId) {
            return sound;
        }
        sound = getHandleType1Next(sound);
    }

    return nullptr;
}

dSndSeSound1_c *dSoundSource_c::getHandleType1ForSoundHandle(nw4r::snd::SoundHandle *handle) {
    dSndSeSound1_c *sound = getHandleType1First();
    while (sound != nullptr) {
        if (sound->getHandle() == handle) {
            return sound;
        }
        sound = getHandleType1Next(sound);
    }

    return nullptr;
}

dSndSeSound2_c *dSoundSource_c::getHandleType2ForSoundId(u32 soundId) {
    dSndSeSound2_c *sound = getHandleType2First();
    while (sound != nullptr) {
        if (sound->isSoundId(soundId) || sound->getHandle()->GetId() == soundId) {
            return sound;
        }
        sound = getHandleType2Next(sound);
    }

    return nullptr;
}

dSndSeSound1_c *dSoundSource_c::getHandleType1ForSoundHandle_Dupe(nw4r::snd::SoundHandle *pHandle) {
    dSndSeSound1_c *sound = getHandleType1First();
    while (sound != nullptr) {
        if (sound->getHandle() == pHandle) {
            return sound;
        }
        sound = getHandleType1Next(sound);
    }

    return nullptr;
}

dSndSeSound2_c *dSoundSource_c::getHandleType2ForSoundHandle_Dupe(nw4r::snd::SoundHandle *pHandle) {
    dSndSeSound2_c *sound = getHandleType2First();
    while (sound != nullptr) {
        if (sound->getHandle() == pHandle) {
            return sound;
        }
        sound = getHandleType2Next(sound);
    }

    return nullptr;
}

void dSoundSource_c::stopSoundHandles(u32 soundId, s32 fadeFrames) {
    {
        dSndSeSound1_c *next, *it;
        it = getHandleType1First();
        while (it != nullptr) {
            next = getHandleType1Next(it);
            if (it->isSoundId(soundId) || it->getHandle()->GetId() == soundId) {
                it->getHandle()->Stop(fadeFrames);
                removeSeHandleType1(it, true);
            }
            it = next;
        }
    }

    {
        dSndSeSound2_c *next, *it;
        it = getHandleType2First();
        while (it != nullptr) {
            next = getHandleType2Next(it);
            if (it->isSoundId(soundId) || it->getHandle()->GetId() == soundId) {
                it->getHandle()->Stop(fadeFrames);
                removeSeHandleType2(it);
            }
            it = next;
        }
    }
}

#pragma push
#pragma optimize_tail_call off
bool dSoundSource_c::isInaudible() {
    return isInaudibleInternal();
}
#pragma pop

void dSoundSource_c::setPolyAttrs(u8 polyAttr0, u8 polyAttr1) {
    mPolyAttr0 = polyAttr0;
    mPolyAttr1 = polyAttr1;
}

void dSoundSource_c::d_s_vt_0x1E4_noop() {
    // never used, never overridden
}

u32 dSoundSource_c::modifySoundId(u32 baseSoundId) {
    if (baseSoundId != -1) {
        SizedString<64> label;

        const char *baseLabel = dSndMgr_c::getSoundLabelString(baseSoundId);
        const char *charLabel = getName();
        label.sprintf("%s_%s", baseLabel, charLabel);
        u32 newLabel = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
        if (newLabel != -1) {
            return newLabel;
        }
    }
    return baseSoundId;
}

u32 dSoundSource_c::modifySoundId(u32 baseSoundId, dSoundSource_c *source) {
    if (baseSoundId != -1 && source != nullptr) {
        SizedString<64> label;

        const char *baseLabel = dSndMgr_c::getSoundLabelString(baseSoundId);
        const char *charLabel = source->getName();
        label.sprintf("%s_%s", baseLabel, charLabel);
        u32 newLabel = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
        if (newLabel != -1) {
            return newLabel;
        }
    }
    return baseSoundId;
}

u32 dSoundSource_c::specializeBgHitSoundId(u32 soundId, u32 polyAttr0, u32 polyAttr1) {
    if (checkUserParamMask(soundId, 0x100)) {
        if (polyAttr0 >= SND_MAT_NONE + 1 && polyAttr0 <= SND_MAT_MAX - 1) {
            const char *baseLabel = dSndMgr_c::getSoundLabelString(soundId);
            SizedString<64> label;
            label.sprintf("%s_%s_%d", baseLabel, getHitEffectName(polyAttr0), polyAttr1);
            u32 newId = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
            if (newId == -1) {
                SizedString<64> label;
                label.sprintf("%s_%s", baseLabel, getHitEffectName(polyAttr0));
                newId = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
            }
            if (newId != -1) {
                return newId;
            }
        }
    }
    return soundId;
}

u32 dSoundSource_c::d_s_vt_0x1E8(u32 soundId) {
    return soundId;
}

u32 dSoundSource_c::getRemoConSoundVariant(u32 soundId) const {
    const char *label = soundIdToSoundLabel(soundId);
    SizedString<64> str;
    str.sprintf("%s_RC", label);
    return soundLabelToSoundId(str);
}

f32 dSoundSource_c::timeAreaCheck() {
    bool check = !dTimeAreaMgr_c::GetInstance()->fn_800B9B60(getRoomId(), GetPosition());
    if (check) {
        return 0.0f;
    }
    f32 tmp = dTimeAreaMgr_c::GetInstance()->fn_800B9CE0(GetPosition());
    if (tmp > 0.0f) {
        tmp = tmp / 500.0f;
        if (tmp > 1.0f) {
            tmp = 1.0f;
        }
    }
    return tmp;
}

nw4r::snd::SoundStartable::StartResult
dSoundSource_c::setupSoundCommon(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *arg) {
    bool holdFlag = *(bool *)arg;
    if (!d_s_vt_0x1AC(soundId)) {
        return onSetupError();
    }

    dSndSeSound_c *seHandle = nullptr;
    bool freshSeSound = false;
    if ((field_0x11C & 0x80000000) == 0) {
        if (holdFlag) {
            seHandle = getHandleType2ForSoundHandle_Dupe(pHandle);
        } else {
            seHandle = getHandleType1ForSoundHandle_Dupe(pHandle);
        }

        if (seHandle == nullptr) {
            if (holdFlag) {
                seHandle = dSndSeSoundPool_c::GetInstance()->acquireSoundType2(this, soundId);
            } else {
                seHandle = dSndSeSoundPool_c::GetInstance()->acquireSoundType1(this, soundId);
            }
            freshSeSound = true;
        }

        if (seHandle == nullptr) {
            return onSetupError();
        }

        if (pHandle != nullptr) {
            seHandle->setHandle(pHandle);
        } else {
            pHandle = seHandle->getHandle();
        }
    }

    StartResult res = setupSound(pHandle, soundId, pStartInfo, arg, holdFlag);
    if (res == START_SUCCESS) {
        if (seHandle != nullptr) {
            if (!holdFlag) {
                if (freshSeSound) {
                    addSeHandleType1(static_cast<dSndSeSound1_c *>(seHandle));
                }
                postStartSound(*pHandle, seHandle, soundId);
            } else {
                if (freshSeSound) {
                    addSeHandleType2(static_cast<dSndSeSound2_c *>(seHandle));
                }
                postHoldSound(*pHandle, seHandle, soundId, 1);
            }
            postSetupSound(pHandle->GetId(), soundId, seHandle);
            seHandle->onStart(this);
            seHandle->onCalc(this);
        }
    } else if (freshSeSound) {
        if (seHandle->getHandleType() == dSndSeSound_c::SE_HANDLE_TYPE_1) {
            dSndSeSoundPool_c::GetInstance()->releaseSoundType1(static_cast<dSndSeSound1_c *>(seHandle), true);
        } else {
            dSndSeSoundPool_c::GetInstance()->releaseSoundType2(static_cast<dSndSeSound2_c *>(seHandle));
        }
    }
    return res;
}
