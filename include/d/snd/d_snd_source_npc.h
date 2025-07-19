#ifndef D_SND_SOURCE_NPC_H
#define D_SND_SOURCE_NPC_H

#include "d/snd/d_snd_anim_sound.h"
#include "d/snd/d_snd_source.h"

// vtable at 8054d540 - placed by implementation of postSetupSound,
// so this is the base class and also instantiated in createSource
class dSndSourceNpc_c : public dSoundSource_c {
public:
    dSndSourceNpc_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSoundSource_c(sourceType, ac, name, pOwnerGroup) {}

    /* 0x1CC */ virtual void postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) override;

    /* 0x1E8 */ virtual u32 d_s_vt_0x1E8(u32 soundId) override {
        return specializeBgHitSoundId(soundId, mPolyAttr0, mPolyAttr1);
    }
    // at 80032af0, explicitly called by dAcNpc_c
    /* 0x1EC */ virtual void d_s_npc_vt_0x1EC(dSndSourceNpc_c *source) {}

private:
};

class dSndSourceNpcAnimBase_c : public dSndSourceNpc_c {
public:
    dSndSourceNpcAnimBase_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceNpc_c(sourceType, ac, name, pOwnerGroup), mAnimSound(this) {}

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

private:
    /* 0x15C */ dSndAnimSound_c mAnimSound;
};

class dSndSourceNpcAnim_c : public dSndSourceNpcAnimBase_c {
public:
    dSndSourceNpcAnim_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceNpcAnimBase_c(sourceType, ac, name, pOwnerGroup) {}

private:
};

#endif
