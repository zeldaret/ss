#include "d/snd/d_snd_se_sound_2.h"

#include "common.h"
#include "d/snd/d_snd_source.h"

dSndSeSound2_c::dSndSeSound2_c() : field_0x128(0), field_0x12C(0), field_0x130(0) {
    mHandleType = SE_HANDLE_TYPE_2;
}

void dSndSeSound2_c::setSource(dSoundSource_c *source, u32 soundId) {
    dSndSeSound_c::setSource(source, soundId);
    field_0x128 = 0;
    field_0x12C = 0;
    field_0x130 = 0;
}

bool dSndSeSound2_c::calcTimer() {
    if (!field_0x130 && field_0x128 != 0) {
        field_0x12C++;
        if (field_0x12C <= field_0x128) {
            return true;
        }
    }

    return false;
}
