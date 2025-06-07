
#include "common.h"
#include "d/snd/d_snd_calc_pitch.h"
#include "d/snd/d_snd_sound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_StrmSoundHandle.h"

// I'm not super happy with these base class function in a separate file,
// but either we add another intermediate class or this is ok. It's weird
// that d_snd_sound has dSndControl_c::stop though

void dSndControl_c::reset() {
    mCurrValue = mResetValue;
    mStartValue = mResetValue;
    mTargetValue = mResetValue;
    mDuration = 0;
    mTimer = 0;
}

void dSndControl_c::stop() {
    // field_0x10 = field_0x04;
    mStartValue = mCurrValue;
    mTargetValue = mCurrValue;
    mDuration = 0;
    mTimer = 0;
}

void dSndControl_c::set(f32 value, s32 frames) {
    stop();
    f32 clamped = value;
    if (clamped > mMax) {
        clamped = mMax;
    } else if (clamped < mMin) {
        clamped = mMin;
    }
    if (clamped != mCurrValue) {
        mTargetValue = clamped;
        if (frames == 0) {
            mCurrValue = clamped;
        } else {
            mDuration = frames;
            mTimer = 0;
        }
    }
}

void dSndControl_c::calc() {
    if (mTimer < mDuration) {
        mTimer++;
        if (mTimer == mDuration) {
            mCurrValue = mTargetValue;
        } else {
            mCurrValue = mStartValue + (mTimer * (mTargetValue - mStartValue) / mDuration);
        }
    }
}

void dSndControlSoundVolume_c::apply(dSndSound_c *sound) {
    sound->SetVolume(mCurrValue, 0);
}

void dSndControlSoundPitch_c::apply(dSndSound_c *sound) {
    sound->SetPitch(mCurrValue);
}

void d_snd_control_sound_float_order() {
    -24.0f;
    24.0f;
}

dSndControlSoundPitchLinear_c::dSndControlSoundPitchLinear_c() : dSndControlSound_c(0.0f, -24.0f, 24.0f) {
    mpOtherControl = nullptr;
}

void dSndControlSoundPitchLinear_c::set(f32 value, s32 frames) {
    if (mpOtherControl != nullptr) {
        mCurrValue = pitchIdxTenthsForPitchScale(mpOtherControl->getCurrentValue());
    }

    dSndControlSound_c::set(value, frames);
}

void dSndControlSoundPitchLinear_c::apply(dSndSound_c *pHandle) {
    pHandle->setPitchRelated(pitchScaleForPitchIdxTenths(mCurrValue), 0);
}

void dSndControlSoundSeqTempoRatio_c::apply(dSndSound_c *sound) {
    nw4r::snd::SeqSoundHandle handle(sound);
    handle.SetTempoRatio(mCurrValue);
}

void dSndControlSoundSeqTrackVolume_c::apply(dSndSound_c *sound) {
    nw4r::snd::SeqSoundHandle handle(sound);
    handle.SetVolume(mMask, mCurrValue);
}

void dSndControlSoundStrmTrackVolume_c::apply(dSndSound_c *sound) {
    nw4r::snd::StrmSoundHandle handle(sound);
    handle.SetVolume(mMask, mCurrValue, 0);
}
