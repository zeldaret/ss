#ifndef NW4R_SND_PLAYER_HEAP_H
#define NW4R_SND_PLAYER_HEAP_H

/*******************************************************************************
 * headers
 */

#include "common.h" // u32

#include "nw4r/snd/snd_SoundMemoryAllocatable.h"

#include "nw4r/ut/ut_LinkList.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class BasicSound; }}}
namespace nw4r { namespace snd { class SoundPlayer; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x26e9c
	class PlayerHeap : public SoundMemoryAllocatable
	{
	// typedefs
	public:
		typedef ut::LinkList<PlayerHeap, 0x18> LinkList;

	// methods
	public:
		// cdtors
		PlayerHeap();
		virtual ~PlayerHeap();

		// virtual function ordering
		// vtable SoundMemoryAllocatable
		virtual void *Alloc(u32 size);

		// methods
		bool Create(void *startAddress, u32 size);
		void Clear();
		void Destroy();

		u32 GetFreeSize() const;

		void AttachSound(BasicSound *sound);
		void DetachSound(BasicSound *sound);

		void AttachSoundPlayer(SoundPlayer *player) { mPlayer = player; }

	// members
	private:
		/* base SoundMemoryAllocatable */	// size 0x04, offset 0x00
		BasicSound			*mSound;		// size 0x04, offset 0x04
		SoundPlayer			*mPlayer;		// size 0x04, offset 0x08
		void				*mStartAddress;	// size 0x04, offset 0x0c
		void				*mEndAddress;	// size 0x04, offset 0x10
		void				*mAllocAddress;	// size 0x04, offset 0x14
	public:
		ut::LinkListNode	mLink;			// size 0x08, offset 0x18
	}; // size 0x20
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_PLAYER_HEAP_H
