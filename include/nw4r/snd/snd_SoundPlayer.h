#ifndef NW4R_SND_SOUND_PLAYER_H
#define NW4R_SND_SOUND_PLAYER_H

/*******************************************************************************
 * headers
 */

#include "common.h" // f32

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_global.h" // AUX_BUS_NUM
#include "nw4r/snd/snd_PlayerHeap.h"

/*******************************************************************************
 * classes
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x26c0f
	// NOTE: different from ketteiban: no remote fields
	class SoundPlayer
	{
	// methods
	public:
		// cdtors
		SoundPlayer();
		~SoundPlayer();

		// methods
		int GetPlayableSoundCount() const { return mPlayableCount; }
		f32 GetVolume() const { return mVolume; }
		void SetVolume(f32 volume);
		f32 GetLpfFreq() const { return mLpfFreq; }
		void SetLpfFreq(f32 freq);
		int GetDefaultOutputLine() const { return mOutputLineFlag; }
		f32 GetMainOutVolume() const { return mMainOutVolume; }
		int GetBiquadFilterType() const { return mBiquadType; }
		f32 GetBiquadFilterValue() const { return mBiquadValue; }
		f32 GetMainSend() const { return mMainSend; }
		f32 GetFxSend(int index) const { return mFxSend[index]; }
		void SetFxSend(AuxBus, f32);

		void SetPlayableSoundCount(int count);
		f32 GetRemoteOutVolume(int remote) const;

		int GetPlayingSoundCount() const { return mSoundList.GetSize(); }
		detail::BasicSound *GetLowestPrioritySound()
		{
			return &mPriorityList.GetFront();
		}

		void Update();
		void PauseAllSound(bool flag, int fadeFrames);
		void StopAllSound(int fadeFrames);

		void RemoveSoundList(detail::BasicSound *sound);
		bool detail_AppendSound(detail::BasicSound *sound);
		void detail_RemoveSound(detail::BasicSound *sound);
		void detail_SetPlayableSoundLimit(int limit);
		bool detail_CanPlaySound(int startPriority);

		void InsertPriorityList(detail::BasicSound *sound);
		void RemovePriorityList(detail::BasicSound *sound);
		void detail_SortPriorityList(detail::BasicSound *sound);
		void detail_SortPriorityList();

		detail::PlayerHeap *detail_AllocPlayerHeap(detail::BasicSound *sound);
		void detail_FreePlayerHeap(detail::BasicSound *sound);
		void detail_AppendPlayerHeap(detail::PlayerHeap *heap);

	// members
	private:
		detail::BasicSound::SoundPlayerPlayLinkList		mSoundList;				// size 0x0c, offset 0x00
		detail::BasicSound::SoundPlayerPriorityLinkList	mPriorityList;			// size 0x0c, offset 0x0c
		detail::PlayerHeap::LinkList					mHeapList;				// size 0x0c, offset 0x18
		int												mPlayableCount;			// size 0x04, offset 0x24
		int												mPlayableLimit;			// size 0x04, offset 0x28
		f32												mVolume;				// size 0x04, offset 0x2c
		f32												mLpfFreq;				// size 0x04, offset 0x30
		int												mOutputLineFlag;		// size 0x04, offset 0x34
		f32												mMainOutVolume;			// size 0x04, offset 0x38
		int												mBiquadType;			// size 0x04, offset 0x3c
		f32												mBiquadValue;			// size 0x04, offset 0x40
		f32												mRemoteOutVolume[4];
		f32												mMainSend;				// size 0x04, offset 0x44
		f32												mFxSend[AUX_BUS_NUM];	// size 0x0c, offset 0x48
	}; // size 0x54
}} // namespace nw4r::snd

#endif // NW4R_SND_SOUND_PLAYER_H
