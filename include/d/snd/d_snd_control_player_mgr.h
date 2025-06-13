#ifndef D_SND_CONTROL_PLAYER_MGR_H
#define D_SND_CONTROL_PLAYER_MGR_H

#include "d/snd/d_snd_control_player.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_SoundPlayer.h"
#include "nw4r/ut/ut_list.h"

SND_DISPOSER_FORWARD_DECL(dSndControlPlayerMgr_c);

class dSndControlPlayerMgr_c {
    SND_DISPOSER_MEMBERS(dSndControlPlayerMgr_c);

public:
    enum PlayerCtrl_e {
        CTRL_VOLUME = 0,
        CTRL_LPF_FREQ = 1,
        CTRL_FX_SEND = 2,

        CTRL_MAX = 3,
    };

    dSndControlPlayerMgr_c();

    void calc();

    static const s32 sNumPlayers;
    static const s32 sPlayerMax;

    nw4r::snd::SoundPlayer *getPlayer1(u32) const;
    nw4r::snd::SoundPlayer *getPlayer2(u32) const;

    void setVolume(u32 playerIdx, f32 value, s32 frames);

    void setLpfFreq(u32 playerIdx, f32 value, s32 frames);
    void setFxSend(u32 playerIdx, f32 value, s32 frames);

    void overrideVolume(u32 playerIdx, f32 volume, s32 frames);
    void restoreVolume(u32 playerIdx, s32 frames);

    void muteAllWorldSounds(s32 fadeFrames);
    void unmuteAllWorldSounds(s32 fadeFrames);

    enum MuteLevel {
        MUTE_FULL = 0,
        MUTE_PARTIAL = 1,

        MUTE_MAX = 2,
    };

    enum VolumeControlGroup {
        /** PLAYER_BGM, PLAYER_BGM_BATTLE */
        CTRL_GROUP_BGM = 0,
        /** PLAYER_TG_SOUND - PLAYER_AREA_IN_WATER_LV */
        CTRL_GROUP_STAGE_EFFECTS = 1,
        /** PLAYER_ENEMY - PLAYER_ENEMY_FOOTSTEP */
        CTRL_GROUP_ENEMY = 2,
        /* PLAYER_OBJECT_1 - PLAYER_TG_SOUND, but not PLAYER_NPC_VOICE */
        CTRL_GROUP_OBJECTS = 3,

        CTRL_GROUP_MAX = 4,
    };
    void setGroupVolumeFlag(VolumeControlGroup group, MuteLevel level);

    void setBgmVolumeDecreaseSpeed(f32 speed);
    void setStageEffectsVolume(f32 volume, s32 fadeFrames);

private:
    void setBgmMuteVolume(f32 volume);
    void setStageEffectsMuteVolume(f32 volume);
    void setEnemyMuteVolume(f32 volume);
    void setObjectMuteVolume(f32 volume);
    void setPlayerVolumeInternal(u32 playerIdx, f32 volume);

    void resetControls();
    void calcVolumes();
    void calcMuteFlags();
    void executeControls();
    void linkCtrl(dSndControlPlayer_c *);
    void unlinkCtrl(dSndControlPlayer_c *);
    void setControlValue(PlayerCtrl_e ctrlType, u32 playerIdx, f32 value, s32 frames);
    f32 getAppliedPlayerVolume(u32 playerIdx) const;
    f32 getControlTarget(PlayerCtrl_e ctrlType, u32 playerIdx) const;

    /* 0x10 */ dSndControlPlayer_c *mpCtrls[CTRL_MAX];
    /* 0x1C */ f32 *mpTargetVolumes;
    /* 0x20 */ f32 *mpMaxVolumeDecreases;
    /* 0x24 */ f32 *mpMaxVolumeIncreases;
    /* 0x28 */ f32 *mpSavedVolumes;
    /* 0x2C */ u32 mOverrideVolumeMask;
    /* 0x30 */ nw4r::ut::List mActiveControls;
    /* 0x3C */ s16 mTimersForGroupVolume[4][2];
};

#endif
