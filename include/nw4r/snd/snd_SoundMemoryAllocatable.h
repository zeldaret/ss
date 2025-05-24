#ifndef NW4R_SND_SOUND_MEMORY_ALLOCATABLE_H
#define NW4R_SND_SOUND_MEMORY_ALLOCATABLE_H

/*******************************************************************************
 * headers
 */

#include <types.h> // u32

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x269c0
	class SoundMemoryAllocatable
	{
	// methods
	public:
		// cdtors
		virtual ~SoundMemoryAllocatable() {}

		// virtual function ordering
		// vtable SoundMemoryAllocatable
		virtual void *Alloc(u32 size) = 0;

	// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04
}} // namespace nw4r::snd

#endif // NW4R_SND_SOUND_MEMORY_ALLOCATABLE_H
