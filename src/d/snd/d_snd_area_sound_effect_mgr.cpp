#include "d/snd/d_snd_area_sound_effect_mgr.h"

#include "common.h"
#include "d/d_sc_game.h"
#include "d/snd/d_snd_checkers.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SoundPlayer.h"
#include "nw4r/snd/snd_SoundStartable.h"

SND_DISPOSER_DEFINE(dSndAreaSoundEffectMgr_c);

dSndAreaSoundEffectMgr_c::dSndAreaSoundEffectMgr_c()
    : field_0x32C(1.0f), field_0x330(1.0f), mActor(dSndMgr_c::GetInstance()->getPlayer()), mTargetVolume(1.0f) {}

void dSndAreaSoundEffectMgr_c::calc() {
    for (int i = 0; i < 3; i++) {
        mSounds[i].calc();
    }
    calcPlayerVolume();
    calcActorVolume();
}

void dSndAreaSoundEffectMgr_c::calcPlayerVolume() {
    if (dSndStateMgr_c::GetInstance()->checkEventFlag(dSndStateMgr_c::EVENT_MUTE_STAGE_EFFECTS_FULL)) {
        dSndControlPlayerMgr_c::GetInstance()->setStageEffectsMuteVolume(0.0f);
    } else if (dSndStateMgr_c::GetInstance()->checkEventFlag(dSndStateMgr_c::EVENT_MUTE_STAGE_EFFECTS_PARTIAL)) {
        dSndControlPlayerMgr_c::GetInstance()->setStageEffectsMuteVolume(0.3f);
    } 

    if (dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_PAUSE)) {
        dSndControlPlayerMgr_c::GetInstance()->setStageEffectsMuteVolume(0.3f);
    }

    if (dSndStateMgr_c::GetInstance()->checkFlag0x258(0x04)) {
        dSndControlPlayerMgr_c::GetInstance()->setStageEffectsMuteVolume(0.3f);
    }
}

void dSndAreaSoundEffectMgr_c::calcActorVolume() {
    // TODO - FPR regswap
    f32 volume = mActor.GetVolume();
    f32 target = mTargetVolume;
    if (volume != target) {
        if (volume - target > 0.05f) {
            volume -= 0.05f;
        } else if (target - volume > 0.05f) {
            volume += 0.05f;
        }

        if (volume < 0.0f) {
            volume = 0.0f;
        } else if (volume > 2.0f) {
            volume = 2.0f;
        }
        mActor.SetVolume(volume);
        setTgSoundVolume(volume, 0);
    }
    mTargetVolume = 1.0f;
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
    nw4r::snd::SoundStartable::StartResult res = mActor.StartSoundReturnStatus(snd, soundId, nullptr);
    if (res == nw4r::snd::SoundStartable::START_SUCCESS) {
        snd->fadeIn(soundId, 30);
    }

    // @bug should probably be `res != nw4r::snd::SoundStartable::START_SUCCESS`
    // all callers seem to ignore return value though
    return res;
}

bool dSndAreaSoundEffectMgr_c::holdInWaterLvSound(f32 depth) {
    if (depth <= 0) {
        return false;
    }

    // TODO: Ugh, maybe convert the enums to unsigned ints?
    bool ret = dSndMgr_c::GetInstance()->holdSound(&mInWaterLvHandle, (unsigned int)SE_A_IN_WATER_LV);
    if (ret) {
        f32 volume = 1.0f;
        if (depth < 600.0f) {
            volume = (depth / 600.0f) * 0.5f + 0.5f;
        }
        mInWaterLvHandle.SetVolume(volume, 0);
    }

    f32 otherVolume = 0.0f;
    if (depth < 300.0f) {
        otherVolume = (depth * -0.7f) / 300.0f + 0.7f;
        if (otherVolume < 0.0f) {
            otherVolume = 0.0f;
        }
    }
    mTargetVolume *= otherVolume;

    return ret;
}

void dSndAreaSoundEffectMgr_c::onEventStart() {
    // no-op
}

void dSndAreaSoundEffectMgr_c::onEventEnd() {
    // no-op
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

void dSndAreaSoundEffectMgr_c::setTgSoundVolume(f32 volume, s32 fadeFrames) {
    nw4r::snd::SoundPlayer *player =
        dSndControlPlayerMgr_c::GetInstance()->getPlayer1(dSndPlayerMgr_c::PLAYER_TG_SOUND);
    SoundVolumeSetter setter;
    setter.mFadeFrames = fadeFrames;
    if (volume > 1.0f) {
        volume = 1.0f;
    } else if (volume < 0.0f) {
        volume = 0.0f;
    }
    setter.mVolume = volume;

    player->ForEachSound(setter, false);
}
