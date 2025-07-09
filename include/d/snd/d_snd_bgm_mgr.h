#ifndef D_SND_BGM_MGR_H
#define D_SND_BGM_MGR_H

#include "common.h"
#include "d/snd/d_snd_actor.h"
#include "d/snd/d_snd_sound.h"
#include "d/snd/d_snd_types.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_FxReverbStdDpl2.h"
#include "nw4r/ut/ut_list.h"

SND_DISPOSER_FORWARD_DECL(dSndBgmMgr_c);

// Previous Ghidra name: FanfareSoundMgr
class dSndBgmMgr_c {
public:
    SND_DISPOSER_MEMBERS(dSndBgmMgr_c);

public:
    static bool isSoundPlayedByBgmBattlePlayer(u32 soundId);
    static bool isBgmBattle2(u32 soundId);

    enum BgmSoundList_e {
        BGM_LIST_PREPARING = 0,
        BGM_LIST_PLAYING = 1,
        BGM_LIST_STOPPING = 2,

        BGM_LIST_MAX = 3,
    };

    dSndBgmMgr_c();

    void restoreEffects();

    bool playBgm(u32 soundId, s32 fadeFrames, bool paused);
    bool prepareBgm(u32 soundId, u32 startOffset);
    void pauseAllBgm();
    void stopAllBgm(s32 fadeFrames);

    dSndBgmSound_c *getSoundHandleForBgm(u32 soundId, u32 startOffset);
    bool isPlayingBgmSound() const;
    bool isPlayingBgmSoundId(u32 soundId) const;

    bool isPreparingBgmSoundId(u32 soundId) const;
    bool isFadingOutBgmSoundId(u32 soundId) const;
    bool isPreparingBgmSoundIdWithStartOffset(u32 soundId, u32 startOffset) const;
    bool isPreparedBgmSoundId(u32 soundId) const;

    void registSound(dSndSound_c *sound);
    void unregistSound(dSndSound_c *sound);

    dSndSound_c *getFanSoundHandleForFan2(u32 soundId);
    dSndSound_c *getSoundHandleCurrentlyPlayingFanSound(u32 soundId);
    bool isCurrentlyPlayingFanSound(u32 soundId) const;
    bool hasPlayingFanSounds() const;

    bool playFanSound(u32 soundId);
    bool prepareFanSound(u32 soundId);

    void playDelayedSound(u32 soundId, s32 delay);
    void cancelDelayedSound();
    void cancelDelayedBgm();

    dSndBgmSound_c *getBgmSoundByIndex(u32 idx);

    bool prepareBossBgm(const char *name);

    /**
     * E.g. Lizalfos fight in ET. Note that in the ET case, this is only called when triggering
     * the fight without a cutscene, the cutscene code runs through "ELizarufos_c_opening"
     */
    bool beginBgmBattleRoom();
    /** Runs when the fight is over, no matter how you started it */
    bool endBgmBattleRoom();

    bool beginBgmBattleMainLoop();

    bool isPlayingAnyBattleMusic();
    void cullTooManyPreparingSounds();

    void removeFromAllBgmSoundLists(dSndBgmSound_c *sound);
    void addToBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound);
    void appendToBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound);

    // Unless there's a Ghidra decompiler bug this function
    // always returns false
    bool weirdCheckAlwaysFalse();

    dSndBgmSound_c *getActiveBgmSound();
    // I don't love this inline but it fixes one regswap
    // TODO look into getting rid of this one
    dSndBgmSound_c *getActiveBgmSound_i() {
        return getActiveBgmSound();
    }

private:
    dSndBgmBattleSound_c *getBgmBattleSound();

    bool stopBgmSound(dSndBgmSound_c *sound, s32 fadeFrames);
    void checkForPrepareStoppedBgmSound(u32 stoppedSoundId);
    dSndBgmSound_c *findNewBgmSoundHandle();
    dSndBgmSound_c *findIdleBgmSoundHandle();
    bool prepareBgmSound(u32 soundId, dSndBgmSound_c *handle, u32 startOffset);
    bool startAdditionalBgm(u32 soundId);

    bool playBattleBgm(u32 soundId, bool intense);

    /** Used to mute other BGM when battle music is playing */
    void setBgmHandleIdxVolume(u32 handleIdx, f32 volume, s32 fadeFrames);

    bool isBgmHandleIdxPlayingSoundId(u32 handleIdx, u32 soundId);
    bool stopBgmHandleIdx(u32 handleIdx, s32 fadeFrames);

    void calcLists();
    void calcStopOldBgmSounds();

    void removeFromBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound);
    bool isInBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound);
    dSndBgmSound_c *getFirstInBgmSoundList(BgmSoundList_e list);
    dSndBgmSound_c *getNextInBgmSoundList(BgmSoundList_e list, dSndBgmSound_c *sound);
    dSndBgmSound_c *getLastInBgmSoundList(BgmSoundList_e list);

    bool startDelayedSound();
    void calcDelayedSound();

    dSndBgmSound_c *getBgmSoundHandleForId(u32 soundId) const;

    dSndSound_c *getFreeFanSoundHandle();
    dSndSound_c *getFanSoundHandleForFan(u32 soundId);
    void stopFanSounds(s32 fadeFrames);

    bool isSoundRegist(dSndSound_c *sound);

    /* 0x010 */ nw4r::snd::FxReverbStdDpl2 mFx;
    /* 0x240 */ UNKWORD field_0x240;
    /* 0x244 */ UNKWORD field_0x244;
    /* 0x248 */ UNKWORD field_0x248;
    /* 0x24C */ u32 mScheduledSoundId;
    /* 0x250 */ s32 mScheduledSoundDelay;
    /* 0x254 */ dSndBgmSound_c *mBgmSounds[7];
    /* 0x270 */ dSndBgmSound_c *field_0x270;
    /* 0x274 */ dSndBgmSound_c *mpPrevActiveBgmSound;
    /* 0x278 */ nw4r::ut::List mBgmSoundLists[BGM_LIST_MAX]; // node offset 0xF0 -> dSndBgmSound_c
    /* 0x29C */ UNKWORD field_0x29C;
    /* 0x2A0 */ dSndActor_c mSoundActor;
    /* 0x2F4 */ f32 field_0x2F4;
    /* 0x2F8 */ s32 field_0x2F8;
    /* 0x2FC */ u8 field_0x2FC;
    /* 0x300 */ s32 field_0x300;
    /* 0x304 */ u8 field_0x304;
    /* 0x305 */ u8 field_0x305;
    /* 0x306 */ u8 field_0x306;
    /* 0x307 */ u8 field_0x307;
    /* 0x308 */ u8 field_0x308;
    /* 0x30C */ u32 field_0x30C[3];
    /* 0x318 */ u32 field_0x318[3];
    /* 0x324 */ u32 field_0x324[3];
    /* 0x330 */ u8 field_0x330[3];
    /* 0x333 */ u8 field_0x333;
    /* 0x334 */ dSndSound_c *mFanSounds[3];
    /* 0x340 */ u8 field_0x340;
    /* 0x344 */ LIST_MEMBER(dSndSound_c, AllSounds); // node offset 0x8
};

#endif
