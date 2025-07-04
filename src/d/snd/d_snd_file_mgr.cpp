#include "d/snd/d_snd_file_mgr.h"

#include "common.h"
#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundArchiveLoader.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"

dSndFileManager::dSndFileManagerGlob dSndFileManager::sGlob;

static dSndFileManager sFileManager;

dSndFileManager *dSndFileManager::create(nw4r::snd::SoundArchive *pArchive, nw4r::snd::SoundHeap *pHeap) {
    sGlob.numFiles = pArchive->detail_GetFileCount();
    sGlob.pData = (const void **)pHeap->Alloc(sGlob.numFiles * sizeof(const void *));
    sGlob.pWaveData = (const void **)pHeap->Alloc(sGlob.numFiles * sizeof(const void *));
    clearGlob();
    nw4r::snd::detail::DisposeCallbackManager::GetInstance().RegisterDisposeCallback(&sFileManager);
    return &sFileManager;
}

void dSndFileManager::clearGlob() {
    for (u32 i = 0; i < sGlob.numFiles; i++) {
        sGlob.pData[i] = nullptr;
        sGlob.pWaveData[i] = nullptr;
    }
}

bool dSndFileManager::loadFileForSound(const nw4r::snd::SoundArchive &pArchive, u32 soundId, nw4r::snd::SoundHeap *pHeap) {
    nw4r::snd::SoundArchive::SoundInfo info;
    if (!pArchive.ReadSoundInfo(soundId, &info)) {
        return false;
    }

    if (sGlob.pData[info.fileId] == nullptr) {
        nw4r::snd::detail::SoundArchiveLoader loader(pArchive);
        const void *data = loader.LoadFile(info.fileId, pHeap);
        if (data == nullptr) {
            return false;
        }
        sGlob.pData[info.fileId] = data;
    }
    return true;
}

bool dSndFileManager::isLoadedFileForSound(nw4r::snd::SoundArchivePlayer *pPlayer, u32 soundId) {
    const nw4r::snd::SoundArchive &pArchive = pPlayer->GetSoundArchive();
    nw4r::snd::SoundArchive::SoundInfo info;
    if (!pArchive.ReadSoundInfo(soundId, &info)) {
        return false;
    }
    
    if (pPlayer->detail_GetFileAddress(info.fileId) == nullptr) {
        return false;
    }

    return true;
}

bool dSndFileManager::isLoadedFileAndWaveForBank(nw4r::snd::SoundArchivePlayer *pPlayer, u32 soundId) {
    const nw4r::snd::SoundArchive &pArchive = pPlayer->GetSoundArchive();
    nw4r::snd::SoundArchive::BankInfo info;
    if (!pArchive.ReadBankInfo(soundId, &info)) {
        return false;
    }
    
    if (pPlayer->detail_GetFileAddress(info.fileId) == nullptr) {
        return false;
    }

    if (pPlayer->detail_GetFileWaveDataAddress(info.fileId) == nullptr) {
        return false;
    }

    return true;
}

bool dSndFileManager::isLoadedFileAndWaveForSound(nw4r::snd::SoundArchivePlayer *pPlayer, u32 soundId) {
    const nw4r::snd::SoundArchive &pArchive = pPlayer->GetSoundArchive();
    nw4r::snd::SoundArchive::SoundInfo info;
    if (!pArchive.ReadSoundInfo(soundId, &info)) {
        return false;
    }
    
    if (pPlayer->detail_GetFileAddress(info.fileId) == nullptr) {
        return false;
    }

    if (pPlayer->detail_GetFileWaveDataAddress(info.fileId) == nullptr) {
        return false;
    }

    return true;
}


void dSndFileManager::InvalidateData(void const *pStart, void const *pEnd) {
    for (u32 i = 0; i < sGlob.numFiles; i++) {
        if (pStart <= sGlob.pData[i] && sGlob.pData[i] <= pEnd) {
            sGlob.pData[i] = nullptr;
        }
    }
}

void dSndFileManager::InvalidateWaveData(void const *pStart, void const *pEnd) {
    for (u32 i = 0; i < sGlob.numFiles; i++) {
        if (pStart <= sGlob.pWaveData[i] && sGlob.pWaveData[i] <= pEnd) {
            sGlob.pWaveData[i] = nullptr;
        }
    }
}

// These two might be inline given that the order is reversed

void const *dSndFileManager::GetFileWaveDataAddress(u32 id) {
    if (id >= mpGlob->numFiles) {
        return nullptr;
    }
    return mpGlob->pWaveData[id];
}

void const *dSndFileManager::GetFileAddress(u32 id) {
    if (id >= mpGlob->numFiles) {
        return nullptr;
    }
    return mpGlob->pData[id];
}
