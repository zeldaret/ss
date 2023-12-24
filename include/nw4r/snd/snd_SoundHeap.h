#ifndef NW4R_SND_SOUND_HEAP_H
#define NW4R_SND_SOUND_HEAP_H
#include "types_nw4r.h"
#include "snd_FrameHeap.h"
#include "snd_SoundMemoryAllocatable.h"
#include <OSMutex.h>

namespace nw4r
{
	namespace snd
	{
		struct SoundHeap : SoundMemoryAllocatable
		{
			SoundHeap();
			virtual ~SoundHeap();      // VMT 0x8
			bool Create(void *, u32);
			void Destroy();
			virtual void * Alloc(u32); // VMT 0xC
			void* Alloc(u32, detail::FrameHeap::AllocCallback, void *);
			void Clear();
			int SaveState();
			void LoadState(int);
			static void DisposeCallbackFunc(void *, u32, void *);

			bool IsValid() { return mFrameHeap.PTR_0x0 != NULL; }

			OSMutex mMutex;               // 0x00
			detail::FrameHeap mFrameHeap; // 0x1C
		};
	}
}

#endif