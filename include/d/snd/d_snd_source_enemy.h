#ifndef D_SND_SOURCE_ENEMY_H
#define D_SND_SOURCE_ENEMY_H

#include "d/snd/d_snd_anim_sound.h"
#include "d/snd/d_snd_source.h"
#include "nw4r/ut/ut_list.h"

class dSndSourceEnemy_c : public dSoundSource_c {
public:
    dSndSourceEnemy_c(u8 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);
    virtual ~dSndSourceEnemy_c();

    /* 0x020 */ virtual void vt_0x020() override;
    /* 0x044 */ virtual void shutdown() override;

    /* 0x114 */ virtual void setBattleBgmRelated(UNKWORD arg) override {
        field_0x164 = arg;
    }
    /* 0x118 */ virtual UNKWORD checkBattleBgmRelated() override;

    /* 0x188 */ virtual void postSetup() override;
    /* 0x194 */ virtual u32 overrideStartSoundId(u32 soundId) override;
    /* 0x1A0 */ virtual u32 overrideHoldSoundId(u32 soundId, bool initial) override;

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
    void unregisterEnemySource();

    /* 0x15C */ nw4r::ut::Node mMgrEnemyLink;
    /* 0x164 */ UNKWORD field_0x164;
    /* 0x168 */ u8 field_0x168;
    /* 0x169 */ u8 field_0x169;
    /* 0x16A */ bool mIsRegisteredEnemy;
};

class dSndSourceEnemyAnimBase_c : public dSndSourceEnemy_c {
public:
    dSndSourceEnemyAnimBase_c(u8 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceEnemy_c(sourceType, ac, name, pOwnerGroup), mAnimSound(this) {}

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
    /* 0x16C */ dSndAnimSound_c mAnimSound;
};

#endif
