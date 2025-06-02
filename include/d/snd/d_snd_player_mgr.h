#ifndef D_SND_PLAYER_MGR_H
#define D_SND_PLAYER_MGR_H

#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_MemorySoundArchive.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"

class dSndPlayerMgr_c;
extern template class SndMgrDisposer<dSndPlayerMgr_c>;

/**
 * The main interface for managing sound stuff. Will delegate to dSndMgr_c for
 * most things, but handles demo (cutscene) sound effects (SE_DEMO*) by itself.
 */
class dSndPlayerMgr_c {
public:
    SndMgrDisposer<dSndPlayerMgr_c> *GetDisposer() {
        return &mDisposer;
    }

    static dSndPlayerMgr_c *GetInstance() {
        return sInstance;
    }

    static dSndPlayerMgr_c *sInstance;
    static SndMgrDisposer<dSndPlayerMgr_c> *sDisposer;

private:
    SndMgrDisposer<dSndPlayerMgr_c> mDisposer;

public:
    dSndPlayerMgr_c();

    void shutdown();
    void calc();

    u32 getFreeSize();
    bool loadDemoArchive(const char *demoArchiveName);
    const char *getSoundArchivePath();
    u32 convertLabelStringToSoundId(const char *label) const;

    nw4r::snd::SoundArchivePlayer &getSoundArchivePlayerForType(u8 type);
    bool canUseThisPlayer(u8 type) const;

    bool checkFlag(u32 mask) const {
        return mFlags & mask;
    }

private:
    /* 0x010 */ u8 field_0x010;
    /* 0x011 */ u8 field_0x011;
    /* 0x014 */ s32 field_0x014;
    /* 0x018 */ s32 field_0x018;
    /* 0x01C */ s32 field_0x01C;
    /* 0x020 */ u32 mFlags;

    virtual nw4r::snd::SoundStartable::StartResult
    startSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const nw4r::snd::SoundStartable::StartInfo *pStartInfo);
    virtual nw4r::snd::SoundStartable::StartResult
    startSound(nw4r::snd::SoundHandle *pHandle, const char *soundLabel, const nw4r::snd::SoundStartable::StartInfo *pStartInfo);

    /* 0x028 */ nw4r::snd::MemorySoundArchive mSoundArchive;
    /* 0x178 */ nw4r::snd::SoundArchivePlayer mSoundArchivePlayer;
};

#endif
