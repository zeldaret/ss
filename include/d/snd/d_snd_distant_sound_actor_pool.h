#ifndef D_SND_DISTANT_SOUND_ACTOR_POOL_H
#define D_SND_DISTANT_SOUND_ACTOR_POOL_H

#include "common.h"
#include "d/snd/d_snd_distant_sound_actor.h"
#include "d/snd/d_snd_misc.h"
#include "d/snd/d_snd_types.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/math/math_types.h"
#include "nw4r/ut/ut_list.h"


SND_DISPOSER_FORWARD_DECL(dSndDistantSoundActorPool_c);

/**
 * A pool for distant sounds actors. Sound sources will borrow
 * actors from this pool when they want to play a sound
 * at a specific position. The finisher target sound will
 * also play using this pool.
 */
class dSndDistantSoundActorPool_c {
    SND_DISPOSER_MEMBERS(dSndDistantSoundActorPool_c);

    static const s32 POOL_SIZE = 64;

public:
    dSndDistantSoundActorPool_c();

    static dSndSourceParam &getSourceParam() {
        return sParam;
    }

    void initialize();
    void calc();
    void onChangeStage();

    dSndDistantSoundActor_c *acquireActor(u32 soundId, const nw4r::math::VEC3 *position, dSoundSource_c *source);
    dSndDistantSoundActor_c *findActiveActor(u32 soundId, dSoundSource_c *source);

    bool startSound(u32 soundId, const nw4r::math::VEC3 *position);
    bool holdSound(u32 soundId, const nw4r::math::VEC3 *position);

    void setAllPause(bool flag, s32 fadeFrames);
    void disableAll();
    void enableAll();

private:
    void addToActiveList(dSndDistantSoundActor_c *actor, u32 id);
    void removeFromActiveList(dSndDistantSoundActor_c *actor);

    /* 0x0010 */ dSndDistantSoundActor_c mSounds[POOL_SIZE];
    /* 0x4210 */ UNKWORD field_0x4210;
    /* 0x4214 */ nw4r::ut::List mActiveActors;

    static dSndSourceParam sParam;
};

#endif
