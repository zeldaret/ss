#include "nw4r/snd/snd_MmlParser.h"
#include "nw4r/snd/snd_MmlSeqTrack.h"
#include "nw4r/snd/snd_SeqPlayer.h"
#include "nw4r/snd/snd_Util.h"

namespace nw4r {
namespace snd {
namespace detail {

bool MmlParser::mPrintVarEnabledFlag = false;

ParseResult MmlParser::Parse(MmlSeqTrack* pTrack, bool doNoteOn) const {
    SeqPlayer* pPlayer = pTrack->GetSeqPlayer();

    SeqTrack::ParserTrackParam& rTrackParam = pTrack->GetParserTrackParam();

    SeqPlayer::ParserPlayerParam& rPlayerParam =
        pPlayer->GetParserPlayerParam();

    SeqArgType argType;
    bool useArgType = false;
    bool doExecCommand = true;

    u32 cmd = ReadByte(&rTrackParam.currentAddr);

    if (cmd == MML_EXECIF) {
        cmd = ReadByte(&rTrackParam.currentAddr);
        doExecCommand = rTrackParam.cmpFlag != false;
    }

    if (cmd == MML_RNDPARAM) {
        cmd = ReadByte(&rTrackParam.currentAddr);
        argType = SEQ_ARG_RANDOM;
        useArgType = true;
    }

    if (cmd == MML_INDPARAM) {
        cmd = ReadByte(&rTrackParam.currentAddr);
        argType = SEQ_ARG_VARIABLE;
        useArgType = true;
    }

    // MML note data, not a command
    if (!(cmd & MML_CMD_MASK)) {
        u8 velocity = ReadByte(&rTrackParam.currentAddr);

        s32 length = ReadArg(&rTrackParam.currentAddr, pPlayer, pTrack,
                             useArgType ? argType : SEQ_ARG_VMIDI);

        int key = cmd + rTrackParam.transpose;

        if (!doExecCommand) {
            return PARSE_RESULT_CONTINUE;
        }

        key = ut::Clamp(key, 0, 127);

        if (!rTrackParam.muteFlag && doNoteOn) {
            NoteOnCommandProc(pTrack, key, velocity, length > 0 ? length : -1,
                              rTrackParam.tieFlag);
        }

        if (rTrackParam.noteWaitFlag) {
            rTrackParam.wait = length;

            if (length == 0) {
                rTrackParam.noteFinishWait = true;
            }
        }
    }
    // MML command
    else {
        s32 arg1 = 0;
        s32 arg2 = 0;

        switch (cmd & MML_CMD_SET_MASK) {
        case 0x80: {
            switch (cmd) {
            case MML_WAIT: {
                s32 time = ReadArg(&rTrackParam.currentAddr, pPlayer, pTrack,
                                   useArgType ? argType : SEQ_ARG_VMIDI);

                if (doExecCommand) {
                    rTrackParam.wait = time;
                }
                break;
            }

            case MML_SET_PRGNO: {
                arg1 = ReadArg(&rTrackParam.currentAddr, pPlayer, pTrack,
                               useArgType ? argType : SEQ_ARG_VMIDI);

                if (doExecCommand) {
                    CommandProc(pTrack, cmd, arg1, arg2);
                }
                break;
            }

            case MML_OPENTRACK: {
                u8 trackNo = ReadByte(&rTrackParam.currentAddr);
                u32 offset = Read24(&rTrackParam.currentAddr);

                if (doExecCommand) {
                    arg1 = trackNo;
                    arg2 = offset;
                    CommandProc(pTrack, cmd, arg1, arg2);
                }
                break;
            }

            case MML_JUMP: {
                u32 offset = Read24(&rTrackParam.currentAddr);

                if (doExecCommand) {
                    arg1 = offset;
                    CommandProc(pTrack, cmd, arg1, arg2);
                }
                break;
            }

            case MML_CALL: {
                u32 offset = Read24(&rTrackParam.currentAddr);

                if (doExecCommand) {
                    arg1 = offset;
                    CommandProc(pTrack, cmd, arg1, arg2);
                }
                break;
            }
            }

            break;
        }

        case 0xB0:
        case 0xC0:
        case 0xD0: {
            u8 arg = ReadArg(&rTrackParam.currentAddr, pPlayer, pTrack,
                             useArgType ? argType : SEQ_ARG_U8);

            if (!doExecCommand) {
                break;
            }

            switch (cmd) {
            case MML_SET_TRANSPOSE:
            case MML_SET_PITCHBEND: {
                arg1 = *reinterpret_cast<s8*>(&arg);
                break;
            }

            default: {
                arg1 = arg;
                break;
            }
            }

            CommandProc(pTrack, cmd, arg1, arg2);
            break;
        }

        case 0x90: {
            if (doExecCommand) {
                CommandProc(pTrack, cmd, arg1, arg2);
            }
            break;
        }

        case 0xE0: {
            arg1 = static_cast<s16>(
                ReadArg(&rTrackParam.currentAddr, pPlayer, pTrack,
                        useArgType ? argType : SEQ_ARG_S16));

            if (doExecCommand) {
                CommandProc(pTrack, cmd, arg1, arg2);
            }
            break;
        }

        case 0xF0: {
            switch (cmd) {
            case MML_ALLOCTRACK: {
                // Skip command argument ("must use alloctrack in startup code")
                (void)Read16(&rTrackParam.currentAddr);
                break;
            }

            case MML_EOF: {
                return PARSE_RESULT_FINISH;
            }

            case MML_EX_COMMAND: {
                u32 cmdex = ReadByte(&rTrackParam.currentAddr);

                switch (cmdex & MML_CMD_SET_MASK) {
                case MML_EX_USERPROC: {
                    arg1 = static_cast<s16>(
                        ReadArg(&rTrackParam.currentAddr, pPlayer, pTrack,
                                useArgType ? argType : SEQ_ARG_S16));

                    if (doExecCommand) {
                        CommandProc(pTrack, (cmd << 8) + cmdex, arg1, arg2);
                    }
                    break;
                }

                case MML_EX_ARITHMETIC:
                case MML_EX_LOGIC: {
                    arg1 = ReadByte(&rTrackParam.currentAddr);
                    arg2 = static_cast<s16>(
                        ReadArg(&rTrackParam.currentAddr, pPlayer, pTrack,
                                useArgType ? argType : SEQ_ARG_S16));

                    if (doExecCommand) {
                        CommandProc(pTrack, (cmd << 8) + cmdex, arg1, arg2);
                    }
                    break;
                }
                }

                // FALLTHROUGH (assume no arguments?)
            }

            default: {
                if (doExecCommand) {
                    CommandProc(pTrack, cmd, arg1, arg2);
                }
                break;
            }
            }

            break;
        }
        }
    }

    return PARSE_RESULT_CONTINUE;
}

void MmlParser::CommandProc(MmlSeqTrack* pTrack, u32 command, s32 arg1,
                            s32 arg2) const {
    SeqPlayer* pPlayer = pTrack->GetSeqPlayer();

    SeqTrack::ParserTrackParam& rTrackParam = pTrack->GetParserTrackParam();

    SeqPlayer::ParserPlayerParam& rPlayerParam =
        pPlayer->GetParserPlayerParam();

    if (command <= MML_CMD_MAX) {
        switch (command) {
        case MML_SET_TEMPO: {
            rPlayerParam.tempo = arg1;
            break;
        }
        case MML_SET_TIMEBASE: {
            rPlayerParam.timebase = arg1;
            break;
        }

        case MML_SET_PRGNO: {
            if (arg1 < SeqTrack::PRGNO_MAX + 1) {
                rTrackParam.prgNo = arg1 & SeqTrack::PRGNO_MAX;
            }
            break;
        }

        case MML_SET_MUTE: {
            pTrack->SetMute(static_cast<SeqMute>(arg1));
            break;
        }

        case MML_SET_TRACK_VOLUME: {
            rTrackParam.volume.InitValue(arg1);
            break;
        }
        case MML_SET_TRACK_VOLUME2: {
            rTrackParam.volume2 = arg1;
            break;
        }
        case MML_SET_PLAYER_VOLUME: {
            rPlayerParam.volume = arg1;
            break;
        }

        case MML_SET_TRANSPOSE: {
            rTrackParam.transpose = arg1;
            break;
        }

        case MML_SET_PITCHBEND: {
            rTrackParam.pitchBend.InitValue(arg1);
            break;
        }
        case MML_SET_BENDRANGE: {
            rTrackParam.bendRange = arg1;
            break;
        }
        case MML_SET_PAN: {
            rTrackParam.pan.InitValue(arg1 - 64);
            break;
        }
        case MML_SET_INITPAN: {
            rTrackParam.initPan = arg1 - 64;
            break;
        }
        case MML_SET_SURROUNDPAN: {
            rTrackParam.surroundPan.InitValue(arg1);
            break;
        }
        case MML_SET_PRIORITY: {
            rTrackParam.priority = arg1;
            break;
        }

        case MML_SET_NOTEWAIT: {
            rTrackParam.noteWaitFlag = arg1;
            break;
        }

        case MML_SET_PORTATIME: {
            rTrackParam.portaTime = arg1;
            break;
        }

        case MML_SET_LFODEPTH: {
            rTrackParam.lfoParam.depth = static_cast<u8>(arg1) / 128.0f;
            break;
        }
        case MML_SET_LFOSPEED: {
            rTrackParam.lfoParam.speed =
                static_cast<u8>(arg1) * (100.0f / 256.0f);
            break;
        }
        case MML_SET_LFOTARGET: {
            rTrackParam.lfoTarget = arg1;
            break;
        }
        case MML_SET_LFORANGE: {
            rTrackParam.lfoParam.range = arg1;
            break;
        }
        case MML_SET_LFODELAY: {
            rTrackParam.lfoParam.delay = arg1 * 5;
            break;
        }

        case MML_SET_SWEEPPITCH: {
            rTrackParam.sweepPitch = arg1 / 32.0f;
            break;
        }

        case MML_SET_ATTACK: {
            rTrackParam.attack = arg1;
            break;
        }
        case MML_SET_DECAY: {
            rTrackParam.decay = arg1;
            break;
        }
        case MML_SET_SUSTAIN: {
            rTrackParam.sustain = arg1;
            break;
        }
        case MML_SET_RELEASE: {
            rTrackParam.release = arg1;
            break;
        }

        case MML_SET_DAMPER: {
            rTrackParam.damperFlag = static_cast<u8>(arg1) >= 64;
            break;
        }

        case MML_SET_TIE: {
            rTrackParam.tieFlag = arg1;
            pTrack->ReleaseAllChannel(-1);
            pTrack->FreeAllChannel();
            break;
        }

        case MML_SET_PORTAMENTO: {
            rTrackParam.portaKey = arg1 + rTrackParam.transpose;
            rTrackParam.portaFlag = true;
            break;
        }
        case MML_SET_PORTASPEED: {
            rTrackParam.portaFlag = arg1 != 0;
            break;
        }
        case MML_SET_LPFFREQ: {
            rTrackParam.lpfFreq = arg1;
            break;
        }

        case MML_SET_FXSEND_A: {
            rTrackParam.fxSend[AUX_A] = arg1;
            break;
        }
        case MML_SET_FXSEND_B: {
            rTrackParam.fxSend[AUX_B] = arg1;
            break;
        }
        case MML_SET_FXSEND_C: {
            rTrackParam.fxSend[AUX_C] = arg1;
            break;
        }
        case MML_SET_MAINSEND: {
            rTrackParam.mainSend = arg1;
            break;
        }

        case MML_PRINTVAR: {
            if (!mPrintVarEnabledFlag) {
                break;
            }

            const volatile s16* pVar = GetVariablePtr(pPlayer, pTrack, arg1);

            // clang-format off

            // Convert absolute index into variable type-relative index
            int varNo = arg1 >= SeqPlayer::VARIABLE_NUM       ? arg1 - SeqPlayer::VARIABLE_NUM
                     : (arg1 >= SeqPlayer::LOCAL_VARIABLE_NUM ? arg1 - SeqPlayer::LOCAL_VARIABLE_NUM : arg1);

            // Determine variable type prefix from absolute index
            // 'T' = Track variable, 'G' = Global (player) variable
            const char* pVarType = arg1 >= SeqPlayer::VARIABLE_NUM       ? "T"
                                : (arg1 >= SeqPlayer::LOCAL_VARIABLE_NUM ? "G" : "");

            // clang-format on

            OSReport("#%08x[%d]: printvar %sVAR_%d(%d) = %d\n", pPlayer,
                     pTrack->GetPlayerTrackNo(), pVarType, varNo, arg1, *pVar);
            break;
        }

        case MML_OPENTRACK: {
            SeqTrack* pNewTrack = pPlayer->GetPlayerTrack(arg1);

            if (pNewTrack != NULL && pNewTrack != pTrack) {
                pNewTrack->Close();
                pNewTrack->SetSeqData(rTrackParam.baseAddr, arg2);
                pNewTrack->Open();
            }
            break;
        }

        case MML_JUMP: {
            rTrackParam.currentAddr = rTrackParam.baseAddr + arg1;
            break;
        }

        case MML_CALL: {
            if (rTrackParam.callStackDepth >= CALL_STACK_MAX_DEPTH) {
                break;
            }

            rTrackParam.callStack[rTrackParam.callStackDepth].address =
                rTrackParam.currentAddr;

            rTrackParam.callStackDepth++;
            rTrackParam.currentAddr = rTrackParam.baseAddr + arg1;
            break;
        }

        case MML_RET: {
            if (rTrackParam.callStackDepth == 0) {
                break;
            }

            rTrackParam.callStackDepth--;

            rTrackParam.currentAddr =
                rTrackParam.callStack[rTrackParam.callStackDepth].address;
            break;
        }

        case MML_LOOP_START: {
            if (rTrackParam.callStackDepth >= CALL_STACK_MAX_DEPTH) {
                break;
            }

            rTrackParam.callStack[rTrackParam.callStackDepth].address =
                rTrackParam.currentAddr;

            rTrackParam.callStack[rTrackParam.callStackDepth].loopCount = arg1;
            rTrackParam.callStackDepth++;
            break;
        }

        case MML_LOOP_END: {
            if (rTrackParam.callStackDepth == 0) {
                break;
            }

            u8 count = rTrackParam.callStack[rTrackParam.callStackDepth - 1].loopCount;

            if (count != 0 && --count == 0) {
                rTrackParam.callStackDepth--;
            } else {
                rTrackParam.callStack[rTrackParam.callStackDepth - 1].loopCount = count;

                rTrackParam.currentAddr =
                    rTrackParam.callStack[rTrackParam.callStackDepth - 1].address;
            }
            break;
        }
        }
    } else if (command <= MML_EX_CMD_MAX) {
        u32 cmdex = command & 0xFF;
        volatile s16* pVar = NULL;

        if ((cmdex & 0xF0) == MML_EX_ARITHMETIC ||
            (cmdex & 0xF0) == MML_EX_LOGIC) {

            pVar = GetVariablePtr(pPlayer, pTrack, arg1);
            if (pVar == NULL) {
                return;
            }
        }

        switch (cmdex) {
        case MML_EX_SET: {
            *pVar = arg2;
            break;
        }

        case MML_EX_APL: {
            *pVar += arg2;
            break;
        }

        case MML_EX_AMI: {
            *pVar -= arg2;
            break;
        }

        case MML_EX_AMU: {
            *pVar *= arg2;
            break;
        }

        case MML_EX_ADV: {
            if (arg2 != 0) {
                *pVar /= arg2;
            }
            break;
        }

        case MML_EX_ALS: {
            if (arg2 >= 0) {
                *pVar <<= arg2;

            } else {
                *pVar >>= -arg2;
            }
            break;
        }

        case MML_EX_RND: {
            bool minus = false;

            if (arg2 < 0) {
                minus = true;
                arg2 = static_cast<s16>(-arg2);
            }

            s32 rand = Util::CalcRandom();
            rand *= arg2 + 1;
            rand >>= 16;

            if (minus) {
                rand = -rand;
            }

            *pVar = rand;
            break;
        }

        case MML_EX_AAD: {
            *pVar &= arg2;
            break;
        }

        case MML_EX_AOR: {
            *pVar |= arg2;
            break;
        }

        case MML_EX_AER: {
            *pVar ^= arg2;
            break;
        }

        case MML_EX_ACO: {
            *pVar = ~static_cast<u16>(arg2);
            break;
        }

        case MML_EX_AMD: {
            if (arg2 != 0) {
                *pVar %= arg2;
            }
            break;
        }

        case MML_EX_EQ: {
            rTrackParam.cmpFlag = *pVar == arg2;
            break;
        }

        case MML_EX_GE: {
            rTrackParam.cmpFlag = *pVar >= arg2;
            break;
        }

        case MML_EX_GT: {
            rTrackParam.cmpFlag = *pVar > arg2;
            break;
        }

        case MML_EX_LE: {
            rTrackParam.cmpFlag = *pVar <= arg2;
            break;
        }

        case MML_EX_LT: {
            rTrackParam.cmpFlag = *pVar < arg2;
            break;
        }

        case MML_EX_NE: {
            rTrackParam.cmpFlag = *pVar != arg2;
            break;
        }

        case MML_EX_USERPROC: {
            break;
        }
        }
    }
}

Channel* MmlParser::NoteOnCommandProc(MmlSeqTrack* pTrack, int key,
                                      int velocity, s32 length,
                                      bool tie) const {
    return pTrack->NoteOn(key, velocity, length, tie);
}

u16 MmlParser::Read16(const u8** ppData) const {
    u16 val = ReadByte(ppData);

    val <<= 8;
    val |= ReadByte(ppData);

    return val;
}

u32 MmlParser::Read24(const u8** ppData) const {
    u32 val = ReadByte(ppData);

    val <<= 8;
    val |= ReadByte(ppData);

    val <<= 8;
    val |= ReadByte(ppData);

    return val;
}

s32 MmlParser::ReadVar(const u8** ppData) const {
    u32 val = 0;
    u8 b;

    do {
        b = ReadByte(ppData);
        val <<= 7;
        val |= b & 0x7F;
    } while (b & 0x80);

    return val;
}

s32 MmlParser::ReadArg(const u8** ppData, SeqPlayer* pPlayer, SeqTrack* pTrack,
                       SeqArgType type) const {
    s32 arg;

    switch (type) {
    case SEQ_ARG_U8: {
        arg = ReadByte(ppData);
        break;
    }

    case SEQ_ARG_S16: {
        arg = Read16(ppData);
        break;
    }

    case SEQ_ARG_VMIDI: {
        arg = ReadVar(ppData);
        break;
    }

    case SEQ_ARG_VARIABLE: {
        u8 i = ReadByte(ppData);

        const volatile s16* pVar = GetVariablePtr(pPlayer, pTrack, i);
        if (pVar != NULL) {
            arg = *pVar;
        }
        break;
    }

    case SEQ_ARG_RANDOM: {
        s16 min = Read16(ppData);
        s16 max = Read16(ppData);

        s32 rand = Util::CalcRandom();
        rand *= (max - min) + 1;
        rand >>= 16;
        rand += min;

        arg = rand;
        break;
    }
    }

    return arg;
}

volatile s16* MmlParser::GetVariablePtr(SeqPlayer* pPlayer, SeqTrack* pTrack,
                                        int i) const {
    if (i < SeqPlayer::VARIABLE_NUM) {
        return pPlayer->GetVariablePtr(i);
    }

    if (i < SeqPlayer::VARIABLE_NUM + SeqTrack::VARIABLE_NUM) {
        return pTrack->GetVariablePtr(i - SeqPlayer::VARIABLE_NUM);
    }

    return NULL;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
