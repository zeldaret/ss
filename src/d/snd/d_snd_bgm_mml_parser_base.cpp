#include "d/snd/d_snd_bgm_mml_parser_base.h"

#include "common.h"
#include "nw4r/snd/snd_SeqFile.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"

dSndBgmMmlParser_c::dSndBgmMmlParser_c(nw4r::snd::SoundArchivePlayer *player, nw4r::snd::SoundArchive *archive)
    : field_0x04(0), mpPlayer(player), mpArchive(archive) {}

void dSndBgmMmlParser_c::initTrack(s32 trackNo, u32 offset) {
    dBgmMmlTrack &trackParam = mTracks[trackNo];
    trackParam.currentAddr = baseAddr + offset;
    trackParam.mTrackNo = trackNo;
    trackParam.mTrackUsed = true;
    trackParam.cmpFlag = 0;
    trackParam.wait = 0;
    trackParam.callStackDepth = 0;
}

bool dSndBgmMmlParser_c::loadAndParse(u32 soundId, u32 trackMask, bool noJumps) {
    if (!readTracks(soundId, trackMask, noJumps)) {
        return false;
    }

    for (int i = 0; i < 16; i++) {
        if (mTracks[i].mTrackUsed) {
            while (Parse(&mTracks[i]) != PARSE_RESULT_FINISH) {}
        }
    }
    return true;
}

bool dSndBgmMmlParser_c::readTracks(u32 soundId, u32 trackMask, bool noJumps) {
    nw4r::snd::SoundArchivePlayer *player = mpPlayer;
    nw4r::snd::SoundArchive *archive = mpArchive;

    field_0x04 = noJumps;
    nw4r::snd::SoundArchive::SoundInfo info;
    if (!archive->ReadSoundInfo(soundId, &info)) {
        return false;
    }

    if (archive->GetSoundType(soundId) != nw4r::snd::SoundArchive::SOUND_TYPE_SEQ) {
        return false;
    }

    const void *addr = player->detail_GetFileAddress(info.fileId);
    if (addr == nullptr) {
        return false;
    }

    nw4r::snd::detail::SeqFileReader reader(addr);
    baseAddr = (const byte_t *)reader.GetBaseAddress();
    for (int i = 0; i < 16; i++) {
        mTracks[i].mTrackUsed = false;
    }
    initTrack(0, 0);

    const byte_t *ptr = baseAddr;
    bool endReached = false;

    for (int i = 0; i < 20; i++) {
        u32 cmd = ReadByte(&ptr);
        switch (cmd) {
            case MML_ALLOC_TRACK:
                Read16(&ptr);
                break;
            case MML_OPEN_TRACK: {
                u8 trackNo = ReadByte(&ptr);
                u32 offset = Read24(&ptr);
                if (((trackMask >> trackNo) & 1) != 0) {
                    initTrack(trackNo, offset);
                }
                break;
            }
            case MML_SET_TIMEBASE:
                ptr += 1;
                break;
            default:
                endReached = true;
                break;
        }

        if (endReached) {
            mTracks[0].currentAddr = ptr - 1;
            break;
        }
    }

    if (!endReached) {
        return false;
    }

    if ((trackMask & 1) == 0) {
        mTracks[0].mTrackUsed = false;
    }
    return true;
}

// clang-format off

// modified from nw4r::snd - some changes I had to make to make it match might be fake
dSndBgmMmlParser_c::ParseResult dSndBgmMmlParser_c::Parse(dBgmMmlTrack *track) const {
	dBgmMmlTrack &trackParam = *track;

	// Had to move these up for regswaps - maybe fake
	s32 commandArg1;
	s32 commandArg2;

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

		s32 length = ReadArg(&trackParam.currentAddr,
		                     useArgType ? argType : SEQ_ARG_VMIDI);

		if (!doExecCommand) 
			return PARSE_RESULT_CONTINUE;

		int key = cmd;

		NoteOnCommandProc(trackParam.mTrackNo, trackParam.wait, key, velocity,
		           length > 0 ? length : -1);
	}
	else
	{
		// MML command
		commandArg1 = 0;
		commandArg2 = 0;

		switch (cmd & MML_CMD_SET_MASK)
		{
		case 0x80:
		{
			switch (cmd)
			{
			case MML_WAIT:
			{
				s32 arg = ReadArg(&trackParam.currentAddr,
				                   useArgType ? argType : SEQ_ARG_VMIDI);

				if (doExecCommand)
					trackParam.wait += arg;
			}
				break;

			case MML_SET_PRGNO:
				commandArg1 = ReadArg(&trackParam.currentAddr,
				               useArgType ? argType : SEQ_ARG_VMIDI);

				if (doExecCommand)
					CommandProc_(track, cmd, commandArg1, commandArg2);

				break;

			case MML_OPEN_TRACK:
			{
				// Had to change u8 -> u32 for regswaps - maybe fake
				u32 trackNo = ReadByte(&trackParam.currentAddr);
				u32 offset = Read24(&trackParam.currentAddr);

				if (doExecCommand)
				{
					commandArg1 = trackNo;
					commandArg2 = offset;
					CommandProc_(track, cmd, commandArg1, commandArg2);
				}
			}
				break;

			case MML_JUMP:
			case MML_CALL:
			{
				u32 offset = Read24(&trackParam.currentAddr);

				if (doExecCommand)
				{
					commandArg1 = offset;
					CommandProc_(track, cmd, commandArg1, commandArg2);
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
			u8 arg = ReadArg(&trackParam.currentAddr,
			                 useArgType ? argType : SEQ_ARG_U8);

			if (argType2 != SEQ_ARG_NONE)
			{
				commandArg2 =
					ReadArg(&trackParam.currentAddr, argType2);
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

			CommandProc_(track, cmd, commandArg1, commandArg2);
			break;
		}

		case 0x90:
			if (doExecCommand)
				CommandProc_(track, cmd, commandArg1, commandArg2);

			break;

		case 0xe0:
			commandArg1 =
				static_cast<s16>(ReadArg(&trackParam.currentAddr, 
			                             useArgType ? argType : SEQ_ARG_S16));

			if (doExecCommand)
				CommandProc_(track, cmd, commandArg1, commandArg2);

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
					return PARSE_RESULT_FINISH;

				break;

			case MML_EX_COMMAND:
			{
				u32 cmdex = ReadByte(&trackParam.currentAddr);

				switch (cmdex & MML_CMD_SET_MASK)
				{
				case MML_EX_USERPROC:
					commandArg1 = static_cast<u16>(
						ReadArg(&trackParam.currentAddr,
						        useArgType ? argType : SEQ_ARG_S16));

					if (doExecCommand)
					{
						CommandProc_(track, (cmd << 8) + cmdex, commandArg1,
						            commandArg2);
					}

					break;

				case MML_EX_ARITHMETIC:
				case MML_EX_LOGIC:
					commandArg1 = ReadByte(&trackParam.currentAddr);
					commandArg2 = static_cast<s16>(
						ReadArg(&trackParam.currentAddr,
						        useArgType ? argType : SEQ_ARG_S16));

					if (doExecCommand)
					{
						CommandProc_(track, (cmd << 8) + cmdex, commandArg1,
						            commandArg2);
					}

					break;
				}
			}
				ATTR_FALLTHROUGH;

			default:
				if (doExecCommand)
					CommandProc_(track, cmd, commandArg1, commandArg2);

				break;
			}
		}
			break;

		case 0xa0:
			NW4RPanicMessage_Line(392, "Invalid seqdata command: %d", cmd);
			break;
		}
	}

	return PARSE_RESULT_CONTINUE;
}

// very stripped down from nw4r::snd
void dSndBgmMmlParser_c::CommandProc_(dBgmMmlTrack *track, u32 command, s32 commandArg1, s32 commandArg2) const {
	dBgmMmlTrack &trackParam = *track;

	switch (command)
	{
	case MML_JUMP:
		if (field_0x04 == 0)
			trackParam.currentAddr = baseAddr + commandArg1;
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

		dBgmMmlCallStack *callStack =
			&trackParam.callStack[trackParam.callStackDepth];

		callStack->address = trackParam.currentAddr;
		callStack->loopFlag = false;

		trackParam.callStackDepth++;
		trackParam.currentAddr = baseAddr + commandArg1;
		break;
	}

	case MML_RET:
	{
		dBgmMmlCallStack *tmp; // needed for lbzu
		dBgmMmlCallStack *callStack = nullptr;

		while (trackParam.callStackDepth > 0)
		{
			tmp = &trackParam.callStack[--trackParam.callStackDepth];

			if (!tmp->loopFlag)
			{
				callStack = tmp;
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

		dBgmMmlCallStack *callStack =
			&trackParam.callStack[trackParam.callStackDepth];

		callStack->address = trackParam.currentAddr;
		callStack->loopFlag = true;
		callStack->loopCount = commandArg1;

		trackParam.callStackDepth++;
	}
		break;

	case MML_LOOP_END:
	{
		if (trackParam.callStackDepth <= 0)
		{
			NW4RWarningMessage_Line(713, "nw4r::snd::MmlParser: unmatched "
											"sequence command \'loop_end\'");
			break;
		}

		dBgmMmlCallStack *callStack =
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

	CommandProc(track->mTrackNo, track->wait, command, commandArg1, commandArg2);
}

// copied from nw4r::snd start
u16 dSndBgmMmlParser_c::Read16(byte_t const **ptr) const
{
	u16 ret = ReadByte(ptr);

	ret <<= 8;
	ret |= ReadByte(ptr);

	return ret;
}

u32 dSndBgmMmlParser_c::Read24(byte_t const **ptr) const
{
	u32 ret = ReadByte(ptr);

	ret <<= 8;
	ret |= ReadByte(ptr);

	ret <<= 8;
	ret |= ReadByte(ptr);

	return ret;
}

s32 dSndBgmMmlParser_c::ReadVar(byte_t const **ptr) const
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

s32 dSndBgmMmlParser_c::ReadArg(byte_t const **ptr,
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
		var = ReadByte(ptr);
		/*
		u8 varNo = ReadByte(ptr);

		s16 const volatile *varPtr = GetVariablePtr(player, track, varNo);

		// ERRATUM: if varPtr is not valid then ReadArg returns garbage
		if (varPtr)
			var = *varPtr;
		*/
	}
		break;

	case SEQ_ARG_RANDOM:
	{
		s32 rand;

		s16 min = Read16(ptr);
		s16 max = Read16(ptr);

		rand = nw4r::snd::detail::Util::CalcRandom();
		rand *= max - min + 1;
		rand >>= 16;
		rand += min;

		var = rand;
	}
		break;
	}

	return var;
}
// clang-format on

void dSndBgmMmlParser_c::CommandProc(u32 trackNo, int wait, u32 command, s32 commandArg1, s32 commandArg2) const {
    // noop
}

void dSndBgmMmlParser_c::NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const {
    // noop
}
