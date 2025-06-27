#ifndef D_SND_SOURCE_ENEMY_H
#define D_SND_SOURCE_ENEMY_H

#include "common.h"
#include "d/snd/d_snd_anim_sound.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/ut/ut_list.h"

class dSndSourceEnemy_c : public dSoundSource_c {
public:
    dSndSourceEnemy_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);
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
    dSndSourceEnemyAnimBase_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
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

protected:
    /* 0x16C */ dSndAnimSound_c mAnimSound;
};

class dSndSourceEnemyAnim_c : public dSndSourceEnemyAnimBase_c {
public:
    dSndSourceEnemyAnim_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceEnemyAnimBase_c(sourceType, ac, name, pOwnerGroup) {}

    /* 0x194 */ virtual u32 overrideStartSoundId(u32 soundId) override;
    /* 0x1A0 */ virtual u32 overrideHoldSoundId(u32 soundId, bool initial) override;

private:
};

class dSndSourceEnemyMultiBase_c : public dSndSourceEnemyAnimBase_c {
public:
    dSndSourceEnemyMultiBase_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceEnemyAnimBase_c(sourceType, ac, name, pOwnerGroup) {
            nw4r::ut::List_Init(&mSubSourceList, 0x14C);
            mAnimSound.setCallback(this);
        }

    /* 0x1A8 */ virtual StartResult
    SetupSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *arg) override {
        bool bHoldFlag = *(bool*)arg;
        if (!bHoldFlag && (mAnimEventValue & 0xF) > 0) {
            dSoundSource_c *otherSource;
            s32 idx = (mAnimEventValue & 0xF) - 1;
            if (idx < 0) {
                otherSource = nullptr;
            } else if (idx >= nw4r::ut::List_GetSize(&mSubSourceList)) {
                otherSource = nullptr;
            } else {
                otherSource = getSubSourceFirst();
                while (idx > 0) {
                    otherSource = getSubSourceNext(otherSource);
                    idx--;
                }
            }
            if (otherSource != nullptr) {
                otherSource->startSound(soundId, pHandle);
                return onSetupError();
            }
        }

        return setupSoundCommon(pHandle, soundId, pStartInfo, arg);
    }

    /* 0x1D8 */ virtual void registerAdditionalSource(dSoundSource_c *other) override {
        if (other == this) {
            return;
        }

        if (other == nullptr) {
            return;
        }
        if (!hasSubSource(other)) {
            appendSubSource(other);
        }
    }
    /* 0x1DC */ virtual bool isMultiSource() const override {
        return true;
    }

private:
    // TODO: maybe created by the macro below
    bool hasSubSource(dSoundSource_c *other) {
        for (dSoundSource_c *it = getSubSourceFirst(); it != nullptr; it = getSubSourceNext(it)) {
            if (it == other) {
                return true;
            }
        }
        return false;
    }

    /* 0x210 */ LIST_MEMBER(dSoundSource_c, SubSource); // -> dSoundSource_c::mSubSourceLink
};

class dSndSourceEnemyMulti_c : public dSndSourceEnemyMultiBase_c {
public:
    dSndSourceEnemyMulti_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceEnemyMultiBase_c(sourceType, ac, name, pOwnerGroup) {}

    /* 0x18C */ virtual void postCalc() override;

private:

};

#endif
