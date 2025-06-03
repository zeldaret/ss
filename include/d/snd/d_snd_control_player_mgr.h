#ifndef D_SND_CONTROL_PLAYER_MGR_H
#define D_SND_CONTROL_PLAYER_MGR_H

#include "d/snd/d_snd_control_player.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_SoundPlayer.h"
#include "nw4r/ut/ut_list.h"

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

    nw4r::snd::SoundPlayer *getPlayer1(s32);
    nw4r::snd::SoundPlayer *getPlayer2(s32);

    void setLpfFreq(u32 playerIdx, f32 value, s32 frames);
    void setFxSend(u32 playerIdx, f32 value, s32 frames);

private:
    void resetControls();
    void executeControls();
    void linkCtrl(dSndControlPlayer_c *);
    void unlinkCtrl(dSndControlPlayer_c *);
    void setControlValue(PlayerCtrl_e ctrlType, u32 playerIdx, f32 value, s32 frames);

    f32 getTargetValue(PlayerCtrl_e ctrlType, u32 playerIdx) const;

    /* 0x10 */ dSndControlPlayer_c *mpCtrls[CTRL_MAX];
    /* 0x1C */ void *field_0x1C;
    /* 0x20 */ void *field_0x20;
    /* 0x24 */ void *field_0x24;
    /* 0x28 */ f32 *field_0x28;
    /* 0x2C */ u32 mControlMask;
    /* 0x30 */ nw4r::ut::List mActiveControls;
    /* 0x3C */ s16 field_0x3C[8];
};

#endif
