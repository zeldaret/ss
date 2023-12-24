#ifndef NW4R_SND_MEMORY_SOUND_ARCHIVE_H
#define NW4R_SND_MEMORY_SOUND_ARCHIVE_H
#include "ut_FileStream.h"
#include "snd_SoundArchive.h"
#include "snd_SoundArchiveFile.h"

namespace nw4r
{
	namespace snd
	{
		struct MemorySoundArchive : SoundArchive
		{
			struct MemoryFileStream;		
				
			const void * mMemory; // at 0x108
			detail::SoundArchiveFileReader mFileReader; // at 0x10c
			
			MemorySoundArchive();
			virtual ~MemorySoundArchive(); // at 0x8
			
			bool Setup(const void *);
			void Shutdown();
			
			virtual const void * detail_GetFileAddress(u32) const;
			virtual const void * detail_GetWaveDataFileAddress(u32) const;
			virtual int detail_GetRequiredStreamBufferSize() const;
			virtual ut::FileStream * OpenStream(void *, int, u32, u32) const; // at 0x18
			virtual ut::FileStream * OpenExtStream(void *, int, const char *, u32, u32) const; // at 0x1c
		};
	}
}

#endif