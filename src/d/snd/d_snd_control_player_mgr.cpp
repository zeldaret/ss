#include "d/snd/d_snd_control_player_mgr.h"

#include "common.h"
#include "d/snd/d_snd_control_player.h"
#include "nw4r/ut/ut_list.h"

template class SndMgrDisposer<dSndControlPlayerMgr_c>;

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

void dSndControlPlayerMgr_c::calc() {}

void dSndControlPlayerMgr_c::executeControls() {
    dSndControlPlayer_c *next;
    dSndControlPlayer_c *iter = static_cast<dSndControlPlayer_c *>(nw4r::ut::List_GetNext(&mActiveControls, nullptr));
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
            // not overridden. The effect of this is that
            // something will set the targetValue to a specific value
            f32 currentVolume = getAppliedPlayerVolume(i);
            f32 targetVolume = mpTargetVolumes[i];
            f32 maxVolumeDecrease = mpMaxVolumeDecreases[i];
            f32 maxVolumeIncrease = mpMaxVolumeDecreases[i];

            if (currentVolume != targetVolume) {
                if (currentVolume - targetVolume > maxVolumeDecrease) {
                    targetVolume = currentVolume - maxVolumeDecrease;
                } else if (targetVolume - currentVolume > maxVolumeIncrease) {
                    targetVolume = currentVolume + maxVolumeIncrease;
                }
                // @bug not actually clamped
                nw4r::ut::Clamp(targetVolume, 0.0f, 2.0f);
                setVolume(i, targetVolume, 0);
            }
            mpTargetVolumes[i] = 1.0f;
            mpMaxVolumeDecreases[i] = 0.1f;
            mpMaxVolumeDecreases[i] = 0.025f;
        }
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

    } else {
        // Volume is not overridden, set volume normally
        setControlValue(CTRL_VOLUME, playerIdx, value, frames);
        mpSavedVolumes[playerIdx] = getControlVolumeTarget(CTRL_VOLUME, playerIdx);
    }
}

void dSndControlPlayerMgr_c::overrideVolume(u32 playerIdx, f32 volume, s32 frames) {
    if (playerIdx >= sNumPlayers) {
        return;
    }
    u32 mask = (1 << playerIdx);
    if ((mOverrideVolumeMask & mask) == 0) {
        mpSavedVolumes[playerIdx] = getControlVolumeTarget(CTRL_VOLUME, playerIdx);
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

f32 dSndControlPlayerMgr_c::getControlVolumeTarget(PlayerCtrl_e ctrlType, u32 playerIdx) const {
    if (ctrlType >= CTRL_MAX) {
        return 1.0f;
    }
    if (playerIdx >= sNumPlayers) {
        return 1.0f;
    }
    return mpCtrls[ctrlType][playerIdx].getTargetValue();
}

void dSndControlPlayerMgr_c::setShortParameterTo5(s32 idx1, s32 idx2) {
    if (idx1 >= 4) {
        return;
    }

    if (idx2 >= 2) {
        return;
    }

    field_0x3C[idx1][idx2] = 5;
}
