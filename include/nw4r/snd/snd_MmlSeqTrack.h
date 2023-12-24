#ifndef NW4R_SND_MML_SEQ_TRACK_H
#define NW4R_SND_MML_SEQ_TRACK_H
#include "snd_SeqTrack.h"
#include "types_nw4r.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct MmlSeqTrack : SeqTrack
			{
				enum
				{
					STACK_FRAME_COUNT = 3
				};
				
				struct MmlParserParam
				{
					bool mPredicate; // at 0x0
					bool BOOL_0x1;
					bool BOOL_0x2;
					u8 mLoopCounters[STACK_FRAME_COUNT]; // at 0x3
					u8 mStackIndex; // at 0x6
					const u8 * mReturnAddresses[STACK_FRAME_COUNT]; // at 0x8
				};
				
				MmlParser * mParser; // at 0xc0
				MmlParserParam mMmlParserParam; // at 0xc4
				
				MmlSeqTrack();
				
				UNKWORD Parse(bool); // at 0xc
			};
		}
	}
}

#endif