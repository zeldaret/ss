#include "d/snd/d_snd_control_player_mgr.h"

#include "common.h"
#include "d/snd/d_snd_control_player.h"
#include "nw4r/ut/ut_list.h"

template class SndMgrDisposer<dSndControlPlayerMgr_c>;

dSndControlPlayerMgr_c::dSndControlPlayerMgr_c() : mControlMask(0) {
    // TODO offsetof
    nw4r::ut::List_Init(&mActiveControls, 0x24);

    mpCtrls[CTRL_VOLUME] = new dSndControlPlayerVolume_c[sNumPlayers]();
    mpCtrls[CTRL_LPF_FREQ] = new dSndControlPlayerLpfFreq_c[sNumPlayers]();
    mpCtrls[CTRL_FX_SEND] = new dSndControlPlayerFxSend_c[sNumPlayers]();

    field_0x28 = new f32[sNumPlayers];
    field_0x1C = new f32[sNumPlayers];
    field_0x20 = new f32[sNumPlayers];
    field_0x24 = new f32[sNumPlayers];
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
        field_0x28[i] = 1.0f;
    }
    mControlMask = 0;
}

f32 dSndControlPlayerMgr_c::getTargetValue(PlayerCtrl_e ctrlType, u32 playerIdx) const {
    if (ctrlType >= CTRL_MAX) {
        return 1.0f;
    }
    if (playerIdx >= sNumPlayers) {
        return 1.0f;
    }
    return mpCtrls[ctrlType][playerIdx].getTargetValue();
}
