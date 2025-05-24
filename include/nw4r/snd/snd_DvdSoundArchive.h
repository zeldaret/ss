#ifndef NW4R_SND_DVD_SOUND_ARCHIVE_H
#define NW4R_SND_DVD_SOUND_ARCHIVE_H

/*******************************************************************************
 * headers
 */

#include "common.h"

// WARNING: DO NOT REORDER these #include directives, data pooling depends on it

// clang-format off
#include "nw4r/ut/ut_FileStream.h" // This needs to be
#include "nw4r/snd/snd_SoundArchive.h" // before this
#include "nw4r/snd/snd_SoundArchiveFile.h" // before this
#include "rvl/DVD/dvd.h"
// clang-format on

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	class DvdSoundArchive : public SoundArchive
	{
	public:
		class DvdFileStream;
		DvdSoundArchive();
		virtual ~DvdSoundArchive(); // at 0x8

		virtual const void *detail_GetFileAddress(u32 /* id */) const {
			return NULL;
		} // at 0xC

		virtual const void *detail_GetWaveDataFileAddress(u32 /* id */) const {
			return NULL;
		} // at 0x10

		virtual int detail_GetRequiredStreamBufferSize() const; // at 0x14

		virtual ut::FileStream *OpenStream(void *pBuffer, int size, u32 offset,
										u32 length) const; // at 0x18

		virtual ut::FileStream *OpenExtStream(
			void *pBuffer, int size, const char *pExtPath, u32 offset,
			u32 length
		) const; // at 0x1C

		bool Open(s32 entrynum);
		bool Open(const char *pPath);

		void Close();

		bool LoadHeader(void *pBuffer, u32 size);
		bool LoadLabelStringData(void *pBuffer, u32 size);

		u32 GetHeaderSize() const {
			return mFileReader.GetInfoChunkSize();
		}
		u32 GetLabelStringDataSize() const {
			return mFileReader.GetLabelStringChunkSize();
		}

	private:
		bool LoadFileHeader();

	private:
		detail::SoundArchiveFileReader mFileReader; // at 0x108
		DVDFileInfo mFileInfo;                      // at 0x14C
		bool mOpen;                                 // at 0x188
	};

	class DvdSoundArchive::DvdFileStream : public ut::FileStream
	{
		DvdFileStream();
		virtual ut::detail::RuntimeTypeInfo const *GetRuntimeTypeInfo() const { return 0; }
		virtual void Close() {}
		virtual s32 Read(void *, u32) { return 0; }
		virtual bool IsBusy() const { return 0; }
		virtual bool CanAsync() const { return 0; }
		virtual bool CanRead() const { return 0; }
		virtual bool CanWrite() const { return 0; }
		virtual u32 GetOffsetAlign() const { return 0; }
		virtual u32 GetSizeAlign() const { return 0; }
		virtual u32 GetBufferAlign() const { return 0; }
		virtual u32 GetSize() const { return 0; }
		virtual void Seek(s32, u32) {}
		virtual void Cancel() {}
		virtual bool CancelAsync(StreamCallback *, void *) { return 0; }
		virtual bool CanSeek() const { return 0; }
		virtual bool CanCancel() const { return 0; }
		virtual u32 Tell() const { return 0; }
		friend class DvdSoundArchive;
	};
}} // namespace nw4r::snd

#endif // NW4R_SND_DVD_SOUND_ARCHIVE_H
