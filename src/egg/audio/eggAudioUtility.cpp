#include <egg/audio/eggAudioRmtSpeakerMgr.h>
#include <egg/audio/eggAudioUtility.h>
#include <nw4r/snd/snd_SoundPlayer.h>
#include <nw4r/snd/snd_SoundSystem.h>

namespace EGG {

AudioUtility::MoveParamMgr AudioUtility::sMoveParamMgr;

MultiArcSimpleAudioMgr *AudioUtility::HBM::sMultiArcSimpleAudioMgr;
SimpleAudioMgr *AudioUtility::HBM::sSimpleAudioMgr;
void (*AudioUtility::HBM::sHBMEffectRestCallback)();
void (*AudioUtility::HBM::sHBMUserCallback)(s32, s32);
u32 AudioUtility::HBM::sHBFadeframe = 0x12;

AudioUtility::MoveParamMgr::MoveParamMgr() {
    init();
}

void AudioUtility::MoveParamMgr::init() {
    nw4r::ut::List_Init(&sMoveParamMgr.mList, 0x28);
}

void AudioUtility::HBM::init(SimpleAudioMgr *mgr, void (*userCallback)(), u32 frame) {
    sSimpleAudioMgr = mgr;
    sHBFadeframe = frame;
    sHBMEffectRestCallback = userCallback;
}

void AudioUtility::HBM::enter() {
    if (sHBMUserCallback != nullptr) {
        (sHBMUserCallback)(0, 0);
    }

    // Regswaps
    u32 i;
    int j;

    if (sSimpleAudioMgr != nullptr) {
        for (i = 0; i < sSimpleAudioMgr->getPlayer()->GetSoundPlayerCount(); i++) {
            sSimpleAudioMgr->getPlayer()->GetSoundPlayer(i).PauseAllSound(true, sHBFadeframe);
        }
    }

    if (sMultiArcSimpleAudioMgr != nullptr) {
        int max = sMultiArcSimpleAudioMgr->field_0x0FC;
        for (j = 0; j < max; j++) {
            for (i = 0; i < sMultiArcSimpleAudioMgr->getPlayer(j)->GetSoundPlayerCount(); i++) {
                sMultiArcSimpleAudioMgr->getPlayer(j)->GetSoundPlayer(i).PauseAllSound(true, sHBFadeframe);
            }
        }
    }

    nw4r::snd::SoundSystem::ClearEffect(nw4r::snd::AUX_A, 0xFA);
    nw4r::snd::SoundSystem::ClearEffect(nw4r::snd::AUX_B, 0xFA);
    nw4r::snd::SoundSystem::ClearEffect(nw4r::snd::AUX_C, 0xFA);
    AudioRmtSpeakerMgr::disconnectAllByForce();
}

void AudioUtility::HBM::exit(bool arg) {
    if (sHBMUserCallback != nullptr) {
        (sHBMUserCallback)(true, arg);
    }

    if (arg) {
        // Regswaps
        u32 i;
        int j;

        if (sSimpleAudioMgr != nullptr) {
            for (i = 0; i < sSimpleAudioMgr->getPlayer()->GetSoundPlayerCount(); i++) {
                sSimpleAudioMgr->getPlayer()->GetSoundPlayer(i).PauseAllSound(false, sHBFadeframe);
            }
        }

        if (sMultiArcSimpleAudioMgr != nullptr) {
            int max = sMultiArcSimpleAudioMgr->field_0x0FC;
            for (j = 0; j < max; j++) {
                for (i = 0; i < sMultiArcSimpleAudioMgr->getPlayer(j)->GetSoundPlayerCount(); i++) {
                    sMultiArcSimpleAudioMgr->getPlayer(j)->GetSoundPlayer(i).PauseAllSound(false, sHBFadeframe);
                }
            }
        }
    }

    if (sHBMEffectRestCallback != nullptr) {
        (sHBMEffectRestCallback)();
    }

    AudioRmtSpeakerMgr::connectAllByForce();
}

} // namespace EGG
