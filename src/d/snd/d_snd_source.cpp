#include "d/snd/d_snd_source.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/snd/d_snd_3d_actor.h"
#include "d/snd/d_snd_3d_manager.h"
#include "d/snd/d_snd_checkers.h"
#include "d/snd/d_snd_distant_sound_actor.h"
#include "d/snd/d_snd_distant_sound_actor_pool.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_se_sound_pool.h"
#include "d/snd/d_snd_source_group.h"
#include "d/snd/d_snd_source_mgr.h"
#include "egg/audio/eggAudioRmtSpeakerMgr.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/ut/ut_list.h"
#include "sized_string.h"

struct d_snd_mgr_unk_6_sinit {
    d_snd_mgr_unk_6_sinit() : field_0x00(0), field_0x04(0.0f) {}

    u32 field_0x00;
    f32 field_0x04;
};

d_snd_mgr_unk_6_sinit d_snd_mgr_unk_6_sinit_instance;

const char *help_i_need_data() {
    return "%s_%s_%d";
}

dSoundSource_c::dSoundSource_c(u8 sourceType, dAcBase_c *actor, const char *name, dSndSourceGroup_c *pOwnerGroup)
    : dSnd3DActor_c(pOwnerGroup->getAmbientParam(), sourceType),
      mpName(name),
      field_0x0F4(0),
      mpActor(actor),
      mSourceType(sourceType),
      field_0x0FE(0),
      field_0x0FF(0),
      field_0x100(0),
      field_0x101(0),
      field_0x102(0),
      field_0x104(0),
      field_0x108(0),
      mpUnkSe(nullptr),
      field_0x11C(0),
      mpOwnerGroup(pOwnerGroup),
      field_0x154(0),
      field_0x158(-1),
      field_0x15A(-1) {
    mSourceCategory = dSndSourceMgr_c::getSourceCategoryForSourceType(sourceType, name);
    // TODO: Offsetof
    nw4r::ut::List_Init(&mDistantSoundList, 0xEC);
    nw4r::ut::List_Init(&mHandleType1List, 0x04);
    nw4r::ut::List_Init(&mHandleType2List, 0x04);
    pOwnerGroup->registerSource(this);
}

dSoundSource_c::~dSoundSource_c() {
    SetUserParam(0);
    vt_0x44();
    removeAllSeHandles();
    detachAllDistantSounds();
    dSndSourceMgr_c::GetInstance()->unregisterSource(this);
    mpOwnerGroup->unregisterSource(this);
}

const nw4r::math::VEC3 &dSoundSource_c::getListenerPosition() const {
    return dSnd3DManager_c::GetInstance()->getSndListenerPos();
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

void dSoundSource_c::setPause(bool flag, int fadeFrames) {
    if (!flag) {
        PauseAllSound(flag, fadeFrames);
        pauseAllDistantSounds(flag, fadeFrames);
        mIsPaused = 0;
    } else if (a_field_0x7E == 0) {
        PauseAllSound(flag, fadeFrames);
        pauseAllDistantSounds(flag, fadeFrames);
        mIsPaused = 1;
    }
}

bool dSoundSource_c::startSound(u32 soundId) {
    return startSound(soundId, nullptr);
}

bool dSoundSource_c::startSound(const char *label) {
    return startSound(soundLabelToSoundId(label));
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
        d_s_vt_0x1C8();
        return START_ERR_USER;
    }

    seHandle->shiftPair(mpUnkSe);
    if (handle != nullptr) {
        seHandle->setHandle(handle);
    } else {
        handle = seHandle->getHandle();
    }

    field_0x11C |= 0x80000000;
    result = detail_StartSound(handle, soundId, nullptr);
    if (result == START_SUCCESS) {
        addSeHandleType1(seHandle);
        d_vt_0x58(*handle, seHandle, soundId);
        d_s_vt_0x1CC(handle->GetId(), soundId, seHandle);
        seHandle->onStart(this);
        seHandle->onCalc(this);
    } else {
        dSndSeSoundPool_c::GetInstance()->releaseSoundType1(seHandle, true);
        handle = nullptr;
    }
    d_s_vt_0x1C8();
    if (pOutHandle != nullptr) {
        *pOutHandle = handle;
    }
    return result;
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
            d_s_vt_0x1C8();
            return nullptr;
        }
    }

    dSndDistantSoundActor_c *ac = dSndDistantSoundActorPool_c::GetInstance()->acquireActor(soundId, position, this);
    if (ac == nullptr) {
        d_s_vt_0x1C8();
        return nullptr;
    }

    seHandle->setAcquiredMaybe(true);
    seHandle->shiftPair(mpUnkSe);
    if (handle != nullptr) {
        seHandle->setHandle(handle);
    } else {
        handle = seHandle->getHandle();
    }

    if (ac->startSound(soundId, *position, handle)) {
        addSeHandleType1(seHandle);
        seHandle->setInUseMaybe(true);
        ac->d_vt_0x58(*handle, seHandle, soundId);
        d_s_vt_0x1CC(handle->GetId(), soundId, seHandle);
        seHandle->onStart(this);
        seHandle->onCalc(this);
    } else {
        dSndSeSoundPool_c::GetInstance()->releaseSoundType1(seHandle, true);
        ac = nullptr;
    }
    d_s_vt_0x1C8();
    return ac;
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

nw4r::snd::SoundHandle *dSoundSource_c::holdSound(u32 soundId, nw4r::snd::SoundHandle *handle) {
    if (soundId == -1) {
        return nullptr;
    }

    dSndSeSound2_c *seHandle = getHandleType2ForSoundId(soundId);
    if (seHandle == nullptr) {
        seHandle = dSndSeSoundPool_c::GetInstance()->acquireSoundType2(this, soundId);
        if (seHandle == nullptr) {
            d_s_vt_0x1C8();
            return nullptr;
        }

        seHandle->shiftPair(mpUnkSe);

        if (handle != nullptr) {
            seHandle->setHandle(handle);
        } else {
            handle = seHandle->getHandle();
        }

        field_0x11C |= 0x80000000;
        if (HoldSound(handle, soundId)) {
            addSeHandleType2(seHandle);
            d_vt_0x5C(*handle, seHandle, soundId, 0);
            d_s_vt_0x1CC(handle->GetId(), soundId, seHandle);
            seHandle->onStart(this);
            seHandle->onCalc(this);
        } else {
            dSndSeSoundPool_c::GetInstance()->releaseSoundType2(seHandle);
            handle = nullptr;
        }
    } else {
        handle = continueHoldingSound(seHandle, soundId, nullptr, nullptr);
    }
    d_s_vt_0x1C8();

    return handle;
}

nw4r::snd::SoundHandle *dSoundSource_c::continueHoldingSound(
    dSndSeSound2_c *seHandle, u32 soundId, dSndDistantSoundActor_c *distant, const nw4r::math::VEC3 *position
) {
    if (!d_s_vt_0x1AC(soundId)) {
        d_s_vt_0x1C8();
        return nullptr;
    }

    u32 actualSoundId = soundId;
    if (!sIsStartingBaseSound) {
        actualSoundId = d_s_vt_0x1A0(soundId, 0);
        if (actualSoundId == -1) {
            d_s_vt_0x1C8();
            return nullptr;
        }
        actualSoundId = d_s_vt_0x1E8(actualSoundId);
        if (actualSoundId == -1) {
            d_s_vt_0x1C8();
            return nullptr;
        }
    }
    nw4r::snd::SoundHandle *handle = seHandle->getHandle();
    seHandle->shiftPair(mpUnkSe);
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
        d_vt_0x5C(*handle, seHandle, soundId, 0);
    } else {
        handle = nullptr;
    }
    d_s_vt_0x1C8();
    return handle;
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
            anotherSoundId = d_s_vt_0x1A0(soundId, 1);
        } else {
            anotherSoundId = d_s_vt_0x194(soundId);
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
        d_s_vt_0x1C8();
        return onSetupError();
    } else {
        StartResult result = dSnd3DActor_c::SetupSound(pHandle, variantSoundId, pStartInfo, arg);
        if (result == START_SUCCESS && !holdFlag) {
            f32 volume = getBaseSoundVolume(variantSoundId, soundId);
            startBaseSound(soundId, volume);
        }
        d_s_vt_0x1C8();
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

nw4r::snd::SoundStartable::StartResult dSoundSource_c::onSetupError() {
    d_s_vt_0x1C8();
    return START_ERR_USER;
}

void dSoundSource_c::d_vt_0x58(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id) {
    // noop
}

void dSoundSource_c::d_vt_0x5C(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id, UNKWORD) {
    // noop
}

void dSoundSource_c::attachDistantSound(dSndDistantSoundActor_c *sound) {
    if (sound != nullptr) {
        sound->setSourceDirectly(this);
        nw4r::ut::List_Append(&mDistantSoundList, sound);
    }
}

void dSoundSource_c::detachDistantSound(dSndDistantSoundActor_c *sound) {
    if (sound != nullptr && sound->isAttachedSource(this)) {
        sound->setSourceDirectly(nullptr);
        nw4r::ut::List_Remove(&mDistantSoundList, sound);
    }
}

void dSoundSource_c::detachAllDistantSounds() {
    dSndDistantSoundActor_c *it = static_cast<dSndDistantSoundActor_c *>(nw4r::ut::List_GetFirst(&mDistantSoundList));
    while (it != nullptr) {
        it->setSourceDirectly(nullptr);
        nw4r::ut::List_Remove(&mDistantSoundList, it);
        it = static_cast<dSndDistantSoundActor_c *>(nw4r::ut::List_GetFirst(&mDistantSoundList));
    }
}

void dSoundSource_c::pauseAllDistantSounds(bool flag, int fadeFrames) {
    for (dSndDistantSoundActor_c *it =
             static_cast<dSndDistantSoundActor_c *>(nw4r::ut::List_GetFirst(&mDistantSoundList));
         it != nullptr; it = static_cast<dSndDistantSoundActor_c *>(nw4r::ut::List_GetNext(&mDistantSoundList, it))) {
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
    nw4r::ut::List_Append(&mHandleType1List, handle);
}

void dSoundSource_c::addSeHandleType2(dSndSeSound2_c *handle) {
    if (handle == nullptr) {
        return;
    }
    if (handle->isAttachedSource()) {
        return;
    }
    handle->attachSource(this);
    nw4r::ut::List_Append(&mHandleType2List, handle);
}

void dSoundSource_c::removeSeHandleType1(dSndSeSound1_c *handle, bool stop) {
    if (handle == nullptr) {
        return;
    }
    if (!handle->isAttachedSource()) {
        return;
    }
    nw4r::ut::List_Remove(&mHandleType1List, handle);
    dSndSeSoundPool_c::GetInstance()->releaseSoundType1(handle, stop);
}

void dSoundSource_c::removeSeHandleType2(dSndSeSound2_c *handle) {
    if (handle == nullptr) {
        return;
    }
    if (!handle->isAttachedSource()) {
        return;
    }
    nw4r::ut::List_Remove(&mHandleType2List, handle);
    dSndSeSoundPool_c::GetInstance()->releaseSoundType2(handle);
}

void dSoundSource_c::removeAllSeHandles() {
    removeAllSeHandlesType1();
    removeAllSeHandlesType2();
}

void dSoundSource_c::removeAllSeHandlesType1() {
    dSndSeSound1_c *sound = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetFirst(&mHandleType1List));
    while (sound != nullptr) {
        removeSeHandleType1(sound, sound->unkRemovalCheck());
        sound = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetFirst(&mHandleType1List));
    }
}

void dSoundSource_c::removeAllSeHandlesType2() {
    dSndSeSound2_c *sound = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetFirst(&mHandleType2List));
    while (sound != nullptr) {
        removeSeHandleType2(sound);
        sound = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetFirst(&mHandleType2List));
    }
}

void dSoundSource_c::calcHandles() {
    calcHandlesType1();
    calcHandlesType2();
}

void dSoundSource_c::calcHandlesType1() {
    dSndSeSound1_c *next, *it;
    it = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetFirst(&mHandleType1List));
    while (it != nullptr) {
        next = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetNext(&mHandleType1List, it));
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
    it = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetFirst(&mHandleType2List));
    while (it != nullptr) {
        next = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetNext(&mHandleType2List, it));
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
    dSndSeSound1_c *sound = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetFirst(&mHandleType1List));
    while (sound != nullptr) {
        if (sound->isSoundId(soundId) || sound->getHandle()->GetId() == soundId) {
            return sound;
        }
        sound = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetNext(&mHandleType1List, sound));
    }

    return nullptr;
}

dSndSeSound1_c *dSoundSource_c::getHandleType1ForSoundHandle(nw4r::snd::SoundHandle *handle) {
    dSndSeSound1_c *sound = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetFirst(&mHandleType1List));
    while (sound != nullptr) {
        if (sound->getHandle() == handle) {
            return sound;
        }
        sound = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetNext(&mHandleType1List, sound));
    }

    return nullptr;
}

dSndSeSound2_c *dSoundSource_c::getHandleType2ForSoundId(u32 soundId) {
    dSndSeSound2_c *sound = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetFirst(&mHandleType2List));
    while (sound != nullptr) {
        if (sound->isSoundId(soundId) || sound->getHandle()->GetId() == soundId) {
            return sound;
        }
        sound = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetNext(&mHandleType2List, sound));
    }

    return nullptr;
}

void dSoundSource_c::stopSoundHandles(u32 soundId, s32 fadeFrames) {
    // TODO: Weird declaration order required for regswaps
    {
        dSndSeSound1_c *next, *it;
        it = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetFirst(&mHandleType1List));
        while (it != nullptr) {
            next = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetNext(&mHandleType1List, it));
            if (it->isSoundId(soundId) || it->getHandle()->GetId() == soundId) {
                it->getHandle()->Stop(fadeFrames);
                removeSeHandleType1(it, true);
            }
            it = next;
        }
    }

    {
        dSndSeSound2_c *it, *next;
        it = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetFirst(&mHandleType2List));
        while (it != nullptr) {
            next = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetNext(&mHandleType2List, it));
            if (it->isSoundId(soundId) || it->getHandle()->GetId() == soundId) {
                it->getHandle()->Stop(fadeFrames);
                removeSeHandleType2(it);
            }
            it = next;
        }
    }
}

u32 dSoundSource_c::modifySoundId(u32 baseSoundId) {
    if (baseSoundId != -1) {
        SizedString<64> label;

        // maybe an inline
        const char *baseLabel = nullptr;
        if (dSndMgr_c::GetInstance()->getArchive() != nullptr) {
            baseLabel = dSndMgr_c::GetInstance()->getArchive()->GetSoundLabelString(baseSoundId);
        }

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

        // maybe an inline
        const char *baseLabel = nullptr;
        if (dSndMgr_c::GetInstance()->getArchive() != nullptr) {
            baseLabel = dSndMgr_c::GetInstance()->getArchive()->GetSoundLabelString(baseSoundId);
        }

        const char *charLabel = source->getName();
        label.sprintf("%s_%s", baseLabel, charLabel);
        u32 newLabel = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
        if (newLabel != -1) {
            return newLabel;
        }
    }
    return baseSoundId;
}

u32 dSoundSource_c::getRemoConSoundVariant(u32 soundId) const {
    const char *label = soundIdToSoundLabel(soundId);
    SizedString<64> str;
    str.sprintf("%s_RC", label);
    return soundLabelToSoundId(str);
}
