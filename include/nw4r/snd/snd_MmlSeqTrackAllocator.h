#ifndef NW4R_SND_MML_SEQ_TRACK_ALLOCATOR_H
#define NW4R_SND_MML_SEQ_TRACK_ALLOCATOR_H
#include "snd_MmlSeqTrack.h"
#include "snd_MmlParser.h"
#include "snd_InstancePool.h"
#include "types_nw4r.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct MmlSeqTrackAllocator : SeqTrackAllocator
			{
				MmlParser * mParser;
				InstancePool<MmlSeqTrack> mPool; // at 0x8
				
				SeqTrack * AllocTrack(SeqPlayer *); // at 0xc
				void FreeTrack(SeqTrack *); // at 0x10
				
				u32 Create(void *, u32);
				void Destroy(void *, u32);
				
				int GetAllocatableTrackCount() const; // at 0x14
			};
		}
	}
}

#endif