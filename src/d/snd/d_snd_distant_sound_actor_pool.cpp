#include "d/snd/d_snd_distant_sound_actor_pool.h"

#include "common.h"
#include "d/snd/d_snd_distant_sound_actor.h"
#include "d/snd/d_snd_source.h"
#include "nw4r/math/math_types.h"
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

void dSndDistantSoundActorPool_c::initialize() {
    field_0x4210 = 0;
    for (int i = 0; i < POOL_SIZE; i++) {
        mSounds[i].loadDefaultParam();
        mSounds[i].setSource(nullptr);
        mSounds[i].initSource(nullptr);
    }
}

void dSndDistantSoundActorPool_c::calc() {
    dSndDistantSoundActor_c *it, *next;
    for (dSndDistantSoundActor_c *it = static_cast<dSndDistantSoundActor_c*>(nw4r::ut::List_GetFirst(&mActiveActors));  it != nullptr; it = next) {
        next = static_cast<dSndDistantSoundActor_c*>(nw4r::ut::List_GetNext(&mActiveActors, it));
        if (it->hasPlayingSounds()) {
            it->updatePosition();
        } else {
            if (it->hasAttachedSource()) {
                it->detachFromSource();
            }
            it->resetHandle();
            removeFromActiveList(it);
        }
    }
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

void dSndDistantSoundActorPool_c::removeFromActiveList(dSndDistantSoundActor_c *actor) {
    if (actor == nullptr) {
        return;
    }
    if (!actor->isActive()) {
        return;
    }
    nw4r::ut::List_Remove(&mActiveActors, actor);
    actor->setActive(false);
}

bool dSndDistantSoundActorPool_c::startSound(u32 soundId, const nw4r::math::VEC3 *position) {
    dSndDistantSoundActor_c *ac = acquireActor(soundId, position, nullptr);
    if (ac != nullptr) {
        return ac->startSound(soundId, *position, nullptr);
    }
    return false;
}

dSndDistantSoundActor_c *dSndDistantSoundActorPool_c::findActiveActor(u32 soundId, dSoundSource_c *source) {
    for (dSndDistantSoundActor_c *it = static_cast<dSndDistantSoundActor_c*>(nw4r::ut::List_GetFirst(&mActiveActors));  it != nullptr; it = static_cast<dSndDistantSoundActor_c*>(nw4r::ut::List_GetNext(&mActiveActors, it))) {
        if (it->isAttachedSource(source) && it->isPlayingSound(soundId)) {
            return it;
        }
    }

    return nullptr;
}

bool dSndDistantSoundActorPool_c::holdSound(u32 soundId, const nw4r::math::VEC3 *position) {
    dSndDistantSoundActor_c *ac = findActiveActor(soundId, nullptr);
    if (ac == nullptr) {
        ac = acquireActor(soundId, position, nullptr);
        if (ac != nullptr) {
            ac->updateSome3DField();
        }
    }
    if (ac != nullptr) {
        return ac->holdSound(soundId, *position, nullptr);
    }
    return false;

}

void dSndDistantSoundActorPool_c::setAllPause(bool flag, s32 fadeFrames) {
    for (dSndDistantSoundActor_c *it = static_cast<dSndDistantSoundActor_c*>(nw4r::ut::List_GetFirst(&mActiveActors));  it != nullptr; it = static_cast<dSndDistantSoundActor_c*>(nw4r::ut::List_GetNext(&mActiveActors, it))) {
        it->setPause(flag, fadeFrames);
    }
}

void dSndDistantSoundActorPool_c::disableAll() {
    for (int i = 0; i < POOL_SIZE; i++) {
        mSounds[i].setDisabled(true);
    }
}

void dSndDistantSoundActorPool_c::enableAll() {
    for (int i = 0; i < POOL_SIZE; i++) {
        mSounds[i].setDisabled(false);
    }
}

void dSndDistantSoundActorPool_c::onChangeStage() {
    nw4r::math::VEC3 v(INFINITY, INFINITY, INFINITY);
    for (int i = 0; i < POOL_SIZE; i++) {
        mSounds[i].SetPosition(v);
    }
}
