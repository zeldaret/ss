#ifndef NW4R_SND_NOTE_ON_H
#define NW4R_SND_NOTE_ON_H
#include "types_nw4r.h"
#include "snd_Channel.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct NoteOnInfo
			{
				int INT_0x0;
				int INT_0x4;
				int INT_0x8;
				int INT_0xC;
				int INT_0x10;
				int INT_0x14;
				int INT_0x18;
				Channel::ChannelCallback mChannelCallback; // at 0x1c
				u32 INT_0x20;
			};
			
			struct NoteOnCallback
			{
				virtual ~NoteOnCallback(); // at 0x8
				virtual bool NoteOn(SeqPlayer *, int, const NoteOnInfo &) = 0; // at 0xc
			};
		}
	}
}

#endif