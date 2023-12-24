#ifndef NW4R_SND_SOUND_3D_MANAGER_H
#define NW4R_SND_SOUND_3D_MANAGER_H
#include "snd_BasicSound.h"
#include "snd_SoundArchive.h"
#include "snd_SoundActor.h"
#include "snd_InstancePool.h"
#include "math_types.h"

namespace nw4r
{
	namespace snd
	{
		struct Sound3DManager :
			detail::BasicSound::AmbientParamUpdateCallback,
			detail::BasicSound::AmbientArgAllocaterCallback
		{
			struct Sound3DActorParam
			{
				u32 WORD_0x0;
				
				SoundArchive::Sound3DParam mSound3DParam; // at 0x4
				
				math::VEC3 mPosition; // at 0xc
				
				Sound3DActorParam();
			};
			
			detail::InstancePool<Sound3DActorParam> mPool; // at 0x8
			Sound3DListener * mListener; // at 0xc
			int INT_0x10;
			float FLOAT_0x14;
			float FLOAT_0x18;
			float FLOAT_0x1C;
			float FLOAT_0x20;
			float FLOAT_0x24;
			
			Sound3DManager();
			
			u32 GetRequiredMemSize(const SoundArchive *);
			
			bool Setup(const SoundArchive *, void *, u32);
			
			void detail_Update(SoundParam *, u32, detail::BasicSound *, const void *, u32);
			
			virtual void Update(SoundParam *, u32, SoundHandle *, const void *, u32);
			
			void * detail_AllocAmbientArg(u32);
			void detail_FreeAmbientArg(void *, const detail::BasicSound *);
		};
	}
}

#endif