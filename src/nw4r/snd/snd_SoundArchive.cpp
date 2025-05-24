#include "nw4r/snd/snd_SoundArchive.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SoundArchive.cpp
 */

/*******************************************************************************
 * headers
 */

#include <cstring>

#include "common.h"

#include "nw4r/snd/snd_SoundArchiveFile.h" // detail::SoundArchiveFileReader

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace ut { class FileStream; }}

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

SoundArchive::SoundArchive() :
	mFileReader	(nullptr)
{
	mExtFileRoot[0]	= '/';
	mExtFileRoot[1]	= '\0';
}

SoundArchive::~SoundArchive() {}

bool SoundArchive::IsAvailable() const
{
	if (!mFileReader)
		return false;

	return true;
}

void SoundArchive::Setup(detail::SoundArchiveFileReader *fileReader)
{
	NW4RAssertPointerNonnull_Line(70, fileReader);

	mFileReader = fileReader;
}

void SoundArchive::Shutdown()
{
	mFileReader		= nullptr;

	mExtFileRoot[0]	= '/';
	mExtFileRoot[1]	= '\0';
}

u32 SoundArchive::GetPlayerCount() const
{
	return mFileReader->GetPlayerCount();
}

u32 SoundArchive::GetGroupCount() const
{
	return mFileReader->GetGroupCount();
}

u32 SoundArchive::ConvertLabelStringToSoundId(char const *label) const
{
	return mFileReader->ConvertLabelStringToSoundId(label);
}

SoundArchive::SoundType SoundArchive::GetSoundType(u32 soundId) const
{
	return mFileReader->GetSoundType(soundId);
}

bool SoundArchive::ReadSoundInfo(u32 soundId, SoundInfo *info) const
{
	return mFileReader->ReadSoundInfo(soundId, info);
}

bool SoundArchive::ReadSeqSoundInfo(u32 soundId, SeqSoundInfo *info) const
{
	return mFileReader->ReadSeqSoundInfo(soundId, info);
}

bool SoundArchive::detail_ReadStrmSoundInfo(u32 soundId,
                                            StrmSoundInfo *info) const
{
	return mFileReader->ReadStrmSoundInfo(soundId, info);
}

bool SoundArchive::detail_ReadWaveSoundInfo(u32 soundId,
                                            WaveSoundInfo *info) const
{
	return mFileReader->ReadWaveSoundInfo(soundId, info);
}

bool SoundArchive::ReadPlayerInfo(u32 playerId, PlayerInfo *info) const
{
	return mFileReader->ReadPlayerInfo(playerId, info);
}

bool SoundArchive::ReadSoundArchivePlayerInfo(
	SoundArchivePlayerInfo *info) const
{
	return mFileReader->ReadSoundArchivePlayerInfo(info);
}

bool SoundArchive::ReadBankInfo(u32 bankId, BankInfo *info) const
{
	return mFileReader->ReadBankInfo(bankId, info);
}

bool SoundArchive::detail_ReadGroupInfo(u32 groupId, GroupInfo *info) const
{
	return mFileReader->ReadGroupInfo(groupId, info);
}

bool SoundArchive::detail_ReadGroupItemInfo(u32 groupId, u32 index,
                                            GroupItemInfo *info) const
{
	return mFileReader->ReadGroupItemInfo(groupId, index, info);
}

u32 SoundArchive::detail_GetFileCount() const
{
	return mFileReader->GetFileCount();
}

bool SoundArchive::detail_ReadFileInfo(u32 fileId, FileInfo *info) const
{
	return mFileReader->ReadFileInfo(fileId, info);
}

bool SoundArchive::detail_ReadFilePos(u32 fileId, u32 index,
                                      FilePos *info) const
{
	return mFileReader->ReadFilePos(fileId, index, info);
}

ut::FileStream *SoundArchive::detail_OpenFileStream(u32 fileId, void *buffer,
                                                    int size) const
{
	FileInfo fileInfo;
	if (!detail_ReadFileInfo(fileId, &fileInfo))
		return nullptr;

	if (fileInfo.extFilePath)
	{
		ut::FileStream *stream = OpenExtStreamImpl(
			buffer, size, fileInfo.extFilePath, 0, fileInfo.fileSize);

		return stream;
	}

	FilePos filePos;
	if (!detail_ReadFilePos(fileId, 0, &filePos))
		return nullptr;

	GroupInfo groupInfo;
	if (!detail_ReadGroupInfo(filePos.groupId, &groupInfo))
		return nullptr;

	GroupItemInfo itemInfo;
	if (!detail_ReadGroupItemInfo(filePos.groupId, filePos.index, &itemInfo))
		return nullptr;

	u32 itemOffset = groupInfo.offset + itemInfo.offset;
	u32 itemSize = itemInfo.size;

	if (groupInfo.extFilePath)
	{
		ut::FileStream *stream = OpenExtStreamImpl(
			buffer, size, groupInfo.extFilePath, itemOffset, itemSize);

		return stream;
	}
	else
	{
		ut::FileStream *stream = OpenStream(buffer, size, itemOffset, itemSize);

		return stream;
	}
}

ut::FileStream *SoundArchive::OpenExtStreamImpl(void *buffer, int size,
                                                char const *extFilePath,
                                                u32 begin, u32 length) const
{
	char const *fullPath;
	char pathBuffer[FILE_PATH_MAX + 1];

	if (extFilePath[0] == '/')
	{
		// absolute path
		fullPath = extFilePath;
	}
	else
	{
		u32 fileLen = std::strlen(extFilePath);
		u32 dirLen = std::strlen(mExtFileRoot);

		if (fileLen + dirLen >= FILE_PATH_MAX + 1)
		{
			NW4RWarningMessage_Line(349, "Too long file path \"%s/%s\"",
			                        mExtFileRoot, extFilePath);

			return nullptr;
		}

		std::strncpy(pathBuffer, mExtFileRoot, dirLen + 1);
		std::strncat(pathBuffer, extFilePath, fileLen + 1);

		fullPath = pathBuffer;
	}

	return OpenExtStream(buffer, size, fullPath, begin, length);
}

}} // namespace nw4r::snd
