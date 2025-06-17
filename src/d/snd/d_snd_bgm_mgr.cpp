#include "d/snd/d_snd_bgm_mgr.h"

#include "common.h"
#include "d/snd/d_snd_bgm_sound.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_sound.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/ut/ut_list.h"

SND_DISPOSER_DEFINE(dSndBgmMgr_c);

dSndBgmMgr_c::dSndBgmMgr_c()
    : field_0x240(2),
      field_0x244(0),
      field_0x248(0),
      mScheduledSoundId(-1),
      mScheduledSoundDelay(0),
      field_0x274(0),
      field_0x29C(0),
      mSoundActor(dSndMgr_c::GetInstance()->getPlayer()),
      field_0x2F4(1.0f),
      field_0x2F8(-1),
      field_0x2FC(0),
      field_0x300(-1),
      field_0x304(0),
      field_0x305(0),
      field_0x306(0),
      field_0x307(0),
      field_0x308(0),
      field_0x333(0),
      field_0x340(0) {
    for (int i = 0; i < BGM_LIST_MAX; i++) {
        // TODO offsetof
        nw4r::ut::List_Init(&mBgmSoundLists[i], 0xF0);
    }

    for (int i = 0; i < 7; i++) {
        if (i == 4) {
            mBgmSounds[i] = new dSndBgmBattleSound_c();
        } else {
            mBgmSounds[i] = new dSndBgmSound_c();
        }
        mBgmSounds[i]->init(mSoundActor, i);
    }

    field_0x270 = new dSndBgmSound_c();
    field_0x270->init(dSndMgr_c::GetInstance()->getPlayer(), 8);

    for (int i = 0; i < 3; i++) {
        mFanSounds[i] = new dSndSound_c();
    }

    nw4r::ut::List_Init(&mAllSoundsList, 8);

    for (int i = 0; i < 3; i++) {
        field_0x30C[i] = 0;
        field_0x318[i] = 0;
        field_0x324[i] = 0;
        field_0x330[i] = 0;
    }
}

void dSndBgmMgr_c::calcLists() {
    // TODO ...

    dSndBgmSound_c *next, *it;
    for (it = getFirstInBgmSoundList(BGM_LIST_PLAYING); it != nullptr; it = next) {
        next = getNextInBgmSoundList(BGM_LIST_PLAYING, it);
        it->calc();
    }

    for (it = getFirstInBgmSoundList(BGM_LIST_STOPPED); it != nullptr; it = next) {
        next = getNextInBgmSoundList(BGM_LIST_STOPPED, it);
        it->calc();
    }

    for (it = getFirstInBgmSoundList(BGM_LIST_PREPARING); it != nullptr; it = next) {
        next = getNextInBgmSoundList(BGM_LIST_PREPARING, it);
        if (!it->IsAttachedSound()) {
            it->cancel();
            removeFromAllBgmSoundLists(it);
        }
    }

    {
        dSndSound_c *next, *it;
        for (it = getAllSoundsFirst(); it != nullptr; it = next) {
            next = getAllSoundsNext(it);
            if (!it->IsAttachedSound()) {
                unregistSound(it);
            }
        }
    }
}

bool dSndBgmMgr_c::prepareBgm(u32 soundId, u32 startOffset) {
    if (isPlayingBgmSoundId(soundId)) {
        return false;
    }
    dSndBgmSound_c *handle = getSoundHandleForBgm(soundId, startOffset);
    bool ret = false;
    if (handle != nullptr) {
        ret = prepareBgmSound(soundId, handle, startOffset);
    }
    startAdditionalBgm(soundId);
    return ret;
}

void dSndBgmMgr_c::stopAllBgm(s32 fadeFrames) {
    for (int i = 0; i < 7; i++) {
        stopBgmSound(mBgmSounds[i], fadeFrames);
    }
    cancelDelayedBgm();
}

bool dSndBgmMgr_c::stopBgmSound(dSndBgmSound_c *sound, s32 fadeFrames) {
    u32 oldId = sound->GetId();
    if (oldId != -1) {
        sound->stop(fadeFrames);
        checkForPrepareStoppedBgmSound(oldId);
        return true;
    }

    return false;
}

void dSndBgmMgr_c::checkForPrepareStoppedBgmSound(u32 stoppedSoundId) {
    // TODO ???
    u32 id = stoppedSoundId == BGM_SKYFIELD ? BGM_SKYFIELD : -1;
    if (id != -1) {
        prepareBgm(id, 0);
    }
}

bool dSndBgmMgr_c::beginBgmBattleRoom() {
    if (isBgmHandleIdxPlayingSoundId(4, BGM_BATTLE_ROOM_MAIN)) {
        return false;
    }

    return playBattleBgm(BGM_BATTLE_ROOM_MAIN, 5);
}

bool dSndBgmMgr_c::endBgmBattleRoom() {
    if (!isBgmHandleIdxPlayingSoundId(4, BGM_BATTLE_ROOM_MAIN)) {
        return false;
    }

    stopBgmHandleIdx(4, 5);
    return playFanSound(BGM_BATTLE_ROOM_OUTRO);
}

void dSndBgmMgr_c::playDelayedSound(u32 soundId, s32 delay) {
    if (soundId == -1) {
        return;
    }
    nw4r::snd::SoundArchive::SoundInfo info;
    u32 playerId;
    if (!dSndMgr_c::GetInstance()->getArchive()->ReadSoundInfo(soundId, &info)) {
        playerId = -1;
    } else {
        playerId = info.playerId;
    }

    switch (playerId) {
        case dSndPlayerMgr_c::PLAYER_BGM:
        case dSndPlayerMgr_c::PLAYER_BGM_BATTLE:
            if (delay == 0) {
                playBgm(soundId, 0, false);
                return;
            }
            prepareBgm(soundId, 0);
            break;
        case dSndPlayerMgr_c::PLAYER_FAN:
            if (delay == 0) {
                playFanSound(soundId);
                return;
            }
            prepareFanSound(soundId);
            break;
        default: return;
    }
    mScheduledSoundId = soundId;
    mScheduledSoundDelay = delay;
}

void dSndBgmMgr_c::cancelDelayedSound() {
    mScheduledSoundId = -1;
    mScheduledSoundDelay = 0;
}

void dSndBgmMgr_c::cancelDelayedBgm() {
    if (mScheduledSoundId == -1) {
        return;
    }

    nw4r::snd::SoundArchive::SoundInfo info;
    u32 playerId;
    if (!dSndMgr_c::GetInstance()->getArchive()->ReadSoundInfo(mScheduledSoundId, &info)) {
        playerId = -1;
    } else {
        playerId = info.playerId;
    }

    if (dSndPlayerMgr_c::isBgmPlayerId(playerId)) {
        cancelDelayedSound();
    }
}

bool dSndBgmMgr_c::startDelayedSound() {
    if (mScheduledSoundId == -1) {
        return false;
    }

    bool ok = false;

    nw4r::snd::SoundArchive::SoundInfo info;
    u32 playerId;
    if (!dSndMgr_c::GetInstance()->getArchive()->ReadSoundInfo(mScheduledSoundId, &info)) {
        playerId = -1;
    } else {
        playerId = info.playerId;
    }
    switch (playerId) {
        case dSndPlayerMgr_c::PLAYER_BGM:
        case dSndPlayerMgr_c::PLAYER_BGM_BATTLE: ok = playBgm(mScheduledSoundId, 0, false); break;
        case dSndPlayerMgr_c::PLAYER_FAN:        ok = playFanSound(mScheduledSoundId); break;
        default:                                 break;
    }

    cancelDelayedSound();
    return ok;
}

void dSndBgmMgr_c::calcDelayedSound() {
    if (mScheduledSoundId == -1) {
        return;
    }

    mScheduledSoundDelay--;
    if (mScheduledSoundDelay == 0) {
        startDelayedSound();
    }
}

// 8036e980 - plays additional music for some reason.
// e.g. Sandship (BGM_D301 / 0x1607) also prepares BGM_D301_INSIDE / 0x1608

void dSndBgmMgr_c::setBgmHandleIdxVolume(u32 handleIdx, f32 volume, s32 fadeFrames) {
    // Using the getter to check validity but then not using the result is a bit weird
    if (getBgmSoundByIndex(handleIdx) != nullptr) {
        mBgmSounds[handleIdx]->setVolume(volume, fadeFrames);
    }
}

bool dSndBgmMgr_c::isPlayingBgmSoundId(u32 soundId) const {
    for (int i = 0; i < 7; i++) {
        if (mBgmSounds[i]->isPlayingSoundId(soundId)) {
            return true;
        }
    }
    return false;
}

bool dSndBgmMgr_c::isFadingOutBgmSoundId(u32 soundId) const {
    for (int i = 0; i < 7; i++) {
        if (mBgmSounds[i]->GetId() == soundId && mBgmSounds[i]->isFadingOutSoundId(soundId)) {
            return true;
        }
    }
    return false;
}

bool dSndBgmMgr_c::isPreparingBgmSoundId(u32 soundId) const {
    for (int i = 0; i < 7; i++) {
        if (mBgmSounds[i]->isPreparingSoundId(soundId)) {
            return true;
        }
    }
    return false;
}

bool dSndBgmMgr_c::isPreparingBgmSoundIdWithStartOffset(u32 soundId, u32 startOffset) const {
    for (int i = 0; i < 7; i++) {
        if (mBgmSounds[i]->isPreparingSoundIdWithStartOffset(soundId, startOffset)) {
            return true;
        }
    }
    return false;
}

bool dSndBgmMgr_c::isPreparedBgmSoundId(u32 soundId) const {
    for (int i = 0; i < 7; i++) {
        if (mBgmSounds[i]->isPreparedSoundId(soundId)) {
            return true;
        }
    }
    return false;
}

dSndBgmSound_c *dSndBgmMgr_c::getBgmSoundByIndex(u32 idx) {
    if (idx >= 7) {
        return nullptr;
    }
    return mBgmSounds[idx];
}

dSndBgmSound_c *dSndBgmMgr_c::getBgmSoundHandleForId(u32 soundId) const {
    for (int i = 0; i < 7; i++) {
        dSndBgmSound_c *snd = mBgmSounds[i];
        if (snd->isPlayingSoundId(soundId)) {
            return snd;
        }
    }
    return nullptr;
}

dSndBgmSound_c *dSndBgmMgr_c::findIdleBgmSoundHandle() {
    for (int i = 0; i < 4; i++) {
        dSndBgmSound_c *snd = mBgmSounds[i];
        if (!snd->hasState()) {
            return snd;
        }
    }
    return nullptr;
}

void dSndBgmMgr_c::addToBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound) {
    if (list < BGM_LIST_MAX && sound != nullptr) {
        removeFromAllBgmSoundLists(sound);
        nw4r::ut::List_Prepend(&mBgmSoundLists[list], sound);
    }
}

void dSndBgmMgr_c::appendToBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound) {
    if (list < BGM_LIST_MAX && sound != nullptr) {
        removeFromAllBgmSoundLists(sound);
        nw4r::ut::List_Append(&mBgmSoundLists[list], sound);
    }
}

void dSndBgmMgr_c::removeFromBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound) {
    if (list < BGM_LIST_MAX && sound != nullptr) {
        if (isInBgmSoundList(list, sound)) {
            nw4r::ut::List_Remove(&mBgmSoundLists[list], sound);
        }
    }
}

void dSndBgmMgr_c::removeFromAllBgmSoundLists(dSndBgmSound_c *sound) {
    for (int i = 0; i < BGM_LIST_MAX; i++) {
        removeFromBgmSoundList(static_cast<BgmSoundList_e>(i), sound);
    }
}

bool dSndBgmMgr_c::isInBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound) {
    if (list >= BGM_LIST_MAX) {
        return false;
    }

    if (sound == nullptr) {
        return false;
    }

    for (dSndBgmSound_c *it = getFirstInBgmSoundList(list); it != nullptr; it = getNextInBgmSoundList(list, it)) {
        if (it == sound) {
            return true;
        }
    }
    return false;
}

dSndBgmSound_c *dSndBgmMgr_c::getFirstInBgmSoundList(BgmSoundList_e list) {
    if (list >= BGM_LIST_MAX) {
        return nullptr;
    }
    return static_cast<dSndBgmSound_c *>(nw4r::ut::List_GetFirst(&mBgmSoundLists[list]));
}

dSndBgmSound_c *dSndBgmMgr_c::getNextInBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound) {
    if (list >= BGM_LIST_MAX) {
        return nullptr;
    }
    return static_cast<dSndBgmSound_c *>(nw4r::ut::List_GetNext(&mBgmSoundLists[list], sound));
}

dSndBgmSound_c *dSndBgmMgr_c::getLastInBgmSoundList(BgmSoundList_e list) {
    if (list >= BGM_LIST_MAX) {
        return nullptr;
    }
    return static_cast<dSndBgmSound_c *>(nw4r::ut::List_GetLast(&mBgmSoundLists[list]));
}

dSndSound_c *dSndBgmMgr_c::getFreeFanSoundHandle() {
    for (int i = 0; i < 3; i++) {
        if (!mFanSounds[i]->IsAttachedSound()) {
            return mFanSounds[i];
        }
    }
    return nullptr;
}

dSndSound_c *dSndBgmMgr_c::getFanSoundHandleForFan(u32 soundId) {
    for (int i = 0; i < 3; i++) {
        if (mFanSounds[i]->GetId() == soundId) {
            return mFanSounds[i];
        }
    }
    return nullptr;
}

dSndSound_c *dSndBgmMgr_c::getSoundHandleCurrentlyPlayingFanSound(u32 soundId) {
    for (int i = 0; i < 3; i++) {
        if (mFanSounds[i]->isPlayingSoundId(soundId)) {
            return mFanSounds[i];
        }
    }
    return nullptr;
}

dSndSound_c *dSndBgmMgr_c::getFanSoundHandleForFan2(u32 soundId) {
    for (int i = 0; i < 3; i++) {
        if (mFanSounds[i]->GetId() == soundId) {
            return mFanSounds[i];
        }
    }
    return nullptr;
}

bool dSndBgmMgr_c::isCurrentlyPlayingFanSound(u32 soundId) const {
    for (int i = 0; i < 3; i++) {
        if (mFanSounds[i]->isPlayingSoundId(soundId)) {
            return true;
        }
    }
    return false;
}

bool dSndBgmMgr_c::hasPlayingFanSounds() const {
    for (int i = 0; i < 3; i++) {
        if (mFanSounds[i]->isPlaying()) {
            return true;
        }
    }
    return false;
}

void dSndBgmMgr_c::stopFanSounds(s32 fadeFrames) {
    for (int i = 0; i < 3; i++) {
        mFanSounds[i]->stop(fadeFrames);
    }
}

void dSndBgmMgr_c::registSound(dSndSound_c *sound) {
    if (sound == nullptr) {
        return;
    }
    unregistSound(sound);
    nw4r::ut::List_Append(&mAllSoundsList, sound);
}

void dSndBgmMgr_c::unregistSound(dSndSound_c *sound) {
    if (isSoundRegist(sound)) {
        nw4r::ut::List_Remove(&mAllSoundsList, sound);
    }
}

bool dSndBgmMgr_c::isSoundRegist(dSndSound_c *sound) {
    if (sound == nullptr) {
        return false;
    }
    for (dSndSound_c *it = getAllSoundsFirst(); it != nullptr; it = getAllSoundsNext(it)) {
        if (it == sound) {
            return true;
        }
    }
    return false;
}
