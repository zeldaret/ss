#include "d/snd/d_snd_distant_sound_actor_pool.h"

#include "nw4r/ut/ut_list.h"

#include <cmath>


SND_DISPOSER_DEFINE(dSndDistantSoundActorPool_c);

dSndSourceParam dSndDistantSoundActorPool_c::sParam;

dSndDistantSoundActorPool_c::dSndDistantSoundActorPool_c() {
    field_0x4210 = 0;
    // TODO offsetof
    nw4r::ut::List_Init(&mList, 0xE4);
    sParam.reset(INFINITY);
}
