#ifndef D_SND_MGR_H
#define D_SND_MGR_H

#include "common.h"
#include "egg/audio/eggAudioMgr.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"

/** The core audio manager used for most sounds. */
class dSndMgr_c : public EGG::SimpleAudioMgr {
public:
    dSndMgr_c();

    virtual void calc() override;
    void initialize(EGG::Heap *heap, u32 size);
    void initHbm(u32 frame);

    static void restoreEffectsCallback();

    static dSndMgr_c *GetInstance() {
        return sInstance;
    }

    static nw4r::snd::SoundArchivePlayer& getPlayer() {
        return *sInstance->ArcPlayer::getPlayer();
    }

    static const char *getSoundLabelString(u32 soundId) {
        const char *label = nullptr;
        if (sInstance->getArchive() != nullptr) {
            label = sInstance->getArchive()->GetSoundLabelString(soundId);
        }
        return label;
    }

    static u32 getPlayerId(u32 soundId) {
        nw4r::snd::SoundArchive::SoundInfo info;
        u32 playerId;
        if (!sInstance->getArchive()->ReadSoundInfo(soundId, &info)) {
            playerId = -1;
        } else {
            playerId = info.playerId;
        }
        return playerId;
    }

private:
    static dSndMgr_c *sInstance;

    /* 0x6C8 */ u8 _0x6C8[0x6CC - 0x6C8];
    /* 0x6CC */ u8 field_0x6CC;
};

#endif
