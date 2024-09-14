#ifndef NW4R_SND_WSD_PLAYER_H
#define NW4R_SND_WSD_PLAYER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicPlayer.h>
#include <nw4r/snd/snd_Channel.h>
#include <nw4r/snd/snd_DisposeCallback.h>
#include <nw4r/snd/snd_Lfo.h>
#include <nw4r/snd/snd_SoundThread.h>
#include <nw4r/snd/snd_WsdFile.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

class WsdPlayer : public BasicPlayer,
                  public DisposeCallback,
                  public SoundThread::PlayerCallback {
public:
    class WsdCallback {
    public:
        virtual ~WsdCallback() {} // at 0x8

        virtual bool GetWaveSoundData(WaveSoundInfo* pSoundInfo,
                                      WaveSoundNoteInfo* pNoteInfo,
                                      WaveData* pWaveData, const void* pWsdData,
                                      int index, int noteIndex,
                                      u32 callbackArg) const = 0; // at 0xC
    };

    enum StartOffsetType {
        START_OFFSET_TYPE_SAMPLE,
        START_OFFSET_TYPE_MILLISEC
    };

public:
    WsdPlayer();

    virtual bool Start();          // at 0xC
    virtual void Stop();           // at 0x10
    virtual void Pause(bool flag); // at 0x14

    virtual bool IsActive() const {
        return mActiveFlag != 0;
    } // at 0x18

    virtual bool IsStarted() const {
        return mStartedFlag != 0;
    } // at 0x1C

    virtual bool IsPause() const {
        return mPauseFlag != 0;
    } // at 0x20

    virtual void InvalidateData(const void* pStart,
                                const void* pEnd); // at 0x50

    virtual void InvalidateWaveData(const void* /* pStart */,
                                    const void* /* pEnd */) {} // at 0x54

    virtual void OnUpdateFrameSoundThread() {
        Update();
    } // at 0x58

    virtual void OnShutdownSoundThread() {
        Stop();
    } // at 0x60

    void InitParam(int voices, const WsdCallback* pCallback, u32 callbackArg);

    bool Prepare(const void* pWsdData, int index, StartOffsetType startType,
                 int startOffset, int voices, const WsdCallback* pCallback,
                 u32 callbackArg);

    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool flag);

    f32 GetPanRange() const {
        return mPanRange;
    }

    int GetVoiceOutCount() const {
        return mVoiceOutCount;
    }

    int GetChannelPriority() const {
        return mPriority;
    }

    const void* GetWsdDataAddress() const {
        return mWsdData;
    }

    bool IsChannelActive() const {
        return mChannel != NULL && mChannel->IsActive();
    }

private:
    static const int DEFAULT_PRIORITY = 64;

private:
    void FinishPlayer();
    void Update();

    bool StartChannel(const WsdCallback* pCallback, u32 callbackArg);
    void CloseChannel();
    void UpdateChannel();

    static void ChannelCallbackFunc(Channel* pDropChannel,
                                    Channel::ChannelCallbackStatus status,
                                    u32 callbackArg);

private:
    bool mActiveFlag;             // at 0x8C
    bool mStartedFlag;            // at 0x8D
    bool mPauseFlag;              // at 0x8E
    bool mWavePlayFlag;           // at 0x8F
    bool mReleasePriorityFixFlag; // at 0x90

    f32 mPanRange;      // at 0x94
    int mVoiceOutCount; // at 0x98
    u8 mPriority;       // at 0x9C

    const WsdCallback* mCallback; // at 0xA0
    u32 mCallbackData;            // at 0xA4

    const void* mWsdData; // at 0xA8
    int mWsdIndex;        // at 0xAC

    StartOffsetType mStartOffsetType; // at 0xB0
    int mStartOffset;                 // at 0xB4

    LfoParam mLfoParam;           // at 0xB8
    WaveSoundInfo mWaveSoundInfo; // at 0xC8
    Channel* mChannel;            // at 0xD4
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
