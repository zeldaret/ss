#include "d/snd/d_snd_sound.h"

#include "common.h"
#include "d/snd/d_snd_mgr.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/ut/ut_list.h"

dSndSound_c::dSndSound_c() : field_0x10(0), field_0x14(0), field_0x15(0), field_0x16(0), field_0x17(0) {
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

    mCtrlUnk.setControl(&mCtrlPitch);
    reset();
}

dSndSound_c::~dSndSound_c() {
    if (mpCtrlSeqTrackVolume != nullptr) {
        delete[] mpCtrlSeqTrackVolume;
    }
    if (mpCtrlStrmTrackVolume != nullptr) {
        delete[] mpCtrlStrmTrackVolume;
    }
}

void dSndSound_c::reset() {
    mCtrlVolume.reset();
    mCtrlVolume.apply(this);
    mCtrlPitch.reset();
    mCtrlPitch.apply(this);
    mCtrlUnk.reset();
    mCtrlUnk.apply(this);
    mpCtrlSeqTrackTempoRatio.reset();
    mpCtrlSeqTrackTempoRatio.apply(this);
    field_0x15 = 0;
    resetTrackVolumes();
}

void dSndSound_c::fadeIn(u32 id, int fadeFrames) {
    if (id != -1) {
        if (fadeFrames != 0) {
            FadeIn(fadeFrames);
        }
        vt_0x08();
        field_0x16 = 1;
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

void dSndSound_c::setSeqTrackMute(u32 trackFlags, nw4r::snd::SeqMute mute) {
    if (isSeqSound()) {
        nw4r::snd::SeqSoundHandle handle(this);
        handle.SetTrackMute(trackFlags, mute);
    }
}

void dSndSound_c::stop() {
    vt_0x08();
    Stop();
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
