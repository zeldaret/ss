#ifndef NW4R_SND_MML_PARSER_H
#define NW4R_SND_MML_PARSER_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_MmlSeqTrack.h"

/*******************************************************************************
 * classes and functions
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class Channel; }}}
namespace nw4r { namespace snd { namespace detail { class SeqPlayer; }}}
namespace nw4r { namespace snd { namespace detail { class SeqTrack; }}}

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x31383
	class MmlParser
	{
	// enums
	private:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x31279
		enum SeqArgType
		{
			SEQ_ARG_NONE,

			SEQ_ARG_U8,
			SEQ_ARG_S16,
			SEQ_ARG_VMIDI,
			SEQ_ARG_RANDOM,
			SEQ_ARG_VARIABLE,
		};

		enum MmlCommand
		{
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

		enum MmlExCommand
		{
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

	// methods
	public:
		// virtual function ordering
		// vtable MmlParser
		virtual void CommandProc(MmlSeqTrack *track, u32 command,
		                         s32 commandArg1, s32 commandArg2) const;
		virtual Channel *NoteOnCommandProc(MmlSeqTrack *track, int key,
		                                   int velocity, s32 length,
		                                   bool tieFlag) const;

		// methods
		MmlSeqTrack::ParseResult Parse(MmlSeqTrack *track, bool doNoteOn) const;

		s16 volatile *GetVariablePtr(SeqPlayer *player, SeqTrack *track,
		                             int varNo) const;

		static void EnablePrintVar(bool enable)
		{
			mPrintVarEnabledFlag = enable;
		}

		static u32 ParseAllocTrack(void const *baseAddress, u32 seqOffset,
		                           u32 *allocTrack);

	private:
		u8 ReadByte(byte_t const **ptr) const { return *(*ptr)++; }
		u16 Read16(byte_t const **ptr) const;
		u32 Read24(byte_t const **ptr) const;
		s32 ReadVar(byte_t const **ptr) const;
		s32 ReadArg(byte_t const **ptr, SeqPlayer *player, SeqTrack *track,
		            SeqArgType argType) const;

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
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_MML_PARSER_H
