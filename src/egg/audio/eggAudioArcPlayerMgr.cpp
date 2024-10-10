#include <egg/audio/eggAudioArcPlayerMgr.h>
#include <egg/audio/eggAudioMgr.h>
#include <nw4r/snd/snd_SoundPlayer.h>
#include <rvl/OS/OSCache.h>

namespace EGG {

using namespace nw4r;

ArcPlayer::ArcPlayer(snd::SoundArchivePlayer *player, snd::SoundHeap *heap)
    : mIsOpeningArchive(false), mIsLoadingGroup(false), mLoadLabelStringData(true), mOpenSndArchive(NULL),
      mActiveSndArchivePlayer(player), mSoundHeap(heap), mStorage(STORAGE_NONE), mSteamBlocks(1) {}

ArcPlayer::~ArcPlayer() {}

bool ArcPlayer::setSteamBlocks(u32 n) {
    if (!mOpenSndArchive) {
        mSteamBlocks = n;
        return true;
    }

    return false;
}

UNKTYPE *ArcPlayer::openArchive(const char *name, snd::SoundHeap *heap, SARC_STORAGE storage) {
    UNKTYPE *ret = NULL;

    switch (storage) {
        case STORAGE_DVD:  ret = openDvdArchive(name, heap); break;

        case STORAGE_NAND: ret = openNandArchive(name, heap); break;

        default: break;
    }

    return ret;
}

UNKTYPE *ArcPlayer::openDvdArchive(const char *name, snd::SoundHeap *heap) {
    u32 headerBufSize;

    if (!heap) {
        heap = mSoundHeap;
    }

    if (mOpenSndArchive) {
        return mActiveSndArchivePlayer;
    }

    mOpenSndArchive = &mDvdSndArchive;

    if (mDvdSndArchive.Open(name)) {
        mIsOpeningArchive = true;

        headerBufSize = mDvdSndArchive.GetHeaderSize();
        mpHeaderBuf = heap->Alloc(headerBufSize, NULL, (void *)'ARCH');
        if (!mDvdSndArchive.LoadHeader(mpHeaderBuf, headerBufSize)) {
            mIsOpeningArchive = false;
            return NULL;
        }

        if (mLoadLabelStringData) {
            headerBufSize = mDvdSndArchive.GetLabelStringDataSize();
            void *stringDataBuf = heap->Alloc(headerBufSize, NULL, (void *)'ARCL');
            mDvdSndArchive.LoadLabelStringData(stringDataBuf, headerBufSize);
        }

        u32 setupBufSize = mActiveSndArchivePlayer->GetRequiredMemSize(mOpenSndArchive);
        void *setupBuf = heap->Alloc(setupBufSize, NULL, (void *)'APLM');

        u32 strmBufSize = mSteamBlocks * mActiveSndArchivePlayer->GetRequiredStrmBufferSize(mOpenSndArchive);
        void *strmBuf = heap->Alloc(strmBufSize, NULL, (void *)'APLS');

        if (!mActiveSndArchivePlayer->Setup(mOpenSndArchive, setupBuf, setupBufSize, strmBuf, strmBufSize)) {
            mIsOpeningArchive = false;
            return NULL;
        }

        mStorage = STORAGE_DVD;
        mIsOpeningArchive = false;
        return mActiveSndArchivePlayer;
    }

    return NULL;
}

UNKTYPE *ArcPlayer::openNandArchive(const char *name, snd::SoundHeap *heap) {
    u32 headerBufSize;

    if (!heap) {
        heap = mSoundHeap;
    }

    if (mOpenSndArchive) {
        return mActiveSndArchivePlayer;
    }

    mOpenSndArchive = &mNandSndArchive;

    if (mNandSndArchive.Open(name)) {
        mIsOpeningArchive = true;

        headerBufSize = mNandSndArchive.GetHeaderSize();
        void *headerBuf = heap->Alloc(headerBufSize, NULL, NULL);
        if (!mNandSndArchive.LoadHeader(headerBuf, headerBufSize)) {
            mIsOpeningArchive = false;
            return NULL;
        }

        if (mLoadLabelStringData) {
            headerBufSize = mNandSndArchive.GetLabelStringDataSize();
            void *stringDataBuf = heap->Alloc(headerBufSize, NULL, NULL);
            mNandSndArchive.LoadLabelStringData(stringDataBuf, headerBufSize);
        }

        u32 setupBufSize = mActiveSndArchivePlayer->GetRequiredMemSize(&mNandSndArchive);
        void *setupBuf = heap->Alloc(setupBufSize, NULL, NULL);

        u32 strmBufSize = mSteamBlocks * mActiveSndArchivePlayer->GetRequiredStrmBufferSize(&mNandSndArchive);
        void *strmBuf = heap->Alloc(strmBufSize, NULL, NULL);

        if (!mActiveSndArchivePlayer->Setup(&mNandSndArchive, setupBuf, setupBufSize, strmBuf, strmBufSize)) {
            mIsOpeningArchive = false;
            return NULL;
        }

        mStorage = STORAGE_NAND;
        mIsOpeningArchive = false;
        return mActiveSndArchivePlayer;
    }

    return NULL;
}

UNKTYPE *ArcPlayer::setupMemoryArchive(const void *work, snd::SoundHeap *heap) {
    if (!heap) {
        heap = mSoundHeap;
    }

    if (mOpenSndArchive) {
        return mActiveSndArchivePlayer;
    }

    mOpenSndArchive = &mMemorySndArchive;

    if (mMemorySndArchive.Setup(work)) {
        mIsOpeningArchive = true;

        u32 neededForMem = mActiveSndArchivePlayer->GetRequiredMemSize(&mMemorySndArchive);
        void *memBuf = heap->Alloc(neededForMem, NULL, NULL);

        u32 neededForStrm = mSteamBlocks * mActiveSndArchivePlayer->GetRequiredStrmBufferSize(&mMemorySndArchive);
        void *strmBuf = heap->Alloc(neededForStrm, NULL, NULL);

        if (!mActiveSndArchivePlayer->Setup(&mMemorySndArchive, memBuf, neededForMem, strmBuf, neededForStrm)) {
            mIsOpeningArchive = false;
            return NULL;
        }

        mStorage = STORAGE_MEM;
        mIsOpeningArchive = false;
        return mActiveSndArchivePlayer;
    }

    return NULL;
}

UNKTYPE ArcPlayer::closeArchive() {
    if (!mOpenSndArchive) {
        return;
    }

    switch (mStorage) {
        case STORAGE_DVD:  mDvdSndArchive.Close(); break;

        case STORAGE_NAND: mNandSndArchive.Close(); break;

        case STORAGE_CNT:
        case STORAGE_MEM:  mMemorySndArchive.Shutdown(); break;
    }

    mActiveSndArchivePlayer->Shutdown();

    mStorage = STORAGE_NONE;
    mOpenSndArchive = NULL;
}

bool ArcPlayer::loadGroup(unsigned int id, snd::SoundHeap *heap, u32 loadBlockSize) {
    if (mStorage == STORAGE_NAND || mStorage == STORAGE_CNT) {
        return true;
    }

    if (!heap) {
        heap = mSoundHeap;
    }

    if (mIsOpeningArchive) {
        return false;
    } else {
        if (!mActiveSndArchivePlayer->IsAvailable()) {
            return false;
        }

        mIsLoadingGroup = true;
        UNKWORD result = mActiveSndArchivePlayer->LoadGroup((u32)id, heap, loadBlockSize);
        mIsLoadingGroup = false;
        return (result != 0);
    }
}

bool ArcPlayer::loadGroup(int id, snd::SoundHeap *heap, u32 loadBlockSize) {
    if (mStorage == STORAGE_NAND || mStorage == STORAGE_CNT) {
        return true;
    }

    if (!heap) {
        heap = mSoundHeap;
    }

    if (mIsOpeningArchive) {
        return false;
    } else {
        if (!mActiveSndArchivePlayer->IsAvailable()) {
            return false;
        }

        mIsLoadingGroup = true;
        UNKWORD result = mActiveSndArchivePlayer->LoadGroup((u32)id, heap, loadBlockSize);
        mIsLoadingGroup = false;
        return (result != 0);
    }
}

bool ArcPlayer::loadGroup(u32 id, snd::SoundHeap *heap, u32 loadBlockSize) {
    if (mStorage == STORAGE_NAND || mStorage == STORAGE_CNT) {
        return true;
    }

    if (!heap) {
        heap = mSoundHeap;
    }

    if (mIsOpeningArchive) {
        return false;
    } else {
        if (!mActiveSndArchivePlayer->IsAvailable()) {
            return false;
        }

        mIsLoadingGroup = true;
        bool b = (mActiveSndArchivePlayer->LoadGroup(id, heap, loadBlockSize) != 0);
        mIsLoadingGroup = false;
        return b;
    }
}

bool ArcPlayer::loadGroup(const char *name, snd::SoundHeap *heap, u32 loadBlockSize) {
    if (mStorage == STORAGE_NAND || mStorage == STORAGE_CNT) {
        return true;
    }

    if (!heap) {
        heap = mSoundHeap;
    }

    if (mIsOpeningArchive) {
        return false;
    } else {
        if (!mActiveSndArchivePlayer->IsAvailable()) {
            return false;
        }

        mIsLoadingGroup = true;
        bool b = (mActiveSndArchivePlayer->LoadGroup(name, heap, loadBlockSize) != 0);
        mIsLoadingGroup = false;
        return b;
    }
}

void ArcPlayer::calc() {
    if (!mIsOpeningArchive) {
        mActiveSndArchivePlayer->Update();
    }
}

void ArcPlayer::stopAllSound() {
    for (int i = 0; i < mActiveSndArchivePlayer->GetSoundPlayerCount(); i++) {
        mActiveSndArchivePlayer->GetSoundPlayer(i).StopAllSound(0);
    }
}

} // namespace EGG
