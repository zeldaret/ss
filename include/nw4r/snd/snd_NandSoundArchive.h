#ifndef NW4R_SND_NAND_SOUND_ARCHIVE_H
#define NW4R_SND_NAND_SOUND_ARCHIVE_H
#include <nand.h>
#include "ut_NandFileStream.h"
#include "snd_SoundArchive.h"
#include "snd_SoundArchiveFile.h"

namespace nw4r
{
	namespace snd
	{
		struct NandSoundArchive : SoundArchive
		{
			struct NandFileStream;
			
			detail::SoundArchiveFileReader mFileReader; // at 0x108
			NANDFileInfo mFileInfo; // at 0x14c
			bool mOpenFlag; // at 0x1d8
			
			NandSoundArchive();

			virtual ~NandSoundArchive(); // at 0x8
			virtual const void * detail_GetFileAddress(u32) const; // at 0xC
			virtual const void * detail_GetWaveDataFileAddress(u32) const; // at 0x10
			virtual int detail_GetRequiredStreamBufferSize() const; // at 0x14
			virtual ut::FileStream * OpenStream(void *, int, u32, u32) const; // at 0x18
			virtual ut::FileStream * OpenExtStream(void *, int, const char *, u32, u32) const; // at 0x1c

			bool Open(const char *);
			
			void Close();
						
			bool LoadFileHeader();
			bool LoadHeader(void *, u32);
			bool LoadLabelStringData(void *, u32);			
		};
	}
}

#endif