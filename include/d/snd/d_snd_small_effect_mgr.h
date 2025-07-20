#ifndef D_SND_SMALL_EFFECT_MGR_H
#define D_SND_SMALL_EFFECT_MGR_H

#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h" // IWYU pragma: export
#include "nw4r/math/math_types.h"
#include "nw4r/snd/snd_SoundHandle.h"

SND_DISPOSER_FORWARD_DECL(dSndSmallEffectMgr_c);

/**
 * An interface for playing the SE_S ("sound effect, small"?) sounds.
 */
class dSndSmallEffectMgr_c {
    SND_DISPOSER_MEMBERS(dSndSmallEffectMgr_c)

    static const s32 NUM_DELAYED_SOUNDS = 2;
    static const s32 NUM_HOLD_SOUNDS = 3;

public:
    enum BattleHitSound_e {
        BATTLE_TUTTI_NORMAL = 0,
        BATTLE_TUTTI_TURN = 1,
        BATTLE_TUTTI_JUMP = 2,
        BATTLE_TUTTI_FINISH = 3,
        BATTLE_TUTTI_GUARDJUST = 5,
    };

    dSndSmallEffectMgr_c();

    void initialize();
    void calc();
    void calcTimer();

    void stopAllSound(s32 fadeFrames);
    void stopAllSoundDemoRelated(s32 fadeFrames); // TODO: better name
    void stopAllSoundExceptEvent(s32 fadeFrames);
    void stopAllSoundExceptEffectOrLink(s32 fadeFrames);

    bool playSound(u32 soundId);
    // used for clawshots cursor, pan depends on where on the screen
    // your cursor is when it activates
    bool playSoundWithPan(u32 soundId, f32 pan);
    bool holdSound(u32 soundId);
    bool holdSoundWithPitch(u32 soundId, f32 pitch);

    // These two do the exact same thing
    bool playSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position);
    bool playSoundAtPosition2(u32 soundId, const nw4r::math::VEC3 *position);

    bool holdBowChargeSound(f32 remainingChargeAmount);
    bool holdFinisherPromptSound(const nw4r::math::VEC3 *position);

    bool playDowsingPingSound(f32 volume, f32 pitch);
    bool holdDowsingNearestSound();

    bool playSirenCountdownSound(s32 timer);
    bool playMinigameCountdownSound();
    bool playMinigameTimeUpSound(s32 timer);
    bool playMinigameStartSound();
    bool playMinigameFinishSound();
    bool playMinigameFinishWhistleSound();
    bool playMinigameScoreUpSound(s32 param);
    bool playMinigameScoreDownSound();
    bool playMinigameMusasabiSound(s32 count);

    bool playSkbSound(u32 soundId);
    bool playDemoSound(u32 soundId, nw4r::snd::SoundHandle *pHandle);

    bool playButtonPressSoundWhenAdvancingTextBoxes(f32);
    void resetButtonPressSound();
    void setButtonPressSound(dSoundSource_c *source);
    bool playBattleHitSound(BattleHitSound_e type, dSoundSource_c *source);

    bool playSoundInternalChecked(u32 soundId, nw4r::snd::SoundHandle *handle);

private:
    bool playSoundInternal(u32 soundId, nw4r::snd::SoundHandle *handle);
    bool playSoundInternal(u32 soundId);
    void stopSounds(u32 playerIdx, u32 soundId, s32 fadeFrames);
    void stopSounds(u32 soundId, s32 fadeFrames);
    bool isPlayingSound(u32 playerIdx, u32 soundId);
    bool isPlayingSound(u32 soundId);

    bool doSideEffects(u32 soundId);

    void setBitsIfAdjacent(dSndBgmDataHarpVarSetBase_c *set, s32 count, s32 target, u32 *pMask);

    /**
     * Finds a sound handle currently playing the given sound,
     * or an idle sound handle,
     * or stops a lower-priority sound if needed and possible.
     */
    nw4r::snd::SoundHandle *getHoldSoundHandle(u32 soundId);
    bool holdSound(u32 soundId, nw4r::snd::SoundHandle *handle);

    /* 0x10 */ s32 field_0x10;
    // used for most sounds
    /* 0x14 */ nw4r::snd::SoundHandle mNormalSound;
    /* 0x18 */ nw4r::snd::SoundHandle mDowsingSoundHandle;
    /* 0x1C */ nw4r::snd::SoundHandle mHoldSoundHandles[NUM_HOLD_SOUNDS];

    /* 0x28 */ u32 mDelayedSoundIds[NUM_DELAYED_SOUNDS];
    /* 0x30 */ s32 mDelayedSoundTimers[NUM_DELAYED_SOUNDS];
    /* 0x38 */ u32 mTextboxAdvanceSound;
    /* 0x3C */ nw4r::snd::SoundHandle mBattleTuttiHandle;
    /* 0x40 */ s16 field_0x40;
    /* 0x42 */ s16 field_0x42;
    /* 0x44 */ u32 field_0x44;
};

#endif
