#ifndef D_SND_ID_MAPPERS_H
#define D_SND_ID_MAPPERS_H

#include "common.h"
#include "d/snd/d_snd_types.h"

u32 getGrpId(dSndSourceGroup_c *pGroup);
u32 getBnkSeId(dSndSourceGroup_c *pGroup);
u32 getSeId(dSndSourceGroup_c *pGroup);

const char *getHitEffectName(u32 polyAttr0);

#endif
