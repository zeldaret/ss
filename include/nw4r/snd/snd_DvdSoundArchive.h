#ifndef NW4R_SND_DVD_SOUND_ARCHIVE_H
#define NW4R_SND_DVD_SOUND_ARCHIVE_H
#include <dvd.h>
#include "ut_DvdLockedFileStream.h"
#include "snd_SoundArchive.h"
#include "snd_SoundArchiveFile.h"

namespace nw4r
{
	namespace snd
	{
		struct DvdSoundArchive : SoundArchive
		{
			struct DvdFileStream;
			
			detail::SoundArchiveFileReader mFileReader; // at 0x108
			DVDFileInfo mFileInfo; // at 0x14c
			bool mOpenFlag; // at 0x188
			
			DvdSoundArchive();

			virtual ~DvdSoundArchive(); // at 0x8
			virtual const void * detail_GetFileAddress(u32) const; // at 0xC
			virtual const void * detail_GetWaveDataFileAddress(u32) const; // at 0x10
			virtual int detail_GetRequiredStreamBufferSize() const; // at 0x14
			virtual ut::FileStream * OpenStream(void *, int, u32, u32) const; // at 0x18
			virtual ut::FileStream * OpenExtStream(void *, int, const char *, u32, u32) const; // at 0x1c

			bool Open(s32); //inlined
			bool Open(const char *);
			
			void Close();
			
			bool LoadFileHeader(); //inlined
			bool LoadHeader(void *, u32);
			bool LoadLabelStringData(void *, u32);
		};
	}
}

#endif