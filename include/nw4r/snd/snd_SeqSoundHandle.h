#ifndef NW4R_SND_SEQ_SOUND_HANDLE_H
#define NW4R_SND_SEQ_SOUND_HANDLE_H

/*******************************************************************************
 * headers
 */

#include "common.h" // nullptr

#include "nw4r/snd/snd_SeqSound.h"
#include "nw4r/ut/ut_NonCopyable.h" // ut::NonCopyable

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
		SeqSoundHandle(SoundHandle*);
		// methods
		bool IsAttachedSound() const { return mSound != nullptr; }

		void DetachSound();

		void WriteVariable(int varNo, s16 value) {
			if (IsAttachedSound())
				mSound->WriteVariable(varNo, value);
		}

	// members
	private:
		/* base ut::NonCopyable */		// size 0x00, offset 0x00
		detail::SeqSound	*mSound;	// size 0x04, offset 0x00
	}; // size 0x04
}} // namespace nw4r::snd

#endif // NW4R_SND_SEQ_SOUND_HANDLE_H
