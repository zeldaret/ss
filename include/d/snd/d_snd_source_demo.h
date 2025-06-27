#ifndef D_SND_SOURCE_DEMO_H
#define D_SND_SOURCE_DEMO_H

#include "d/snd/d_snd_source_anim_sound.h"

class dSndSourceDemo_c : public dSndSourceAnimSound_c {
public:
    dSndSourceDemo_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);
};

#endif
