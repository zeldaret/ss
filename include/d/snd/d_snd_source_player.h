#ifndef D_SND_SOURCE_PLAYER_H
#define D_SND_SOURCE_PLAYER_H

#include "d/snd/d_snd_source_anim_sound.h"

class dSndSourcePlayer_c : public dSndSourceAnimSound_c {
public:
    dSndSourcePlayer_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

    /* 0x200 */ u8 _0x200[0x238 - 0x200];
};

#endif
