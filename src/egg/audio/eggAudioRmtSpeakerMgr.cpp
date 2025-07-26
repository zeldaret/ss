#include "egg/audio/eggAudioRmtSpeakerMgr.h"

#include "nw4r/snd/snd_SoundSystem.h"

#include "rvl/OS.h" // IWYU pragma: export

namespace EGG {

bool AudioRmtSpeakerMgr::sAudioRmtSpeakerConnectCanncelSw;
u32 AudioRmtSpeakerMgr::mTaskFinishCount;
u32 AudioRmtSpeakerMgr::mTaskRequestCount;
bool AudioRmtSpeakerMgr::sTask;

u8 AudioRmtSpeakerMgr::sAudioRmtSpeakerWpadVolume = 0x58;

AudioRmtSpeakerTask AudioRmtSpeakerMgr::sConnectTask[0x14];

void AudioRmtSpeakerMgr::setupCallback(s32 arg1, s32 arg2) {
    if (arg2 == 0) {
        if (sConnectTask[mTaskFinishCount].mpCallback != nullptr) {
            (sConnectTask[mTaskFinishCount].mpCallback)(arg1, arg2);
        }
        sAudioRmtSpeakerWpadVolume = WPADGetSpeakerVolume();
    } else {
        setup(arg1, sConnectTask[mTaskFinishCount].mpCallback);
    }
    sConnectTask[mTaskFinishCount].field_0x01 = true;
}

void AudioRmtSpeakerMgr::shutdownCallback(s32 arg1, s32 arg2) {
    if ((u32)arg2 + 1 <= 1) {
        if (sConnectTask[mTaskFinishCount].mpCallback != nullptr) {
            (sConnectTask[mTaskFinishCount].mpCallback)(arg1, arg2);
        }
    } else {
        shutdown(arg1, sConnectTask[mTaskFinishCount].mpCallback);
    }
    sConnectTask[mTaskFinishCount].field_0x01 = true;
}

void AudioRmtSpeakerMgr::add_task(s32 i, WPADCallback *pCallback, bool enable) {
    BOOL intr = OSDisableInterrupts();

    u32 index = mTaskRequestCount;
    sConnectTask[index].mChannel = i;
    sConnectTask[index].field_0x00 = enable;
    sConnectTask[index].mpCallback = pCallback;
    sConnectTask[index].field_0x01 = false;
    if (++mTaskRequestCount >= 0x14) {
        mTaskRequestCount = 0;
    }

    OSRestoreInterrupts(intr);
}

void AudioRmtSpeakerMgr::doSetup(s32 i, WPADCallback *pCallback) {
    if (!nw4r::snd::SoundSystem::GetRemoteSpeaker(i).Setup(pCallback)) {
        add_task(i, pCallback, true);
        sConnectTask[mTaskRequestCount].field_0x01 = true;
    }
}

void AudioRmtSpeakerMgr::doShutdown(s32 i, WPADCallback *pCallback) {
    nw4r::snd::SoundSystem::GetRemoteSpeaker(i).Shutdown(pCallback);
}

void AudioRmtSpeakerMgr::calc() {
    if (!sTask) {
        if (mTaskRequestCount != mTaskFinishCount) {
            if (sConnectTask[mTaskFinishCount].field_0x00) {
                doSetup(sConnectTask[mTaskFinishCount].mChannel, setupCallback);
            } else {
                doShutdown(sConnectTask[mTaskFinishCount].mChannel, shutdownCallback);
            }
            sTask = true;
        }
    } else if (sConnectTask[mTaskFinishCount].field_0x01) {
        sTask = false;
        mTaskFinishCount++;
        if (mTaskFinishCount >= 0x14) {
            mTaskFinishCount = 0;
        }
    }
}

void AudioRmtSpeakerMgr::setupCallbackDirect(s32 arg1, s32 arg2) {
    if (arg2 == 0) {
        sAudioRmtSpeakerWpadVolume = WPADGetSpeakerVolume();
    } else {
        doSetup(arg1, setupCallbackDirect);
    }
}

void AudioRmtSpeakerMgr::shutdownCallbackDirect(s32 arg1, s32 arg2) {
    if (arg2 == -1) {
        return;
    }
    if (arg2 == 0) {
        return;
    }
    doShutdown(arg1, shutdownCallbackDirect);
}

void AudioRmtSpeakerMgr::setup(s32 i, WPADCallback *pCallback) {
    WPADDeviceType ty;
    if (!sAudioRmtSpeakerConnectCanncelSw && WPADProbe(i, &ty) != WPAD_ERR_NO_CONTROLLER) {
        add_task(i, pCallback, true);
    }
}

void AudioRmtSpeakerMgr::shutdown(s32 i, WPADCallback *pCallback) {
    WPADDeviceType ty;
    if (!sAudioRmtSpeakerConnectCanncelSw) {
        if (WPADProbe(i, &ty) == WPAD_ERR_NO_CONTROLLER) {
            if (!nw4r::snd::SoundSystem::GetRemoteSpeaker(i).IsEnabledOutput()) {
                return;
            }
        }
        add_task(i, pCallback, false);
    }
}

void AudioRmtSpeakerMgr::connectAllByForce() {
    WPADDeviceType ty;
    sAudioRmtSpeakerConnectCanncelSw = false;
    for (int i = 0; i < 4; i++) {
        if (WPADProbe(i, &ty) != WPAD_ERR_NO_CONTROLLER) {
            doSetup(i, setupCallbackDirect);
        }
    }
}

void AudioRmtSpeakerMgr::disconnectAllByForce() {
    WPADDeviceType ty;
    for (int i = 0; i < 4; i++) {
        WPADProbe(i, &ty); // ignoring result here
        if (nw4r::snd::SoundSystem::GetRemoteSpeaker(i).IsAvailable()) {
            doShutdown(i, shutdownCallbackDirect);
        }
    }
    sAudioRmtSpeakerConnectCanncelSw = true;
}

u8 AudioRmtSpeakerMgr::getWpadVolume() {
    return sAudioRmtSpeakerWpadVolume;
}

} // namespace EGG
