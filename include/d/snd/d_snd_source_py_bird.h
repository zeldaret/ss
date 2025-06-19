#ifndef D_SND_SOURCE_PY_BIRD_H
#define D_SND_SOURCE_PY_BIRD_H

#include "d/snd/d_snd_source_anim_sound.h"

class dSndSourcePyBird_c : public dSndSourceAnimSound_c {
public:
    dSndSourcePyBird_c(u8 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceAnimSound_c(sourceType, ac, name, pOwnerGroup), mSeqVarValue(0) {}

    virtual ~dSndSourcePyBird_c() {}

    /* 0x104 */ virtual void load(void *data, const char *name) override;

    /* 0x1CC */ virtual void postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) override;

private:
    /* 0x200 */ s32 mSeqVarValue;
};

#endif
