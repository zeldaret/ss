#include "d/snd/d_snd_bgm_sound_battle.h"

#include "common.h"
#include "d/snd/d_snd_bgm_seq_config.h"
#include "d/snd/d_snd_bgm_sound.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"

dSndBgmBattleSound_c::dSndBgmBattleSound_c()
    : mpBgmBattleConfig(nullptr), mMuteGroupActiveMask(0), mPrevTick(0), field_0x190(0), field_0x191(1) {}

void dSndBgmBattleSound_c::cancel() {
    dSndBgmSound_c::cancel();
    mMuteGroupActiveMask = 0;
    mPrevTick = 0;
    field_0x190 = 0;
    field_0x191 = 1;
}

void dSndBgmBattleSound_c::loadSeqConfig(u32 soundId) {
    if (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(soundId) == nw4r::snd::SoundArchive::SOUND_TYPE_SEQ) {
        if (soundId == BGM_BATTLE2) {
            mpSeqConfig = dSndBgmSeqConfig::getConfig(soundId, 0);
        }

        if (mpSeqConfig == nullptr) {
            mpSeqConfig = dSndBgmSeqConfig::getConfig(soundId, 1);
        }
    }
}

void dSndBgmBattleSound_c::fadeIn(u32 id, s32 fadeFrames) {
    if (id == -1) {
        return;
    }

    // TODO - hmmm
    mpBgmBattleConfig = nullptr;
    mpBgmBattleConfig = dSndBgmBattleConfig::getConfig(GetId());

    dSndBgmSound_c::fadeIn(id, fadeFrames);

    if (GetId() != BGM_BATTLE2) {
        field_0x191 = 0;
    }
}

void dSndBgmBattleSound_c::setTrackGroupMuted(u32 groupId) {
    mMuteGroupActiveMask |= (1 << groupId);
}

void dSndBgmBattleSound_c::setTrackGroupUnmuted(u32 groupId) {
    mMuteGroupActiveMask &= ~(1 << groupId);
}

bool dSndBgmBattleSound_c::startMainBattleLoop() {
    if (isPlaying() && readSeqTrackVariable(0) != 1) {
        // This seq variable allows the seq sound to jump from intro to main loop
        writeSeqTrackVariable(0, 1);
        mpSeqConfig = dSndBgmSeqConfig::getConfig(GetId(), 1);
        return true;
    }

    return false;
}

void dSndBgmBattleSound_c::calcSeqPlaySamplePosition() {
    if (!isSeqSound()) {
        return;
    }

    nw4r::snd::SeqSoundHandle handle(this);
    s32 tick = handle.GetTick();
    s32 tickDiff = tick - mPrevTick;

    if (tickDiff <= 0) {
        return;
    }

    mPrevTick = tick;
    s32 timeBase = mpSeqConfig != nullptr ? mpSeqConfig->mTimebase : 0x180;
    mBgmVar3 = readSeqTrackVariable(3);
    if (mBgmVar3 >= 0) {
        // Var >= 0 - seq itself controls play position
        mSeqPlaySamplePosition = tick % mSeqTimebase;
        mSeqPlaySamplePosition += mSeqTimebase * mBgmVar3;
    } else if (mpSeqConfig != nullptr) {
        mSeqPlaySamplePosition += tickDiff;
        if (mSeqPlaySamplePosition < (mpSeqConfig->mLoopEnd + 1) * timeBase) {
            // not looped yet, no adjustment needed
            return;
        }
        // looped, simple adjustment - simplified compared to dSndBgmSound_c which always calculates
        // mSeqPlaySamplePosition freshly. This function instead uses the difference
        mSeqPlaySamplePosition -= (timeBase * (mpSeqConfig->mLoopEnd - mpSeqConfig->mLoopStart));
    }
}
