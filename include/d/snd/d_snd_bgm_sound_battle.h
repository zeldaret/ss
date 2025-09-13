#ifndef D_SND_BGM_SOUND_BATTLE_H
#define D_SND_BGM_SOUND_BATTLE_H

#include "d/snd/d_snd_bgm_sound.h"
#include "d/snd/d_snd_types.h"

class dSndBgmBattleSound_c : public dSndBgmSound_c {
public:
    dSndBgmBattleSound_c();

    virtual void cancel() override;                       // vt 0x08
    virtual void fadeIn(u32 id, s32 fadeFrames) override; // vt 0x10
    virtual bool isBattleBgmSound() const override {
        return true;
    } // vt 0x2C
    virtual void loadSeqConfig(u32 soundId) override; // vt 0x34
    virtual void postCalc() override {
        calcSeqPlaySamplePosition();
    } // vt 0x38
    virtual void calcSeqPlaySamplePosition() override; // vt 0x3C
    virtual void loadCallbacks(u32 soundId) override;  // vt 0x40

    /**
     * Called when the player is even closer to an enemy.
     * Adds some more instruments to the battle sound.
     */
    bool startMainBattleLoop();

private:
    void setTrackGroupMuted(u32 groupId);
    void setTrackGroupUnmuted(u32 groupId);

    /* 0x184 */ const dSndBgmBattleConfig *mpBgmBattleConfig;
    /* 0x188 */ u32 mMuteGroupActiveMask;
    /* 0x18C */ u32 mPrevTick;
    /* 0x190 */ u8 field_0x190;
    /* 0x191 */ u8 field_0x191;
};

#endif
