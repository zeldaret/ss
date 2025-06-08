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

public:
    dSndSmallEffectMgr_c();

    bool playSound(u32 soundId);
    bool playSoundWithPan(u32 soundId, f32 pan);
    void playSoundWithPitch(u32 soundId, f32 pitch);


    bool playButtonPressSoundWhenAdvancingTextBoxes(f32);
    void resetButtonPressSound();
    void setButtonPressSound(dSoundSource_c *source);

private:
    bool playSoundInternal(u32 soundId);
    void stopSounds(u32 playerIdx, u32 soundId, s32 fadeFrames);
    void stopSounds(u32 soundId, s32 fadeFrames);
    bool isPlayingSound(u32 playerIdx, u32 soundId);
    bool isPlayingSound(u32 soundId);

    /* 0x10 */ nw4r::snd::SoundHandle mHandle1;
    // used for most sounds
    /* 0x14 */ nw4r::snd::SoundHandle mNormalSound;
    /* 0x18 */ nw4r::snd::SoundHandle mHandle3;
    // apparently used for shield gauge sounds, but maybe not given
    // that the callers appear unreachable
    /* 0x1C */ nw4r::snd::SoundHandle mShieldGaugeHandles[3];

    /* 0x28 */ s32 field_0x28;
    /* 0x2C */ s32 field_0x2C;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ s32 field_0x34;
    /* 0x38 */ u32 mTextboxAdvanceSound;
    /* 0x3C */ nw4r::snd::SoundHandle mHandle4;
    /* 0x40 */ u16 field_0x40;
    /* 0x42 */ u16 field_0x42;
    /* 0x44 */ s32 field_0x44;
};

#endif
