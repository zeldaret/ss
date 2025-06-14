#include "d/snd/d_snd_distant_sound_actor_pool.h"

#include "common.h"
#include "d/snd/d_snd_distant_sound_actor.h"
#include "d/snd/d_snd_source.h"
#include "m/m_vec.h"
#include "nw4r/ut/ut_list.h"

#include <cmath>

SND_DISPOSER_DEFINE(dSndDistantSoundActorPool_c);

dSndSourceParam dSndDistantSoundActorPool_c::sParam;

dSndDistantSoundActorPool_c::dSndDistantSoundActorPool_c() {
    field_0x4210 = 0;
    // TODO offsetof
    nw4r::ut::List_Init(&mActiveActors, 0xE4);
    sParam.reset(INFINITY);
}

dSndDistantSoundActor_c *
dSndDistantSoundActorPool_c::acquireActor(u32 soundId, const nw4r::math::VEC3 *position, dSoundSource_c *source) {
    if (soundId == -1) {
        return nullptr;
    }

    dSndDistantSoundActor_c *ac = nullptr;
    int i = 0;
    while (ac == nullptr) {
        ac = &mSounds[field_0x4210];
        field_0x4210++;
        if (field_0x4210 >= POOL_SIZE) {
            field_0x4210 = 0;
        }
        if (ac->isActive()) {
            i++;
            ac = nullptr;
            if (i >= POOL_SIZE) {
                break;
            }
        }
    }

    if (ac != nullptr) {
        if (source != nullptr) {
            ac->initSource(source);
            source->attachDistantSound(ac);
        } else {
            ac->loadDefaultParam();
            ac->setSource(nullptr);
        }
        ac->SetPosition(*position);
        ac->updateListenerParameters();
        ac->updateCameraTargetParameters();

        addToActiveList(ac, soundId);

        return ac;
    }
    return nullptr;
}

void dSndDistantSoundActorPool_c::addToActiveList(dSndDistantSoundActor_c *actor, u32 id) {
    if (actor == nullptr) {
        return;
    }
    if (actor->isActive()) {
        return;
    }
    nw4r::ut::List_Append(&mActiveActors, actor);
    actor->setActive(true);
}

void dSndDistantSoundActorPool_c::onChangeStage() {
    mVec3_c v(INFINITY, INFINITY, INFINITY);
    for (int i = 0; i < POOL_SIZE; i++) {
        mSounds[i].SetPosition(v);
    }
}
