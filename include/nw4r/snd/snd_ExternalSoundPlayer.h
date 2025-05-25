#ifndef NW4R_SND_EXTERNAL_SOUND_PLAYER_H
#define NW4R_SND_EXTERNAL_SOUND_PLAYER_H

/*******************************************************************************
 * headers
 */

#include "nw4r/snd/snd_BasicSound.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x27049
	class ExternalSoundPlayer
	{
	// methods
	public:
		// methods
		ExternalSoundPlayer();
		~ExternalSoundPlayer();
		int GetPlayingSoundCount() const { return mSoundList.GetSize(); }
		int GetPlayableSoundCount() const { return mPlayableCount; }
		void SetPlayableSoundCount(int count);

		bool AppendSound(BasicSound *sound);
		void RemoveSound(BasicSound *sound);

		bool detail_CanPlaySound(int startPriority);

	private:
		BasicSound *GetLowestPrioritySound();

	// members
	private:
		BasicSound::ExtSoundPlayerPlayLinkList	mSoundList;		// size 0x0c, offset 0x00
		int										mPlayableCount;	// size 0x04, offset 0x0c
	}; // size 0x10
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_EXTERNAL_SOUND_PLAYER_H
