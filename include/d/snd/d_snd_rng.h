#ifndef D_SND_RNG_H
#define D_SND_RNG_H

#include "common.h"

class dSndRng_c {
public:
    dSndRng_c();
    virtual ~dSndRng_c() {}

    bool rndBool(s32 chance);
    u32 rndInt(s32 max);

private:
    void init();

    /* 0x08 */ u64 field_0x08;
    /* 0x10 */ u64 field_0x10;
};

#endif
