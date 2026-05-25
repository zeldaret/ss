#include "d/snd/d_snd_rng.h"

#include "rvl/OS/OSTime.h"

dSndRng_c::dSndRng_c() : field_0x08(0), field_0x10(0) {
    init();
}

void dSndRng_c::init() {
    u32 tick = OSGetTick();
    field_0x08 = ((tick << 0x10) & 0xFFF0000) | tick >> 0x10;
    field_0x10 = field_0x08;
}

u32 dSndRng_c::rndInt(s32 max) {
    field_0x08 = field_0x08 * 0x690379B2B2E3D431ULL + 0x508EBD;
    return (u32)(((u64)(field_0x08 >> 32) * (u64)(u32)max) >> 32);
}

bool dSndRng_c::rndBool(s32 chance) {
    if (chance >= 100) {
        return true;
    } else if (chance <= 0) {
        return false;
    }
    return rndInt(100) < chance;
}
