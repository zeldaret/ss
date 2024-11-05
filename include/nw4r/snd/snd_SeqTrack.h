#ifndef NW4R_SND_SEQ_TRACK_H
#define NW4R_SND_SEQ_TRACK_H

#include "nw4r/snd/snd_Channel.h"
#include "nw4r/snd/snd_Lfo.h"
#include "nw4r/snd/snd_MoveValue.h"
#include "nw4r/types_nw4r.h"
#include "rvl/WPAD.h" // IWYU pragma: export

namespace nw4r {
namespace snd {

enum SeqMute {
    MUTE_OFF,
    MUTE_NO_STOP,
    MUTE_RELEASE,
    MUTE_STOP
};

enum ParseResult {
    PARSE_RESULT_CONTINUE,
    PARSE_RESULT_FINISH
};

namespace detail {

class SeqTrack {
public:
    static const int VARIABLE_NUM = 16;
    static const int PRGNO_MAX = 0xFFFF;

    struct CallStack {
        bool loopFlag;
        u8 loopCount;
        const u8 *address;
    };

    struct ParserTrackParam {
        const u8 *baseAddr;    // at 0x0
        const u8 *currentAddr; // at 0x4
        bool cmpFlag;          // at 0x8
        bool noteWaitFlag;     // at 0x9
        bool tieFlag;          // at 0xA
        bool monophonicFlag;   // at 0xB

        CallStack callStack[3]; // at 0xC
        u8 callStackDepth;      // at 0x24
        s32 wait;               // 0x28

        bool muteFlag;       // at 0x2C
        bool silenceFlag;    // at 0x2D
        bool noteFinishWait; // at 0x2E
        bool portaFlag;      // at 0x2F
        bool damperFlag;     // at 0x30

        int bankNo; // at 0x34
        int prgNo;  // at 0x38

        LfoParam lfoParam; // at 0x3C

        u8 lfoTarget;   // at 0x4C
        f32 sweepPitch; // at 0x50

        MoveValue<u8, s16> volume;      // at 0x54
        MoveValue<s8, s16> pan;         // at 0x5A
        MoveValue<s8, s16> surroundPan; // at 0x60
        MoveValue<s8, s16> pitchBend;   // at 0x64

        u8 volume2;       // at 0x6C
        u8 velocityRange; // at 0x6D

        u8 bendRange; // at 0x6E

        s8 initPan; // at 0x6F

        s8 transpose; // at 0x70
        u8 priority;  // at 0x71

        u8 portaKey;  // at 0x72
        u8 portaTime; // at 0x73

        u8 attack;  // at 0x74
        u8 decay;   // at 0x75
        u8 sustain; // at 0x76
        u8 release; // at 0x77

        s16 envHold;            // 0x78
        u8 mainSend;            // at 0x7A
        u8 fxSend[AUX_BUS_NUM]; // at 0x7B
        u8 biquadType;          // 0x7E
        f32 lpfFreq;            // at 0x80
        f32 biquadValue;        // at 0x84
    };

public:
    SeqTrack();
    virtual ~SeqTrack(); // at 0x8

    virtual ParseResult Parse(bool doNoteOn) = 0; // at 0xC

    bool IsOpened() const {
        return mOpenFlag;
    }

    void SetPlayerTrackNo(int no);
    u8 GetPlayerTrackNo() const {
        return mPlayerTrackNo;
    }

    void InitParam();
    void SetSeqData(const void *pBase, s32 offset);

    void Open();
    void Close();

    void UpdateChannelLength();
    void UpdateChannelRelease(Channel *pChannel);

    int ParseNextTick(bool doNoteOn);

    void StopAllChannel();
    void ReleaseAllChannel(int release);
    void PauseAllChannel(bool flag);
    void AddChannel(Channel *pChannel);
    void UpdateChannelParam();
    void FreeAllChannel();

    void SetMute(SeqMute mute);
    void SetSilence(bool, int);
    void SetVolume(f32 volume);
    void SetPitch(f32 pitch);

    ParserTrackParam &GetParserTrackParam() {
        return mParserTrackParam;
    }

    volatile s16 *GetVariablePtr(int i);

    SeqPlayer *GetSeqPlayer() {
        return mPlayer;
    }
    void SetSeqPlayer(SeqPlayer *pPlayer) {
        mPlayer = pPlayer;
    }

    Channel *GetLastChannel() const {
        return mChannelList;
    }

    Channel *NoteOn(int key, int velocity, s32 length, bool tie);

private:
    static const int DEFAULT_PRIORITY = 64;
    static const int DEFAULT_BENDRANGE = 2;
    static const int DEFAULT_PORTA_KEY = 60;
    static const int DEFAULT_VARIABLE_VALUE = -1;

private:
    static void ChannelCallbackFunc(Channel *pDropChannel, Channel::ChannelCallbackStatus status, u32 callbackArg);

private:
    u8 mPlayerTrackNo; // at 0x4
    bool mOpenFlag;    // at 0x5

    f32 mExtVolume;                            // at 0x8
    f32 mExtPitch;                             // at 0xC
    f32 mExtPan;                               // at 0x10
    f32 mExtSurroundPan;                       // at 0x14
    f32 mPanRange;                             // at 0x18
    ParserTrackParam mParserTrackParam;        // at 0x1C
    volatile s16 mTrackVariable[VARIABLE_NUM]; // at 0xA0
    SeqPlayer *mPlayer;                        // at 0xC0
    Channel *mChannelList;                     // at 0xC4
    // ???
    f32 mExtLpfFreq;                            // at 0x1C
    f32 mExtMainSend;                           // at 0x20
    f32 mExtFxSend[AUX_BUS_NUM];                // at 0x24
    f32 mExtRemoteSend[WPAD_MAX_CONTROLLERS];   // at 0x30
    f32 mExtRemoteFxSend[WPAD_MAX_CONTROLLERS]; // at 0x40
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
