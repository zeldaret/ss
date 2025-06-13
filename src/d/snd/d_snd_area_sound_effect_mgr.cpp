#include "d/snd/d_snd_area_sound_effect_mgr.h"

#include "common.h"
#include "d/d_sc_game.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_SoundStartable.h"

SND_DISPOSER_DEFINE(dSndAreaSoundEffectMgr_c);

dSndAreaSoundEffectMgr_c::dSndAreaSoundEffectMgr_c()
    : field_0x328(0),
      field_0x32C(1.0f),
      field_0x330(1.0f),
      mActor(dSndMgr_c::GetInstance()->getPlayer()),
      field_0x388(1.0f) {}

void dSndAreaSoundEffectMgr_c::calc() {
    for (int i = 0; i < 3; i++) {
        mSounds[i].calc();
    }
}

bool dSndAreaSoundEffectMgr_c::startSound(u32 soundId, u32 handleIdx) {
    if (dSndPlayerMgr_c::GetInstance()->checkFlag(0x2)) {
        return false;
    }

    if (isPlayingSound(soundId)) {
        return false;
    }

    if (handleIdx > 2) {
        return false;
    }

    if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL) {
        return false;
    }

    dSndAreaSound_c *snd = &mSounds[handleIdx];
    nw4r::snd::SoundStartable::StartResult res = mActor.detail_StartSound(snd, soundId, nullptr);
    if (res == nw4r::snd::SoundStartable::START_SUCCESS) {
        snd->fadeIn(soundId, 30);
    }

    // @bug should probably be `res != nw4r::snd::SoundStartable::START_SUCCESS`
    // all callers seem to ignore return value though
    return res;
}

void dSndAreaSoundEffectMgr_c::stopSounds(s32 fadeFrames) {
    for (u32 i = 0; i < 3; i++) {
        mSounds[i].stop(fadeFrames);
    }
}

bool dSndAreaSoundEffectMgr_c::isPlayingAnySound() const {
    for (int i = 0; i < 3; i++) {
        if (mSounds[i].IsAttachedSound()) {
            return true;
        }
    }
    return false;
}

bool dSndAreaSoundEffectMgr_c::isPlayingSound(u32 soundId) const {
    for (int i = 0; i < 3; i++) {
        if (mSounds[i].GetId() == soundId) {
            return true;
        }
    }
    return false;
}
