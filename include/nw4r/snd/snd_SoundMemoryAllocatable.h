#ifndef NW4R_SOUND_MEMORY_ALLOCATABLE_H
#define NW4R_SOUND_MEMORY_ALLOCATABLE_H
#include "types_nw4r.h"

namespace nw4r
{
	namespace snd
	{
		struct SoundMemoryAllocatable
		{
			virtual ~SoundMemoryAllocatable() {} // at 0x8
			virtual void * Alloc(u32) = 0; // at 0xc
		};
	}
}

#endif