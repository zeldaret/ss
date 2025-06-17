#ifndef D_SND_RNG_MGR_H
#define D_SND_RNG_MGR_H

#include "d/snd/d_snd_rng.h"
#include "d/snd/d_snd_util.h"

SND_DISPOSER_FORWARD_DECL(dSndRngMgr_c)

class dSndRngMgr_c : public dSndRng_c {
public:
    SND_DISPOSER_MEMBERS(dSndRngMgr_c)

    u32 rndIntRange(s32 min, s32 max);

public:
    dSndRngMgr_c() {}
};

#endif
