#ifndef D_SND_SOURCE_TG_SOUND_H
#define D_SND_SOURCE_TG_SOUND_H

#include "d/snd/d_snd_source.h"

class dSndSourceTgSound_c : public dSoundSource_c {
public:
    dSndSourceTgSound_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

private:
    /* 0x15C */ u8 _0x15C[0x164 - 0x15C];
};

#endif
