#ifndef NW4R_SND_SOUND_3D_ACTOR_H
#define NW4R_SND_SOUND_3D_ACTOR_H
#include "snd_BasicSound.h"
#include "snd_SoundActor.h"
#include "math_types.h"

namespace nw4r
{
	namespace snd
	{
		struct Sound3DActor :
			detail::SoundActor,
			detail::BasicSound::AmbientArgUpdateCallback
		{
			Sound3DManager & mManager; // at 0xac
			SoundArchivePlayer & mArchivePlayer; // at 0xb0
			UNKWORD WORD_0xB4;
			math::VEC3 mPosition; // at 0xb8
			
			Sound3DActor(SoundArchivePlayer &, Sound3DManager &);
			
			~Sound3DActor();
			
			UNKWORD detail_SetupSound(SoundHandle *,
				u32,
				detail::BasicSound::AmbientArgInfo *,
				detail::ExternalSoundPlayer *,
				bool,
				const SoundStartable::StartInfo *);
			
			void SetPosition(const math::VEC3 &);
			
			void detail_Update(void *, const detail::BasicSound *);

			static void ClearUpdateCallback(SoundHandle& handle);
		};
	}
}

#endif