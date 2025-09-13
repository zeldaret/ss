#include "egg/audio/eggAudioSystem.h"

#include "nw4r/snd/snd_SoundSystem.h"


namespace EGG {

AudioSystem *AudioSystem::sInstanse;

AudioSystem::AudioSystem() {
    mSavedMasterVolume = 1.0f;
    mShutdownStatus = 0;
    mResetStatus = 0;
    sInstanse = this;
}

AudioSystem::~AudioSystem() {}

void AudioSystem::reset(s32 ms) {
    if (mShutdownStatus == 0 && mResetStatus == 0) {
        mSavedMasterVolume = nw4r::snd::SoundSystem::GetMasterVolume();
        nw4r::snd::SoundSystem::SetMasterVolume(0.0f, ms * (1.0f / 60.f * 1000.0f));
        mResetStatus = 1;
    }
}

void AudioSystem::recoverReset() {
    if (mShutdownStatus == 0) {
        nw4r::snd::SoundSystem::SetMasterVolume(mSavedMasterVolume, 0);
        mResetStatus = 0;
    }
}

void AudioSystem::shutdown(s32 ms) {
    if (mShutdownStatus == 0) {
        mShutdownStatus = 1;
        nw4r::snd::SoundSystem::SetMasterVolume(0.0f, ms * (1.0f / 60.f * 1000.0f));
    }
}

void AudioSystem::calc() {
    f32 masterVolume = nw4r::snd::SoundSystem::GetMasterVolume();

    if (mShutdownStatus == 1 && masterVolume == 0.0f) {
        nw4r::snd::SoundSystem::PrepareReset();
        nw4r::snd::SoundSystem::WaitForResetReady();
        mShutdownStatus = 2;
    }

    if (mShutdownStatus != 2 && mResetStatus == 1 && masterVolume == 0.0f) {
        mResetStatus = 2;
    }
}

} // namespace EGG
