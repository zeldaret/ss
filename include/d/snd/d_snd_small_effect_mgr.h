#ifndef D_SND_SMALL_EFFECT_MGR_H
#define D_SND_SMALL_EFFECT_MGR_H

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

    void playSound(u32 soundId);
    void playSoundWithPitch(u32 soundId, f32 pitch);
    void playButtonPressSoundWhenAdvancingTextBoxes(f32);

private:
    /* 0x10 */ void *field_0x10;
    /* 0x14 */ void *field_0x14;
    /* 0x18 */ void *field_0x18;
    /* 0x1C */ nw4r::snd::SoundHandle mHandles[3];
};

#endif
