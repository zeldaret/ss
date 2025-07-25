#ifndef EGG_AUDIO_REMOTE_SPEAKER_MANAGER_H
#define EGG_AUDIO_REMOTE_SPEAKER_MANAGER_H

#include "common.h"

#include "rvl/WPAD.h" // IWYU pragma: export

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

    static void setup(s32 i, WPADCallback *pCallback);
    static void shutdown(s32 i, WPADCallback *pCallback);

    static void connectAllByForce();
    static void disconnectAllByForce();

    static u8 getWpadVolume();

private:
    static void setupCallback(s32, s32);
    static void shutdownCallback(s32, s32);
    static void add_task(s32 i, WPADCallback *pCallback, bool);
    static void doSetup(s32 i, WPADCallback *pCallback);
    static void doShutdown(s32 i, WPADCallback *pCallback);
    static void setupCallbackDirect(s32, s32);
    static void shutdownCallbackDirect(s32, s32);
    static bool sAudioRmtSpeakerConnectCanncelSw;
    static u32 mTaskFinishCount;
    static u32 mTaskRequestCount;
    static bool sTask;

    static u8 sAudioRmtSpeakerWpadVolume;

    static AudioRmtSpeakerTask sConnectTask[0x14];
};

} // namespace EGG

#endif
