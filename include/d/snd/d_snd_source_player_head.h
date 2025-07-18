#ifndef D_SND_SOURCE_PLAYER_HEAD_H
#define D_SND_SOURCE_PLAYER_HEAD_H

#include "common.h"
#include "d/snd/d_snd_rng_link_voice.h"
#include "d/snd/d_snd_source.h"
#include "nw4r/snd/snd_SoundHandle.h"

class dSndSourcePlayerHead_c : public dSoundSource_c {
public:
    dSndSourcePlayerHead_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

    bool startVoiceSoundInternal(u32 id);

    /* 0x0D4 */ virtual bool startVoiceLine(u32 id) override;

    /* 0x0E0 */ virtual void stopCurrentActionSound() override {
        mSoundHandle.Stop(2);
        field_0x22A0 = SND_LV_MAX;
    }
    /* 0x0E4 */ virtual void stopActionSound(u32 id) override {
        if (isCurrentActionSound(id)) {
            stopCurrentActionSound();
        }
    }
    /* 0x0E8 */ virtual bool isPlayingActionSound() const override {
        return mSoundHandle.IsAttachedSound();
    }
    /* 0x0EC */ virtual bool isCurrentActionSound(u32 id) const override {
        return isPlayingActionSound() && id == mLastPlayedSound;
    }

    // TODO - some of these might not be inlines. It's probably possible to implement them
    // as not inlines in the source file for the correct order, but maybe when we figure out how
    // to fix the order in d_snd_source_mgr we might also be able to fix it here that way.

    /* 0x0D8 */ virtual bool startVoiceLine(const char *label) override {
        return false;
    }

    /* 0x184 */ virtual void preCalc() override {}

    /* 0x194 */ virtual u32 overrideStartSoundId(u32 soundId) override {
        return soundId;
    }

    /* 0x1EC */ virtual bool d_vt_0x30() override {
        return false;
    }

    /* 0x1F0 */ virtual bool vt_1F0() {
        return false;
    }

    /* 0x1F4 */ virtual UNKWORD d_vt_0x3C() override {
        return 1;
    }

private:
    void configureVoiceRngs();

    /* 0x015C */ u8 _0x015C[0x0160 - 0x015C];
    /* 0x0160 */ dSndRngLinkVoice_c mLinkVoiceRngs[SND_LV_MAX];
    /* 0x22A0 */ u32 field_0x22A0;
    /* 0x22A4 */ s32 mLastPlayedSound;
    /* 0x22A8 */ nw4r::snd::SoundHandle mSoundHandle;
};

#endif
