#ifndef D_SND_SOURCE_ANIM_SOUND_H
#define D_SND_SOURCE_ANIM_SOUND_H

#include "d/snd/d_snd_anim_sound.h"
#include "d/snd/d_snd_source.h"

class dSndSourceAnimSound_c : public dSoundSource_c {
public:
    dSndSourceAnimSound_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSoundSource_c(sourceType, ac, name, pOwnerGroup), mAnimSound(this) {}

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

#endif
