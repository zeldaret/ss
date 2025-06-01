#include "d/snd/d_snd_sound.h"

#include "common.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/ut/ut_list.h"
#include "toBeSorted/music_mgrs.h"

dSndSound_c::dSndSound_c()
    : mPrevStartOffset(0), mIsPreparing(false), mPauseFlag(false), mIsRunning(false), mIsFadingOut(false) {
    nw4r::ut::List_Init(&mList, 0x24);

    mpCtrlSeqTrackVolume = new dSndControlSoundSeqTrackVolume_c[16]();
    for (int i = 0; i < 16; i++) {
        mpCtrlSeqTrackVolume[i].setMask(1 << i);
    }

    mpCtrlStrmTrackVolume = new dSndControlSoundStrmTrackVolume_c[3]();
    // @bug should be 3, not 16
    for (int i = 0; i < 16; i++) {
        mpCtrlStrmTrackVolume[i].setMask(1 << i);
    }

    mLinearPitch.setControl(&mCtrlPitch);
    resetControls();
}

dSndSound_c::~dSndSound_c() {
    if (mpCtrlSeqTrackVolume != nullptr) {
        delete[] mpCtrlSeqTrackVolume;
    }
    if (mpCtrlStrmTrackVolume != nullptr) {
        delete[] mpCtrlStrmTrackVolume;
    }
}

void dSndSound_c::cancel() {
    if (canCancel()) {
        resetControls();
        mIsRunning = false;
        mIsFadingOut = false;
        mIsPreparing = false;
        mPrevStartOffset = 0;
        fn_80373900(FANFARE_SOUND_MGR, this);
    }
}

void dSndSound_c::resetControls() {
    mCtrlVolume.reset();
    mCtrlVolume.apply(this);
    mCtrlPitch.reset();
    mCtrlPitch.apply(this);
    mLinearPitch.reset();
    mLinearPitch.apply(this);
    mpCtrlSeqTempoRatio.reset();
    mpCtrlSeqTempoRatio.apply(this);
    mPauseFlag = 0;
    resetTrackVolumes();
}

void dSndSound_c::executeCtrls() {
    if (mIsRunning && !IsAttachedSound()) {
        cancel();
    }

    dSndControlSound_c *next;
    dSndControlSound_c *iter = static_cast<dSndControlSound_c *>(nw4r::ut::List_GetNext(&mList, nullptr));
    while (iter != nullptr) {
        next = static_cast<dSndControlSound_c *>(nw4r::ut::List_GetNext(&mList, iter));
        iter->calc();
        iter->apply(this);
        if (iter->isFinished()) {
            unlinkCtrl(iter);
        }
        iter = next;
    }
}

void dSndSound_c::fadeIn(u32 id, s32 fadeFrames) {
    if (id != -1) {
        if (fadeFrames != 0) {
            FadeIn(fadeFrames);
        }
        cancel();
        mIsRunning = true;
    }
}

void dSndSound_c::stop(s32 fadeFrames) {
    if (isRunning()) {
        Stop(fadeFrames);

        if (fadeFrames != 0) {
            mIsFadingOut = true;
        } else {
            cancel();
        }
    } else {
        if (!isPreparing()) {
            cancel();
        }
    }
}

void dSndSound_c::pause(bool pauseFlag, s32 fadeFrames) {
    if (!IsAttachedSound()) {
        return;
    }

    if ((pauseFlag && !IsPause()) || (!pauseFlag && IsPause())) {
        Pause(pauseFlag, fadeFrames);
        mPauseFlag = pauseFlag;
    }
}

bool dSndSound_c::isStrmSound() {
    if (!IsAttachedSound()) {
        return false;
    }
    return dSndMgr_c::GetInstance()->getArchive()->GetSoundType(GetId()) == nw4r::snd::SoundArchive::SOUND_TYPE_STRM;
}

bool dSndSound_c::isWaveSound() {
    if (!IsAttachedSound()) {
        return false;
    }
    return dSndMgr_c::GetInstance()->getArchive()->GetSoundType(GetId()) == nw4r::snd::SoundArchive::SOUND_TYPE_WAVE;
}

bool dSndSound_c::isSeqSound() {
    if (!IsAttachedSound()) {
        return false;
    }
    return dSndMgr_c::GetInstance()->getArchive()->GetSoundType(GetId()) == nw4r::snd::SoundArchive::SOUND_TYPE_SEQ;
}

void dSndSound_c::linkCtrl(dSndControlSound_c *ctrl) {
    if (ctrl == nullptr) {
        return;
    }
    if (ctrl->isLinked()) {
        return;
    }
    ctrl->setLinked(true);
    nw4r::ut::List_Append(&mList, ctrl);
}

void dSndSound_c::unlinkCtrl(dSndControlSound_c *ctrl) {
    if (ctrl == nullptr) {
        return;
    }
    if (!ctrl->isLinked()) {
        return;
    }
    ctrl->setLinked(false);
    nw4r::ut::List_Remove(&mList, ctrl);
}

s16 dSndSound_c::readSeqTrackVariable(int varNo) {
    if (!isSeqSound()) {
        return -1;
    }
    s16 val = -1;
    nw4r::snd::SeqSoundHandle handle(this);
    handle.ReadVariable(varNo, &val);
    return val;
}

void dSndSound_c::writeSeqTrackVariable(int varNo, s16 value) {
    if (!isSeqSound()) {
        return;
    }
    nw4r::snd::SeqSoundHandle handle(this);
    handle.WriteVariable(varNo, value);
}

void dSndSound_c::setControlValue(dSndControlSound_c *ctrl, f32 value, s32 frames) {
    ctrl->set(value, frames);

    if (ctrl->isFinished()) {
        unlinkCtrl(ctrl);
    } else {
        linkCtrl(ctrl);
    }

    ctrl->apply(this);
}

void dSndSound_c::setVolume(f32 volume, s32 frames) {
    if (IsAttachedSound()) {
        setControlValue(&mCtrlVolume, volume, frames);
    }
}

void dSndSound_c::setPitchRelated(f32 pitch, s32 frames) {
    if (IsAttachedSound()) {
        setControlValue(&mCtrlPitch, pitch, frames);
    }
}

void dSndSound_c::setLinearPitch(f32 pitch, s32 frames) {
    if (IsAttachedSound()) {
        setControlValue(&mLinearPitch, pitch, frames);
    }
}

void dSndSound_c::resetTrackVolumes() {
    if (mpCtrlSeqTrackVolume != nullptr) {
        for (int i = 0; i < 16; i++) {
            mpCtrlSeqTrackVolume[i].reset();
            mpCtrlSeqTrackVolume[i].apply(this);
        }
    }

    if (mpCtrlStrmTrackVolume != nullptr) {
        for (int i = 0; i < 3; i++) {
            mpCtrlStrmTrackVolume[i].reset();
            mpCtrlStrmTrackVolume[i].apply(this);
        }
    }
}

void dSndSound_c::setTrackVolume(u32 trackFlags, f32 volume, s32 frames) {
    if (trackFlags != 0 && IsAttachedSound()) {
        switch (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(GetId())) {
            case nw4r::snd::SoundArchive::SOUND_TYPE_SEQ: {
                for (int i = 0; i < 16; i++) {
                    if ((trackFlags & (u16)(1 << i)) != 0) {
                        setControlValue(&mpCtrlSeqTrackVolume[i], volume, frames);
                    }
                }
                break;
            }
            case nw4r::snd::SoundArchive::SOUND_TYPE_STRM: {
                for (int i = 0; i < 3; i++) {
                    if ((trackFlags & (u16)(1 << i)) != 0) {
                        setControlValue(&mpCtrlStrmTrackVolume[i], volume, frames);
                    }
                }
            }
            default: break;
        }
    }
}

void dSndSound_c::setStrmTrackVolume(u32 trackFlags, f32 volume, s32 frames) {
    if (trackFlags != 0 && IsAttachedSound()) {
        switch (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(GetId())) {
            case nw4r::snd::SoundArchive::SOUND_TYPE_STRM: {
                for (int i = 0; i < 3; i++) {
                    if ((trackFlags & (u16)(1 << i)) != 0) {
                        setControlValue(&mpCtrlStrmTrackVolume[i], volume, frames);
                    }
                }
            }
            default: break;
        }
    }
}

void dSndSound_c::setSingleSeqTrackVolume(u16 index, f32 volume, s32 frames) {
    if (IsAttachedSound() && index < 16) {
        switch (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(GetId())) {
            case nw4r::snd::SoundArchive::SOUND_TYPE_SEQ: {
                setControlValue(&mpCtrlSeqTrackVolume[index], volume, frames);
            }
            default: break;
        }
    }
}

void dSndSound_c::setSingleStrmTrackVolume(u16 index, f32 volume, s32 frames) {
    if (IsAttachedSound() && index < 3) {
        switch (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(GetId())) {
            case nw4r::snd::SoundArchive::SOUND_TYPE_STRM: {
                setControlValue(&mpCtrlStrmTrackVolume[index], volume, frames);
            }
            default: break;
        }
    }
}

void dSndSound_c::setEachSeqTrackVolume(u32 trackFlags, f32 frames) {
    for (int i = 0; i < 16; i++) {
        if ((trackFlags & (u16)(1 << i)) != 0) {
            setSingleSeqTrackVolume(i, 1.0f, (u32)frames);
        } else {
            setSingleSeqTrackVolume(i, 0.0f, (u32)frames);
        }
    }
}

void dSndSound_c::setEachStrmTrackVolume(u32 trackFlags, f32 frames) {
    for (int i = 0; i < 3; i++) {
        if ((trackFlags & (u16)(1 << i)) != 0) {
            setSingleStrmTrackVolume(i, 1.0f, (u32)frames);
        } else {
            setSingleStrmTrackVolume(i, 0.0f, (u32)frames);
        }
    }
}

f32 dSndSound_c::getCurrentStrmTrackVolume(u32 index) const {
    if (index >= 3) {
        return 0.0f;
    }
    return mpCtrlStrmTrackVolume[index].getCurrentValue();
}

void dSndSound_c::setSeqTempoRatio(f32 ratio, s32 frames) {
    if (isSeqSound()) {
        setControlValue(&mpCtrlSeqTempoRatio, ratio, frames);
    }
}

void dSndSound_c::setSeqTrackMute(u32 trackFlags, nw4r::snd::SeqMute mute) {
    if (isSeqSound()) {
        nw4r::snd::SeqSoundHandle handle(this);
        handle.SetTrackMute(trackFlags, mute);
    }
}

nw4r::snd::SoundStartable::StartResult dSndSound_c::prepareSound(u32 soundId, u32 startOffset) {
    if (soundId == -1) {
        return nw4r::snd::SoundStartable::START_ERR_USER;
    }

    if (cannotStart()) {
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
            fn_80372920(FANFARE_SOUND_MGR);
        }
    }

    nw4r::snd::SoundStartable::StartResult res;
    if (startOffset == 0) {
        res = dSndMgr_c::GetInstance()->getPlayer()->detail_PrepareSound(this, soundId, nullptr);
    } else {
        nw4r::snd::SoundStartable::StartInfo info;
        info.enableFlag |= nw4r::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
        info.startOffsetType = nw4r::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC;
        info.startOffset = startOffset;
        res = dSndMgr_c::GetInstance()->getPlayer()->detail_PrepareSound(this, soundId, &info);
    }

    if (res == nw4r::snd::SoundStartable::START_SUCCESS) {
        onPreparing(soundId, startOffset);
    } else {
        cancel();
    }

    return res;
}

nw4r::snd::SoundStartable::StartResult dSndSound_c::prepareSound(const char *label, u32 startOffset) {
    u32 id = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
    return prepareSound(id, startOffset);
}

void dSndSound_c::onPreparing(u32 soundId, u32 startOffset) {
    if (soundId != -1) {
        cancel();
        mPrevStartOffset = startOffset;
        mIsPreparing = true;
        fn_803738B0(FANFARE_SOUND_MGR, this);
    }
}

void dSndSound_c::forceStop() {
    cancel();
    Stop(0);
}

// Might be a separate file
void dSndControl_c::configure(f32 value, f32 min, f32 max) {
    mResetValue = value;
    mMin = min;
    mMax = max;
    mCurrValue = value;
    mStartValue = value;
    mTargetValue = value;
    reset();
}
