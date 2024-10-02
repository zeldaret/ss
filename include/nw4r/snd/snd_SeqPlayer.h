#ifndef NW4R_SND_SEQ_PLAYER_H
#define NW4R_SND_SEQ_PLAYER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicPlayer.h>
#include <nw4r/snd/snd_DisposeCallback.h>
#include <nw4r/snd/snd_SoundThread.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// Forward declarations
class Channel;
class NoteOnInfo;
class NoteOnCallback;
class SeqTrack;
class SeqTrackAllocator;

class SeqPlayer : public BasicPlayer,
                  public DisposeCallback,
                  public SoundThread::PlayerCallback {
public:
    struct ParserPlayerParam {
        u8 volume;                // at 0x0
        u8 priority;              // at 0x1
        u8 timebase;              // at 0x2
        u16 tempo;                // at 0x4
        NoteOnCallback* callback; // at 0x8
    };

    enum OffsetType { OFFSET_TYPE_TICK, OFFSET_TYPE_MILLISEC };

    enum SetupResult {
        SETUP_SUCCESS,
        SETUP_ERR_CANNOT_ALLOCATE_TRACK,
        SETUP_ERR_UNKNOWN
    };

    static const int LOCAL_VARIABLE_NUM = 16;
    static const int GLOBAL_VARIABLE_NUM = 16;
    static const int VARIABLE_NUM = LOCAL_VARIABLE_NUM + GLOBAL_VARIABLE_NUM;

    static const int TRACK_NUM = 16;

public:
    SeqPlayer();
    virtual ~SeqPlayer(); // at 0x8

    virtual bool Start();          // at 0xC
    virtual void Stop();           // at 0x10
    virtual void Pause(bool flag); // at 0x14

    virtual bool IsActive() const {
        return mActiveFlag;
    } // at 0x18

    virtual bool IsStarted() const {
        return mStartedFlag;
    } // at 0x1C

    virtual bool IsPause() const {
        return mPauseFlag;
    } // at 0x20

    virtual void InvalidateData(const void* pStart,
                                const void* pEnd); // at 0x50

    virtual void InvalidateWaveData(const void* /* pStart */,
                                    const void* /* pEnd */) {} // at 0x54

    virtual void ChannelCallback(Channel* /* pChannel */) {} // at 0x58

    virtual void OnUpdateFrameSoundThread() {
        Update();
    } // at 0x5C

    virtual void OnShutdownSoundThread() {
        Stop();
    } // at 0x60

    void InitParam(int voices, NoteOnCallback* pCallback);

    SetupResult Setup(SeqTrackAllocator* pAllocator, u32 allocTrackFlags,
                      int voices, NoteOnCallback* pCallback);
    void SetSeqData(const void* pBase, s32 offset);

    void Skip(OffsetType type, int offset);

    void SetTempoRatio(f32 tempo);
    void SetChannelPriority(int priority);
    void SetReleasePriorityFix(bool flag);

    void SetLocalVariable(int i, s16 value);
    static void SetGlobalVariable(int i, s16 value);

    void SetTrackVolume(u32 trackFlags, f32 volume);
    void SetTrackPitch(u32 trackFlags, f32 pitch);

    SeqTrack* GetPlayerTrack(int i);
    volatile s16* GetVariablePtr(int i);
    void Update();

    Channel* NoteOn(int bankNo, const NoteOnInfo& rInfo);

    template <typename T>
    void SetTrackParam(u32 trackFlags, void (SeqTrack::*pSetter)(T), T param) {
        ut::AutoInterruptLock lock;

        for (int i = 0; i < TRACK_NUM && trackFlags != 0;
             trackFlags >>= 1, i++) {

            if (trackFlags & 1) {
                SeqTrack* pTrack = GetPlayerTrack(i);

                if (pTrack != NULL) {
                    (pTrack->*pSetter)(param);
                }
            }
        }
    }

    bool IsReleasePriorityFix() const {
        return mReleasePriorityFixFlag;
    }

    f32 GetPanRange() const {
        return mPanRange;
    }

    f32 GetBaseTempo() const {
        return mTempoRatio * (mParserParam.timebase * mParserParam.tempo) /
               60000.0f;
    }

    int GetVoiceOutCount() const {
        return mVoiceOutCount;
    }

    ParserPlayerParam& GetParserPlayerParam() {
        return mParserParam;
    }

private:
    static const int DEFAULT_TEMPO = 120;
    static const int DEFAULT_TIMEBASE = 48;
    static const int DEFAULT_PRIORITY = 64;
    static const int DEFAULT_VARIABLE_VALUE = -1;

    static const int MAX_SKIP_TICK_PER_FRAME = 768;

private:
    void CloseTrack(int i);
    void SetPlayerTrack(int i, SeqTrack* pTrack);

    void FinishPlayer();
    void UpdateChannelParam();
    int ParseNextTick(bool doNoteOn);

    void UpdateTick(int msec);
    void SkipTick();

    static void InitGlobalVariable();

private:
    bool mActiveFlag;             // at 0x8C
    bool mStartedFlag;            // at 0x8D
    bool mPauseFlag;              // at 0x8E
    bool mReleasePriorityFixFlag; // at 0x8F

    f32 mPanRange;                                   // at 0x90
    f32 mTempoRatio;                                 // at 0x94
    f32 mTickFraction;                               // at 0x98
    u32 mSkipTickCounter;                            // at 0x9C
    f32 mSkipTimeCounter;                            // at 0xA0
    int mVoiceOutCount;                              // at 0xA4
    ParserPlayerParam mParserParam;                  // at 0xA8
    SeqTrackAllocator* mSeqTrackAllocator;           // at 0xB4
    SeqTrack* mTracks[TRACK_NUM];                    // at 0xB8
    volatile s16 mLocalVariable[LOCAL_VARIABLE_NUM]; // at 0xF8
    u32 mTickCounter;                                // at 0x118

    static volatile s16 mGlobalVariable[LOCAL_VARIABLE_NUM];
    static bool mGobalVariableInitialized; // TYPO
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
