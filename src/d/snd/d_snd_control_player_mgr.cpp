#include "d/snd/d_snd_control_player_mgr.h"

#include "common.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_control_player.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/ut/ut_list.h"
#include "toBeSorted/music_mgrs.h"

struct FanfareMuteFlagsApplier {
    ~FanfareMuteFlagsApplier() {}
    virtual void operator()(nw4r::snd::SoundHandle &handle) {
        // The logic here is inverted compared to the others - Fanfares seem to
        // mute things by default unless otherwise speciefied
        u32 id = handle.GetId();
        if (dSndBgmMgr_c::GetInstance()->getSoundHandleCurrentlyPlayingFanSound(id)) {
            u32 userParam = dSndMgr_c::GetInstance()->getArchive()->GetSoundUserParam(id);
            if (!(userParam & dSndPlayerMgr_c::FANFARE_UNMUTE_BGM)) {
                dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                    dSndControlPlayerMgr_c::CTRL_GROUP_BGM, dSndControlPlayerMgr_c::MUTE_FULL
                );
                dSndControlPlayerMgr_c::GetInstance()->setBgmVolumeDecreaseSpeed(0.2f);
            }

            if (fn_80364DA0(ENEMY_SOUND_MGR)) {
                if (!(userParam & dSndPlayerMgr_c::FANFARE_UNMUTE_STAGE_EFFECTS)) {
                    dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                        dSndControlPlayerMgr_c::CTRL_GROUP_STAGE_EFFECTS, dSndControlPlayerMgr_c::MUTE_FULL
                    );
                }
                if (!(userParam & dSndPlayerMgr_c::FANFARE_UNMUTE_ENEMY)) {
                    dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                        dSndControlPlayerMgr_c::CTRL_GROUP_ENEMY, dSndControlPlayerMgr_c::MUTE_FULL
                    );
                }
                if (!(userParam & dSndPlayerMgr_c::FANFARE_UNMUTE_OBJECTS)) {
                    dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                        dSndControlPlayerMgr_c::CTRL_GROUP_OBJECTS, dSndControlPlayerMgr_c::MUTE_FULL
                    );
                }
            }
        }
    };
};

struct SmallMuteFlagsApplier {
    ~SmallMuteFlagsApplier() {}
    virtual void operator()(nw4r::snd::SoundHandle &handle) {
        u32 userParam = dSndMgr_c::GetInstance()->getArchive()->GetSoundUserParam(handle.GetId());

        if (userParam & dSndPlayerMgr_c::MUTE_BGM_FULL) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_BGM, dSndControlPlayerMgr_c::MUTE_FULL
            );
        } else if (userParam & dSndPlayerMgr_c::MUTE_BGM_PART) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_BGM, dSndControlPlayerMgr_c::MUTE_PARTIAL
            );
        }

        if (userParam & dSndPlayerMgr_c::MUTE_STAGE_EFFECTS_FULL) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_STAGE_EFFECTS, dSndControlPlayerMgr_c::MUTE_FULL
            );
        } else if (userParam & dSndPlayerMgr_c::MUTE_STAGE_EFFECTS_PART) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_STAGE_EFFECTS, dSndControlPlayerMgr_c::MUTE_PARTIAL
            );
        }

        if (userParam & dSndPlayerMgr_c::MUTE_ENEMY_FULL) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_ENEMY, dSndControlPlayerMgr_c::MUTE_FULL
            );
        } else if (userParam & dSndPlayerMgr_c::MUTE_ENEMY_PART) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_ENEMY, dSndControlPlayerMgr_c::MUTE_PARTIAL
            );
        }

        if (userParam & dSndPlayerMgr_c::MUTE_OBJECTS_FULL) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_OBJECTS, dSndControlPlayerMgr_c::MUTE_FULL
            );
        } else if (userParam & dSndPlayerMgr_c::MUTE_OBJECTS_PART) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_OBJECTS, dSndControlPlayerMgr_c::MUTE_PARTIAL
            );
        }
    };
};

struct EventMuteFlagsApplier {
    ~EventMuteFlagsApplier() {}
    virtual void operator()(nw4r::snd::SoundHandle &handle) {
        u32 userParam = dSndPlayerMgr_c::GetInstance()->getEventMuteMask(handle.GetId());

        if (userParam & dSndPlayerMgr_c::MUTE_BGM_FULL) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_BGM, dSndControlPlayerMgr_c::MUTE_FULL
            );
        } else if (userParam & dSndPlayerMgr_c::MUTE_BGM_PART) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_BGM, dSndControlPlayerMgr_c::MUTE_PARTIAL
            );
        }

        if (userParam & dSndPlayerMgr_c::MUTE_STAGE_EFFECTS_FULL) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_STAGE_EFFECTS, dSndControlPlayerMgr_c::MUTE_FULL
            );
        } else if (userParam & dSndPlayerMgr_c::MUTE_STAGE_EFFECTS_PART) {
            dSndControlPlayerMgr_c::GetInstance()->setGroupVolumeFlag(
                dSndControlPlayerMgr_c::CTRL_GROUP_STAGE_EFFECTS, dSndControlPlayerMgr_c::MUTE_PARTIAL
            );
        }
    };
};

static FanfareMuteFlagsApplier fanfareApplier;
static SmallMuteFlagsApplier smallApplier;
static EventMuteFlagsApplier eventApplier;

SND_DISPOSER_DEFINE(dSndControlPlayerMgr_c);

dSndControlPlayerMgr_c::dSndControlPlayerMgr_c() : mOverrideVolumeMask(0) {
    // TODO offsetof
    nw4r::ut::List_Init(&mActiveControls, 0x24);

    mpCtrls[CTRL_VOLUME] = new dSndControlPlayerVolume_c[sNumPlayers]();
    mpCtrls[CTRL_LPF_FREQ] = new dSndControlPlayerLpfFreq_c[sNumPlayers]();
    mpCtrls[CTRL_FX_SEND] = new dSndControlPlayerFxSend_c[sNumPlayers]();

    mpSavedVolumes = new f32[sNumPlayers];
    mpTargetVolumes = new f32[sNumPlayers];
    mpMaxVolumeDecreases = new f32[sNumPlayers];
    mpMaxVolumeIncreases = new f32[sNumPlayers];
}

void dSndControlPlayerMgr_c::calc() {
    calcMuteFlags();
    calcVolumes();
    executeControls();
}

void dSndControlPlayerMgr_c::executeControls() {
    dSndControlPlayer_c *next;
    dSndControlPlayer_c *iter = static_cast<dSndControlPlayer_c *>(nw4r::ut::List_GetFirst(&mActiveControls));
    while (iter != nullptr) {
        next = static_cast<dSndControlPlayer_c *>(nw4r::ut::List_GetNext(&mActiveControls, iter));
        iter->calc();
        iter->apply();
        if (iter->isFinished()) {
            unlinkCtrl(iter);
        }
        iter = next;
    }
}

void dSndControlPlayerMgr_c::calcVolumes() {
    for (int i = 0; i < sNumPlayers; i++) {
        if ((mOverrideVolumeMask & (1 << i)) != 0) {
            // overridden volume
            mpTargetVolumes[i] = mpSavedVolumes[i];
        } else {
            // not overridden.
            f32 currentVolume = getAppliedPlayerVolume(i);
            f32 targetVolume = mpTargetVolumes[i];
            f32 maxVolumeDecrease = mpMaxVolumeDecreases[i];
            f32 maxVolumeIncrease = mpMaxVolumeIncreases[i];

            if (currentVolume != targetVolume) {
                if (currentVolume - targetVolume > maxVolumeDecrease) {
                    targetVolume = currentVolume - maxVolumeDecrease;
                } else if (targetVolume - currentVolume > maxVolumeIncrease) {
                    targetVolume = currentVolume + maxVolumeIncrease;
                }
                // @bug not actually clamped
                (void)nw4r::ut::Clamp(targetVolume, 0.0f, 2.0f);
                setVolume(i, targetVolume, 0);
            }
            mpTargetVolumes[i] = 1.0f;
            mpMaxVolumeDecreases[i] = 0.1f;
            mpMaxVolumeIncreases[i] = 0.025f;
        }
    }
}

void dSndControlPlayerMgr_c::calcMuteFlags() {
    getPlayer1(dSndPlayerMgr_c::PLAYER_FAN)->ForEachSound(fanfareApplier, false);
    getPlayer1(dSndPlayerMgr_c::PLAYER_SMALL_IMPORTANT)->ForEachSound(smallApplier, false);
    getPlayer1(dSndPlayerMgr_c::PLAYER_SMALL_NORMAL)->ForEachSound(smallApplier, false);
    getPlayer1(dSndPlayerMgr_c::PLAYER_EVENT)->ForEachSound(eventApplier, false);

    f32 volumesByGroup[CTRL_GROUP_MAX];
    for (int i = 0; i < CTRL_GROUP_MAX; i++) {
        volumesByGroup[i] = 1.0f;
        if (mTimersForGroupVolume[i][MUTE_PARTIAL] > 0) {
            mTimersForGroupVolume[i][MUTE_PARTIAL]--;
            volumesByGroup[i] = 0.3f;
        }

        if (mTimersForGroupVolume[i][MUTE_FULL] > 0) {
            mTimersForGroupVolume[i][MUTE_FULL]--;
            volumesByGroup[i] = 0.0f;
        }
    }

    if (volumesByGroup[CTRL_GROUP_BGM] < 1.0f) {
        setBgmMuteVolume(volumesByGroup[CTRL_GROUP_BGM]);
    }

    if (volumesByGroup[CTRL_GROUP_STAGE_EFFECTS] < 1.0f) {
        setStageEffectsMuteVolume(volumesByGroup[CTRL_GROUP_STAGE_EFFECTS]);
    }

    if (volumesByGroup[CTRL_GROUP_ENEMY] < 1.0f) {
        setEnemyMuteVolume(volumesByGroup[CTRL_GROUP_ENEMY]);
    }

    if (volumesByGroup[CTRL_GROUP_OBJECTS] < 1.0f) {
        setObjectMuteVolume(volumesByGroup[CTRL_GROUP_OBJECTS]);
    }
}

void dSndControlPlayerMgr_c::linkCtrl(dSndControlPlayer_c *ctrl) {
    if (ctrl == nullptr) {
        return;
    }
    if (ctrl->isLinked()) {
        return;
    }
    ctrl->setLinked(true);
    nw4r::ut::List_Append(&mActiveControls, ctrl);
}

void dSndControlPlayerMgr_c::unlinkCtrl(dSndControlPlayer_c *ctrl) {
    if (ctrl == nullptr) {
        return;
    }
    if (!ctrl->isLinked()) {
        return;
    }
    ctrl->setLinked(false);
    nw4r::ut::List_Remove(&mActiveControls, ctrl);
}

void dSndControlPlayerMgr_c::setVolume(u32 playerIdx, f32 value, s32 frames) {
    if (playerIdx >= sNumPlayers) {
        return;
    }

    u32 mask = (1 << playerIdx);
    if ((mOverrideVolumeMask & mask) != 0) {
        // Volume is overridden, set saved volume as to not interrupt override
        mpSavedVolumes[playerIdx] = value;
        setPlayerVolumeInternal(playerIdx, value);
    } else {
        // Volume is not overridden, set volume normally
        setControlValue(CTRL_VOLUME, playerIdx, value, frames);
        mpSavedVolumes[playerIdx] = getControlTarget(CTRL_VOLUME, playerIdx);
    }
}

void dSndControlPlayerMgr_c::overrideVolume(u32 playerIdx, f32 volume, s32 frames) {
    if (playerIdx >= sNumPlayers) {
        return;
    }
    u32 mask = (1 << playerIdx);
    if ((mOverrideVolumeMask & mask) == 0) {
        mpSavedVolumes[playerIdx] = getControlTarget(CTRL_VOLUME, playerIdx);
        mOverrideVolumeMask |= mask;
    }
    setControlValue(CTRL_VOLUME, playerIdx, volume, frames);
}

void dSndControlPlayerMgr_c::restoreVolume(u32 playerIdx, s32 frames) {
    if (playerIdx >= sNumPlayers) {
        return;
    }
    u32 mask = (1 << playerIdx);
    if ((mOverrideVolumeMask & mask) == 0) {
        return;
    }
    mOverrideVolumeMask &= ~mask;
    setControlValue(CTRL_VOLUME, playerIdx, mpSavedVolumes[playerIdx], frames);
}

void dSndControlPlayerMgr_c::setLpfFreq(u32 playerIdx, f32 value, s32 frames) {
    if (playerIdx < sNumPlayers) {
        setControlValue(CTRL_LPF_FREQ, playerIdx, value, frames);
    }
}

void dSndControlPlayerMgr_c::setFxSend(u32 playerIdx, f32 value, s32 frames) {
    if (playerIdx < sNumPlayers) {
        setControlValue(CTRL_FX_SEND, playerIdx, value, frames);
    }
}

void dSndControlPlayerMgr_c::setControlValue(PlayerCtrl_e ctrlType, u32 playerIdx, f32 value, s32 frames) {
    if (ctrlType < CTRL_MAX && playerIdx < sNumPlayers) {
        dSndControlPlayer_c *ctrl = &mpCtrls[ctrlType][playerIdx];
        ctrl->set(value, frames);
        if (ctrl->isFinished()) {
            unlinkCtrl(ctrl);
        } else {
            linkCtrl(ctrl);
        }
        ctrl->apply();
    }
}

void dSndControlPlayerMgr_c::resetControls() {
    for (s32 i = 0; i < sNumPlayers; i++) {
        for (s32 ty = 0; ty < CTRL_MAX; ty++) {
            mpCtrls[ty][i].reset();
        }
        mpSavedVolumes[i] = 1.0f;
    }
    mOverrideVolumeMask = 0;
}

f32 dSndControlPlayerMgr_c::getAppliedPlayerVolume(u32 playerIdx) const {
    if (playerIdx >= sNumPlayers) {
        return 0.0f;
    }
    return getPlayer1(playerIdx)->GetVolume();
}

f32 dSndControlPlayerMgr_c::getControlTarget(PlayerCtrl_e ctrlType, u32 playerIdx) const {
    if (ctrlType >= CTRL_MAX) {
        return 1.0f;
    }
    if (playerIdx >= sNumPlayers) {
        return 1.0f;
    }
    return mpCtrls[ctrlType][playerIdx].getTargetValue();
}

// TODO - not sure what this actually does
void dSndControlPlayerMgr_c::unmutePlayer(u32 idx, s32 frames) {
    if (idx >= sNumPlayers) {
        return;
    }

    restoreVolume(idx, 0);
    overrideVolume(idx, 0.0f, 0);
    restoreVolume(idx, frames);
}


void dSndControlPlayerMgr_c::unmuteScenePlayers(s32 frames) {
    for (u32 i = dSndPlayerMgr_c::PLAYER_ENEMY; i < dSndPlayerMgr_c::PLAYER_AREA; i++) {
        unmutePlayer(i, frames);
    }
}

void dSndControlPlayerMgr_c::muteScenePlayers(s32 frames) {
    for (u32 i = dSndPlayerMgr_c::PLAYER_ENEMY; i < dSndPlayerMgr_c::PLAYER_AREA; i++) {
        overrideVolume(i, 0.0f, frames);
    }
}

void dSndControlPlayerMgr_c::setGroupVolumeFlag(VolumeControlGroup group, MuteLevel level) {
    if (group >= CTRL_GROUP_MAX) {
        return;
    }

    if (level >= MUTE_MAX) {
        return;
    }

    mTimersForGroupVolume[group][level] = 5;
}

void dSndControlPlayerMgr_c::setPlayerVolumeInternal(u32 playerIdx, f32 volume) {
    if (playerIdx >= sNumPlayers) {
        return;
    }
    // @bug not actually clamped
    (void)nw4r::ut::Clamp(volume, 0.0f, 2.0f);

    if (mpTargetVolumes[playerIdx] > volume) {
        mpTargetVolumes[playerIdx] = volume;
    }

    u32 mask = (1 << playerIdx);
    if ((mOverrideVolumeMask & mask) != 0) {
        mpSavedVolumes[playerIdx] = mpTargetVolumes[playerIdx];
    }
}

void dSndControlPlayerMgr_c::setBgmMuteVolume(f32 volume) {
    for (u32 i = dSndPlayerMgr_c::PLAYER_BGM; i < dSndPlayerMgr_c::PLAYER_BGM_BATTLE + 1; i++) {
        setPlayerVolumeInternal(i, volume);
    }
}

void dSndControlPlayerMgr_c::setBgmVolumeDecreaseSpeed(f32 decrease) {}

void dSndControlPlayerMgr_c::setStageEffectsVolume(f32 volume, s32 fadeFrames) {
    for (u32 i = dSndPlayerMgr_c::PLAYER_TG_SOUND; i <= dSndPlayerMgr_c::PLAYER_AREA_IN_WATER_LV; i++) {
        setVolume(i, volume, fadeFrames);
    }
}

void dSndControlPlayerMgr_c::setStageEffectsMuteVolume(f32 volume) {
    for (u32 i = dSndPlayerMgr_c::PLAYER_TG_SOUND; i <= dSndPlayerMgr_c::PLAYER_AREA_IN_WATER_LV; i++) {
        setPlayerVolumeInternal(i, volume);
    }
}

void dSndControlPlayerMgr_c::setEnemyMuteVolume(f32 volume) {
    for (u32 i = dSndPlayerMgr_c::PLAYER_ENEMY; i < dSndPlayerMgr_c::PLAYER_ENEMY_FOOTSTEP + 1; i++) {
        setPlayerVolumeInternal(i, volume);
    }
}

void dSndControlPlayerMgr_c::setObjectMuteVolume(f32 volume) {
    for (u32 i = dSndPlayerMgr_c::PLAYER_OBJECT_1; i < dSndPlayerMgr_c::PLAYER_TG_SOUND + 1; i++) {
        if (i != dSndPlayerMgr_c::PLAYER_NPC_VOICE) {
            setPlayerVolumeInternal(i, volume);
        }
    }
}
