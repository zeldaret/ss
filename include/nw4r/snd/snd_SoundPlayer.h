#ifndef NW4R_SND_SOUND_PLAYER_H
#define NW4R_SND_SOUND_PLAYER_H
#include "types_nw4r.h"

namespace nw4r
{
	namespace snd
	{
		class SoundPlayer
		{
		public:

			f32 GetVolume() const { return mVolume; }
			f32 detail_GetMainOutVolume() const { return mMainOutVolume; }

			void StopAllSound(int);

			bool detail_IsEnabledOutputLine() const;
			int detail_GetOutputLine() const;
			f32 detail_GetRemoteOutVolume(int) const;
			UNKTYPE detail_FreePlayerHeap(detail::BasicSound *);
			UNKTYPE detail_InsertPriorityList(detail::BasicSound *);
			UNKTYPE detail_RemoveSoundList(detail::BasicSound *);
			UNKTYPE detail_RemovePriorityList(detail::BasicSound *);

		private:
			char UNK_0x0[0x28];
			f32 mVolume; // at 0x28
			bool mEnableOutputLine; // at 0x2C
			int mOutputLine; // at 0x30
			f32 mMainOutVolume; // at 0x34
		};
	}
}

#endif