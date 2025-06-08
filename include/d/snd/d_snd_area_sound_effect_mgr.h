#ifndef D_SND_AREA_SOUND_EFFECT_MGR_H
#define D_SND_AREA_SOUND_EFFECT_MGR_H

#include "common.h"
#include "d/snd/d_snd_actor.h"
#include "d/snd/d_snd_area_sound.h"
#include "d/snd/d_snd_util.h"

SND_DISPOSER_FORWARD_DECL(dSndAreaSoundEffectMgr_c);

/**
 * A singleton sound actor for playing persistent environmental background
 * sound effects. Turn this off and the Faron birds will stop singing.
 *
 * Plays sounds like SE_A_F100 (hence the name, not sure if it's the real meaning).
 */
class dSndAreaSoundEffectMgr_c {
    SND_DISPOSER_MEMBERS(dSndAreaSoundEffectMgr_c)

public:
    dSndAreaSoundEffectMgr_c();

    bool startSound(u32 soundId, u32 handleIdx);
    void calc();
    void stopSounds(s32 fadeFrames);

private:
    bool isPlayingAnySound() const;
    bool isPlayingSound(u32 soundId) const;

    /* 0x010 */ dSndAreaSound_c mSounds[3];
    /* 0x328 */ UNKWORD field_0x328;
    /* 0x32C */ f32 field_0x32C;
    /* 0x330 */ f32 field_0x330;
    /* 0x334 */ dSndActor_c mActor;
    /* 0x388 */ f32 field_0x388;
};

#endif
