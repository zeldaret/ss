#ifndef D_SND_DISTANT_SOUND_ACTOR_POOL_H
#define D_SND_DISTANT_SOUND_ACTOR_POOL_H

#include "common.h"
#include "d/snd/d_snd_misc.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_distant_sound_actor.h"
#include "nw4r/ut/ut_list.h"

SND_DISPOSER_FORWARD_DECL(dSndDistantSoundActorPool_c);

/**
 * A pool for distant sounds actors. Sound sources will borrow
 * actors from this pool when they want to play a sound
 * at a specific position.
 */
class dSndDistantSoundActorPool_c {
    SND_DISPOSER_MEMBERS(dSndDistantSoundActorPool_c);

public:
    dSndDistantSoundActorPool_c();

    static dSndSourceParam& getSourceParam() {
        return sParam;
    }

private:
    /* 0x0010 */ dSndDistantSoundActor_c mSounds[64];
    /* 0x4210 */ UNKWORD field_0x4210;
    /* 0x4214 */ nw4r::ut::List mList;

    static dSndSourceParam sParam;
};

#endif
