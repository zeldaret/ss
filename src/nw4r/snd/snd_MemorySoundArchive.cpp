#include "nw4r/snd/snd_MemorySoundArchive.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_MemorySoundArchive.cpp
 */

/*******************************************************************************
 * headers
 */

#include <cstring> // std::memcpy
#include <new>

#include "common.h"

#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundArchiveFile.h" // SoundArchiveFileReader

#include "nw4r/ut/ut_FileStream.h"
#include "nw4r/ut/ut_algorithm.h"
#include "nw4r/ut/ut_RuntimeTypeInfo.h" // IWYU pragma: keep (need the complete type)

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

MemorySoundArchive::MemorySoundArchive() :
	mData	(nullptr)
{
}

MemorySoundArchive::~MemorySoundArchive() {}

bool MemorySoundArchive::Setup(void const *soundArchiveData)
{
	NW4RAssertPointerNonnull_Line(65, soundArchiveData);
	NW4RAssertAligned_Line(66, soundArchiveData, 4);

	mFileReader.Init(soundArchiveData);
	SoundArchive::Setup(&mFileReader);

	void const *infoChunk =
		ut::AddOffsetToPtr(soundArchiveData, mFileReader.GetInfoChunkOffset());

	mFileReader.SetInfoChunk(infoChunk, mFileReader.GetInfoChunkSize());

	void const *stringChunk = ut::AddOffsetToPtr(
		soundArchiveData, mFileReader.GetLabelStringChunkOffset());

	mFileReader.SetStringChunk(stringChunk,
	                           mFileReader.GetLabelStringChunkSize());

	mData = soundArchiveData;

	return true;
}

void MemorySoundArchive::Shutdown()
{
	mData = nullptr;

	SoundArchive::Shutdown();
}

void const *MemorySoundArchive::detail_GetFileAddress(u32 fileId) const
{
	SoundArchive::FilePos filePos;
	if (!detail_ReadFilePos(fileId, 0, &filePos))
		return nullptr;

	SoundArchive::GroupInfo groupInfo;
	if (!detail_ReadGroupInfo(filePos.groupId, &groupInfo))
		return nullptr;

	SoundArchive::GroupItemInfo itemInfo;
	if (!detail_ReadGroupItemInfo(filePos.groupId, filePos.index, &itemInfo))
		return nullptr;

	if (groupInfo.extFilePath)
		return nullptr;

	return ut::AddOffsetToPtr(mData, groupInfo.offset + itemInfo.offset);
}

void const *MemorySoundArchive::detail_GetWaveDataFileAddress(u32 fileId) const
{
	SoundArchive::FilePos filePos;
	if (!detail_ReadFilePos(fileId, 0, &filePos))
		return nullptr;

	SoundArchive::GroupInfo groupInfo;
	if (!detail_ReadGroupInfo(filePos.groupId, &groupInfo))
		return nullptr;

	SoundArchive::GroupItemInfo itemInfo;
	if (!detail_ReadGroupItemInfo(filePos.groupId, filePos.index, &itemInfo))
		return nullptr;

	if (groupInfo.extFilePath)
		return nullptr;

	return ut::AddOffsetToPtr(mData, groupInfo.waveDataOffset
	                                     + itemInfo.waveDataOffset);
}

ut::FileStream *MemorySoundArchive::OpenStream(void *buffer, int size,
                                               u32 begin, u32 length) const
{
	if (!mData)
		return nullptr;

	if (size < sizeof(MemoryFileStream))
		return nullptr;

	return new (buffer)
		MemoryFileStream(ut::AddOffsetToPtr(mData, begin), length);
}

ut::FileStream *MemorySoundArchive::OpenExtStream(void *, int, char const *,
                                                  u32, u32) const
{
	NW4RWarningMessage_Line(187,
	                        "Cannot OpenExtStream for MemorySoundArchive\n");

	return nullptr;
}

int MemorySoundArchive::detail_GetRequiredStreamBufferSize() const
{
	return sizeof(MemoryFileStream);
}

MemorySoundArchive::MemoryFileStream::MemoryFileStream(void const *buffer,
                                                       u32 size) :
	mBuffer		(buffer),
	mSize		(size),
	mPosition	(0)
{
}

void MemorySoundArchive::MemoryFileStream::Close()
{
	mBuffer		= nullptr;
	mSize		= 0;
	mPosition	= 0;
}

s32 MemorySoundArchive::MemoryFileStream::Read(void *buf, u32 length)
{
	s32 readBytes = ut::Min(length, mSize - mPosition);
	std::memcpy(buf, ut::AddOffsetToPtr(mBuffer, mPosition), readBytes);

	return readBytes;
}

void MemorySoundArchive::MemoryFileStream::Seek(s32 offset, u32 origin)
{
	switch (origin)
	{
	case FileStream::SEEK_ORIGIN_SET:
		mPosition = offset;
		break;

	case FileStream::SEEK_ORIGIN_CUR:
		mPosition += offset;
		break;

	case FileStream::SEEK_ORIGIN_END:
		mPosition = mSize - offset;
		break;

	default:
		NW4RPanicMessage_Line(234, "Unsupported Seek origin");
		break;
	}
}

}} // namespace nw4r::snd
