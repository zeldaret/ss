#include "nw4r/snd/snd_SoundArchivePlayer.h"
#include "nw4r/snd/snd_Bank.h"
#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_PlayerHeap.h"
#include "nw4r/snd/snd_SeqFile.h"
#include "nw4r/snd/snd_SoundArchiveFile.h"
#include "nw4r/snd/snd_SoundArchiveLoader.h"
#include "nw4r/snd/snd_SoundPlayer.h"
#include "nw4r/snd/snd_MmlSeqTrack.h"
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundSystem.h"
#include "nw4r/snd/snd_StrmChannel.h"

namespace nw4r {
namespace snd {

SoundArchivePlayer::SoundArchivePlayer()
    : mSoundArchive(NULL),
      mGroupTable(NULL),
      mFileTable(NULL),
      mFileManager(NULL),
      mSeqCallback(*this),
      mWsdCallback(*this),
      mSeqUserprocCallback(NULL),
      mSeqUserprocCallbackArg(0),
      mSoundPlayerCount(0),
      mSoundPlayers(NULL),
      mMmlSeqTrackAllocator(&mMmlParser),
      mSetupBufferAddress(NULL),
      mSetupBufferSize(0) {

    detail::DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);
}

SoundArchivePlayer::~SoundArchivePlayer() {
    detail::DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(
        this);
}

bool SoundArchivePlayer::IsAvailable() const {
    if (mSoundArchive == NULL) {
        return false;
    }

    return mSoundArchive->IsAvailable();
}

bool SoundArchivePlayer::Setup(const SoundArchive* pArchive, void* pMramBuffer,
                               u32 mramBufferSize, void* pStrmBuffer,
                               u32 strmBufferSize) {
    if (!SoundSystem::IsInitializedSoundSystem()) {
        return false;
    }
    
    if (!SetupMram(pArchive, pMramBuffer, mramBufferSize)) {
        return false;
    }

    if (!SetupStrmBuffer(pArchive, pStrmBuffer, strmBufferSize)) {
        return false;
    }

    mSeqTrackAllocator = &mMmlSeqTrackAllocator;
    return true;
}

void SoundArchivePlayer::Shutdown() {
    mSoundArchive = NULL;
    mGroupTable = NULL;
    mFileTable = NULL;
    mFileManager = NULL;
    mSeqTrackAllocator = NULL;

    for (int i = 0; i < mSoundPlayerCount; i++) {
        mSoundPlayers[i].~SoundPlayer();
    }

    mSoundPlayerCount = 0;
    mSoundPlayers = NULL;

    mStrmBufferPool.Shutdown();

    if (mSetupBufferAddress != NULL) {
        mSeqSoundInstanceManager.Destroy(mSetupBufferAddress, mSetupBufferSize);
        mStrmSoundInstanceManager.Destroy(mSetupBufferAddress, mSetupBufferSize);
        mWaveSoundInstanceManager.Destroy(mSetupBufferAddress, mSetupBufferSize);
        mMmlSeqTrackAllocator.Destroy(mSetupBufferAddress, mSetupBufferSize);

        mSetupBufferAddress = NULL;
        mSetupBufferSize = 0;
    }

}

u32 SoundArchivePlayer::GetRequiredMemSize(const SoundArchive* pArchive) {
    u32 size = 0;

    u32 playerCount = pArchive->GetPlayerCount();
    size += ut::RoundUp(playerCount * sizeof(SoundPlayer), 4);

    for (u32 i = 0; i < playerCount; i++) {
        SoundArchive::PlayerInfo info;
        if (!pArchive->ReadPlayerInfo(i, &info)) {
            continue;
        }

        for (int j = 0; j < info.playableSoundCount; j++) {
            if (info.heapSize == 0) {
                continue;
            }

            size += ut::RoundUp( sizeof(detail::PlayerHeap), 4);
            size = ut::RoundUp(size, 0x20);
            size += ut::RoundUp(info.heapSize, 4);
        }
    }

    size += ut::RoundUp(
        pArchive->GetGroupCount() * sizeof(Group) + sizeof(GroupTable*), 4);

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        // clang-format off
        size += ut::RoundUp(info.seqSoundCount  * sizeof(detail::SeqSound),    4);
        size += ut::RoundUp(info.strmSoundCount * sizeof(detail::StrmSound),   4);
        size += ut::RoundUp(info.waveSoundCount * sizeof(detail::WaveSound),   4);
        size += ut::RoundUp(info.seqTrackCount  * sizeof(detail::MmlSeqTrack), 4);
        // clang-format on
    }
    size += ut::RoundUp(pArchive->GetFileCount()  * sizeof(File) + sizeof(FileTable*), 4);

    return size;
}

u32 SoundArchivePlayer::GetRequiredStrmBufferSize(
    const SoundArchive* pArchive) {
    int strmChannels = 0;

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        strmChannels = info.strmChannelCount;
    }

    // TODO: How is this calculated?
    return strmChannels * 0xA000;
}

bool SoundArchivePlayer::SetupMram(const SoundArchive* pArchive, void* pBuffer,
                                   u32 bufferSize) {
    void* pEndPtr = static_cast<u8*>(pBuffer) + bufferSize;
    void* pPtr = pBuffer;

    if (!SetupSoundPlayer(pArchive, &pPtr, pEndPtr)) {
        return false;
    }

    if (!CreateGroupAddressTable(pArchive, &pPtr, pEndPtr)) {
        return false;
    }

    if (!CreateFileAddressTable(pArchive, &pPtr, pEndPtr)) {
        return false;
    }

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        if (!SetupSeqSound(pArchive, info.seqSoundCount, &pPtr, pEndPtr)) {
            return false;
        }

        if (!SetupStrmSound(pArchive, info.strmSoundCount, &pPtr, pEndPtr)) {
            return false;
        }

        if (!SetupWaveSound(pArchive, info.waveSoundCount, &pPtr, pEndPtr)) {
            return false;
        }

        if (!SetupSeqTrack(pArchive, info.seqTrackCount, &pPtr, pEndPtr)) {
            return false;
        }
    }

    mSoundArchive = pArchive;
    mSetupBufferAddress = pBuffer;
    mSetupBufferSize = bufferSize;

    return true;
}

detail::PlayerHeap *SoundArchivePlayer::CreatePlayerHeap(void **ppBuffer, void *pEnd, u32 bufferSize) {
    void *pHeapEnd = ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, sizeof(detail::PlayerHeap)), 4);

    if (ut::ComparePtr(pHeapEnd, pEnd) > 0) {
        return NULL;
    }

    void *pHeapBuffer = *ppBuffer;
    *ppBuffer = pHeapEnd;
    detail::PlayerHeap *pHeap = new (pHeapBuffer) detail::PlayerHeap();

    *ppBuffer = ut::RoundUp(*ppBuffer, 0x20);
    pHeapEnd = ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, bufferSize), 4);

    if (ut::ComparePtr(pHeapEnd, pEnd) > 0) {
        return NULL;
    }

    pHeapBuffer = *ppBuffer;
    *ppBuffer = pHeapEnd;

    bool ok = pHeap->Create(pHeapBuffer, bufferSize);

    if (!ok) {
        return NULL;
    }

    return pHeap;
}

bool SoundArchivePlayer::SetupSoundPlayer(const SoundArchive* pArchive,
                                          void** ppBuffer, void* pEnd) {
    u32 playerCount = pArchive->GetPlayerCount();
    u32 requireSize = playerCount * sizeof(SoundPlayer);

    void* pPlayerEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pPlayerEnd, pEnd) > 0) {
        return false;
    }

    void* pPlayerBuffer = *ppBuffer;
    *ppBuffer = pPlayerEnd;

    mSoundPlayers = static_cast<SoundPlayer*>(pPlayerBuffer);
    mSoundPlayerCount = playerCount;

    u8* pPtr = static_cast<u8*>(pPlayerBuffer);

    for (u32 i = 0; i < playerCount; i++, pPtr += sizeof(SoundPlayer)) {
        SoundPlayer* pPlayer = new (pPtr) SoundPlayer();

        SoundArchive::PlayerInfo info;
        if (!pArchive->ReadPlayerInfo(i, &info)) {
            continue;
        }

        pPlayer->SetPlayableSoundCount(info.playableSoundCount);

        if (info.heapSize == 0) {
            continue;
        }

        for (int j = 0; j < info.playableSoundCount; j++) {
            detail::PlayerHeap *pHeap = CreatePlayerHeap(ppBuffer, pEnd, info.heapSize);

            if (pHeap == NULL) {
                return false;
            }

            pPlayer->detail_AppendPlayerHeap(pHeap);
        }
        pPlayer->detail_SetPlayableSoundLimit(info.playableSoundCount);
    }

    return true;
}

bool SoundArchivePlayer::CreateGroupAddressTable(const SoundArchive* pArchive,
                                                 void** ppBuffer, void* pEnd) {
    u32 requireSize =
        pArchive->GetGroupCount() * sizeof(Group) + sizeof(GroupTable*);

    void* pTableEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pTableEnd, pEnd) > 0) {
        return false;
    }

    mGroupTable = static_cast<GroupTable*>(*ppBuffer);
    *ppBuffer = pTableEnd;

    mGroupTable->count = pArchive->GetGroupCount();

    for (int i = 0; i < mGroupTable->count; i++) {
        mGroupTable->items[i].address = NULL;
        mGroupTable->items[i].waveDataAddress = NULL;
    }

    return true;
}

bool SoundArchivePlayer::CreateFileAddressTable(const SoundArchive *pArchive, void **ppBuffer, void *pEnd) {
    u32 requireSize =
        pArchive->GetFileCount() * sizeof(File) + sizeof(FileTable*);

    void* pTableEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pTableEnd, pEnd) > 0) {
        return false;
    }

    mFileTable = static_cast<FileTable*>(*ppBuffer);
    *ppBuffer = pTableEnd;

    mFileTable->count = pArchive->GetFileCount();

    for (int i = 0; i < mFileTable->count; i++) {
        mFileTable->items[i].address = NULL;
        mFileTable->items[i].waveDataAddress = NULL;
    }

    return true;
}

bool SoundArchivePlayer::SetupSeqSound(const SoundArchive* pArchive, int sounds,
                                       void** ppBuffer, void* pEnd) {
#pragma unused(pArchive)

    u32 requireSize = sounds * sizeof(detail::SeqSound);

    void* pSoundEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pSoundEnd, pEnd) > 0) {
        return false;
    }

    mSeqSoundInstanceManager.Create(*ppBuffer, requireSize);
    *ppBuffer = pSoundEnd;

    return true;
}

bool SoundArchivePlayer::SetupWaveSound(const SoundArchive* pArchive,
                                        int sounds, void** ppBuffer,
                                        void* pEnd) {
#pragma unused(pArchive)

    u32 requireSize = sounds * sizeof(detail::WaveSound);

    void* pSoundEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pSoundEnd, pEnd) > 0) {
        return false;
    }

    mWaveSoundInstanceManager.Create(*ppBuffer, requireSize);
    *ppBuffer = pSoundEnd;

    return true;
}

bool SoundArchivePlayer::SetupStrmSound(const SoundArchive* pArchive,
                                        int sounds, void** ppBuffer,
                                        void* pEnd) {
#pragma unused(pArchive)

    u32 requireSize = sounds * sizeof(detail::StrmSound);

    void* pSoundEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pSoundEnd, pEnd) > 0) {
        return false;
    }

    mStrmSoundInstanceManager.Create(*ppBuffer, requireSize);
    *ppBuffer = pSoundEnd;

    return true;
}

bool SoundArchivePlayer::SetupSeqTrack(const SoundArchive* pArchive, int tracks,
                                       void** ppBuffer, void* pEnd) {
#pragma unused(pArchive)

    u32 requireSize = tracks * sizeof(detail::MmlSeqTrack);

    void* pTrackEnd =
        ut::RoundUp(ut::AddOffsetToPtr(*ppBuffer, requireSize), 4);

    if (ut::ComparePtr(pTrackEnd, pEnd) > 0) {
        return false;
    }

    mMmlSeqTrackAllocator.Create(*ppBuffer, requireSize);
    *ppBuffer = pTrackEnd;

    return true;
}

bool SoundArchivePlayer::SetupStrmBuffer(const SoundArchive* pArchive,
                                         void* pBuffer, u32 bufferSize) {
    if (bufferSize < GetRequiredStrmBufferSize(pArchive)) {
        return false;
    }

    int strmChannels = 0;

    SoundArchive::SoundArchivePlayerInfo info;
    if (pArchive->ReadSoundArchivePlayerInfo(&info)) {
        strmChannels = info.strmChannelCount;
    }

    mStrmBufferPool.Setup(pBuffer, bufferSize, strmChannels);
    return true;
}

void SoundArchivePlayer::Update() {
    for (u32 i = 0; i < mSoundPlayerCount; i++) {
        GetSoundPlayer(i).Update();
    }

    mSeqSoundInstanceManager.SortPriorityList();
    mStrmSoundInstanceManager.SortPriorityList();
    mWaveSoundInstanceManager.SortPriorityList();
}

const SoundArchive& SoundArchivePlayer::GetSoundArchive() const {
    return *mSoundArchive;
}

SoundPlayer& SoundArchivePlayer::GetSoundPlayer(u32 i) {
    return mSoundPlayers[i];
}

const void *SoundArchivePlayer::GetFileData(u32 id) const {
    if (mFileTable == NULL) {
        return NULL;
    }
    if (id >= mFileTable->count) {
        return NULL;
    }
    return mFileTable->items[id].address;
}

const void* SoundArchivePlayer::detail_GetFileAddress(u32 id) const {
    // TODO prologue scheduling
    if (mFileManager != NULL) {
        const void* pAddr = mFileManager->GetFileAddress(id);
        if (pAddr != NULL) {
            return pAddr;
        }
    }

    const void* pAddr = mSoundArchive->detail_GetFileAddress(id);
    if (pAddr != NULL) {
        return pAddr;
    }

    pAddr = GetFileData(id);
    if (pAddr != NULL) {
        return pAddr;
    }


    SoundArchive::FileInfo file;
    if (!mSoundArchive->detail_ReadFileInfo(id, &file)) {
        return NULL;
    }

    for (unsigned int i = 0; i < file.filePosCount; i++) {
        SoundArchive::FilePos pos;
        if (!mSoundArchive->detail_ReadFilePos(id, i, &pos)) {
            continue;
        }

        const void* pGroup = GetGroupAddress(pos.groupId);
        if (pGroup == NULL) {
            continue;
        }

        SoundArchive::GroupItemInfo item;
        if (mSoundArchive->detail_ReadGroupItemInfo(pos.groupId, pos.index,
                                                    &item)) {
            return static_cast<const u8*>(pGroup) + item.offset;
        }
    }

    return NULL;
}

const void *SoundArchivePlayer::GetFileWaveData(u32 id) const {
    if (mFileTable == NULL) {
        return NULL;
    }
    if (id >= mFileTable->count) {
        return NULL;
    }
    return mFileTable->items[id].waveDataAddress;
}

const void* SoundArchivePlayer::detail_GetFileWaveDataAddress(u32 id) const {
    // TODO prologue scheduling
    if (mFileManager != NULL) {
        const void* pAddr = mFileManager->GetFileWaveDataAddress(id);
        if (pAddr != NULL) {
            return pAddr;
        }
    }

    const void* pAddr = mSoundArchive->detail_GetWaveDataFileAddress(id);
    if (pAddr != NULL) {
        return pAddr;
    }

    pAddr = GetFileWaveData(id);
    if (pAddr != NULL) {
        return pAddr;
    }

    SoundArchive::FileInfo file;
    if (!mSoundArchive->detail_ReadFileInfo(id, &file)) {
        return NULL;
    }

    for (unsigned int i = 0; i < file.filePosCount; i++) {
        SoundArchive::FilePos pos;
        if (!mSoundArchive->detail_ReadFilePos(id, i, &pos)) {
            continue;
        }

        const void* pGroup = GetGroupWaveDataAddress(pos.groupId);
        if (pGroup == NULL) {
            continue;
        }

        SoundArchive::GroupItemInfo item;
        if (mSoundArchive->detail_ReadGroupItemInfo(pos.groupId, pos.index,
                                                    &item)) {
            return static_cast<const u8*>(pGroup) + item.waveDataOffset;
        }
    }

    return NULL;
}

const void* SoundArchivePlayer::GetGroupAddress(u32 id) const {
    if (mGroupTable == NULL) {
        return NULL;
    }

    if (id >= mGroupTable->count) {
        return NULL;
    }

    return mGroupTable->items[id].address;
}

void SoundArchivePlayer::SetGroupAddress(u32 id, const void* pAddr) {
    if (mGroupTable == NULL) {
        return;
    }

    // @bug Index not validated
    mGroupTable->items[id].address = pAddr;
}

const void* SoundArchivePlayer::GetGroupWaveDataAddress(u32 id) const {
    if (mGroupTable == NULL) {
        return NULL;
    }

    if (id >= mGroupTable->count) {
        return NULL;
    }

    return mGroupTable->items[id].waveDataAddress;
}

void SoundArchivePlayer::SetGroupWaveDataAddress(u32 id, const void* pAddr) {
    if (mGroupTable == NULL) {
        return;
    }

    // @bug Index not validated
    mGroupTable->items[id].waveDataAddress = pAddr;
}

SoundStartable::StartResult SoundArchivePlayer::detail_SetupSoundImpl(
    SoundHandle* pHandle, u32 id, detail::BasicSound::AmbientInfo* pAmbientInfo, SoundActor* pSoundActor, bool hold, const SoundStartable::StartInfo* pStartInfo) {
    // TODO all of this is broken
    if (!IsAvailable()) {
        return SoundStartable::START_ERR_NOT_AVAILABLE;
    }

    if (pHandle->IsAttachedSound()) {
        pHandle->DetachSound();
    }

    SoundArchive::SoundInfo sndInfo;
    if (!mSoundArchive->ReadSoundInfo(id, &sndInfo)) {
        return SoundStartable::START_ERR_INVALID_SOUNDID;
    }

    u32 playerId = sndInfo.playerId;
    int playerPriority = sndInfo.playerPriority;

    SoundStartable::StartInfo::StartOffsetType startType =
        SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC;

    int startOffset = 0;

    if (pStartInfo != NULL) {
        if (pStartInfo->enableFlag &
            SoundStartable::StartInfo::ENABLE_START_OFFSET) {
            startType = pStartInfo->startOffsetType;
            startOffset = pStartInfo->startOffset;
        }

        if (pStartInfo->enableFlag &
            SoundStartable::StartInfo::ENABLE_PLAYER_ID) {
            playerId = pStartInfo->playerId;
        }

        if (pStartInfo->enableFlag &
            SoundStartable::StartInfo::ENABLE_PLAYER_PRIORITY) {
            playerPriority = pStartInfo->playerPriority;
        }
    }

    int playerPriorityStart = hold ? playerPriority - 1 : playerPriority;

    SoundPlayer& rPlayer = GetSoundPlayer(playerId);
    detail::BasicSound* pSound = NULL;

    switch (mSoundArchive->GetSoundType(id)) {
    case SOUND_TYPE_SEQ: {
        SoundArchive::SeqSoundInfo seqInfo;
        if (!mSoundArchive->ReadSeqSoundInfo(id, &seqInfo)) {
            return SoundStartable::START_ERR_INVALID_SOUNDID;
        }

        detail::SeqSound* pSeqSound = rPlayer.detail_AllocSeqSound(
            playerPriority, playerPriorityStart, NULL, NULL, id,
            &mSeqSoundInstanceManager);

        if (pSeqSound == NULL) {
            return SoundStartable::START_ERR_LOW_PRIORITY;
        }

        pSeqSound->SetId(id);

        SoundStartable::StartResult result = PrepareSeqImpl(
            pSeqSound, &sndInfo, &seqInfo, startType, startOffset, 1);

        if (result != SoundStartable::START_SUCCESS) {
            pSeqSound->Shutdown();
            return result;
        }

        pSound = pSeqSound;
        break;
    }

    case SOUND_TYPE_STRM: {
        SoundArchive::StrmSoundInfo strmInfo;
        if (!mSoundArchive->detail_ReadStrmSoundInfo(id, &strmInfo)) {
            return SoundStartable::START_ERR_INVALID_SOUNDID;
        }

        detail::StrmSound* pStrmSound = rPlayer.detail_AllocStrmSound(
            playerPriority, playerPriorityStart, NULL, NULL, id,
            &mStrmSoundInstanceManager);

        if (pStrmSound == NULL) {
            return SoundStartable::START_ERR_LOW_PRIORITY;
        }

        pStrmSound->SetId(id);

        SoundStartable::StartResult result = PrepareStrmImpl(
            pStrmSound, &sndInfo, &strmInfo, startType, startOffset, 1);

        if (result != SoundStartable::START_SUCCESS) {
            pStrmSound->Shutdown();
            return result;
        }

        pSound = pStrmSound;
        break;
    }

    case SOUND_TYPE_WAVE: {
        SoundArchive::WaveSoundInfo waveInfo;
        if (!mSoundArchive->detail_ReadWaveSoundInfo(id, &waveInfo)) {
            return SoundStartable::START_ERR_INVALID_SOUNDID;
        }

        detail::WaveSound* pWaveSound = rPlayer.detail_AllocWaveSound(
            playerPriority, playerPriorityStart, NULL, NULL, id,
            &mWaveSoundInstanceManager);

        if (pWaveSound == NULL) {
            return SoundStartable::START_ERR_LOW_PRIORITY;
        }

        pWaveSound->SetId(id);

        SoundStartable::StartResult result = PrepareWaveSoundImpl(
            pWaveSound, &sndInfo, &waveInfo, startType, startOffset, 1);

        if (result != SoundStartable::START_SUCCESS) {
            pWaveSound->Shutdown();
            return result;
        }

        pSound = pWaveSound;
        break;
    }

    default: {
        return SoundStartable::START_ERR_INVALID_SOUNDID;
    }
    }

    pHandle->detail_AttachSound(pSound);
    return SoundStartable::START_SUCCESS;
}

SoundStartable::StartResult SoundArchivePlayer::PrepareSeqImpl(
    detail::SeqSound *pSound, const SoundArchive::SoundInfo *pSndInfo, const SoundArchive::SeqSoundInfo *pSeqInfo,
    SoundStartable::StartInfo::StartOffsetType startType, int startOffset, int voices
) {
    const void *pSeqBin = detail_GetFileAddress(pSndInfo->fileId);
    ut::FileStream *pFileStream = NULL;

    if (pSeqBin == NULL) {
        detail::PlayerHeap *pHeap = pSound->GetPlayerHeap();
        if (pHeap == NULL) {
            return SoundStartable::START_ERR_NOT_DATA_LOADED;
        }

        void *pStreamBuffer = pSound->GetFileStreamBuffer();
        u32 streamBufferSize = pSound->GetFileStreamBufferSize();

        pFileStream = mSoundArchive->detail_OpenFileStream(pSndInfo->fileId, pStreamBuffer, streamBufferSize);

        if (pFileStream == NULL) {
            return SoundStartable::START_ERR_CANNOT_OPEN_FILE;
        }

        if (pHeap->GetFreeSize() < pFileStream->GetSize()) {
            pFileStream->Close();
            return SoundStartable::START_ERR_NOT_ENOUGH_PLAYER_HEAP;
        }
    }

    detail::SeqPlayer::SetupResult result = pSound->Setup(mSeqTrackAllocator, pSeqInfo->allocTrack, &mSeqCallback);

    while (result != detail::SeqPlayer::SETUP_SUCCESS) {
        if (result == detail::SeqPlayer::SETUP_ERR_CANNOT_ALLOCATE_TRACK) {
            if (mSeqSoundInstanceManager.GetActiveCount() == 1) {
                if (pFileStream != NULL) {
                    pFileStream->Close();
                }

                return SoundStartable::START_ERR_LOW_PRIORITY;
            }

            detail::BasicSound *pDropSound = mSeqSoundInstanceManager.GetLowestPrioritySound();

            if (pSound == pDropSound) {
                if (pFileStream != NULL) {
                    pFileStream->Close();
                }

                return SoundStartable::START_ERR_LOW_PRIORITY;
            }

            pDropSound->Stop(0);
            result = pSound->Setup(mSeqTrackAllocator, pSeqInfo->allocTrack, &mSeqCallback);
        } else {
            return SoundStartable::START_ERR_UNKNOWN;
        }
    }

    pSound->SetInitialVolume(pSndInfo->volume / 127.0f);
    pSound->SetChannelPriority(pSeqInfo->channelPriority);
    pSound->SetReleasePriorityFix(pSeqInfo->releasePriorityFixFlag);
    pSound->SetRemoteFilter(pSndInfo->remoteFilter);
    pSound->SetPanMode(pSndInfo->panMode);
    pSound->SetPanCurve(pSndInfo->panCurve);

    detail::SeqPlayer::OffsetType seqOffsetType;

    switch (startType) {
        case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC: {
            seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_MILLISEC;
            break;
        }

        case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK: {
            seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_TICK;
            break;
        }

        case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE: {
            seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_TICK;
            startOffset = 0;
            break;
        }

        default: {
            seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_TICK;
            startOffset = 0;
            break;
        }
    }

    if (pSeqBin != NULL) {
        detail::SeqFileReader reader(pSeqBin);
        pSound->Prepare(reader.GetBaseAddress(), pSeqInfo->dataOffset, seqOffsetType, startOffset);
    } else {
        pSound->Prepare(pFileStream, pSeqInfo->dataOffset, seqOffsetType, startOffset);
    }

    return SoundStartable::START_SUCCESS;
}

SoundStartable::StartResult
SoundArchivePlayer::detail_SetupSound(SoundHandle *pHandle, u32 id, bool hold, const StartInfo *pStartInfo) {
    return detail_SetupSoundImpl(pHandle, id, NULL, NULL, hold, pStartInfo);
}

SoundStartable::StartResult SoundArchivePlayer::PrepareStrmImpl(
    detail::StrmSound* pSound, const SoundArchive::SoundInfo* pSndInfo,
    const SoundArchive::StrmSoundInfo* pStrmInfo,
    SoundStartable::StartInfo::StartOffsetType startType, int startOffset,
    int voices) {

    // StrmSoundInfo is empty in this version of NW4R
#pragma unused(pStrmInfo)

    detail::StrmPlayer::StartOffsetType strmOffsetType;

    switch (startType) {
    case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_MILLISEC;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
        break;
    }

    default: {
        strmOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }
    }

    void* pStreamBuffer = pSound->GetFileStreamBuffer();
    u32 streamBufferSize = pSound->GetFileStreamBufferSize();

    ut::FileStream* pFileStream = mSoundArchive->detail_OpenFileStream(
        pSndInfo->fileId, pStreamBuffer, streamBufferSize);

    if (pFileStream == NULL) {
        return SoundStartable::START_ERR_CANNOT_OPEN_FILE;
    }

    if (!pSound->Prepare(&mStrmBufferPool, strmOffsetType, startOffset, voices,
                         pFileStream)) {
        return SoundStartable::START_ERR_UNKNOWN;
    }

    pSound->SetInitialVolume(pSndInfo->volume / 127.0f);
    pSound->SetRemoteFilter(pSndInfo->remoteFilter);
    pSound->SetPanMode(pSndInfo->panMode);
    pSound->SetPanCurve(pSndInfo->panCurve);

    return SoundStartable::START_SUCCESS;
}

SoundStartable::StartResult SoundArchivePlayer::PrepareWaveSoundImpl(
    detail::WaveSound* pSound, const SoundArchive::SoundInfo* pSndInfo,
    const SoundArchive::WaveSoundInfo* pWsdInfo,
    SoundStartable::StartInfo::StartOffsetType startType, int startOffset,
    int voices) {

    const void* pWsdBin = detail_GetFileAddress(pSndInfo->fileId);
    if (pWsdBin == NULL) {
        return SoundStartable::START_ERR_NOT_DATA_LOADED;
    }

    detail::WsdPlayer::StartOffsetType wsdOffsetType;

    switch (startType) {
    case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC: {
        wsdOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_MILLISEC;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK: {
        wsdOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }

    case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE: {
        wsdOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_SAMPLE;
        break;
    }

    default: {
        wsdOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_SAMPLE;
        startOffset = 0;
        break;
    }
    }

    if (!pSound->Prepare(pWsdBin, pWsdInfo->subNo, wsdOffsetType, startOffset,
                         &mWsdCallback, pSndInfo->fileId)) {
        return SoundStartable::START_ERR_UNKNOWN;
    }

    pSound->SetInitialVolume(pSndInfo->volume / 127.0f);
    pSound->SetChannelPriority(pWsdInfo->channelPriority);
    pSound->SetReleasePriorityFix(pWsdInfo->releasePriorityFixFlag);
    pSound->SetRemoteFilter(pSndInfo->remoteFilter);
    pSound->SetPanMode(pSndInfo->panMode);
    pSound->SetPanCurve(pSndInfo->panCurve);

    return SoundStartable::START_SUCCESS;
}

bool SoundArchivePlayer::LoadGroup(u32 id, SoundMemoryAllocatable* pAllocatable,
                                   u32 blockSize) {
    if (!IsAvailable()) {
        return false;
    }

    if (id >= mSoundArchive->GetGroupCount()) {
        return false;
    }

    if (GetGroupAddress(id) != NULL) {
        return true;
    }

    if (pAllocatable == NULL) {
        return false;
    }

    detail::SoundArchiveLoader loader(*mSoundArchive);

    void* pWaveBuffer;
    const void* pGroup =
        loader.LoadGroup(id, pAllocatable, &pWaveBuffer, blockSize);

    if (pGroup == NULL) {
        return NULL;
    }

    SetGroupAddress(id, pGroup);
    SetGroupWaveDataAddress(id, pWaveBuffer);

    return true;
}

bool SoundArchivePlayer::LoadGroup(const char* pLabel,
                                   SoundMemoryAllocatable* pAllocatable,
                                   u32 blockSize) {
    u32 id = mSoundArchive->ConvertLabelStringToGroupId(pLabel);
    if (id == SoundArchive::INVALID_ID) {
        return false;
    }

    return LoadGroup(id, pAllocatable, blockSize);
}

void SoundArchivePlayer::InvalidateData(const void* pStart, const void* pEnd) {
    if (mFileTable != NULL) {
        for (int i = 0; i < mFileTable->count; i++) {
            const void* pAddr = mFileTable->items[i].address;

            if (pStart <= pAddr && pAddr <= pEnd) {
                mFileTable->items[i].address = NULL;
            }
        }
    }

    if (mGroupTable != NULL) {
        for (int i = 0; i < mGroupTable->count; i++) {
            const void* pAddr = mGroupTable->items[i].address;

            if (pStart <= pAddr && pAddr <= pEnd) {
                mGroupTable->items[i].address = NULL;
            }
        }
    }
}

void SoundArchivePlayer::InvalidateWaveData(const void* pStart,
                                            const void* pEnd) {
    if (mFileTable != NULL) {
        for (int i = 0; i < mFileTable->count; i++) {
            const void* pAddr = mFileTable->items[i].waveDataAddress;

            if (pStart <= pAddr && pAddr <= pEnd) {
                mFileTable->items[i].waveDataAddress = NULL;
            }
        }
    }

    if (mGroupTable != NULL) {
        for (int i = 0; i < mGroupTable->count; i++) {
            const void* pAddr = mGroupTable->items[i].waveDataAddress;

            if (pStart <= pAddr && pAddr <= pEnd) {
                mGroupTable->items[i].waveDataAddress = NULL;
            }
        }
    }
}

detail::Channel*
SoundArchivePlayer::SeqNoteOnCallback::NoteOn(detail::SeqPlayer* pSeqPlayer,
                                              int bankNo,
                                              const detail::NoteOnInfo& rInfo) {
#pragma unused(bankNo)

    if (!mSoundArchivePlayer.IsAvailable()) {
        return NULL;
    }

    const SoundArchive& rArchive = mSoundArchivePlayer.GetSoundArchive();
    u32 soundId = pSeqPlayer->GetId();

    SoundArchive::SeqSoundInfo seqInfo;
    if (!rArchive.ReadSeqSoundInfo(soundId, &seqInfo)) {
        return NULL;
    }

    SoundArchive::BankInfo bankInfo;
    if (!rArchive.ReadBankInfo(seqInfo.bankId, &bankInfo)) {
        return NULL;
    }

    const void* pBankBin =
        mSoundArchivePlayer.detail_GetFileAddress(bankInfo.fileId);

    if (pBankBin == NULL) {
        return NULL;
    }

    detail::Bank bank(pBankBin);

    const void* pWaveData =
        mSoundArchivePlayer.detail_GetFileWaveDataAddress(bankInfo.fileId);

    if (pWaveData == NULL) {
        return NULL;
    }

    bank.SetWaveDataAddress(pWaveData);
    return bank.NoteOn(rInfo);
}

bool SoundArchivePlayer::WsdCallback::GetWaveSoundData(
    detail::WaveSoundInfo* pSoundInfo, detail::WaveSoundNoteInfo* pNoteInfo,
    detail::WaveInfo* pWaveData, const void* pWsdData, int index, int noteIndex,
    u32 callbackArg) const {

    u32 fileId = callbackArg;

    if (!mSoundArchivePlayer.IsAvailable()) {
        return false;
    }

    const void* pWaveAddr =
        mSoundArchivePlayer.detail_GetFileWaveDataAddress(fileId);

    if (pWaveAddr == NULL) {
        return false;
    }

    detail::WsdFileReader reader(pWsdData);

    if (!reader.ReadWaveSoundInfo(pSoundInfo, index)) {
        return false;
    }

    if (!reader.ReadWaveSoundNoteInfo(pNoteInfo, index, noteIndex)) {
        return false;
    }

    if (!reader.ReadWaveParam(pNoteInfo->waveIndex, pWaveData, pWaveAddr)) {
        return false;
    }

    return true;
}

} // namespace snd
} // namespace nw4r
