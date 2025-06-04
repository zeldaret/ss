#ifndef D_SND_CONTROL_PLAYER_MGR_H
#define D_SND_CONTROL_PLAYER_MGR_H

#include "d/snd/d_snd_control_player.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_SoundPlayer.h"
#include "nw4r/ut/ut_list.h"

/*
Num players: 0x15 = 21
Notes on player groups:
0, 1

9, 10, 11

12, 13, 14, 16, 17

17, 18, 19, 20


0: BGM

3: UI Sfx, confirmation, pointer reset
4: UI SFX, get fruit
5, 7: Player walk
8: Player Equipment

12: bomb explode, refresh fruit sprout
13: environmental sound effects (wind), bomb fuse
14: enemies/bugs

17: TgSound
*/

class dSndControlPlayerMgr_c;
extern template class SndMgrDisposer<dSndControlPlayerMgr_c>;

class dSndControlPlayerMgr_c {
public:
    SndMgrDisposer<dSndControlPlayerMgr_c> *GetDisposer() {
        return &mDisposer;
    }

    static dSndControlPlayerMgr_c *GetInstance() {
        return sInstance;
    }

    static dSndControlPlayerMgr_c *sInstance;
    static SndMgrDisposer<dSndControlPlayerMgr_c> *sDisposer;

private:
    SndMgrDisposer<dSndControlPlayerMgr_c> mDisposer;

public:
    enum PlayerCtrl_e {
        CTRL_VOLUME,
        CTRL_LPF_FREQ,
        CTRL_FX_SEND,

        CTRL_MAX,
    };

    dSndControlPlayerMgr_c();

    void calc();

    static const s32 sNumPlayers;

    nw4r::snd::SoundPlayer *getPlayer1(u32) const;
    nw4r::snd::SoundPlayer *getPlayer2(u32) const;

    void setVolume(u32 playerIdx, f32 value, s32 frames);

    void setLpfFreq(u32 playerIdx, f32 value, s32 frames);
    void setFxSend(u32 playerIdx, f32 value, s32 frames);

    void overrideVolume(u32 playerIdx, f32 volume, s32 frames);
    void restoreVolume(u32 playerIdx, s32 frames);

    void setShortParameterTo5(s32 idx1, s32 idx2);

private:
    void resetControls();
    void calcVolumes();
    void executeControls();
    void linkCtrl(dSndControlPlayer_c *);
    void unlinkCtrl(dSndControlPlayer_c *);
    void setControlValue(PlayerCtrl_e ctrlType, u32 playerIdx, f32 value, s32 frames);
    f32 getAppliedPlayerVolume(u32 playerIdx) const;
    f32 getControlVolumeTarget(PlayerCtrl_e ctrlType, u32 playerIdx) const;

    /* 0x10 */ dSndControlPlayer_c *mpCtrls[CTRL_MAX];
    /* 0x1C */ f32 *mpTargetVolumes;
    /* 0x20 */ f32 *mpMaxVolumeDecreases;
    /* 0x24 */ f32 *mpMaxVolumeIncreases;
    /* 0x28 */ f32 *mpSavedVolumes;
    /* 0x2C */ u32 mOverrideVolumeMask;
    /* 0x30 */ nw4r::ut::List mActiveControls;
    /* 0x3C */ s16 field_0x3C[4][2];
};

#endif
