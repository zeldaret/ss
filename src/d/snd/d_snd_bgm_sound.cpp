#include "d/snd/d_snd_bgm_sound.h"

#include "common.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_bgm_mml_parsers.h"
#include "d/snd/d_snd_bgm_seq_config.h"
#include "d/snd/d_snd_bgm_seq_data_mgr.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_sound.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/snd/snd_StrmSoundHandle.h"
#include "nw4r/snd/snd_WaveSoundHandle.h"

dSndBgmSoundHarpMgr_c *dSndBgmSound_c::spGlobalHarpMgr;

dSndBgmSound_c::dSndBgmSound_c()
    : mpStartable(nullptr),
      mpSoundSource(nullptr),
      mIndex(7),
      field_0x104(0),
      mSeqTempo(-1),
      mBgmFlags(0),
      mDidRewindPlaySamplePosition(false),
      mBgmVar3(-1),
      mSeqTimebase(0x180),
      mPlaySamplePosition(0),
      mpHarpMgr(&mHarpMgr),
      mpSeqConfig(nullptr),
      mSeqPlaySamplePosition(-1),
      // not initialized!!!
      // mpStopCallback(nullptr),
      mpCalcCallback(nullptr),
      cb3(nullptr),
      cb4(nullptr),
      cb5(nullptr),
      cb6(nullptr),
      cb7(nullptr),
      cb8(nullptr),
      cb9(nullptr),
      cb10(nullptr) {
    for (int i = 0; i < 2; i++) {
        field_0x14C[i] = 0;
        field_0x154[i] = 0.0f;
    }

    if (spGlobalHarpMgr == nullptr) {
        spGlobalHarpMgr = new dSndBgmSoundHarpMgr_c();
    }
}

void dSndBgmSound_c::init(nw4r::snd::SoundStartable &startable, s32 idx) {
    mpStartable = &startable;
    mIndex = idx;
}

void dSndBgmSound_c::cancel() {
    if (hasState()) {
        dSndSound_c::cancel();
        mBgmFlags = 0;
        field_0x104 = 0;
        mDidRewindPlaySamplePosition = false;
        mpHarpMgr = &mHarpMgr;
        mpSeqConfig = nullptr;
        mPlaySamplePosition = 0;
        mSeqPlaySamplePosition = -1;
        mBgmVar3 = -1;
        mSeqTempo = -1;
        mSeqTimebase = 0x180;
        mHarpMgr.reset();
        for (int i = 0; i < 2; i++) {
            field_0x14C[i] = 0;
            field_0x154[i] = 0.0f;
        }
        dSndBgmMgr_c::GetInstance()->removeFromAllBgmSoundLists(this);
    }
}

void dSndBgmSound_c::calc() {
    dSndSound_c::calc();
    mDidRewindPlaySamplePosition = false;
    if (mIsRunning) {
        if (!IsAttachedSound()) {
            if (mpStopCallback != nullptr) {
                (mpStopCallback)(false);
                mpStopCallback = nullptr;
            }
            cancel();
        } else {
            field_0x104++;
            postCalc();
            calcPlaySamplePosition();
            if (mpCalcCallback != nullptr) {
                (mpCalcCallback)(this);
            }
        }
    }
}

void dSndBgmSound_c::calcPlaySamplePosition() {
    if (!isSeqSound()) {
        u32 oldPos = mPlaySamplePosition;
        mPlaySamplePosition = getPlaySamplePosition();
        if (mPlaySamplePosition < oldPos) {
            mDidRewindPlaySamplePosition = true;
        }
    }
}

bool dSndBgmSound_c::isBgmBattle() const {
    // TODO index constants
    if (mIndex == 4) {
        return true;
    }
    return (mBgmFlags & BGM_FLAG_BATTLE) != 0;
}

nw4r::snd::SoundStartable::StartResult dSndBgmSound_c::startBgmSound(u32 soundId, s32 fadeFrames, u32 startOffset) {
    if (isPlaying()) {
        stop(0);
    }

    nw4r::snd::SoundStartable::StartResult res;
    if (isPrepared() || isPreparing()) {
        if (soundId != GetId()) {
            return nw4r::snd::SoundStartable::START_ERR_USER;
        }
        if (startOffset != 0 && mPrevStartOffset != startOffset) {
            return nw4r::snd::SoundStartable::START_ERR_USER;
        }

        (void)IsPrepared(); // return value ignored
        StartPrepared();
        res = nw4r::snd::SoundStartable::START_SUCCESS;
    } else {
        if (startOffset == 0) {
            if (mpSoundSource != nullptr) {
                res = mpSoundSource->startSound(soundId, this, nullptr);
            } else {
                res = mpStartable->StartSoundReturnStatus(this, soundId, nullptr);
            }
        } else {
            nw4r::snd::SoundStartable::StartInfo info;
            info.enableFlag |= nw4r::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;

            getHarpData(soundId);
            if (mpHarpMgr != nullptr) {
                mSeqTempo = mpHarpMgr->getTempo();
            }

            if (mSeqTempo > 0 && dSndMgr_c::GetInstance()->getArchive()->GetSoundType(soundId) ==
                                     nw4r::snd::SoundArchive::SOUND_TYPE_SEQ) {
                mSeqPlaySamplePosition = (mSeqTempo * startOffset * 0xC0);
                mSeqPlaySamplePosition /= 120000;
                info.startOffsetType = nw4r::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_TICK;
                info.startOffset = mSeqPlaySamplePosition;
            } else {
                info.startOffsetType = nw4r::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC;
                info.startOffset = startOffset;
            }

            if (mpSoundSource != nullptr) {
                // TODO - is it a @bug that no pStartInfo is passed here?
                res = mpSoundSource->startSound(soundId, this, nullptr);
            } else {
                res = mpStartable->StartSoundReturnStatus(this, soundId, &info);
            }
        }
    }

    if (res == nw4r::snd::SoundStartable::START_SUCCESS) {
        fadeIn(soundId, fadeFrames);
    } else {
        cancel();
    }

    return res;
}

void dSndBgmSound_c::fadeIn(u32 id, s32 fadeFrames) {
    dSndSound_c::fadeIn(id, fadeFrames);
    u32 param = dSndMgr_c::GetInstance()->getArchive()->GetSoundUserParam(id);
    if (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(id) == nw4r::snd::SoundArchive::SOUND_TYPE_STRM) {
        // Mute all seq tracks but the first
        setStrmTrackVolume(~(1 << 0), 0.0f, 0);
        // Unmute second and third based on flags
        if ((param & 0x10000) != 0) {
            setStrmTrackVolume((1 << 1), 1.0f, 0);
        }
        if ((param & 0x20000) != 0) {
            setStrmTrackVolume((1 << 2), 1.0f, 0);
        }
    }

    loadCallbacks(id);
    loadSeqConfig(id);
    getHarpData(id);
    if (mpHarpMgr != nullptr) {
        mSeqTempo = mpHarpMgr->getTempo();
    }

    if ((param & 0x40000000) != 0) {
        mBgmFlags |= BGM_FLAG_0x10 | BGM_FLAG_0x8;
        dSndStateMgr_c::GetInstance()->onEventFlag(dSndStateMgr_c::EVENT_0x400000);
    }

    if ((param & 0x10000000) != 0) {
        mBgmFlags |= BGM_FLAG_0x10;
        dSndStateMgr_c::GetInstance()->onEventFlag(dSndStateMgr_c::EVENT_0x400000);
    }

    if ((param & 0x800000) != 0) {
        mBgmFlags |= BGM_FLAG_0x40;
    }

    if (dSndPlayerMgr_c::isSoundPlayedByBgmBattlePlayer(id)) {
        mBgmFlags |= BGM_FLAG_BATTLE;
    }

    if (id == BGM_MUTE) {
        dSndBgmMgr_c::GetInstance()->appendToBgmSoundList(dSndBgmMgr_c::BGM_LIST_PLAYING, this);
    } else {
        dSndBgmMgr_c::GetInstance()->addToBgmSoundList(dSndBgmMgr_c::BGM_LIST_PLAYING, this);
    }

    if (dSndBgmMgr_c::GetInstance()->getActiveBgmSound() == this) {
        mpHarpMgr->setPlaySamplePosition(0);
    }
}

void dSndBgmSound_c::loadSeqConfig(u32 soundId) {
    if (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(soundId) == nw4r::snd::SoundArchive::SOUND_TYPE_SEQ) {
        mpSeqConfig = dSndBgmSeqConfig::getConfig(soundId, 1);
        if (mpSeqConfig != nullptr) {
            mSeqTimebase = mpSeqConfig->mTimebase;
        }
    }
}

// Largely a copy of dSndSound_c::prepareSound, with changes to support different startable
nw4r::snd::SoundStartable::StartResult dSndBgmSound_c::prepareSound(u32 soundId, u32 startOffset) {
    if (soundId == -1) {
        return nw4r::snd::SoundStartable::START_ERR_USER;
    }

    if (isPlaying()) {
        return nw4r::snd::SoundStartable::START_ERR_USER;
    }

    if (isPreparingSoundId(soundId)) {
        if (mPrevStartOffset == startOffset) {
            return nw4r::snd::SoundStartable::START_ERR_USER;
        }
        forceStop();
    } else {
        if (isPreparing()) {
            forceStop();
        } else {
            dSndBgmMgr_c::GetInstance()->cullTooManyPreparingSounds();
        }
    }

    nw4r::snd::SoundStartable::StartResult res;
    if (startOffset == 0) {
        if (mpSoundSource != nullptr) {
            mpSoundSource->markPrepared();
            res = mpSoundSource->PrepareSoundReturnStatus(this, soundId, nullptr);
        } else {
            res = mpStartable->PrepareSoundReturnStatus(this, soundId, nullptr);
        }
    } else {
        nw4r::snd::SoundStartable::StartInfo info;
        info.enableFlag |= nw4r::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
        info.startOffsetType = nw4r::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC;
        info.startOffset = startOffset;
        if (mpSoundSource != nullptr) {
            mpSoundSource->markPrepared();
            // TODO - is it a @bug that no pStartInfo is passed here?
            res = mpSoundSource->PrepareSoundReturnStatus(this, soundId, nullptr);
        } else {
            res = mpStartable->PrepareSoundReturnStatus(this, soundId, &info);
        }
    }

    if (res == nw4r::snd::SoundStartable::START_SUCCESS) {
        onPreparing(soundId, startOffset);
    } else {
        cancel();
    }

    return res;
}

nw4r::snd::SoundStartable::StartResult dSndBgmSound_c::prepareSound(const char *label, u32 startOffset) {
    return prepareSound(dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label), startOffset);
}

void dSndBgmSound_c::onPreparing(u32 soundId, u32 startOffset) {
    if (soundId == -1) {
        return;
    }

    dSndSound_c::onPreparing(soundId, startOffset);
    getHarpData(soundId);
    if (dSndPlayerMgr_c::isSoundPlayedByBgmBattlePlayer(soundId)) {
        mBgmFlags |= BGM_FLAG_BATTLE;
    }
    dSndBgmMgr_c::GetInstance()->addToBgmSoundList(dSndBgmMgr_c::BGM_LIST_PREPARING, this);
}

void dSndBgmSound_c::stop(s32 fadeFrames) {
    if (isRunning()) {
        if (mpStopCallback != nullptr) {
            (mpStopCallback)(true);
            mpStopCallback = nullptr;
        }
        Stop(fadeFrames);
        if (fadeFrames != 0) {
            mIsFadingOut = true;
            dSndBgmMgr_c::GetInstance()->addToBgmSoundList(dSndBgmMgr_c::BGM_LIST_STOPPING, this);
        } else {
            cancel();
        }
    } else if (!isPreparing()) {
        cancel();
    }
}

void dSndBgmSound_c::pause(bool pauseFlag, s32 fadeFrames) {
    if (isFadingOut()) {
        return;
    }

    if (isPlaying()) {
        if (pauseFlag == false) {
            if (mIndex != 4 && dSndBgmMgr_c::GetInstance()->weirdCheckAlwaysFalse()) {
                setVolume(0.0f, 10);
            }
            mBgmFlags &= ~BGM_FLAG_PAUSED;
        }
        dSndSound_c::pause(pauseFlag, fadeFrames);
    }
}

void dSndBgmSound_c::pause(s32 fadeFrames) {
    pause(true, fadeFrames);
    mBgmFlags |= BGM_FLAG_PAUSED;
}

u32 dSndBgmSound_c::getPlaySamplePosition() {
    if (isRunning()) {
        u32 soundId = GetId();
        if (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(soundId) == nw4r::snd::SoundArchive::SOUND_TYPE_SEQ) {
            return getSeqSoundPlaySamplePosition();
        }

        if (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(soundId) == nw4r::snd::SoundArchive::SOUND_TYPE_STRM) {
            nw4r::snd::StrmSoundHandle handle(this);
            return handle.GetPlaySamplePosition();
        }

        if (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(soundId) == nw4r::snd::SoundArchive::SOUND_TYPE_WAVE) {
            nw4r::snd::WaveSoundHandle handle(this);
            return handle.GetPlaySamplePosition();
        }
    }

    return -1;
}

u32 dSndBgmSound_c::getStrmPlaySamplePosition() {
    if (isRunning()) {
        u32 soundId = GetId();
        if (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(soundId) == nw4r::snd::SoundArchive::SOUND_TYPE_STRM) {
            nw4r::snd::StrmSoundHandle handle(this);
            return handle.GetPlaySamplePosition();
        }
    }

    return -1;
}

u32 dSndBgmSound_c::getWavePlaySamplePosition() {
    if (isRunning()) {
        u32 soundId = GetId();
        if (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(soundId) == nw4r::snd::SoundArchive::SOUND_TYPE_WAVE) {
            nw4r::snd::WaveSoundHandle handle(this);
            return handle.GetPlaySamplePosition();
        }
    }

    return -1;
}

u32 dSndBgmSound_c::getSeqSoundPlaySamplePosition() {
    if (isRunning()) {
        calcSeqPlaySamplePosition();
        return mSeqPlaySamplePosition;
    } else {
        return -1;
    }
}

void dSndBgmSound_c::calcSeqPlaySamplePosition() {
    if (!isSeqSound()) {
        return;
    }

    nw4r::snd::SeqSoundHandle handle(this);
    s32 tick = handle.GetTick();
    mBgmVar3 = readSeqTrackVariable(3);
    if (mBgmVar3 >= 0) {
        // Var >= 0 - seq itself controls play position
        mSeqPlaySamplePosition = tick % mSeqTimebase;
        if (mSeqPlaySamplePosition == 0) {
            mBgmVar3++;
            if (mpSeqConfig != nullptr && mBgmVar3 > mpSeqConfig->mLoopEnd) {
                mBgmVar3 = mpSeqConfig->mLoopStart;
            }
        }
        mSeqPlaySamplePosition += mSeqTimebase * mBgmVar3;
    } else if (mpSeqConfig != nullptr) {
        if (tick < (mpSeqConfig->mLoopEnd + 1) * mSeqTimebase) {
            // not looped yet, no adjustment needed
            mSeqPlaySamplePosition = tick;
            return;
        }
        s32 startOffset = mpSeqConfig->mLoopStart * mSeqTimebase;

        // wrap around loop, making sure to account for start offset
        tick -= startOffset;
        tick = tick % (mSeqTimebase * (mpSeqConfig->mLoopEnd - mpSeqConfig->mLoopStart));
        mSeqPlaySamplePosition = tick + startOffset;
    } else {
        // Fallback, not controlled by seq no config override
        mSeqPlaySamplePosition = tick;
    }
}

void dSndBgmSound_c::applyVars() {
    if (isRunning()) {
        mpHarpMgr->setPlaySamplePosition(getPlaySamplePosition());
    }
}

void dSndBgmSound_c::onBecomeActive() {
    if (mpHarpMgr != nullptr) {
        mpHarpMgr->resetPrevIdx();
    }
    mHarpMgr.resetPrevIdx();
    spGlobalHarpMgr->resetPrevIdx();
}

dSndBgmDataHarpVarSetBase_c *dSndBgmSound_c::getCurrentHarpVarSet() {
    if (isRunning()) {
        return mpHarpMgr->getCurrentVarSet();
    }
    return nullptr;
}

void dSndBgmSound_c::getHarpData(u32 soundId) {
    if (soundId == -1) {
        return;
    }

    dSndBgmSoundHarpMgr_c *mgr = dSndBgmSeqDataMgr_c::GetInstance()->getHarpMgrForSoundId(soundId);
    if (mgr != nullptr) {
        mpHarpMgr = mgr;
    } else {
        loadNewHarpData(soundId);
    }
}

bool dSndBgmSound_c::loadNewHarpData(u32 soundId) {
    if (soundId == -1) {
        return false;
    }

    if (!IsAttachedSound()) {
        return false;
    }

    bool ok = false;
    if (!mHarpMgr.isLoaded()) {
        dSndBgmMmlParserHarp_c parser(&mHarpMgr);
        // result unused
        (void)dSndBgmMgr_c::isSoundPlayedByBgmBattlePlayer(soundId);
        if ((ok = parser.parseData(soundId, (1 << 15) | (1 << 0)))) {
            mHarpMgr.setLoaded();
        }
    }

    if (soundId == BGM_D301_INSIDE) {
        spGlobalHarpMgr->reset();
        if (!spGlobalHarpMgr->isLoaded()) {
            dSndBgmMmlParserHarp_c parser(spGlobalHarpMgr);
            if ((ok = parser.parseData("BGM_D301_INSIDE_B", soundId, (1 << 15) | (1 << 0)))) {
                spGlobalHarpMgr->setLoaded();
            }
        }
    } else if (soundId == BGM_D003_3) {
        spGlobalHarpMgr->reset();
        if (!spGlobalHarpMgr->isLoaded()) {
            dSndBgmMmlParserHarp_c parser(spGlobalHarpMgr);
            if ((ok = parser.parseData("BGM_D003_3_B", soundId, (1 << 15) | (1 << 0)))) {
                spGlobalHarpMgr->setLoaded();
            }
        }
    } else {
        if (soundId == BGM_D101) {
            spGlobalHarpMgr->reset();
            if (!spGlobalHarpMgr->isLoaded()) {
                dSndBgmMmlParserHarp_c parser(spGlobalHarpMgr);
                if ((ok = parser.parseData("BGM_D101_hell", soundId, (1 << 15) | (1 << 0)))) {
                    spGlobalHarpMgr->setLoaded();
                }
            }
        }
    }
    return ok;
}
