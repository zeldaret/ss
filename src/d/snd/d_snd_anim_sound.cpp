#include "d/snd/d_snd_anim_sound.h"

#include "common.h"
#include "d/snd/d_snd_source.h"

#include <cmath>

dSndAnimSound_c::dSndAnimSound_c(dSoundSource_c *source)
    : mSound(*source), mpData(nullptr), field_0x94(1.0f), field_0x98(INFINITY), mNeedFrameReset(false) {}

void dSndAnimSound_c::setData(const void *data, const char *name) {
    mDirection = nw4r::snd::AnimSound::FORWARD;
    field_0x94 = 1.0f;
    if (data == mpData) {
        return;
    }

    mpData = data;
    if (mpData != nullptr) {
        mNeedFrameReset = true;
        mSound.Setup(data);
    } else {
        mSound.Shutdown();
    }
}

void dSndAnimSound_c::setFrame(f32 frame) {
    if (mpData == nullptr) {
        mSound.Shutdown();
        return;
    }
    if (frame >= mSound.GetAnimDuration()) {
        return;
    }
    if (mNeedFrameReset) {
        if (frame <= field_0x98) {
            mSound.ResetFrame(0.0f, 0);
        } else {
            mSound.ResetFrame(frame, 0);
        }
        mNeedFrameReset = false;
    }
    mSound.UpdateFrame(frame, mDirection);
}

void dSndAnimSound_c::setRate(f32 rate) {
    if (rate < 0.0f) {
        mDirection = nw4r::snd::AnimSound::BACKWARD;
    } else {
        mDirection = nw4r::snd::AnimSound::FORWARD;
    }
    field_0x94 = rate;
}

void dSndAnimSound_c::resetFrame(f32 frame) {
    if (mpData == nullptr || mSound.GetAnimDuration() <= frame) {
        return;
    }
    mNeedFrameReset = false;
    mSound.ResetFrame(frame, 0);
}

void dSndAnimSound_c::animCallback(int, s32, const char *, UNKWORD arg, void *userData) {
    reinterpret_cast<dSoundSource_c *>(userData)->onAnimSoundEvent(arg);
}

void dSndAnimSound_c::setCallback(dSoundSource_c *source) {
    mSound.SetCallback(&animCallback, source);
}

void dSndAnimSound_c::setCallback(nw4r::snd::AnimSound::Callback cb, void *userData) {
    mSound.SetCallback(cb, userData);
}
