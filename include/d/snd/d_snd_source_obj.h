#ifndef D_SND_SOURCE_OBJ_H
#define D_SND_SOURCE_OBJ_H

#include "d/snd/d_snd_anim_sound.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/math/math_types.h"

class dSndSourceObj_c : public dSoundSource_c {
public:
    dSndSourceObj_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSoundSource_c(sourceType, ac, name, pOwnerGroup) {}

    /* 0x188 */ virtual void postSetup() override;

    /* 0x1CC */ virtual void postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) override;

    /* 0x1E8 */ virtual u32 d_s_vt_0x1E8(u32 soundId) override {
        return specializeBgHitSoundId(soundId, mPolyAttr0, mPolyAttr1);
    }

private:
    // Probably not a problem for weak function order since getName is emitted
    // earlier through an explicit call
    bool isName(const char *name) const {
        return streq(name, getName());
    }
};

class dSndSourceObjLightShaft_c : public dSndSourceObj_c {
public:
    dSndSourceObjLightShaft_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceObj_c(sourceType, ac, name, pOwnerGroup) {}

    /* 0x1EC */ virtual void setPosition(const nw4r::math::VEC3 &position) override;
};

class dSndSourceObjAnimBase_c : public dSndSourceObj_c {
public:
    dSndSourceObjAnimBase_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceObj_c(sourceType, ac, name, pOwnerGroup), mAnimSound(this) {}

    /* 0x1A8 */ virtual StartResult
    SetupSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *arg) override {
        return setupSoundCommon(pHandle, soundId, pStartInfo, arg);
    }

    /* 0x1D4 */ virtual dSndAnimSound_c *getAnimSound() override {
        return &mAnimSound;
    }

    /* 0x100 */ virtual bool hasAnimSound() override {
        return true;
    }
    /* 0x104 */ virtual void load(void *data, const char *name) override {
        mAnimSound.setData(data, name);
    }
    /* 0x108 */ virtual void setFrame(f32 frame) override {
        if (isInaudibleInternal()) {
            mAnimSound.resetFrame(frame);
        } else {
            mAnimSound.setFrame(frame);
        }
    }
    /* 0x10C */ virtual void setRate(f32 frame) override {
        mAnimSound.setRate(frame);
    }

protected:
    /* 0x15C */ dSndAnimSound_c mAnimSound;
};

class dSndSourceObjAnim_c : public dSndSourceObjAnimBase_c {
public:
    dSndSourceObjAnim_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceObjAnimBase_c(sourceType, ac, name, pOwnerGroup) {}
};

#endif
