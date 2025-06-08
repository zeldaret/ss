#ifndef D_SND_CHECKERS_H
#define D_SND_CHECKERS_H

#include "nw4r/snd/snd_SoundHandle.h"
#include "toBeSorted/music_mgrs.h"

class SoundPropertiesChecker {
public:
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

        if (fn_803721F0(FANFARE_SOUND_MGR, pHandle.GetId())) {
            if (mpCounter3 != nullptr) {
                (*mpCounter3)++;
            } else {
                mCounter3++;
            }
            return;
        }

        if (fn_803720E0(FANFARE_SOUND_MGR, pHandle.GetId())) {
            if (mpCounter3 != nullptr) {
                (*mpCounter3)++;
            } else {
                mCounter3++;
            }
            return;
        }

        if (fn_80372070(FANFARE_SOUND_MGR, pHandle.GetId())) {
            if (mpCounter4 != nullptr) {
                (*mpCounter4)++;
            } else {
                mCounter4++;
            }
            return;
        }

        if (fn_803734C0(FANFARE_SOUND_MGR, pHandle.GetId()) && !fn_80373550(FANFARE_SOUND_MGR, pHandle.GetId())) {
            mCounter3++;
        }
    }

private:
    /* 0x04 */ u32 mSoundId;
    /* 0x08 */ u32 mPlayCounter;
    /* 0x0C */ u32 mPauseCounter;
    /* 0x10 */ u32 mCounter3;
    /* 0x14 */ u32 mCounter4;
    /* 0x18 */ u32 *mpPlayCounter;
    /* 0x1C */ u32 *mpPauseCounter;
    /* 0x20 */ u32 *mpCounter3;
    /* 0x24 */ u32 *mpCounter4;
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
    ~SoundStopper() {}

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
