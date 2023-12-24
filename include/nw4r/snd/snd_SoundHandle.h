#ifndef NW4R_SND_SOUND_HANDLE_H
#define NW4R_SND_SOUND_HANDLE_H
#include "types_nw4r.h"
#include "snd_BasicSound.h"

namespace nw4r
{
	namespace snd
	{
		struct SoundHandle
		{
			detail::BasicSound * mSound; // at 0x0
			
			void detail_AttachSound(detail::BasicSound *);
			void detail_AttachSoundAsTempHandle(detail::BasicSound *);
			void DetachSound();
			
			inline SoundHandle() : mSound(NULL) {}
			
			inline ~SoundHandle()
			{
				DetachSound();
			}

			inline detail::BasicSound * detail_GetAttachedSound() { return mSound; }

			inline bool IsAttachedSound() const { return mSound != NULL; }

			inline void StartPrepared() 
			{
				if (IsAttachedSound()) mSound->StartPrepared();
			}

			inline u32 GetId() const { return IsAttachedSound() ? mSound->GetId() : -1; }
		};
	}
}

#endif