#ifndef D_SND_SE_SOUND_2_H
#define D_SND_SE_SOUND_2_H

#include "common.h"
#include "d/snd/d_snd_se_sound.h"

class dSndSeSound2_c : public dSndSeSound_c {
public:
    dSndSeSound2_c();

    // vtable at 0x000
    /* vt 0x08 */ virtual void setSource(dSoundSource_c *source, u32 soundId) override;

    bool calcTimer();

private:
    /* 0x128 */ u32 field_0x128;
    /* 0x12C */ u32 field_0x12C;
    /* 0x130 */ u8 field_0x130;
};

#endif
