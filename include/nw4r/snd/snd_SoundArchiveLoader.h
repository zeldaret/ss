#ifndef NW4R_SND_SOUND_ARCHIVE_LOADER_H
#define NW4R_SND_SOUND_ARCHIVE_LOADER_H
#include "snd_SoundMemoryAllocatable.h"
#include "ut_FileStream.h"
#include "types_nw4r.h"
#include <OSMutex.h>

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct FileStreamHandle
			{
				ut::FileStream * mFileStream;
				
				inline FileStreamHandle(ut::FileStream * pFileStream) : mFileStream(pFileStream) {}
				
				inline ~FileStreamHandle()
				{
					if (mFileStream) mFileStream->Close();
				}
				
				inline operator bool() const
				{
					return mFileStream;
				}
				
				inline ut::FileStream * operator->()
				{
					return mFileStream;
				}
			};
			
			struct SoundArchiveLoader
			{
				OSMutex mMutex; // at 0x0
				const SoundArchive & mArchive; // at 0x18
				u8 mBuffer[0x200]; // at 0x1c
				ut::FileStream * mFileStream; // at 0x21c
				
				SoundArchiveLoader(const SoundArchive &);
				~SoundArchiveLoader();
				
				void * LoadGroup(u32, SoundMemoryAllocatable *, void **, u32);
			};
		}
	}
}

#endif