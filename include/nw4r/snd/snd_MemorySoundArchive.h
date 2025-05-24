#ifndef NW4R_SND_MEMORY_SOUND_ARCHIVE_H
#define NW4R_SND_MEMORY_SOUND_ARCHIVE_H

/*******************************************************************************
 * headers
 */

#include <types.h>

// WARNING: DO NOT REORDER these #include directives, data pooling depends on it

// clang-format off
#include "../ut/FileStream.h" // This needs to be
#include "SoundArchive.h" // before this
// clang-format on

#include "SoundArchiveFile.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// TODO: get a specific source (name, game id)
	/* dwarfv1_megadump.txt:284789-284793
	 * This is the struct info for nw4hbm::snd::MemorySoundArchive from an
	 * older version of the HBM library that actually uses it, but it seems
	 * similar enough to the current version, so it is used as a reference here.
	 */
	class MemorySoundArchive : public SoundArchive
	{
	// nested types
	private:
		class MemoryFileStream;

	// methods
	public:
		// cdtors
		MemorySoundArchive();
		virtual ~MemorySoundArchive();

		// virtual function ordering
		// vtable SoundArchive
		virtual void const *detail_GetFileAddress(u32 fileId) const;
		virtual void const *detail_GetWaveDataFileAddress(u32 fileId) const;
		virtual int detail_GetRequiredStreamBufferSize() const;
		virtual ut::FileStream *OpenStream(void *buffer, int size, u32 begin,
		                                   u32 length) const;
		virtual ut::FileStream *OpenExtStream(void *buffer, int size,
		                                      char const *extFilePath,
		                                      u32 begin, u32 length) const;

		// methods
		bool Setup(void const *soundArchiveData);
		void Shutdown();

	// members
	private:
		/* base SoundArchive */							// size 0x108, offset 0x000
		void							const *mData;	// size 0x004, offset 0x108
		detail::SoundArchiveFileReader	mFileReader;	// size 0x044, offset 0x10c
	}; // size 0x150

	// NOTE: Must be completed after MemorySoundArchive for data ordering
	// TODO: get a specific source (name, game id)
	/* dwarfv1_megadump.txt:293462-293467
	 * This is the struct info for
	 * nw4hbm::snd::MemorySoundArchive::MemoryFileStream from an older version
	 * of the HBM library that actually uses it, but it seems similar enough to
	 * the current version, so it is used as a reference here.
	 */
	class MemorySoundArchive::MemoryFileStream : public ut::FileStream
	{
	// methods
	public:
		// cdtors
		MemoryFileStream(void const *buffer, u32 size);
		virtual ~MemoryFileStream() {}

		// virtual function ordering
		// vtable ut::IOStream
		virtual void Close();
		virtual s32 Read(void *buf, u32 length);

		// vtable ut::FileStream
		virtual void Seek(s32 offset, u32 origin);

		// For instantiation ordering (WARNING: Do not rearrange)
		virtual bool CanSeek() const { return true; }
		virtual bool CanCancel() const { return true; }
		virtual bool CanAsync() const { return false; }
		virtual bool CanRead() const { return true; }
		virtual bool CanWrite() const { return false; }
		virtual u32 Tell() const { return mPosition; }
		virtual u32 GetSize() const { return mSize; }

	// members
	private:
		/* base ut::FileStream */	// size 0x14, offset 0x00
		void	const *mBuffer;		// size 0x04, offset 0x14
		u32		mSize;				// size 0x04, offset 0x18
		u32		mPosition;			// size 0x04, offset 0x1c
	}; // size 0x20
}} // namespace nw4r::snd

#endif // NW4R_SND_MEMORY_SOUND_ARCHIVE_H
