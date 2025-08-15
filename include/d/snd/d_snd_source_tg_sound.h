#ifndef D_SND_SOURCE_TG_SOUND_H
#define D_SND_SOURCE_TG_SOUND_H

#include "d/snd/d_snd_source.h"

class dSndSourceTgSound_c : public dSoundSource_c {
public:
    dSndSourceTgSound_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

    /* 0x184 */ virtual void preCalc() override {}
    /* 0x188 */ virtual void postSetup() override;
    /* 0x18C */ virtual void postCalc() override;

    /* 0x1CC */ virtual void postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) override;

    void setTgActive(bool active);

private:
    /* 0x15C */ u32 mTgSoundId;
    /* 0x160 */ bool mTgActive;
};

#endif
