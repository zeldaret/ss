#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundArchiveFile.h"
#include <string.h>

namespace nw4r {
namespace snd {

SoundArchive::SoundArchive() : mFileReader(NULL) {
    mExtFileRoot[0] = '/';
    mExtFileRoot[1] = '\0';
}

SoundArchive::~SoundArchive() {}

bool SoundArchive::IsAvailable() const {
    return mFileReader;
}

void SoundArchive::Setup(detail::SoundArchiveFileReader* pReader) {
    mFileReader = pReader;
}

void SoundArchive::Shutdown() {
    mFileReader = NULL;
    mExtFileRoot[0] = '/';
    mExtFileRoot[1] = '\0';
}

u32 SoundArchive::GetPlayerCount() const {
    return mFileReader->GetPlayerCount();
}

u32 SoundArchive::GetGroupCount() const {
    return mFileReader->GetGroupCount();
}

const char* SoundArchive::GetSoundLabelString(u32 id) const {
    return mFileReader->GetSoundLabelString(id);
}

u32 SoundArchive::ConvertLabelStringToSoundId(const char* pLabel) const {
    return mFileReader->ConvertLabelStringToSoundId(pLabel);
}

u32 SoundArchive::ConvertLabelStringToPlayerId(const char* pLabel) const {
    return mFileReader->ConvertLabelStringToPlayerId(pLabel);
}

u32 SoundArchive::ConvertLabelStringToGroupId(const char* pLabel) const {
    return mFileReader->ConvertLabelStringToGroupId(pLabel);
}

u32 SoundArchive::GetSoundUserParam(u32 id) const {
    return mFileReader->GetSoundUserParam(id);
}

SoundType SoundArchive::GetSoundType(u32 id) const {
    return mFileReader->GetSoundType(id);
}

bool SoundArchive::ReadSoundInfo(u32 id, SoundInfo* pInfo) const {
    return mFileReader->ReadSoundInfo(id, pInfo);
}

bool SoundArchive::ReadSeqSoundInfo(u32 id, SeqSoundInfo* pInfo) const {
    return mFileReader->ReadSeqSoundInfo(id, pInfo);
}

bool SoundArchive::detail_ReadStrmSoundInfo(u32 id,
                                            StrmSoundInfo* pInfo) const {
    return mFileReader->ReadStrmSoundInfo(id, pInfo);
}

bool SoundArchive::detail_ReadWaveSoundInfo(u32 id,
                                            WaveSoundInfo* pInfo) const {
    return mFileReader->ReadWaveSoundInfo(id, pInfo);
}

bool SoundArchive::ReadPlayerInfo(u32 id, PlayerInfo* pInfo) const {
    return mFileReader->ReadPlayerInfo(id, pInfo);
}

bool SoundArchive::ReadSoundArchivePlayerInfo(
    SoundArchivePlayerInfo* pInfo) const {
    return mFileReader->ReadSoundArchivePlayerInfo(pInfo);
}

bool SoundArchive::detail_ReadSound3DParam(u32 id, Sound3DParam* pParam) const {
    return mFileReader->ReadSound3DParam(id, pParam);
}

bool SoundArchive::ReadBankInfo(u32 id, BankInfo* pInfo) const {
    return mFileReader->ReadBankInfo(id, pInfo);
}

bool SoundArchive::detail_ReadGroupInfo(u32 id, GroupInfo* pInfo) const {
    return mFileReader->ReadGroupInfo(id, pInfo);
}

bool SoundArchive::detail_ReadGroupItemInfo(u32 groupId, u32 itemId,
                                            GroupItemInfo* pInfo) const {
    return mFileReader->ReadGroupItemInfo(groupId, itemId, pInfo);
}

bool SoundArchive::detail_ReadFileInfo(u32 id, FileInfo* pInfo) const {
    return mFileReader->ReadFileInfo(id, pInfo);
}

bool SoundArchive::detail_ReadFilePos(u32 fileId, u32 posId,
                                      FilePos* pPos) const {
    return mFileReader->ReadFilePos(fileId, posId, pPos);
}

ut::FileStream* SoundArchive::detail_OpenFileStream(u32 id, void* pBuffer,
                                                    int bufferSize) const {

    FileInfo fileInfo;
    if (!detail_ReadFileInfo(id, &fileInfo)) {
        return NULL;
    }

    if (fileInfo.extFilePath != NULL) {
        return OpenExtStreamImpl(pBuffer, bufferSize, fileInfo.extFilePath, 0,
                                 fileInfo.fileSize);
    }

    FilePos filePos;
    if (!detail_ReadFilePos(id, 0, &filePos)) {
        return NULL;
    }

    GroupInfo groupInfo;
    if (!detail_ReadGroupInfo(filePos.groupId, &groupInfo)) {
        return NULL;
    }

    GroupItemInfo groupItemInfo;
    if (!detail_ReadGroupItemInfo(filePos.groupId, filePos.index,
                                  &groupItemInfo)) {
        return NULL;
    }

    u32 offset = groupInfo.offset + groupItemInfo.offset;
    u32 size = groupItemInfo.size;

    if (groupInfo.extFilePath != NULL) {
        return OpenExtStreamImpl(pBuffer, bufferSize, groupInfo.extFilePath,
                                 offset, size);
    }

    return OpenStream(pBuffer, bufferSize, offset, size);
}

ut::FileStream* SoundArchive::detail_OpenGroupStream(u32 id, void* pBuffer,
                                                     int bufferSize) const {
    GroupInfo groupInfo;
    if (!detail_ReadGroupInfo(id, &groupInfo)) {
        return NULL;
    }

    if (groupInfo.extFilePath != NULL) {
        return OpenExtStreamImpl(pBuffer, bufferSize, groupInfo.extFilePath,
                                 groupInfo.offset, groupInfo.size);
    }

    return OpenStream(pBuffer, bufferSize, groupInfo.offset, groupInfo.size);
}

ut::FileStream*
SoundArchive::detail_OpenGroupWaveDataStream(u32 id, void* pBuffer,
                                             int bufferSize) const {
    GroupInfo groupInfo;
    if (!detail_ReadGroupInfo(id, &groupInfo)) {
        return NULL;
    }

    if (groupInfo.extFilePath != NULL) {
        return OpenExtStreamImpl(pBuffer, bufferSize, groupInfo.extFilePath,
                                 groupInfo.waveDataOffset,
                                 groupInfo.waveDataSize);
    }

    return OpenStream(pBuffer, bufferSize, groupInfo.waveDataOffset,
                      groupInfo.waveDataSize);
}

ut::FileStream* SoundArchive::OpenExtStreamImpl(void* pBuffer, int bufferSize,
                                                const char* pExtPath,
                                                u32 offset, u32 size) const {
    char pathBuffer[FILE_PATH_MAX];
    const char* pFullPath;

    if (pExtPath[0] == '/') {
        pFullPath = pExtPath;
    } else {
        u32 fileLen = strlen(pExtPath);
        u32 dirLen = strlen(mExtFileRoot);

        if (fileLen + dirLen >= FILE_PATH_MAX) {
            return NULL;
        }

        strncpy(pathBuffer, mExtFileRoot, dirLen + 1);
        strncat(pathBuffer, pExtPath, fileLen + 1);

        pFullPath = pathBuffer;
    }

    return OpenExtStream(pBuffer, bufferSize, pFullPath, offset, size);
}

void SoundArchive::SetExternalFileRoot(const char* pExtFileRoot) {
    u32 len = strlen(pExtFileRoot);
    u32 nullPos = len;

    if (pExtFileRoot[len - 1] != '/') {
        mExtFileRoot[len] = '/';
        nullPos++;
    }

    mExtFileRoot[nullPos] = '\0';

    // @bug Long path can overflow mExtFileRoot buffer
    strncpy(mExtFileRoot, pExtFileRoot, len);
}

} // namespace snd
} // namespace nw4r
