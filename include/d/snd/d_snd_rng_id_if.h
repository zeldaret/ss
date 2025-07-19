#ifndef D_SND_RNG_ID_IF_H
#define D_SND_RNG_ID_IF_H

#include "d/snd/d_snd_rng_id.h"

/** A class for picking sound ID variants at random without repeats. */
class dSndRngIdIf_c : protected dSndRngId_c {
public:
    /* 0x0C */ virtual bool doNextIdNoReuse();
    /* 0x10 */ virtual bool doRndId();
    /* 0x14 */ virtual bool doRndIdNoReuse();
    /* 0x18 */ virtual bool doRndIdNotSame();

    /* 0x1C */ virtual bool handleId(u32 id) = 0;
};

#endif
