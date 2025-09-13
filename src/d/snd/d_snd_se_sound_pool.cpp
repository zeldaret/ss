#include "d/snd/d_snd_se_sound_pool.h"

#include "common.h"
#include "d/snd/d_snd_se_sound_1.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/ut/ut_list.h"

SND_DISPOSER_DEFINE(dSndSeSoundPool_c);

dSndSeSoundPool_c::dSndSeSoundPool_c() {
    // TODO offsetof
    nw4r::ut::List_Init(&mFreeType1List, 0x0C);
    nw4r::ut::List_Init(&mActiveType1List, 0x0C);
    nw4r::ut::List_Init(&mFreeType2List, 0x0C);

    for (dSndSeSound1_c *sound = mSoundsType1; sound < &mSoundsType1[POOL_SIZE]; sound++) {
        releaseSoundType1(sound, true);
    }

    for (dSndSeSound2_c *sound = mSoundsType2; sound < &mSoundsType2[POOL_SIZE]; sound++) {
        releaseSoundType2(sound);
    }
}

void dSndSeSoundPool_c::calc() {
    dSndSeSound1_c *it, *next;
    for (it = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetFirst(&mActiveType1List)); it != nullptr; it = next) {
        next = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetNext(&mActiveType1List, it));
        if (!it->isAttachedSound()) {
            it->clear();
            nw4r::ut::List_Remove(&mActiveType1List, it);
            nw4r::ut::List_Append(&mFreeType1List, it);
        }
    }
}

dSndSeSound1_c *dSndSeSoundPool_c::acquireSoundType1(dSoundSource_c *source, u32 soundId) {
    if (soundId == -1) {
        return nullptr;
    }

    for (dSndSeSound1_c *it = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetFirst(&mFreeType1List)); it != nullptr;
         it = static_cast<dSndSeSound1_c *>(nw4r::ut::List_GetNext(&mFreeType1List, it))) {
        if (!it->isAttachedSound()) {
            it->setSource(source, soundId);
            removeSoundType1(it);
            return it;
        }
    }
    return nullptr;
}

dSndSeSound2_c *dSndSeSoundPool_c::acquireSoundType2(dSoundSource_c *source, u32 soundId) {
    if (soundId == -1) {
        return nullptr;
    }

    for (dSndSeSound2_c *it = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetFirst(&mFreeType2List)); it != nullptr;
         it = static_cast<dSndSeSound2_c *>(nw4r::ut::List_GetNext(&mFreeType2List, it))) {
        if (!it->isAttachedSound()) {
            it->setSource(source, soundId);
            removeSoundType2(it);
            return it;
        }
    }
    return nullptr;
}

void dSndSeSoundPool_c::releaseSoundType1(dSndSeSound1_c *sound, bool stop) {
    if (sound == nullptr) {
        return;
    }

    if (sound->isAttachedSound()) {
        if (stop) {
            sound->stop(0);
            sound->clear();
            nw4r::ut::List_Append(&mFreeType1List, sound);
        } else {
            nw4r::ut::List_Append(&mActiveType1List, sound);
        }
    } else {
        sound->clear();
        nw4r::ut::List_Append(&mFreeType1List, sound);
    }
}

void dSndSeSoundPool_c::releaseSoundType2(dSndSeSound2_c *sound) {
    if (sound == nullptr) {
        return;
    }

    sound->stop(0);
    sound->clear();
    nw4r::ut::List_Append(&mFreeType2List, sound);
}

void dSndSeSoundPool_c::removeSoundType1(dSndSeSound1_c *sound) {
    if (sound == nullptr) {
        return;
    }

    nw4r::ut::List_Remove(&mFreeType1List, sound);
}

void dSndSeSoundPool_c::removeSoundType2(dSndSeSound2_c *sound) {
    if (sound == nullptr) {
        return;
    }

    nw4r::ut::List_Remove(&mFreeType2List, sound);
}
