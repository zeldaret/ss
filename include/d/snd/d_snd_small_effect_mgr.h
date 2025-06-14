#ifndef D_SND_SMALL_EFFECT_MGR_H
#define D_SND_SMALL_EFFECT_MGR_H

#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h" // IWYU pragma: export
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

    void calc();

    bool playSound(u32 soundId);
    // used for clawshots cursor, pan depends on where on the screen
    // your cursor is when it activates
    bool playSoundWithPan(u32 soundId, f32 pan);
    void holdSoundWithPitch(u32 soundId, f32 pitch);
    bool playSkbSound(u32 soundId);


    bool playButtonPressSoundWhenAdvancingTextBoxes(f32);
    void resetButtonPressSound();
    void setButtonPressSound(dSoundSource_c *source);

    void playSound(u32 soundId, nw4r::snd::SoundHandle *handle);
    bool playBattleHitSound(BattleHitSound_e type, dSoundSource_c *source);

private:
    bool playSoundInternal(u32 soundId);
    void stopSounds(u32 playerIdx, u32 soundId, s32 fadeFrames);
    void stopSounds(u32 soundId, s32 fadeFrames);
    bool isPlayingSound(u32 playerIdx, u32 soundId);
    bool isPlayingSound(u32 soundId);

    /**
     * Finds a sound handle currently playing the given sound,
     * or an idle sound handle,
     * or stops a lower-priority sound if needed and possible.
     */
    nw4r::snd::SoundHandle *getHoldSoundHandle(u32 soundId);

    /* 0x10 */ s32 field_0x10;
    // used for most sounds
    /* 0x14 */ nw4r::snd::SoundHandle mNormalSound;
    /* 0x18 */ nw4r::snd::SoundHandle mHandle3;
    /* 0x1C */ nw4r::snd::SoundHandle mHoldSoundHandles[NUM_HOLD_SOUNDS];

    /* 0x28 */ u32 mDelayedSoundIds[NUM_DELAYED_SOUNDS];
    /* 0x30 */ s32 mDelayedSoundTimers[NUM_DELAYED_SOUNDS];
    /* 0x38 */ u32 mTextboxAdvanceSound;
    /* 0x3C */ nw4r::snd::SoundHandle mBattleTuttiHandle;
    /* 0x40 */ u16 field_0x40;
    /* 0x42 */ u16 field_0x42;
    /* 0x44 */ s32 field_0x44;
};

#endif
