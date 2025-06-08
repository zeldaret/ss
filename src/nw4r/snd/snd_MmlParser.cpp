#include "nw4r/snd/snd_MmlParser.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_MmlParser.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_Lfo.h" // LfoParam
#include "nw4r/snd/snd_MmlSeqTrack.h"
#include "nw4r/snd/snd_MoveValue.h"
#include "nw4r/snd/snd_SeqPlayer.h"
#include "nw4r/snd/snd_SeqTrack.h"
#include "nw4r/snd/snd_Util.h" // Util::CalcRandom

#include "nw4r/ut/ut_algorithm.h"

#include <rvl/OS/OSError.h>

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * macros
 */

// player/global/track variable limit stuff

// clang-format off
#define AllVarMin_		0

#define PlayerVarMin_	AllVarMin_
#define PlayerVarMax_	(PlayerVarMin_ + SeqPlayer::PLAYER_VARIABLE_NUM)

#define GlobalVarMin_	PlayerVarMax_
#define GlobalVarMax_	(GlobalVarMin_ + SeqPlayer::GLOBAL_VARIABLE_NUM)

#define TrackVarMin_	GlobalVarMax_
#define TrackVarMax_	(TrackVarMin_ + SeqTrack::TRACK_VARIABLE_NUM)

#define AllVarMax_		TrackVarMax_
// clang-format on

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class Channel; }}}

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
	// .sbss
	bool MmlParser::mPrintVarEnabledFlag;
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

MmlSeqTrack::ParseResult MmlParser::Parse(MmlSeqTrack *track,
                                          bool doNoteOn) const
{
	NW4RAssertPointerNonnull_Line(49, track);

	SeqPlayer *player = track->GetSeqPlayer();
	NW4RAssertPointerNonnull_Line(51, player);

	SeqTrack::ParserTrackParam &trackParam = track->GetParserTrackParam();
	SeqPlayer::ParserPlayerParam &playerParam ATTR_UNUSED =
		player->GetParserPlayerParam();

	SeqArgType argType;
	SeqArgType argType2 = SEQ_ARG_NONE;

	bool useArgType = false;
	bool doExecCommand = true;

	u32 cmd = ReadByte(&trackParam.currentAddr);

	if (cmd == MML_EXEC_IF)
	{
		cmd = ReadByte(&trackParam.currentAddr);
		doExecCommand = trackParam.cmpFlag != false;
	}

	if (cmd == MML_ARG_2_S16)
	{
		cmd = ReadByte(&trackParam.currentAddr);
		argType2 = SEQ_ARG_S16;
	}
	else if (cmd == MML_ARG_2_RANDOM)
	{
		cmd = ReadByte(&trackParam.currentAddr);
		argType2 = SEQ_ARG_RANDOM;
	}
	else if (cmd == MML_ARG_2_VARIABLE)
	{
		cmd = ReadByte(&trackParam.currentAddr);
		argType2 = SEQ_ARG_VARIABLE;
	}

	if (cmd == MML_ARG_1_RANDOM)
	{
		cmd = ReadByte(&trackParam.currentAddr);
		argType = SEQ_ARG_RANDOM;
		useArgType = true;
	}
	else if (cmd == MML_ARG_1_VARIABLE)
	{
		cmd = ReadByte(&trackParam.currentAddr);
		argType = SEQ_ARG_VARIABLE;
		useArgType = true;
	}

	if (!(cmd & MML_CMD_MASK))
	{
		// MML note data, not a command
		u8 velocity = ReadByte(&trackParam.currentAddr);

		s32 length = ReadArg(&trackParam.currentAddr, player, track,
		                     useArgType ? argType : SEQ_ARG_VMIDI);

		int key = cmd + trackParam.transpose;

		if (!doExecCommand)
			return MmlSeqTrack::PARSE_RESULT_CONTINUE;

		key = ut::Clamp(key, 0, 127);

		if (!trackParam.muteFlag && doNoteOn)
		{
			NoteOnCommandProc(track, key, velocity, length > 0 ? length : -1,
			                  trackParam.tieFlag);
		}

		if (trackParam.noteWaitFlag)
		{
			trackParam.wait = length;

			if (length == 0)
				trackParam.noteFinishWait = true;
		}
	}
	else
	{
		// MML command
		s32 commandArg1 = 0;
		s32 commandArg2 = 0;

		switch (cmd & MML_CMD_SET_MASK)
		{
		case 0x80:
		{
			switch (cmd)
			{
			case MML_WAIT:
			{
				s32 arg = ReadArg(&trackParam.currentAddr, player, track,
				                   useArgType ? argType : SEQ_ARG_VMIDI);

				if (doExecCommand)
					trackParam.wait = arg;
			}
				break;

			case MML_SET_PRGNO:
				commandArg1 = ReadArg(&trackParam.currentAddr, player, track,
				               useArgType ? argType : SEQ_ARG_VMIDI);

				if (doExecCommand)
					CommandProc(track, cmd, commandArg1, commandArg2);

				break;

			case MML_OPEN_TRACK:
			{
				u8 trackNo = ReadByte(&trackParam.currentAddr);
				u32 offset = Read24(&trackParam.currentAddr);

				if (doExecCommand)
				{
					commandArg1 = trackNo;
					commandArg2 = offset;
					CommandProc(track, cmd, commandArg1, commandArg2);
				}
			}
				break;

			case MML_JUMP:
			{
				u32 offset = Read24(&trackParam.currentAddr);

				if (doExecCommand)
				{
					commandArg1 = offset;
					CommandProc(track, cmd, commandArg1, commandArg2);
				}
			}
				break;

			case MML_CALL:
			{
				u32 offset = Read24(&trackParam.currentAddr);

				if (doExecCommand)
				{
					commandArg1 = offset;
					CommandProc(track, cmd, commandArg1, commandArg2);
				}
			}
				break;
			}

			break;
		}

		case 0xb0:
		case 0xc0:
		case 0xd0:
		{
			u8 arg = ReadArg(&trackParam.currentAddr, player, track,
			                 useArgType ? argType : SEQ_ARG_U8);

			if (argType2 != SEQ_ARG_NONE)
			{
				commandArg2 =
					ReadArg(&trackParam.currentAddr, player, track, argType2);
			}

			if (!doExecCommand)
				break;

			switch (cmd)
			{
			case MML_SET_TRANSPOSE:
			case MML_SET_PITCH_BEND:
				commandArg1 = *reinterpret_cast<s8 *>(&arg);
				break;

			default:
				commandArg1 = *reinterpret_cast<u8 *>(&arg);
				break;
			}

			CommandProc(track, cmd, commandArg1, commandArg2);
			break;
		}

		case 0x90:
			if (doExecCommand)
				CommandProc(track, cmd, commandArg1, commandArg2);

			break;

		case 0xe0:
			commandArg1 =
				static_cast<s16>(ReadArg(&trackParam.currentAddr, player, track,
			                             useArgType ? argType : SEQ_ARG_S16));

			if (doExecCommand)
				CommandProc(track, cmd, commandArg1, commandArg2);

			break;

		case 0xf0:
		{
			switch (cmd)
			{
			case MML_ALLOC_TRACK:
				Read16(&trackParam.currentAddr);
				NW4RPanicMessage_Line(
					312, "seq: must use alloctrack in startup code");

				break;

			case MML_EOF:
				if (doExecCommand)
					return MmlSeqTrack::PARSE_RESULT_FINISH;

				break;

			case MML_EX_COMMAND:
			{
				u32 cmdex = ReadByte(&trackParam.currentAddr);

				switch (cmdex & MML_CMD_SET_MASK)
				{
				case MML_EX_USERPROC:
					commandArg1 = static_cast<u16>(
						ReadArg(&trackParam.currentAddr, player, track,
						        useArgType ? argType : SEQ_ARG_S16));

					if (doExecCommand)
					{
						CommandProc(track, (cmd << 8) + cmdex, commandArg1,
						            commandArg2);
					}

					break;

				case MML_EX_ARITHMETIC:
				case MML_EX_LOGIC:
					commandArg1 = ReadByte(&trackParam.currentAddr);
					commandArg2 = static_cast<s16>(
						ReadArg(&trackParam.currentAddr, player, track,
						        useArgType ? argType : SEQ_ARG_S16));

					if (doExecCommand)
					{
						CommandProc(track, (cmd << 8) + cmdex, commandArg1,
						            commandArg2);
					}

					break;
				}
			}
				ATTR_FALLTHROUGH;

			default:
				if (doExecCommand)
					CommandProc(track, cmd, commandArg1, commandArg2);

				break;
			}
		}
			break;

		case 0xa0:
			NW4RPanicMessage_Line(392, "Invalid seqdata command: %d", cmd);
			break;
		}

	}

	return MmlSeqTrack::PARSE_RESULT_CONTINUE;
}

void MmlParser::CommandProc(MmlSeqTrack *track, u32 command, s32 commandArg1,
                            s32 commandArg2) const
{
	NW4RAssertPointerNonnull_Line(421, track);

	SeqPlayer *player = track->GetSeqPlayer();
	NW4RAssertPointerNonnull_Line(423, player);

	SeqTrack::ParserTrackParam &trackParam = track->GetParserTrackParam();
	SeqPlayer::ParserPlayerParam &playerParam = player->GetParserPlayerParam();

	if (command <= MML_CMD_MAX)
	{
		switch (command)
		{
		case MML_SET_TEMPO:
			playerParam.tempo =
				ut::Clamp<int>(commandArg1, TEMPO_MIN, TEMPO_MAX);
			break;

		case MML_SET_TIMEBASE:
			playerParam.timebase = commandArg1;
			break;

		case MML_SET_PRGNO:
			if (commandArg1 < 0x10000)
			{
				trackParam.prgNo = commandArg1 & 0xffff;
			}
			else
			{
				NW4RWarningMessage_Line(
					449, "nw4r::snd::MmlParser: too large prg No. %d",
					commandArg1);
			}

			break;

		case MML_SET_MUTE:
			track->SetMute(static_cast<SeqMute>(commandArg1));
			break;

		case MML_SET_TRACK_VOLUME:
			trackParam.volume.SetTarget(commandArg1, commandArg2);
			break;

		case MML_SET_TRACK_VOLUME2:
			trackParam.volume2 = commandArg1;
			break;

		case MML_SET_TRACK_VELOCITY_RANGE:
			trackParam.velocityRange = commandArg1;
			break;

		case MML_SET_PLAYER_VOLUME:
			playerParam.volume = commandArg1;
			break;

		case MML_SET_TRANSPOSE:
			trackParam.transpose = commandArg1;
			break;

		case MML_SET_PITCH_BEND:
			trackParam.pitchBend.SetTarget(commandArg1, commandArg2);
			break;

		case MML_SET_BEND_RANGE:
			trackParam.bendRange = commandArg1;
			break;

		case MML_SET_PAN:
			trackParam.pan.SetTarget(commandArg1 - PAN_CENTER, commandArg2);
			break;

		case MML_SET_INIT_PAN:
			trackParam.initPan = commandArg1 - PAN_CENTER;
			break;

		case MML_SET_SURROUND_PAN:
			trackParam.surroundPan.SetTarget(commandArg1, commandArg2);
			break;

		case MML_SET_PRIORITY:
			trackParam.priority = commandArg1;
			break;

		case MML_SET_NOTE_WAIT:
			trackParam.noteWaitFlag = commandArg1;
			break;

		case MML_SET_PORTATIME:
			trackParam.portaTime = commandArg1;
			break;

		case MML_SET_LFO_DEPTH:
			trackParam.lfoParam.depth = static_cast<u8>(commandArg1) / 128.0f;
			break;

		case MML_SET_LFO_SPEED:
			trackParam.lfoParam.speed =
				static_cast<u8>(commandArg1) * (100.0f / 256.0f);
			break;

		case MML_SET_LFO_TARGET:
			trackParam.lfoTarget = commandArg1;
			break;

		case MML_SET_LFO_RANGE:
			trackParam.lfoParam.range = commandArg1;
			break;

		case MML_SET_LFO_DELAY:
			trackParam.lfoParam.delay = commandArg1 * 5;
			break;

		case MML_SET_SWEEP_PITCH:
			trackParam.sweepPitch = commandArg1 / 64.0f;
			break;

		case MML_SET_ATTACK:
			trackParam.attack = commandArg1;
			break;

		case MML_SET_DECAY:
			trackParam.decay = commandArg1;
			break;

		case MML_SET_SUSTAIN:
			trackParam.sustain = commandArg1;
			break;

		case MML_SET_RELEASE:
			trackParam.release = commandArg1;
			break;

		case MML_SET_ENV_HOLD:
			trackParam.envHold = commandArg1 & 0xff;
			break;

		case MML_RESET_ADSR:
			trackParam.attack = 0xff;
			trackParam.decay = 0xff;
			trackParam.sustain = 0xff;
			trackParam.release = 0xff;
			trackParam.envHold = 0xff;

			break;

		case MML_SET_DAMPER:
			trackParam.damperFlag = static_cast<u8>(commandArg1) >= 64;
			break;

		case MML_SET_TIE:
			trackParam.tieFlag = commandArg1;
			track->ReleaseAllChannel(-1);
			track->FreeAllChannel();
			break;

		case MML_SET_MONOPHONIC:
			trackParam.monophonicFlag = commandArg1;

			if (trackParam.monophonicFlag)
			{
				track->ReleaseAllChannel(-1);
				track->FreeAllChannel();
			}

			break;

		case MML_SET_PORTAMENTO:
			trackParam.portaKey = commandArg1 + trackParam.transpose;
			trackParam.portaFlag = true;
			break;

		case MML_SET_PORTASPEED:
			trackParam.portaFlag = commandArg1 != 0;
			break;

		case MML_SET_LPF_FREQ:
			trackParam.lpfFreq = (commandArg1 - 64) / 64.0f;
			break;

		case MML_SET_BIQUAD_TYPE:
			trackParam.biquadType = commandArg1;
			break;

		case MML_SET_BIQUAD_VALUE:
			trackParam.biquadValue = commandArg1 / 127.0f;
			break;

		case MML_SET_FX_SEND_A:
			trackParam.fxSend[AUX_A] = commandArg1;
			break;

		case MML_SET_FX_SEND_B:
			trackParam.fxSend[AUX_B] = commandArg1;
			break;

		case MML_SET_FX_SEND_C:
			trackParam.fxSend[AUX_C] = commandArg1;
			break;

		case MML_SET_MAIN_SEND:
			trackParam.mainSend = commandArg1;
			break;

		case MML_PRINT_VAR:
			if (mPrintVarEnabledFlag)
			{
				s16 const volatile * const varPtr =
					GetVariablePtr(player, track, commandArg1);

#define GetVarType_(varNo_)	\
	((varNo_) >= TrackVarMin_ ? "T" : (varNo_) >= GlobalVarMin_ ? "G" : "")

#define GetAdjustedVarNo_(varNo_)							\
	((varNo_) >= TrackVarMin_    ? (varNo_) - TrackVarMin_	\
	 : (varNo_) >= GlobalVarMin_ ? (varNo_) - GlobalVarMin_	\
	                             : (varNo_))

				OSReport("#%08x[%d]: printvar %sVAR_%d(%d) = %d\n", player,
				         track->GetPlayerTrackNo(), GetVarType_(commandArg1),
				         GetAdjustedVarNo_(commandArg1), commandArg1, *varPtr);

#undef GetVarType_
#undef GetAdjustedVarNo_
			}

			break;

		case MML_OPEN_TRACK:
		{
			SeqTrack *newTrack = player->GetPlayerTrack(commandArg1);

			if (!newTrack)
			{
				NW4RWarningMessage_Line(
					644,
					"nw4r::snd::MmlParser: opentrack for not allocated track");
				break;
			}

			if (newTrack == track)
			{
				NW4RWarningMessage_Line(
					649, "nw4r::snd::MmlParser: opentrack for self track");
				break;
			}

			newTrack->Close();
			newTrack->SetSeqData(trackParam.baseAddr, commandArg2);
			newTrack->Open();
		}
			break;

		case MML_JUMP:
			trackParam.currentAddr = trackParam.baseAddr + commandArg1;
			break;

		case MML_CALL:
		{
			if (trackParam.callStackDepth >= CALL_STACK_DEPTH)
			{
				NW4RWarningMessage_Line(665,
				                        "nw4r::snd::MmlParser: cannot \'call\' "
				                        "because already too deep");
				break;
			}

			SeqTrack::CallStack *callStack =
				&trackParam.callStack[trackParam.callStackDepth];

			callStack->address = trackParam.currentAddr;
			callStack->loopFlag = false;

			trackParam.callStackDepth++;
			trackParam.currentAddr = trackParam.baseAddr + commandArg1;
			break;
		}

		case MML_RET:
		{
			SeqTrack::CallStack *callStack = nullptr;

			while (trackParam.callStackDepth)
			{
				trackParam.callStackDepth--;

				if (!trackParam.callStack[trackParam.callStackDepth].loopFlag)
				{
					callStack = &trackParam.callStack[trackParam.callStackDepth];
					break;
				}
			}

			if (!callStack)
			{
				NW4RWarningMessage_Line(
					688,
					"nw4r::snd::MmlParser: unmatched sequence command \'ret\'");

				break;
			}

			trackParam.currentAddr = callStack->address;
		}
			break;

		case MML_LOOP_START:
		{
			if (trackParam.callStackDepth >= CALL_STACK_DEPTH)
			{
				NW4RWarningMessage_Line(
					698, "nw4r::snd::MmlParser: cannot \'loop_start\' because "
				         "already too deep");

				break;
			}

			SeqTrack::CallStack *callStack =
				&trackParam.callStack[trackParam.callStackDepth];

			callStack->address = trackParam.currentAddr;
			callStack->loopCount = commandArg1;
			callStack->loopFlag = true;

			trackParam.callStackDepth++;
		}
			break;

		case MML_LOOP_END:
		{
			if (trackParam.callStackDepth == 0)
			{
				NW4RWarningMessage_Line(713, "nw4r::snd::MmlParser: unmatched "
				                             "sequence command \'loop_end\'");
				break;
			}

			SeqTrack::CallStack *callStack =
				&trackParam.callStack[trackParam.callStackDepth - 1];

			if (!callStack->loopFlag)
			{
				NW4RWarningMessage_Line(719, "nw4r::snd::MmlParser: unmatched "
				                             "sequence command \'loop_end\'");
				break;
			}

			u8 loop_count = callStack->loopCount;

			if (loop_count && --loop_count == 0)
			{
				trackParam.callStackDepth--;
			}
			else
			{
				callStack->loopCount = loop_count;

				trackParam.currentAddr = callStack->address;
			}
		}
			break;
		}
	}
	else if (command <= MML_EX_CMD_MAX)
	{
		u32 cmd = command >> 8;
		u32 cmdex = command & 0xff;

		NW4RAssert_Line(742, cmd == MML_EX_COMMAND);

		s16 volatile *varPtr = nullptr;

		if ((cmdex & 0xf0) == MML_EX_ARITHMETIC
		    || (cmdex & 0xf0) == MML_EX_LOGIC)
		{
			varPtr = GetVariablePtr(player, track, commandArg1);
			if (!varPtr)
				return;
		}

		switch (cmdex)
		{
		case MML_EX_SET:
			*varPtr = commandArg2;
			break;

		case MML_EX_APL:
			*varPtr += commandArg2;
			break;

		case MML_EX_AMI:
			*varPtr -= commandArg2;
			break;

		case MML_EX_AMU:
			*varPtr *= commandArg2;
			break;

		case MML_EX_ADV:
			if (commandArg2 != 0)
				*varPtr /= commandArg2;

			break;

		case MML_EX_ALS:
			if (commandArg2 >= 0)
				*varPtr <<= commandArg2;
			else
				*varPtr >>= -commandArg2;

			break;

		case MML_EX_RND:
		{
			bool minus_flag = false;

			if (commandArg2 < 0)
			{
				minus_flag = true;
				commandArg2 = static_cast<s16>(-commandArg2);
			}

			s32 rand = Util::CalcRandom();
			rand *= commandArg2 + 1;
			rand >>= 16;

			if (minus_flag)
				rand = -rand;

			*varPtr = rand;
			break;
		}

		case MML_EX_AAD:
			*varPtr &= commandArg2;
			break;

		case MML_EX_AOR:
			*varPtr |= commandArg2;
			break;

		case MML_EX_AER:
			*varPtr ^= commandArg2;
			break;

		case MML_EX_ACO:
			*varPtr = ~static_cast<u16>(commandArg2);
			break;

		case MML_EX_AMD:
			if (commandArg2 != 0)
				*varPtr %= commandArg2;

			break;

		case MML_EX_EQ:
			trackParam.cmpFlag = *varPtr == commandArg2;
			break;

		case MML_EX_GE:
			trackParam.cmpFlag = *varPtr >= commandArg2;
			break;

		case MML_EX_GT:
			trackParam.cmpFlag = *varPtr > commandArg2;
			break;

		case MML_EX_LE:
			trackParam.cmpFlag = *varPtr <= commandArg2;
			break;

		case MML_EX_LT:
			trackParam.cmpFlag = *varPtr < commandArg2;
			break;

		case MML_EX_NE:
			trackParam.cmpFlag = *varPtr != commandArg2;
			break;

		case MML_EX_USERPROC:
			player->CallSeqUserprocCallback(commandArg1, track);
			break;
		}
	}
}

Channel *MmlParser::NoteOnCommandProc(MmlSeqTrack *track, int key, int velocity,
                                      s32 length, bool tieFlag) const
{
	return track->NoteOn(key, velocity, length, tieFlag);
}

u16 MmlParser::Read16(byte_t const **ptr) const
{
	u16 ret = ReadByte(ptr);

	ret <<= 8;
	ret |= ReadByte(ptr);

	return ret;
}

u32 MmlParser::Read24(byte_t const **ptr) const
{
	u32 ret = ReadByte(ptr);

	ret <<= 8;
	ret |= ReadByte(ptr);

	ret <<= 8;
	ret |= ReadByte(ptr);

	return ret;
}

s32 MmlParser::ReadVar(byte_t const **ptr) const
{
	s32 ret = 0;
	byte_t b;

	for (int i = 0;; i++)
	{
		NW4RAssert_Line(940, i < 4);

		b = ReadByte(ptr);
		ret <<= 7;
		ret |= b & 0x7f;

		if (!(b & 0x80))
			break;
	}

	return ret;
}

s32 MmlParser::ReadArg(byte_t const **ptr, SeqPlayer *player, SeqTrack *track,
                       SeqArgType argType) const
{
	s32 var;

	switch (argType)
	{
	case SEQ_ARG_U8:
		var = ReadByte(ptr);
		break;

	case SEQ_ARG_S16:
		var = Read16(ptr);
		break;

	case SEQ_ARG_VMIDI:
		var = ReadVar(ptr);
		break;

	case SEQ_ARG_VARIABLE:
	{
		u8 varNo = ReadByte(ptr);

		s16 const volatile *varPtr = GetVariablePtr(player, track, varNo);

		// ERRATUM: if varPtr is not valid then ReadArg returns garbage
		if (varPtr)
			var = *varPtr;
	}
		break;

	case SEQ_ARG_RANDOM:
	{
		s32 rand;

		s16 min = Read16(ptr);
		s16 max = Read16(ptr);

		rand = Util::CalcRandom();
		rand *= max - min + 1;
		rand >>= 16;
		rand += min;

		var = rand;
	}
		break;
	}

	return var;
}

s16 volatile *MmlParser::GetVariablePtr(SeqPlayer *player, SeqTrack *track,
                                        int varNo) const
{
	NW4RAssertHeaderClampedLRValue_Line(1014, varNo, AllVarMin_, AllVarMax_);

	if (varNo < GlobalVarMax_)
		return player->GetVariablePtr(varNo);

	if (varNo < TrackVarMax_)
		return track->GetVariablePtr(varNo - TrackVarMin_);

	return nullptr;
}

u32 MmlParser::ParseAllocTrack(void const *baseAddress, u32 seqOffset,
                               u32 *allocTrack)
{
	NW4RAssertPointerNonnull_Line(1051, baseAddress);
	NW4RAssertPointerNonnull_Line(1052, allocTrack);

	byte_t const *ptr =
		static_cast<byte_t const *>(ut::AddOffsetToPtr(baseAddress, seqOffset));

	if (*ptr != MML_ALLOC_TRACK)
	{
		*allocTrack = 1;
		return seqOffset;
	}
	else
	{
		u32 tracks = *++ptr;

		tracks <<= 8;
		tracks |= *++ptr;

		*allocTrack = tracks;
		return seqOffset + 3;
	}
}

}}} // namespace nw4r::snd::detail
