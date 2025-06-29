#ifndef D_SND_BGM_MML_PARSER_BASE_H
#define D_SND_BGM_MML_PARSER_BASE_H

#include "common.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"

// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2cdd6
struct dBgmMmlCallStack
{
    bool	loopFlag;		// size 0x01, offset 0x00
    u8		loopCount;		// size 0x01, offset 0x01
    /* 2 bytes padding */
    byte_t	const *address;	// size 0x04, offset 0x04
}; // size 0x08

/**
 * Size: 0x28
 */
struct dBgmMmlTrack {
    /* 0x00 */ const byte_t *currentAddr;
    /* 0x04 */ u8 mTrackNo;
    /* 0x05 */ bool mTrackUsed;
    /* 0x06 */ u8 cmpFlag;
    /* 0x08 */ s32 wait;
    /* 0x0C */ dBgmMmlCallStack callStack[3];
    /* 0x24 */ s32 callStackDepth;
};

/**
 * @brief Parses harp key data
 *
 * Partial copy of nw4r::snd::detail::MmlParser
 */
class dSndBgmMmlParser_c {
private:
    // [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2d70c
    // Swapped - or maybe just returns a boolean
    enum ParseResult {
        PARSE_RESULT_FINISH,
        PARSE_RESULT_CONTINUE,
    };

    // [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x31279
    enum SeqArgType {
        SEQ_ARG_NONE,

        SEQ_ARG_U8,
        SEQ_ARG_S16,
        SEQ_ARG_VMIDI,
        SEQ_ARG_RANDOM,
        SEQ_ARG_VARIABLE,
    };

    enum MmlCommand {
        MML_CMD_MIN = 0x80, // <80 -> MML note, not a command
        MML_CMD_MAX = 0xff,

        MML_CMD_MASK = 0x80,
        MML_CMD_SET_MASK = 0xf0,

        MML_WAIT = 0x80,
        MML_SET_PRGNO,

        MML_OPEN_TRACK = 0x88,
        MML_JUMP,
        MML_CALL,

        MML_ARG_1_RANDOM = 0xa0,
        MML_ARG_1_VARIABLE,
        MML_EXEC_IF,
        MML_ARG_2_S16,
        MML_ARG_2_RANDOM,
        MML_ARG_2_VARIABLE,

        MML_SET_TIMEBASE = 0xb0,
        MML_SET_ENV_HOLD,
        MML_SET_MONOPHONIC,
        MML_SET_TRACK_VELOCITY_RANGE,
        MML_SET_BIQUAD_TYPE,
        MML_SET_BIQUAD_VALUE,

        MML_SET_PAN = 0xc0,
        MML_SET_TRACK_VOLUME,
        MML_SET_PLAYER_VOLUME,
        MML_SET_TRANSPOSE,
        MML_SET_PITCH_BEND,
        MML_SET_BEND_RANGE,
        MML_SET_PRIORITY,
        MML_SET_NOTE_WAIT,
        MML_SET_TIE,
        MML_SET_PORTAMENTO,
        MML_SET_LFO_DEPTH,
        MML_SET_LFO_SPEED,
        MML_SET_LFO_TARGET,
        MML_SET_LFO_RANGE,
        MML_SET_PORTASPEED,
        MML_SET_PORTATIME,

        MML_SET_ATTACK = 0xd0,
        MML_SET_DECAY,
        MML_SET_SUSTAIN,
        MML_SET_RELEASE,
        MML_LOOP_START,
        MML_SET_TRACK_VOLUME2,
        MML_PRINT_VAR,
        MML_SET_SURROUND_PAN,
        MML_SET_LPF_FREQ,
        MML_SET_FX_SEND_A,
        MML_SET_FX_SEND_B,
        MML_SET_MAIN_SEND,
        MML_SET_INIT_PAN,
        MML_SET_MUTE,
        MML_SET_FX_SEND_C,
        MML_SET_DAMPER,

        MML_SET_LFO_DELAY = 0xe0,
        MML_SET_TEMPO,
        MML_SET_E2,
        MML_SET_SWEEP_PITCH,

        MML_RESET_ADSR = 0xfb,
        MML_LOOP_END,
        MML_RET,
        MML_ALLOC_TRACK,
        MML_EOF
    };

    enum MmlExCommand {
        MML_EX_COMMAND = 0xf0,

        MML_EX_CMD_MAX = 0xffff,

        MML_EX_ARITHMETIC = 0x80,
        MML_EX_SET = 0x80,
        MML_EX_APL,
        MML_EX_AMI,
        MML_EX_AMU,
        MML_EX_ADV,
        MML_EX_ALS,
        MML_EX_RND,
        MML_EX_AAD,
        MML_EX_AOR,
        MML_EX_AER,
        MML_EX_ACO,
        MML_EX_AMD,

        MML_EX_LOGIC = 0x90,
        MML_EX_EQ = 0x90,
        MML_EX_GE,
        MML_EX_GT,
        MML_EX_LE,
        MML_EX_LT,
        MML_EX_NE,

        MML_EX_USERPROC = 0xe0,
    };

public:
    dSndBgmMmlParser_c(nw4r::snd::SoundArchivePlayer *player, nw4r::snd::SoundArchive *archive);

    virtual void CommandProc(u32 trackNo, int wait, u32 command, s32 commandArg1, s32 commandArg2) const;
    virtual void NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const;

protected:
    bool loadAndParse(u32 soundId, u32 trackMask, bool noJumps);

private:
    bool readTracks(u32 soundId, u32 trackMask, bool noJumps);
    void initTrack(s32 trackNo, u32 offset);
    ParseResult Parse(dBgmMmlTrack *track) const;
    void CommandProc_(dBgmMmlTrack *track, u32 command, s32 commandArg1, s32 commandArg2) const;

    u8 ReadByte(byte_t const **ptr) const {
        return *(*ptr)++;
    }
    u16 Read16(byte_t const **ptr) const;
    u32 Read24(byte_t const **ptr) const;
    s32 ReadVar(byte_t const **ptr) const;
    s32 ReadArg(byte_t const **ptr, SeqArgType argType) const;

    // static members
public:
    static int const TEMPO_MAX = 1023;
    static int const TEMPO_MIN = 0;
    static int const CALL_STACK_DEPTH = 3;
    static int const SURROUND_PAN_CENTER;
    static int const PAN_CENTER = 64;

private:
    static bool mPrintVarEnabledFlag;

    // members
private:
    /* vtable */ // size 0x04, offset 0x00
    /* 0x04 */ u8 field_0x04;
    /* 0x08 */ nw4r::snd::SoundArchivePlayer *mpPlayer;
    /* 0x0C */ nw4r::snd::SoundArchive *mpArchive;
    /* 0x10 */ const byte_t *baseAddr;
    /* 0x14 */ dBgmMmlTrack mTracks[16];
}; // size 0x04

#endif
