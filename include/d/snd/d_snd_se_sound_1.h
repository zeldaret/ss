#ifndef D_SND_SE_SOUND_1_H
#define D_SND_SE_SOUND_1_H

#include "common.h"
#include "d/snd/d_snd_se_sound.h"

class dSndSeSound1_c : public dSndSeSound_c {
public:
    dSndSeSound1_c();

    // vtable at 0x000
    /* vt 0x08 */ virtual void setSource(dSoundSource_c *source, u32 soundId) override;
};

#endif
