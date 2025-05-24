#ifndef NW4R_SND_SEQ_TRACK_H
#define NW4R_SND_SEQ_TRACK_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_Channel.h"
#include "nw4r/snd/snd_global.h" // AUX_BUS_NUM
#include "nw4r/snd/snd_Lfo.h" // LfoParam
#include "nw4r/snd/snd_MoveValue.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class SeqPlayer; }}}

namespace nw4r { namespace snd
{
	// why is this not attacked to SeqTrack or something?
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2d764
	enum SeqMute
	{
		MUTE_OFF,
		MUTE_NO_STOP,
		MUTE_RELEASE,
		MUTE_STOP,
	};
}} // namespace nw4r::snd

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2d7c3
	class SeqTrack
	{
	// early constants
	public:
		static int const CALL_STACK_DEPTH = 3;

	// enums
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2d70c
		enum ParseResult
		{
			PARSE_RESULT_CONTINUE,
			PARSE_RESULT_FINISH,
		};

	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2cdd6
		struct CallStack
		{
			bool	loopFlag;		// size 0x01, offset 0x00
			u8		loopCount;		// size 0x01, offset 0x01
			/* 2 bytes padding */
			byte_t	const *address;	// size 0x04, offset 0x04
		}; // size 0x08

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2d032
		struct ParserTrackParam
		{
			byte_t				const *baseAddr;				// size 0x04, offset 0x00
			byte_t				const *currentAddr;				// size 0x04, offset 0x04
			bool				cmpFlag;						// size 0x01, offset 0x08
			bool				noteWaitFlag;					// size 0x01, offset 0x09
			bool				tieFlag;						// size 0x01, offset 0x0a
			bool				monophonicFlag;					// size 0x01, offset 0x0b
			CallStack			callStack[CALL_STACK_DEPTH];	// size 0x18, offset 0x0c
			u8					callStackDepth;					// size 0x01, offset 0x24
			/* 3 bytes padding */
			s32					wait;							// size 0x04, offset 0x28
			bool				muteFlag;						// size 0x01, offset 0x2c
			bool				silenceFlag;					// size 0x01, offset 0x2d
			bool				noteFinishWait;					// size 0x01, offset 0x2e
			bool				portaFlag;						// size 0x01, offset 0x2f
			bool				damperFlag;						// size 0x01, offset 0x30
			/* 3 bytes padding */
			int					bankNo;							// size 0x04, offset 0x34
			int					prgNo;							// size 0x04, offset 0x38
			LfoParam			lfoParam;						// size 0x10, offset 0x3c
			u8					lfoTarget;						// size 0x01, offset 0x4c
			/* 3 bytes padding */
			f32					sweepPitch;						// size 0x04, offset 0x50
			MoveValue<u8, s16>	volume;							// size 0x06, offset 0x54
			MoveValue<s8, s16>	pan;							// size 0x06, offset 0x5a
			MoveValue<s8, s16>	surroundPan;					// size 0x06, offset 0x60
			u8					volume2;						// size 0x01, offset 0x66
			u8					velocityRange;					// size 0x01, offset 0x67
			s8					pitchBend;						// size 0x01, offset 0x68
			u8					bendRange;						// size 0x01, offset 0x69
			s8					initPan;						// size 0x01, offset 0x6a
			s8					transpose;						// size 0x01, offset 0x6b
			u8					priority;						// size 0x01, offset 0x6c
			u8					portaKey;						// size 0x01, offset 0x6d
			u8					portaTime;						// size 0x01, offset 0x6e
			u8					attack;							// size 0x01, offset 0x6f
			u8					decay;							// size 0x01, offset 0x70
			u8					sustain;						// size 0x01, offset 0x71
			u8					release;						// size 0x01, offset 0x72
			/* 1 byte padding */
			s16					envHold;						// size 0x02, offset 0x74
			u8					mainSend;						// size 0x01, offset 0x76
			u8					fxSend[AUX_BUS_NUM];			// size 0x03, offset 0x77
			u8					biquadType;						// size 0x01, offset 0x7a
			/* 1 byte padding */
			f32					lpfFreq;						// size 0x04, offset 0x7c
			f32					biquadValue;					// size 0x04, offset 0x80
		}; // size 0x84

	// methods
	public:
		// cdtors
		SeqTrack();
		virtual ~SeqTrack();

		// virtual function ordering
		// vtable SeqTrack
		virtual ParseResult Parse(bool doNoteOn) = 0;

		// methods
		void InitParam();
		void SetSeqData(void const *seqBase, s32 seqOffset);
		void Open();
		void Close();

		u8 GetPlayerTrackNo() const { return mPlayerTrackNo; }
		bool IsOpened() const { return mOpenFlag; }
		ParserTrackParam &GetParserTrackParam() { return mParserTrackParam; }
		s16 volatile *GetVariablePtr(int varNo);
		SeqPlayer *GetSeqPlayer() { return mSeqPlayer; }
		Channel *GetLastChannel() const { return mChannelList; }

		void SetPlayerTrackNo(int playerTrackNo);
		void SetVolume(f32);
		void SetPitch(f32);
		void SetPan(f32);
		void SetSurroundPan(f32);
		void SetPanRange(f32);
		void SetMute(SeqMute mute);
		void SetSilence(bool, int);
		void SetModDepth(f32);
		void SetModSpeed(f32);
		void SetBiquadFilter(int, f32);
		void SetLpfFreq(f32);
		void SetSeqPlayer(SeqPlayer *player) { mSeqPlayer = player; }

		void AddChannel(Channel *channel);

		void UpdateChannelLength();
		void UpdateChannelParam();
		void UpdateChannelRelease(Channel *channel);

		void ReleaseAllChannel(int release);
		void PauseAllChannel(bool flag);
		void StopAllChannel();
		void FreeAllChannel();

		Channel *NoteOn(int key, int velocity, s32 length, bool tieFlag);

		int ParseNextTick(bool doNoteOn);

		static void ChannelCallbackFunc(Channel *dropChannel,
		                                Channel::ChannelCallbackStatus status,
		                                u32 userData);

	// static members
	public:
		static int const MUTE_RELEASE_VALUE = -1; // maybe?
		static int const PAUSE_RELEASE_VALUE;

		static int const TRACK_VARIABLE_NUM = 16;

		static int const PARSER_PARAM_SIZE; // sizeof(ParserTrackParam) probably

		static int const MAX_ENVELOPE_VALUE = 127;
		static int const INVALID_ENVELOPE = 255;

		static int const DEFAULT_PORTA_KEY = 60;
		static int const DEFAULT_BENDRANGE = 2;
		static int const DEFAULT_PRIORITY = 64;

		/* static int const CALL_STACK_DEPTH = 3; */ // see above

	// members
	private:
		/* vtable */														// size 0x04, offset 0x00
		u8					mPlayerTrackNo;									// size 0x01, offset 0x04
		bool				mOpenFlag;										// size 0x01, offset 0x05
		/* 2 bytes padding */
		f32					mExtVolume;										// size 0x04, offset 0x08
		f32					mExtPitch;										// size 0x04, offset 0x0c
		f32					mExtPan;										// size 0x04, offset 0x10
		f32					mExtSurroundPan;								// size 0x04, offset 0x14
		f32					mPanRange;										// size 0x04, offset 0x18
		ParserTrackParam	mParserTrackParam;								// size 0x84, offset 0x1c
		s16					volatile mTrackVariable[TRACK_VARIABLE_NUM];	// size 0x20, offset 0xa0
		SeqPlayer			*mSeqPlayer;									// size 0x04, offset 0xc0
		Channel				*mChannelList;									// size 0x04, offset 0xc4
	}; // size 0xc8
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_SEQ_TRACK_H
