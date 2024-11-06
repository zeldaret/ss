#ifndef NW4R_SND_REMOTE_SPEAKER_H
#define NW4R_SND_REMOTE_SPEAKER_H

#include "rvl/OS.h"   // IWYU pragma: export
#include "rvl/WENC.h" // IWYU pragma: export
#include "rvl/WPAD.h" // IWYU pragma: export

namespace nw4r {
namespace snd {

class RemoteSpeaker {
public:
    enum SpeakerState {
        STATE_INVALID,
        STATE_EXEC_SPEAKER_ON,
        STATE_SPEAKER_ON,
        STATE_EXEC_SPEAKER_PLAY,
        STATE_SPEAKER_PLAY,
        STATE_EXEC_SPEAKER_OFF,
        STATE_SPEAKER_OFF
    };

    enum SpeakerCommand {
        COMMAND_NONE,
        COMMAND_SPEAKER_ON,
        COMMAND_SPEAKER_PLAY,
        COMMAND_SPEAKER_OFF
    };

    static const int SAMPLES_PER_AUDIO_PACKET = 40;

public:
    RemoteSpeaker();

    void InitParam();
    bool Setup(WPADCallback pCallback);
    void Shutdown(WPADCallback pCallback);

    bool EnableOutput(bool enable);
    bool IsEnabledOutput() const;

    void Update();
    void UpdateStreamData(const s16 *pRmtSamples);

    bool IsAvailable() const {
        return mState == STATE_SPEAKER_PLAY;
    }

    void SetChannelIndex(int index) {
        mChannelIndex = index;
    }

private:
    static const int SAMPLES_PER_ENCODED_PACKET = (SAMPLES_PER_AUDIO_PACKET + 1) / 2;

    static const int CONTINUOUS_PLAY_INTERVAL_MINUTES = 8;

private:
    void ClearParam();
    void ExecCommand(SpeakerCommand command);

    bool IsAllSampleZero(const s16 *pRmtSamples);
    void NotifyCallback(s32 chan, s32 result);

    static void SpeakerOnCallback(s32 chan, s32 result);
    static void SpeakerPlayCallback(s32 chan, s32 result);
    static void SpeakerOffCallback(s32 chan, s32 result);

    static void ContinueAlarmHandler(OSAlarm *pAlarm, OSContext *pCtx);
    static void IntervalAlarmHandler(OSAlarm *pAlarm, OSContext *pCtx);

private:
    bool mInitFlag;          // at 0x0
    bool mPlayFlag;          // at 0x1
    bool mEnableFlag;        // at 0x2
    bool mFirstEncodeFlag;   // at 0x3
    bool mValidCallbackFlag; // at 0x4
    bool mCommandBusyFlag;   // at 0x5
    bool mContinueFlag;      // at 0x6
    volatile bool mIntervalFlag;     // at 0x7
    SpeakerState mState;             // at 0x8
    SpeakerCommand mUserCommand;     // at 0xC
    SpeakerCommand mInternalCommand; // at 0x10
    WENCInfo mEncodeInfo;            // at 0x14
    int mChannelIndex;               // at 0x34
    WPADCallback *mWpadCallback;      // at 0x38
    OSAlarm mContinueAlarm;          // at 0x40
    OSAlarm mIntervalAlarm;          // at 0x70
    s64 mContinueBeginTime;          // at 0xA0
};

} // namespace snd
} // namespace nw4r

#endif
