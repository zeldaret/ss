#ifndef NW4R_SND_REMOTE_SPEAKER_MANAGER_H
#define NW4R_SND_REMOTE_SPEAKER_MANAGER_H
#include "nw4r/snd/snd_RemoteSpeaker.h"
#include "rvl/OS.h"   // IWYU pragma: export
#include "rvl/WPAD.h" // IWYU pragma: export


namespace nw4r {
namespace snd {
namespace detail {

class RemoteSpeakerManager {
public:
    static RemoteSpeakerManager &GetInstance();

    RemoteSpeaker &GetRemoteSpeaker(int i);

    void Setup();
    void Shutdown();

private:
    static const int SPEAKER_ALARM_HZ = 150;

    static const int SPEAKER_ALARM_PERIOD_NSEC = static_cast<int>(1.0f / SPEAKER_ALARM_HZ * 1000 * 1000 * 1000);

private:
    RemoteSpeakerManager();

    static void RemoteSpeakerAlarmProc(OSAlarm *pAlarm, OSContext *pCtx);

private:
    bool mInitialized;                            // at 0x0
    OSAlarm mRemoteSpeakerAlarm;                  // at 0x8
    RemoteSpeaker mSpeaker[WPAD_MAX_CONTROLLERS]; // at 0x38
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
