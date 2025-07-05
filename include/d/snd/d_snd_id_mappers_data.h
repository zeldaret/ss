#ifndef D_SND_ID_MAPPERS_DATA_H
#define D_SND_ID_MAPPERS_DATA_H

#include "common.h"

struct ActorBaseNamePair {
    const char *variant;
    const char *base;
};

extern const ActorBaseNamePair sActorBaseNamePairs[];
extern const s32 sNumActorBaseNamePairs;

extern const char *sSndHitEffects[];

#endif
