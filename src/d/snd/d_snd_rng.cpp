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
    // TODO
    return 0;
/*
    s32 temp_r0;
    s32 temp_r11;

    temp_r11 = this->unkC;
    this->unkC = (temp_r11 * 0xB2E3D431) + 0x508EBD;
    temp_r0 = MULTU_HI(temp_r11, 0xB2E3D431) + (this->unk8 * 0xB2E3D431) + (temp_r11 * 0x690379B2) + M2C_CARRY;
    this->unk8 = temp_r0;
    return MULTU_HI(temp_r0, arg0) + (0 * arg0);
=>

    lo_1 = (lo_0 * 0xB2E3D431) + 0x508EBD;
    hi_1 = MULTU_HI(lo_0, 0xB2E3D431) + (hi_0 * 0xB2E3D431) + (lo_0 * 0x690379B2) + M2C_CARRY;
    return MULTU_HI(hi_1, arg0) + (0 * arg0);
*/
}

bool dSndRng_c::rndBool(s32 chance) {
    if (chance >= 100) {
        return true;
    } else if (chance <= 0) {
        return false;
    }
    return rndInt(100) < chance;
}
