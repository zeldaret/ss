#ifndef D_SND_MGR_H
#define D_SND_MGR_H

#include "egg/audio/eggAudioMgr.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"

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

    static nw4r::snd::SoundArchivePlayer* getPlayer() {
        return sInstance->ArcPlayer::getPlayer();
    }

private:
    static dSndMgr_c *sInstance;

    /* 0x6C8 */ u8 _0x6C8[0x6CC - 0x6C8];
    /* 0x6CC */ u8 field_0x6CC;
};

#endif
