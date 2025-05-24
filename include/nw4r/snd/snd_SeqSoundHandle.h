#ifndef NW4R_SND_SEQ_SOUND_HANDLE_H
#define NW4R_SND_SEQ_SOUND_HANDLE_H

/*******************************************************************************
 * headers
 */

#include <types.h> // nullptr

#include "../ut/inlines.h" // ut::NonCopyable

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class SeqSound; }}}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e9b6
	class SeqSoundHandle : private ut::NonCopyable
	{
	// methods
	public:
		// methods
		bool IsAttachedSound() const { return mSound != nullptr; }

		void DetachSound();

	// members
	private:
		/* base ut::NonCopyable */		// size 0x00, offset 0x00
		detail::SeqSound	*mSound;	// size 0x04, offset 0x00
	}; // size 0x04
}} // namespace nw4r::snd

#endif // NW4R_SND_SEQ_SOUND_HANDLE_H
