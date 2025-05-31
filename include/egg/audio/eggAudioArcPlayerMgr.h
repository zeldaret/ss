#ifndef EGG_AUDIO_ARC_PLAYER_MANAGER_H
#define EGG_AUDIO_ARC_PLAYER_MANAGER_H

#include "common.h"
#include "nw4r/snd/snd_DvdSoundArchive.h"
#include "nw4r/snd/snd_MemorySoundArchive.h"
#include "nw4r/snd/snd_NandSoundArchive.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"
#include "nw4r/snd/snd_SoundHeap.h"

namespace EGG {

class ArcPlayer {
public:
    enum SARC_STORAGE {
        STORAGE_NONE,
        STORAGE_DVD,
        STORAGE_NAND,
        STORAGE_CNT,
        STORAGE_MEM
    };

    ArcPlayer(nw4r::snd::SoundArchivePlayer *, nw4r::snd::SoundHeap *);
    virtual ~ArcPlayer(); // at 0x8
    bool setSteamBlocks(u32);
    void stopAllSound();

    u32 changeNameToId(const char *name) {
        u32 id = -1;
        if (mOpenSndArchive != nullptr) {
            id = mOpenSndArchive->ConvertLabelStringToSoundId(name);
        }
        return id;
    }
    nw4r::snd::SoundArchivePlayer *getPlayer() {
        return mActiveSndArchivePlayer;
    }

    nw4r::snd::SoundArchive *getArchive() {
        return mOpenSndArchive;
    }

    void setLoadStringLabels(bool bLoad) {
        mLoadLabelStringData = bLoad;
    }

    virtual UNKTYPE *openArchive(const char *, nw4r::snd::SoundHeap *, SARC_STORAGE); // at 0xC
    virtual UNKTYPE *openDvdArchive(const char *, nw4r::snd::SoundHeap *);            // at 0x10
    virtual UNKTYPE *openNandArchive(const char *, nw4r::snd::SoundHeap *);           // at 0x14
    virtual UNKTYPE *setupMemoryArchive(const void *, nw4r::snd::SoundHeap *);        // at 0x18

    virtual UNKTYPE *setupMemoryArchive(const void *p, nw4r::snd::SoundHeap *heap, s32) // at 0x1C
    {
        return setupMemoryArchive(p, heap);
    }

    virtual void closeArchive();                                       // at 0x20
    virtual bool loadGroup(unsigned int, nw4r::snd::SoundHeap *, u32); // at 0x24
    virtual bool loadGroup(int, nw4r::snd::SoundHeap *, u32);          // at 0x28
    virtual bool loadGroup(u32, nw4r::snd::SoundHeap *, u32);          // at 0x2C
    virtual bool loadGroup(const char *, nw4r::snd::SoundHeap *, u32); // at 0x30
    virtual void calc();                                               // at 0x34

    virtual bool startSound(nw4r::snd::SoundHandle *handle, u32 id) // at 0x38
    {
        return mActiveSndArchivePlayer->StartSound(handle, id);
    }

    virtual bool startSound(nw4r::snd::SoundHandle *handle, unsigned int id) // at 0x3C
    {
        return ArcPlayer::startSound(handle, (u32)id);
    }

    virtual bool startSound(nw4r::snd::SoundHandle *handle, const char *name) // at 0x40
    {
        return ArcPlayer::startSound(handle, changeNameToId(name));
    }

    virtual bool prepareSound(nw4r::snd::SoundHandle *handle, u32 id) // at 0x44
    {
        return mActiveSndArchivePlayer->PrepareSound(handle, id);
    }

    virtual bool prepareSound(nw4r::snd::SoundHandle *handle, unsigned int id) // at 0x48
    {
        return ArcPlayer::prepareSound(handle, (u32)id);
    }

    virtual bool prepareSound(nw4r::snd::SoundHandle *handle, const char *name) // at 0x4C
    {
        return ArcPlayer::prepareSound(handle, changeNameToId(name));
    }

    virtual bool holdSound(nw4r::snd::SoundHandle *handle, u32 id) // at 0x50
    {
        return mActiveSndArchivePlayer->HoldSound(handle, id);
    }

    virtual bool holdSound(nw4r::snd::SoundHandle *handle, unsigned int id) // at 0x54
    {
        return ArcPlayer::holdSound(handle, (u32)id);
    }

    virtual bool holdSound(nw4r::snd::SoundHandle *handle, const char *name) // at 0x58
    {
        return ArcPlayer::holdSound(handle, changeNameToId(name));
    }

private:
    /* 0x004 */ bool mIsOpeningArchive;
    /* 0x005 */ bool mIsLoadingGroup;
    /* 0x006 */ bool mLoadLabelStringData;
    /* 0x008 */ nw4r::snd::SoundArchive *mOpenSndArchive;
    /* 0x00C */ nw4r::snd::DvdSoundArchive mDvdSndArchive;
    /* 0x198 */ nw4r::snd::NandSoundArchive mNandSndArchive;
    /* 0x374 */ nw4r::snd::MemorySoundArchive mMemorySndArchive;
    /* 0x4C4 */ nw4r::snd::SoundArchivePlayer *mActiveSndArchivePlayer;
    /* 0x4C8 */ nw4r::snd::SoundHeap *mSoundHeap;
    /* 0x4CC */ SARC_STORAGE mStorage;
    /* 0x4D0 */ u32 mSteamBlocks;
    /* 0x4D4 */ void *mpHeaderBuf;
};

} // namespace EGG

#endif
