#ifndef NW4R_SND_WAVE_SOUND_HANDLE_H
#define NW4R_SND_WAVE_SOUND_HANDLE_H

/*******************************************************************************
 * headers
 */

#include "common.h" // nullptr

#include "nw4r/snd/snd_WaveSound.h" // snd::WaveSound

#include "nw4r/ut/ut_NonCopyable.h" // ut::NonCopyable

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class WaveSound; }}}
namespace nw4r { namespace snd { class SoundHandle; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x30f8a
	class WaveSoundHandle : private ut::NonCopyable
	{
	// methods
	public:
		// cdtors
		WaveSoundHandle(SoundHandle *handle);
		~WaveSoundHandle() { DetachSound(); }

		// methods
		bool IsAttachedSound() const { return mSound != nullptr; }

		u32 GetPlaySamplePosition() const {
			if (!IsAttachedSound())
				return -1;
			return mSound->GetPlaySamplePosition();
		}

		void DetachSound();

	// members
	private:
		/* base NonCopyable */			// size 0x00, offset 0x00
		detail::WaveSound	*mSound;	// size 0x04, offset 0x00
	}; // size 0x04
}} // namespace nw4r::snd

#endif // NW4R_SND_WAVE_SOUND_HANDLE_H
