#ifndef D_SND_BGM_SOUND_H
#define D_SND_BGM_SOUND_H

#include "common.h"
#include "d/snd/d_snd_bgm_sound_harp_mgr.h"
#include "d/snd/d_snd_sound.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/ut/ut_list.h"

class dSndBgmSound_c : public dSndSound_c {
    enum BgmSoundFlags_e {
        BGM_FLAG_BATTLE = 0x1,
        BGM_FLAG_0x8 = 0x8,
        BGM_FLAG_0x10 = 0x10,
        BGM_FLAG_PAUSED = 0x20,
        BGM_FLAG_0x40 = 0x40,
    };

public:
    typedef void (*StopCallback)(bool);
    typedef void (*CalcCallback)(dSndBgmSound_c *sound);

    dSndBgmSound_c();

    virtual void cancel() override;                                                                           // vt 0x08
    virtual void calc() override;                                                                             // vt 0x0C
    virtual void fadeIn(u32 id, s32 fadeFrames) override;                                                     // vt 0x10
    virtual nw4r::snd::SoundStartable::StartResult prepareSound(u32 soundId, u32 startOffset) override;       // vt 0x14
    virtual nw4r::snd::SoundStartable::StartResult prepareSound(const char *label, u32 startOffset) override; // vt 0x18
    virtual void onPreparing(u32 soundId, u32 startOffset) override;                                          // vt 0x1C
    virtual void stop(s32 fadeFrames) override;                                                               // vt 0x20
    virtual void pause(bool pauseFlag, s32 fadeFrames) override;                                              // vt 0x24

    virtual void init(nw4r::snd::SoundStartable &startable, s32 idx); // vt 0x28
    virtual bool isBattleBgmSound() const {
        return false;
    }                                         // vt 0x2C
    virtual void pause(s32 fadeFrames);       // vt 0x30
    virtual void loadSeqConfig(u32 soundId);  // vt 0x34
    virtual void postCalc() {}                // vt 0x38
    virtual void calcSeqPlaySamplePosition(); // vt 0x3C
    virtual void loadCallbacks(u32 soundId);  // vt 0x40

    nw4r::snd::SoundStartable::StartResult startBgmSound(u32 soundId, s32 fadeFrames, u32 startOffset);

    bool isPreparedSoundId(u32 soundId) const {
        return IsPrepared() && !mIsRunning && GetId() == soundId;
    }

    bool isPrepared() const {
        return IsPrepared() && !mIsRunning;
    }

    void applyVars();
    void onBecomeActive();
    bool isBgmBattle() const;

    const dSndBgmDataHarpVarSetBase_c *getCurrentHarpVarSet();

    u32 getStrmPlaySamplePosition();
    u32 getWavePlaySamplePosition();

protected:
    void getHarpData(u32 soundId);
    bool loadNewHarpData(u32 soundId);

    void calcPlaySamplePosition();
    u32 getPlaySamplePosition();
    u32 getSeqSoundPlaySamplePosition();
    static dSndBgmSoundHarpMgr_c *spGlobalHarpMgr;

    /* 0x0F0 */ nw4r::ut::Node mBgmMgrNode;
    /* 0x0F8 */ nw4r::snd::SoundStartable *mpStartable;
    /* 0x0FC */ dSoundSource_c *mpSoundSource;
    /* 0x100 */ s32 mIndex;
    /* 0x104 */ UNKWORD field_0x104;
    /* 0x108 */ s32 mSeqTempo;
    /* 0x10C */ u32 mBgmFlags;
    /* 0x110 */ bool mDidRewindPlaySamplePosition;
    /* 0x114 */ s32 mBgmVar3;
    /* 0x118 */ s32 mSeqTimebase;
    /* 0x11C */ u32 mPlaySamplePosition;
    /* 0x120 */ dSndBgmSoundHarpMgr_c mHarpMgr;
    /* 0x140 */ dSndBgmSoundHarpMgr_c *mpHarpMgr;
    /* 0x144 */ const dSndBgmSeqConfig *mpSeqConfig;
    /* 0x148 */ s32 mSeqPlaySamplePosition;
    /* 0x14C */ UNKWORD field_0x14C[2];
    /* 0x154 */ f32 field_0x154[2];
    /* 0x15C */ StopCallback mpStopCallback;
    /* 0x160 */ CalcCallback mpCalcCallback;
    /* 0x164 */ void *cb3;
    /* 0x168 */ void *cb4;
    /* 0x16C */ void *cb5;
    /* 0x170 */ void *cb6;
    /* 0x174 */ void *cb7;
    /* 0x178 */ void *cb8;
    /* 0x17C */ void *cb9;
    /* 0x180 */ void *cb10;
};

#endif
