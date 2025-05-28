#ifndef NW4R_SND_EXTERNAL_SOUND_PLAYER_H
#define NW4R_SND_EXTERNAL_SOUND_PLAYER_H

/*******************************************************************************
 * headers
 */

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_global.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { class SoundActor; }}

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
		void StopAllSound(int fadeFrames);
		void PauseAllSound(bool flag, int fadeFrames);
		void DetachSoundActorAll(SoundActor *sound);

		bool AppendSound(BasicSound *sound);
		void RemoveSound(BasicSound *sound);

		bool detail_CanPlaySound(int startPriority);

		template <typename TForEachFunc>
		TForEachFunc ForEachSound(TForEachFunc pFunc, bool reverse) {
			if (reverse) {
				detail::BasicSound::ExtSoundPlayerPlayLinkList::ReverseIterator it = mSoundList.GetBeginReverseIter();

				while (it != mSoundList.GetEndReverseIter()) {
					detail::BasicSound::ExtSoundPlayerPlayLinkList::ReverseIterator curr = it;

					SoundHandle handle;
					handle.detail_AttachSoundAsTempHandle(&*curr);
					pFunc(handle);

					if (handle.IsAttachedSound()) {
						++it;
					}
				}
			} else {
				NW4R_RANGE_FOR_NO_AUTO_INC(it, mSoundList) {
					decltype(it) curItr = it++;
					SoundHandle handle;
					handle.detail_AttachSoundAsTempHandle(&*curItr);
					pFunc(handle);
				}
			}

			return pFunc;
		}

	private:
		BasicSound *GetLowestPrioritySound();

	// members
	private:
		BasicSound::ExtSoundPlayerPlayLinkList	mSoundList;		// size 0x0c, offset 0x00
		int										mPlayableCount;	// size 0x04, offset 0x0c
	}; // size 0x10
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_EXTERNAL_SOUND_PLAYER_H
