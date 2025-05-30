#include "d/snd/d_snd_actor.h"

#include "common.h"
#include "d/snd/d_snd_checkers.h"
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "toBeSorted/music_mgrs.h"

dSndActor_c::dSndActor_c(nw4r::snd::SoundArchivePlayer &rStartable) : nw4r::snd::SoundActor(rStartable) {}

void d_snd_actor_SoundHandle_dtor() {
    nw4r::snd::SoundHandle handle[2];
}

void SoundPropertiesChecker::operator()(nw4r::snd::SoundHandle &pHandle) {
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
