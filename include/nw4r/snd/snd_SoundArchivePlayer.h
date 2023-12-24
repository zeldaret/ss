#ifndef NW4R_SND_SOUND_ARCHIVE_PLAYER_H
#define NW4R_SND_SOUND_ARCHIVE_PLAYER_H
#include "snd_SoundStartable.h"
#include "snd_SoundHandle.h"
#include "snd_DisposeCallback.h"
#include "snd_SoundMemoryAllocatable.h"
#include "types_nw4r.h"

namespace nw4r
{
	namespace snd
	{
		struct SoundArchivePlayer : detail::DisposeCallback, SoundStartable
		{
			SoundArchivePlayer();
			~SoundArchivePlayer();

			bool IsAvailable() const;
			bool Setup(const SoundArchive *, void *, u32, void *, u32);

			void Shutdown();
			u32 GetRequiredMemSize(const SoundArchive *);
			u32 GetRequiredStrmBufferSize(const SoundArchive *);

			void Update();

			SoundArchive * GetSoundArchive() const;
			SoundPlayer * GetSoundPlayer(u32);

			UNKWORD detail_SetupSound(SoundHandle *,
				u32,
				detail::BasicSound::AmbientArgInfo *,
				detail::ExternalSoundPlayer *,
				bool,
				const StartInfo *);

			UNKWORD LoadGroup(u32, SoundMemoryAllocatable *, u32);
			UNKWORD LoadGroup(const char *, SoundMemoryAllocatable *, u32);

			virtual void InvalidateData(const void *, const void *);
			virtual void InvalidateWaveData(const void *, const void *);
			virtual UNKWORD detail_ConvertLabelStringToSoundId(const char *);

			char UNK_0x4[0x8];
			SoundHandle mHandle; // at 0xC
			char UNK_0x1C[0x14];
			u32 WORD_0x30;
			char UNK_0x34[0xAC];
		};
	}
}

#endif