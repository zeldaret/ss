#include "d/snd/d_snd_control_player.h"

#include "common.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_sound.h"
#include "nw4r/snd/snd_SoundPlayer.h"
#include "nw4r/snd/snd_global.h"

dSndControlPlayerVolumeBase_c::dSndControlPlayerVolumeBase_c() : dSndControlPlayer_c(1.0f, 0.0f, 1.0f) {
    mpPlayer = nullptr;
    mIndex = 0;
}

void dSndControlPlayer_c::reset() {
    dSndControl_c::reset();
    apply();
}

void dSndControlPlayer_c::setIndex1(s32 idx) {
    mpPlayer = dSndControlPlayerMgr_c::GetInstance()->getPlayer1(idx);
    mIndex = idx;
}

void dSndControlPlayer_c::setIndex2(s32 idx) {
    mpPlayer = dSndControlPlayerMgr_c::GetInstance()->getPlayer2(idx);
    mIndex = idx;
}

void dSndControlPlayer_c::stop() {
    mCurrValue = getAppliedValue();
    dSndControl_c::stop();
}

void dSndControlPlayerVolume_c::apply() {
    mpPlayer->SetVolume(mCurrValue);
}

f32 dSndControlPlayerVolume_c::getAppliedValue() {
    return mpPlayer->GetVolume();
}

void dSndControlPlayerLpfFreq_c::apply() {
    mpPlayer->SetLpfFreq(mCurrValue);
}

f32 dSndControlPlayerLpfFreq_c::getAppliedValue() {
    return mpPlayer->GetLpfFreq();
}

void dSndControlPlayerFxSend_c::apply() {
    if (mCurrValue < mMin) {
        mCurrValue = mMin;
    }
    if (mCurrValue >= mMax) {
        mCurrValue = mMax;
    }
    mpPlayer->SetFxSend(nw4r::snd::AUX_B, mCurrValue);
}

extern "C" bool fn_8035F030(s32);

f32 dSndControlPlayerFxSend_c::getAppliedValue() {
    int auxBus = nw4r::snd::AUX_A;
    if (fn_8035F030(mIndex)) {
        auxBus = nw4r::snd::AUX_B;
    }
    return mpPlayer->GetFxSend(auxBus);
}
