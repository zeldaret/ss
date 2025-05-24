#ifndef NW4R_SND_MCS_SOUND_ARCHIVE_H
#define NW4R_SND_MCS_SOUND_ARCHIVE_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "SoundArchive.h"

#include "nw4r/ut/ut_FileStream.h"

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	class McsSoundArchive : public SoundArchive
	{
		class McsFileStream;
		McsSoundArchive();
	};

	class McsSoundArchive::McsFileStream : public ut::FileStream
	{
		McsFileStream();
		virtual ut::detail::RuntimeTypeInfo const *GetRuntimeTypeInfo() const { return 0; }
		virtual void Close() {}
		virtual s32 Read(void *, u32) { return 0; }
		virtual bool IsBusy() const { return 0; }
		virtual bool CanAsync() const { return 0; }
		virtual bool CanRead() const { return 0; }
		virtual bool CanWrite() const { return 0; }
		virtual u32 GetSize() const { return 0; }
		virtual void Seek(s32, u32) {}
		virtual void Cancel() {}
		virtual bool CancelAsync(StreamCallback *, void *) { return 0; }
		virtual bool CanSeek() const { return 0; }
		virtual bool CanCancel() const { return 0; }
		virtual u32 Tell() const { return 0; }
		friend class McsSoundArchive;
	};
}} // namespace nw4r::snd

#endif // NW4R_SND_MCS_SOUND_ARCHIVE_H
