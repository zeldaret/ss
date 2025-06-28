#ifndef D_SND_SOURCE_PLAYER_HEAD_H
#define D_SND_SOURCE_PLAYER_HEAD_H

#include "d/snd/d_snd_source.h"

class dSndSourcePlayerHead_c : public dSoundSource_c {
public:
    dSndSourcePlayerHead_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

    /* 0x15C */ u8 _0x15C[0x22B0 - 0x15C];
};

#endif
