#ifndef D_SND_CHECKERS_H
#define D_SND_CHECKERS_H

#include "common.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "nw4r/snd/snd_SoundHandle.h"

class SoundPropertiesChecker {
public:
    // @bug forgot to initialize mCounter4. see `dSndSourceEquipment_c::cbPcHit` for one consequence
    SoundPropertiesChecker(u32 soundId) : mSoundId(soundId), mPlayCounter(0), mPauseCounter(0), mCounter3(0), mpPlayCounter(nullptr), mpPauseCounter(nullptr), mpCounter3(nullptr), mpCounter4(nullptr) {}
    virtual void operator()(nw4r::snd::SoundHandle &pHandle) {
        if (mSoundId != -1 && mSoundId != pHandle.GetId()) {
            return;
        }

        if (mpPlayCounter != nullptr) {
            (*mpPlayCounter)++;
        } else {
            mPlayCounter++;
        }

        if (pHandle.IsPause()) {
            if (mpPauseCounter != nullptr) {
                (*mpPauseCounter)++;
            } else {
                mPauseCounter++;
            }
            return;
        }

        if (dSndBgmMgr_c::GetInstance()->isPreparedBgmSoundId(pHandle.GetId())) {
            if (mpCounter3 != nullptr) {
                (*mpCounter3)++;
            } else {
                mCounter3++;
            }
            return;
        }

        if (dSndBgmMgr_c::GetInstance()->isPreparingBgmSoundId(pHandle.GetId())) {
            if (mpCounter3 != nullptr) {
                (*mpCounter3)++;
            } else {
                mCounter3++;
            }
            return;
        }

        if (dSndBgmMgr_c::GetInstance()->isFadingOutBgmSoundId(pHandle.GetId())) {
            if (mpCounter4 != nullptr) {
                (*mpCounter4)++;
            } else {
                mCounter4++;
            }
            return;
        }

        if (dSndBgmMgr_c::GetInstance()->getFanSoundHandleForFan2(pHandle.GetId()) != nullptr &&
            !dSndBgmMgr_c::GetInstance()->isCurrentlyPlayingFanSound(pHandle.GetId())) {
            // Why not mpCounter3???
            mCounter3++;
        }
    }

public:
    /* 0x04 */ u32 mSoundId;
    /* 0x08 */ s32 mPlayCounter;
    /* 0x0C */ s32 mPauseCounter;
    /* 0x10 */ s32 mCounter3;
    /* 0x14 */ s32 mCounter4;
    /* 0x18 */ s32 *mpPlayCounter;
    /* 0x1C */ s32 *mpPauseCounter;
    /* 0x20 */ s32 *mpCounter3;
    /* 0x24 */ s32 *mpCounter4;
};

// used at the very least when trying to finish Demise
class IsCurrentSoundIdChecker {
public:
    IsCurrentSoundIdChecker(u32 id, bool *pResult) : mSoundId(id), mpResult(pResult) {}
    ~IsCurrentSoundIdChecker() {}

    virtual void operator()(nw4r::snd::SoundHandle &pHandle) {
        if (mSoundId == pHandle.GetId()) {
            *mpResult = true;
        }
    }

private:
    /* 0x04 */ u32 mSoundId;
    /* 0x08 */ bool *mpResult;
};

class SoundStopper {
public:
    SoundStopper(u32 id, s32 fadeFrames) : mSoundId(id), mFadeFrames(fadeFrames) {}
    // ~SoundStopper() {}

    virtual void operator()(nw4r::snd::SoundHandle &pHandle) {
        if (mSoundId == pHandle.GetId()) {
            pHandle.Stop(mFadeFrames);
        }
    }

private:
    /* 0x04 */ u32 mSoundId;
    /* 0x08 */ s32 mFadeFrames;
};

#endif
