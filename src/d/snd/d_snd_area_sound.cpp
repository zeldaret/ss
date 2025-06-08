#include "d/snd/d_snd_area_sound.h"

#include "common.h"
#include "d/snd/d_snd_sound.h"

dSndAreaSound_c::dSndAreaSound_c()
    : field_0xF0(false), mpCalcCallback(nullptr), field_0xF8(nullptr), field_0xFC(nullptr), field_0x100(nullptr) {}

void dSndAreaSound_c::cancel() {
    dSndSound_c::cancel();
    field_0xF0 = false;
}

void dSndAreaSound_c::calc() {
    if (isFadingOut() && mPauseFlag) {
        if (GetRemainingFadeFrames() <= 0) {
            Stop(0);
            cancel();
        }
    }

    dSndSound_c::calc();
    if (mpCalcCallback != nullptr) {
        (mpCalcCallback)(*this);
    }
}

void dSndAreaSound_c::fadeIn(u32 id, s32 fadeFrames) {
    dSndSound_c::fadeIn(id, fadeFrames);
    initCallback(id);
}

void dSndAreaSound_c::pause(s32 fadeFrames) {
    if (isRunning()) {
        if (fadeFrames != 0) {
            pause(true, fadeFrames);
            mIsFadingOut = true;
        } else {
            Stop(fadeFrames);
            cancel();
        }
    } else {
        cancel();
    }
}

void dSndAreaSound_c::unpause(s32 fadeFrames) {
    if (isFadingOut()) {
        pause(false, fadeFrames);
        mIsFadingOut = false;
    }
}

void dSndAreaSound_c::initCallback(u32 id) {
    Callback cb = nullptr;
    mpCalcCallback = nullptr;
    field_0xF8 = nullptr;
    field_0xFC = nullptr;
    field_0x100 = nullptr;
    field_0x104 = nullptr;
}
