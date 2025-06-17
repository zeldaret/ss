
#include "d/snd/d_snd_rng_mgr.h"

#include "d/snd/d_snd_util.h"

SND_DISPOSER_DEFINE(dSndRngMgr_c)

u32 dSndRngMgr_c::rndIntRange(s32 min, s32 max) {
    return min + rndInt(max - min);
}
