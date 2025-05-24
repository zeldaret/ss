#ifndef NW4R_SND_STRM_SOUND_HANDLE_H
#define NW4R_SND_STRM_SOUND_HANDLE_H

/*******************************************************************************
 * headers
 */

#include "common.h" // nullptr

#include "nw4r/ut/ut_NonCopyable.h" // ut::NonCopyable

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class StrmSound; }}}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x3074e
	class StrmSoundHandle : private ut::NonCopyable
	{
	// methods
	public:
		// methods
		bool IsAttachedSound() const { return mSound != nullptr; }

		void DetachSound();

	// members
	private:
		/* base NonCopyable */			// size 0x00, offset 0x00
		detail::StrmSound	*mSound;	// size 0x04, offset 0x00
	}; // size 0x04
}} // namespace nw4r::snd

#endif // NW4R_SND_STRM_SOUND_HANDLE_H
