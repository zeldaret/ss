#ifndef NW4R_SND_MML_PARSER_H
#define NW4R_SND_MML_PARSER_H
#include "types_nw4r.h"
#include "snd_MmlSeqTrack.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct MmlParser
			{
				u16 Read16(const u8 **) const; //inlined
				u32 Read24(const u8 **) const; //inlined
				u32 ReadVar(const u8 **) const; //inlined
				
				volatile s16 * GetVariablePtr(SeqPlayer *, SeqTrack *, int) const;
				
				UNKWORD Parse(MmlSeqTrack *, bool) const;
				virtual void CommandProc(MmlSeqTrack *, u32, s32, s32) const; // at 0x8
				virtual Channel * NoteOnCommandProc(MmlSeqTrack *, int, int, s32, bool) const; // at 0xc
				
				enum SeqArgType
				{
					SEQ_ARG_TYPE_IMMEDIATE_BYTE,
					SEQ_ARG_TYPE_IMMEDIATE_16,
					SEQ_ARG_TYPE_IMMEDIATE_VAR,
					SEQ_ARG_TYPE_RANDOM,
					SEQ_ARG_TYPE_VARIABLE
				};
				
				s32 ReadArg(const u8 **, SeqPlayer *, SeqTrack *, SeqArgType) const;
				
				inline u8 ReadByte(const u8 ** pPointer) const
				{
					return *(*pPointer)++;
				}
				
				static bool mPrintVarEnabledFlag;
			};
		}
	}
}

#endif