#ifndef NW4R_SND_CHANNEL_H
#define NW4R_SND_CHANNEL_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Common.h>
#include <nw4r/snd/snd_EnvGenerator.h>
#include <nw4r/snd/snd_Lfo.h>
#include <nw4r/snd/snd_MoveValue.h>
#include <nw4r/snd/snd_Voice.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

class Channel {
public:
    enum ChannelCallbackStatus {
        CALLBACK_STATUS_STOPPED,
        CALLBACK_STATUS_DROP,
        CALLBACK_STATUS_FINISH,
        CALLBACK_STATUS_CANCEL
    };

    typedef void (*ChannelCallback)(Channel* pDropChannel,
                                    ChannelCallbackStatus status,
                                    u32 callbackArg);

    enum LfoTarget { LFO_TARGET_PITCH, LFO_TARGET_VOLUME, LFO_TARGET_PAN };

public:
    Channel();
    ~Channel();

    void InitParam(ChannelCallback pCallback, u32 callbackArg);
    void Update(bool periodic);
    void Start(const WaveData& rData, int length, u32 offset);
    void Release();
    void Stop();

    void SetAttack(int attack) {
        mEnvelope.SetAttack(attack);
    }
    void SetDecay(int decay) {
        mEnvelope.SetDecay(decay);
    }
    void SetSustain(int sustain) {
        mEnvelope.SetSustain(sustain);
    }
    void SetRelease(int release) {
        mEnvelope.SetRelease(release);
    }
    bool IsRelease() const {
        return mEnvelope.GetStatus() == EnvGenerator::STATUS_RELEASE;
    }

    void SetLfoParam(const LfoParam& rParam) {
        mLfo.SetParam(rParam);
    }
    void SetLfoTarget(LfoTarget target) {
        mLfoTarget = target;
    }

    void Pause(bool pause) {
        mPauseFlag = pause;
        mVoice->Pause(pause);
    }
    void SetReleasePriorityFix(bool fix) {
        mReleasePriorityFixFlag = fix;
    }

    bool IsPause() const {
        return mPauseFlag != false;
    }
    bool IsActive() const {
        return mActiveFlag;
    }
    bool IsAutoUpdateSweep() const {
        return mAutoSweep;
    }

    void SetUserVolume(f32 volume) {
        mUserVolume = volume;
    }
    void SetUserPitch(f32 pitch) {
        mUserPitch = pitch;
    }
    void SetUserPitchRatio(f32 ratio) {
        mUserPitchRatio = ratio;
    }
    void SetUserPan(f32 pan) {
        mUserPan = pan;
    }
    void SetUserSurroundPan(f32 pan) {
        mUserSurroundPan = pan;
    }
    void SetUserLpfFreq(f32 freq) {
        mUserLpfFreq = freq;
    }

    void SetRemoteFilter(int filter) {
        mRemoteFilter = filter;
    }
    void SetOutputLine(int flag) {
        mOutputLineFlag = flag;
    }

    void SetMainOutVolume(f32 volume) {
        mMainOutVolume = volume;
    }
    void SetMainSend(f32 send) {
        mMainSend = send;
    }
    void SetFxSend(AuxBus bus, f32 send) {
        mFxSend[bus] = send;
    }

    void SetRemoteOutVolume(int remote, f32 volume) {
        mRemoteOutVolume[remote] = volume;
    }
    void SetRemoteSend(int remote, f32 send) {
        mRemoteSend[remote] = send;
    }
    void SetRemoteFxSend(int remote, f32 send) {
        mRemoteFxSend[remote] = send;
    }

    void UpdateSweep(int count);
    void SetSweepParam(f32 pitch, int time, bool autoUpdate);
    f32 GetSweepValue() const;

    void SetInitVolume(f32 volume) {
        mInitVolume = volume;
    }
    void SetInitPan(f32 pan) {
        mInitPan = pan;
    }
    void SetInitSurroundPan(f32 pan) {
        mInitSurroundPan = pan;
    }
    void SetTune(f32 tune) {
        mTune = tune;
    }
    void SetSilence(bool silence, int fadeTime) {
        mSilenceVolume.SetTarget(silence ? 0 : SILENCE_VOLUME_MAX, fadeTime);
    }

    void SetKey(int key) {
        mKey = key;
    }
    void SetOriginalKey(int key) {
        mOriginalKey = key;
    }

    s32 GetLength() const {
        return mLength;
    }
    void SetLength(s32 length) {
        mLength = length;
    }

    void SetPanMode(PanMode mode) {
        mPanMode = mode;
    }
    void SetPanCurve(PanCurve curve) {
        mPanCurve = curve;
    }

    Channel* GetNextTrackChannel() const {
        return mNextLink;
    }
    void SetNextTrackChannel(Channel* pChannel) {
        mNextLink = pChannel;
    }

    static Channel* AllocChannel(int channels, int voices, int priority,
                                 ChannelCallback pCallback, u32 callbackArg);
    static void FreeChannel(Channel* pChannel);

private:
    static const u8 SILENCE_VOLUME_MAX = 255;

    static const int KEY_INIT = 60;
    static const int ORIGINAL_KEY_INIT = 60;

    static const int PRIORITY_RELEASE = 1;

private:
    static void VoiceCallbackFunc(Voice* pDropVoice,
                                  Voice::VoiceCallbackStatus status,
                                  void* pCallbackArg);

private:
    EnvGenerator mEnvelope; // at 0x0
    Lfo mLfo;               // at 0x18
    u8 mLfoTarget;          // at 0x30

    bool mPauseFlag;              // at 0x31
    bool mActiveFlag;             // at 0x32
    bool mAllocFlag;              // at 0x33
    bool mAutoSweep;              // at 0x34
    bool mReleasePriorityFixFlag; // at 0x35

    f32 mUserVolume;      // at 0x38
    f32 mUserPitchRatio;  // at 0x3C
    f32 mUserPan;         // at 0x40
    f32 mUserSurroundPan; // at 0x44
    f32 mUserLpfFreq;     // at 0x48

    int mRemoteFilter;   // at 0x4C
    int mOutputLineFlag; // at 0x50

    f32 mMainOutVolume;       // at 0x54
    f32 mMainSend;            // at 0x58
    f32 mFxSend[AUX_BUS_NUM]; // at 0x5C

    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // at 0x68
    f32 mRemoteSend[WPAD_MAX_CONTROLLERS];      // at 0x78
    f32 mRemoteFxSend[WPAD_MAX_CONTROLLERS];    // at 0x88

    f32 mUserPitch;    // at 0x98
    f32 mSweepPitch;   // at 0x9C
    int mSweepCounter; // at 0xA0
    int mSweepLength;  // at 0xA4

    f32 mInitVolume;                   // at 0xA8
    f32 mInitPan;                      // at 0xAC
    f32 mInitSurroundPan;              // at 0xB0
    f32 mTune;                         // at 0xB4
    MoveValue<u8, u16> mSilenceVolume; // at 0xB8

    int mKey;         // at 0xC0
    int mOriginalKey; // at 0xC4
    int mLength;      // at 0xC8

    PanMode mPanMode;   // at 0xCC
    PanCurve mPanCurve; // at 0xD0

    ChannelCallback mCallback; // at 0xD4
    u32 mCallbackData;         // at 0xD8

    Voice* mVoice;      // at 0xDC
    Channel* mNextLink; // at 0xE0

public:
    NW4R_UT_LIST_NODE_DECL(); // at 0xE4
};

NW4R_UT_LIST_TYPEDEF_DECL(Channel);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
