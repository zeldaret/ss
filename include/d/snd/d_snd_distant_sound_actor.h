#ifndef D_SND_DISTANT_SOUND_ACTOR_H
#define D_SND_DISTANT_SOUND_ACTOR_H

#include "common.h"
#include "d/snd/d_snd_3d_actor.h"
#include "d/snd/d_snd_types.h"
#include "nw4r/math/math_types.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/ut/ut_list.h"

/**
 * A sound actor to be borrowed by sound sources when they want to play
 * sounds at a specific position.
 *
 * Sound sources correspond to actors, but e.g. projectile hit effects, grass
 * cutting sounds, sword hit effects, ... have to be played where the hit occurred.
 * This pool allows sound sources to borrow sound actors for these purposes.
 */
class dSndDistantSoundActor_c : public dSnd3DActor_c {
public:
    dSndDistantSoundActor_c();
    virtual void setPause(bool flag, int fadeFrames) override;
    virtual void d_vt_0x58(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id) override;
    virtual void d_vt_0x5C(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id, UNKWORD) override;

    void initSource(dSoundSource_c *pSource);
    void setSourceDirectly(dSoundSource_c *pSource);
    void loadDefaultParam();
    void detachFromSource();

    void updatePosition();

    bool startSound(u32 soundId, const nw4r::math::VEC3 &position, nw4r::snd::SoundHandle *pHandle);
    bool holdSound(u32 soundId, const nw4r::math::VEC3 &position, nw4r::snd::SoundHandle *pHandle);

    bool isAttachedSource(dSoundSource_c *source) const {
        return mpSoundSource == source;
    }

    nw4r::snd::SoundHandle *getHandle() {
        return mpSoundHandle;
    }

    void setSource(dSoundSource_c *source) {
        mpSoundSource = source;
    }

    bool isActive() const {
        return mIsActive;
    }

    void setActive(bool value) {
        mIsActive = value;
    }

    void setUseSourcePosition(bool value) {
        mUseSourcePosition = true;
    }

private:
    /* 0x0E4 */ nw4r::ut::Node mPoolLink;   // sound actor pool link
    /* 0x0EC */ nw4r::ut::Node mSourceLink; // sound source link
    /* 0x0F4 */ UNKWORD field_0x0F4;
    /* 0x0F8 */ dSoundSource_c *mpSoundSource;
    /* 0x0FC */ nw4r::snd::SoundHandle mSoundHandle;
    /* 0x100 */ nw4r::snd::SoundHandle *mpSoundHandle;
    /* 0x104 */ bool mIsActive;
    /* 0x105 */ bool mUseSourcePosition;
};

#endif
