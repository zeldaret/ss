#include "d/snd/d_snd_bgm_sound_harp_mgr.h"

#include "common.h"

dSndBgmSoundHarpMgr_c::dSndBgmSoundHarpMgr_c()
    : mpCurrVarSet(nullptr), mSoundId(-1), mTempo(-1), mPrevIdx(-1), mIsLoaded(false) {
    reset();
}

void dSndBgmSoundHarpMgr_c::setSoundId(u32 soundId) {
    mSoundId = soundId;
}

void dSndBgmSoundHarpMgr_c::setLoaded() {
    mIsLoaded = true;
}

void dSndBgmSoundHarpMgr_c::reset() {
    mSoundId = -1;
    mTempo = -1;
    mPrevIdx = -1;
    mHarpData.resetVars();
    mpCurrVarSet = 0;
    mIsLoaded = false;
}

void dSndBgmSoundHarpMgr_c::setPlaySamplePosition(s32 position) {
    if (position < 0) {
        return;
    }
    s32 idx = mHarpData.getIdxForPosition(position);
    if (idx != mPrevIdx && idx >= 0) {
        mHarpData.writeSeqVars(idx);
        mpCurrVarSet = mHarpData.getUsableVarSet(idx);
        mPrevIdx = idx;
    }
}

const dSndBgmDataHarpVarSetBase_c *dSndBgmSoundHarpMgr_c::getCurrentVarSet() {
    return mpCurrVarSet;
}
