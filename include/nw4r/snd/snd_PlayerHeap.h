#ifndef NW4R_SND_PLAYER_HEAP_H
#define NW4R_SND_PLAYER_HEAP_H
#include "types_nw4r.h"
#include "snd_SoundHeap.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct PlayerHeap : SoundHeap
			{
				~PlayerHeap();
				UNKTYPE * Alloc(u32);
			};
		}
	}
}

#endif