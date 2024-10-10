#include <egg/audio/eggAudioRmtSpeakerMgr.h>
#include <nw4r/snd/snd_SoundSystem.h>
#include <rvl/OS.h>

namespace EGG {

bool AudioRmtSpeakerMgr::sAudioRmtSpeakerConnectCanncelSw;
u32 AudioRmtSpeakerMgr::mTaskFinishCount;
u32 AudioRmtSpeakerMgr::mTaskRequestCount;
bool AudioRmtSpeakerMgr::sTask;

u8 AudioRmtSpeakerMgr::sAudioRmtSpeakerWpadVolume = 0x58;

AudioRmtSpeakerTask AudioRmtSpeakerMgr::sTasks[0x14];

void AudioRmtSpeakerMgr::setupCallback(s32 arg1, s32 arg2) {
    if (arg2 == 0) {
        if (sTasks[mTaskFinishCount].mpCallback != nullptr) {
            (sTasks[mTaskFinishCount].mpCallback)(arg1, arg2);
        }
        sAudioRmtSpeakerWpadVolume = WPADGetSpeakerVolume();
    } else {
        fn_804B6D80(arg1, sTasks[mTaskFinishCount].mpCallback);
    }
    sTasks[mTaskFinishCount].field_0x01 = true;
}

void AudioRmtSpeakerMgr::shutdownCallback(s32 arg1, s32 arg2) {
    if ((u32)arg2 + 1 <= 1) {
        if (sTasks[mTaskFinishCount].mpCallback != nullptr) {
            (sTasks[mTaskFinishCount].mpCallback)(arg1, arg2);
        }
    } else {
        fn_804B6DE0(arg1, sTasks[mTaskFinishCount].mpCallback);
    }
    sTasks[mTaskFinishCount].field_0x01 = true;
}

void AudioRmtSpeakerMgr::fn_804B6AF0(s32 i, WPADCallback *pCallback, bool enable) {
    BOOL intr = OSDisableInterrupts();

    u32 index = mTaskRequestCount;
    sTasks[index].mChannel = i;
    sTasks[index].field_0x00 = enable;
    sTasks[index].mpCallback = pCallback;
    sTasks[index].field_0x01 = false;
    if (++mTaskRequestCount >= 0x14) {
        mTaskRequestCount = 0;
    }

    OSRestoreInterrupts(intr);
}

void AudioRmtSpeakerMgr::fn_804B6B80(s32 i, WPADCallback *pCallback) {
    if (!nw4r::snd::SoundSystem::GetRemoteSpeaker(i).Setup(pCallback)) {
        fn_804B6AF0(i, pCallback, true);
        sTasks[mTaskRequestCount].field_0x01 = true;
    }
}

void AudioRmtSpeakerMgr::fn_804B6C00(s32 i, WPADCallback *pCallback) {
    nw4r::snd::SoundSystem::GetRemoteSpeaker(i).Shutdown(pCallback);
}

void AudioRmtSpeakerMgr::calc() {
    if (!sTask) {
        if (mTaskRequestCount != mTaskFinishCount) {
            if (sTasks[mTaskFinishCount].field_0x00) {
                fn_804B6B80(sTasks[mTaskFinishCount].mChannel, setupCallback);
            } else {
                fn_804B6C00(sTasks[mTaskFinishCount].mChannel, shutdownCallback);
            }
            sTask = true;
        }
    } else if (sTasks[mTaskFinishCount].field_0x01) {
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
        fn_804B6B80(arg1, setupCallbackDirect);
    }
}

void AudioRmtSpeakerMgr::shutdownCallbackDirect(s32 arg1, s32 arg2) {
    if (arg2 == -1) {
        return;
    }
    if (arg2 == 0) {
        return;
    }
    fn_804B6C00(arg1, shutdownCallbackDirect);
}

void AudioRmtSpeakerMgr::fn_804B6D80(s32 i, WPADCallback *pCallback) {
    WPADDeviceType ty;
    if (!sAudioRmtSpeakerConnectCanncelSw && WPADProbe(i, &ty) != WPAD_ERR_NO_CONTROLLER) {
        fn_804B6AF0(i, pCallback, true);
    }
}

void AudioRmtSpeakerMgr::fn_804B6DE0(s32 i, WPADCallback *pCallback) {
    WPADDeviceType ty;
    if (!sAudioRmtSpeakerConnectCanncelSw) {
        if (WPADProbe(i, &ty) == WPAD_ERR_NO_CONTROLLER) {
            if (!nw4r::snd::SoundSystem::GetRemoteSpeaker(i).IsEnabledOutput()) {
                return;
            }
        }
        fn_804B6AF0(i, pCallback, false);
    }
}

void AudioRmtSpeakerMgr::connectAllByForce() {
    WPADDeviceType ty;
    sAudioRmtSpeakerConnectCanncelSw = false;
    for (int i = 0; i < 4; i++) {
        if (WPADProbe(i, &ty) != WPAD_ERR_NO_CONTROLLER) {
            fn_804B6B80(i, setupCallbackDirect);
        }
    }
}

void AudioRmtSpeakerMgr::disconnectAllByForce() {
    WPADDeviceType ty;
    for (int i = 0; i < 4; i++) {
        WPADProbe(i, &ty); // ignoring result here
        if (nw4r::snd::SoundSystem::GetRemoteSpeaker(i).IsAvailable()) {
            fn_804B6C00(i, shutdownCallbackDirect);
        }
    }
    sAudioRmtSpeakerConnectCanncelSw = true;
}

u8 AudioRmtSpeakerMgr::getWpadVolume() {
    return sAudioRmtSpeakerWpadVolume;
}

} // namespace EGG
