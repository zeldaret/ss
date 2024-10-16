#include "egg/audio/eggAudioSystem.h"

#include "nw4r/snd/snd_SoundSystem.h"


namespace EGG {

AudioSystem *AudioSystem::sInstanse;

AudioSystem::AudioSystem() {
    field_0x00 = 1.0f;
    field_0x08 = 0;
    field_0x04 = 0;
    sInstanse = this;
}

AudioSystem::~AudioSystem() {}

void AudioSystem::fn_804B7270(s32 frame) {
    if (field_0x08 == 0 && field_0x04 == 0) {
        field_0x00 = nw4r::snd::SoundSystem::GetMasterVolume();
        nw4r::snd::SoundSystem::SetMasterVolume(0.0f, frame * 16.666667f);
        field_0x04 = 1;
    }
}

void AudioSystem::fn_804B7370() {
    if (field_0x08 == 0) {
        nw4r::snd::SoundSystem::SetMasterVolume(field_0x00, 0);
        field_0x04 = 0;
    }
}

void AudioSystem::fn_804B73D0(s32 frame) {
    if (field_0x08 == 0) {
        field_0x08 = 1;
        nw4r::snd::SoundSystem::SetMasterVolume(0.0f, frame * 16.666667f);
    }
}

void AudioSystem::calc() {
    f32 masterVolume = nw4r::snd::SoundSystem::GetMasterVolume();

    if (field_0x08 == 1 && masterVolume == 0.0f) {
        nw4r::snd::SoundSystem::PrepareReset();
        nw4r::snd::SoundSystem::WaitForResetReady();
        field_0x08 = 2;
    }

    if (field_0x08 != 2 && field_0x04 == 1 && masterVolume == 0.0f) {
        field_0x04 = 2;
    }
}

} // namespace EGG
