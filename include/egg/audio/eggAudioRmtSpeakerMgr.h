#ifndef EGG_AUDIO_REMOTE_SPEAKER_MANAGER_H
#define EGG_AUDIO_REMOTE_SPEAKER_MANAGER_H

#include <common.h>
#include <rvl/WPAD.h>

namespace EGG {

// Size 0xC
struct AudioRmtSpeakerTask {
    bool field_0x00;
    bool field_0x01;
    s32 mChannel;
    WPADCallback *mpCallback;
};

class AudioRmtSpeakerMgr {
public:
    static void calc();

    static void fn_804B6D80(s32 i, WPADCallback *pCallback);
    static void fn_804B6DE0(s32 i, WPADCallback *pCallback);

    static void connectAllByForce();
    static void disconnectAllByForce();

    static u8 getWpadVolume();

private:
    static void setupCallback(s32, s32);
    static void shutdownCallback(s32, s32);
    static void fn_804B6AF0(s32 i, WPADCallback *pCallback, bool);
    static void fn_804B6B80(s32 i, WPADCallback *pCallback);
    static void fn_804B6C00(s32 i, WPADCallback *pCallback);
    static void setupCallbackDirect(s32, s32);
    static void shutdownCallbackDirect(s32, s32);
    static bool sAudioRmtSpeakerConnectCanncelSw;
    static u32 mTaskFinishCount;
    static u32 mTaskRequestCount;
    static bool sTask;

    static u8 sAudioRmtSpeakerWpadVolume;

    static AudioRmtSpeakerTask sTasks[0x14];
};

} // namespace EGG

#endif
