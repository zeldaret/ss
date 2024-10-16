#ifndef NW4R_SND_SEQ_TRACK_H
#define NW4R_SND_SEQ_TRACK_H

#include "nw4r/snd/snd_Channel.h"
#include "nw4r/snd/snd_Lfo.h"
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

    struct ParserTrackParam {
        const u8 *baseAddr;    // at 0x0
        const u8 *currentAddr; // at 0x4
        s32 wait;              // at 0x8

        bool muteFlag;       // at 0xC
        bool silenceFlag;    // at 0xD
        bool noteFinishWait; // at 0xE
        bool portaFlag;      // at 0xF
        bool damperFlag;     // at 0x10

        int bankNo; // at 0x14
        int prgNo;  // at 0x18

        LfoParam lfoParam; // at 0x1C

        u8 lfoTarget;   // at 0x2C
        f32 sweepPitch; // at 0x30

        u8 volume;  // at 0x34
        u8 volume2; // at 0x35

        s8 pitchBend; // at 0x36
        u8 bendRange; // at 0x37

        s8 pan;         // at 0x38
        s8 initPan;     // at 0x39
        s8 surroundPan; // at 0x3A

        s8 transpose; // at 0x3B
        u8 priority;  // at 0x3C

        u8 portaKey;  // at 0x3D
        u8 portaTime; // at 0x3E

        u8 attack;  // at 0x3F
        u8 decay;   // at 0x40
        u8 sustain; // at 0x41
        u8 release; // at 0x42

        u8 mainSend;            // at 0x43
        u8 fxSend[AUX_BUS_NUM]; // at 0x44
        u8 lpfFreq;             // at 0x47
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

    f32 mExtVolume;                             // at 0x8
    f32 mExtPitch;                              // at 0xC
    f32 mExtPan;                                // at 0x10
    f32 mExtSurroundPan;                        // at 0x14
    f32 mPanRange;                              // at 0x18
    f32 mExtLpfFreq;                            // at 0x1C
    f32 mExtMainSend;                           // at 0x20
    f32 mExtFxSend[AUX_BUS_NUM];                // at 0x24
    f32 mExtRemoteSend[WPAD_MAX_CONTROLLERS];   // at 0x30
    f32 mExtRemoteFxSend[WPAD_MAX_CONTROLLERS]; // at 0x40

    ParserTrackParam mParserTrackParam;        // at 0x50
    volatile s16 mTrackVariable[VARIABLE_NUM]; // at 0x98
    SeqPlayer *mPlayer;                        // at 0xB8
    Channel *mChannelList;                     // at 0xBC
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
