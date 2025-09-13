#include "d/snd/d_snd_se_sound_1.h"

#include "common.h"
#include "d/snd/d_snd_source.h"

dSndSeSound1_c::dSndSeSound1_c() {
    mHandleType = SE_HANDLE_TYPE_1;
}

void dSndSeSound1_c::setSource(dSoundSource_c *source, u32 soundId) {
    dSndSeSound_c::setSource(source, soundId);
}
