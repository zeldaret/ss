#ifndef D_SND_ID_MAPPERS_H
#define D_SND_ID_MAPPERS_H

#include "common.h"

// TODO move somewhere else, or maybe remove entirely
// could plausibly belong in d/col/bg
enum dSndMaterial_e {
    SND_MAT_NONE = 0,
    SND_MAT_TUTI = 1,
    SND_MAT_ROCK = 2,
    SND_MAT_SAND = 3,
    SND_MAT_GRASS = 4,
    SND_MAT_TREE = 5,
    SND_MAT_LAVA = 6,
    SND_MAT_WATER = 7,
    SND_MAT_STONE = 8,
    SND_MAT_LOTUS = 9,
    SND_MAT_METAL = 10,
    SND_MAT_NUMA = 11,
    SND_MAT_TUTA = 12,
    SND_MAT_LIFE = 13,
    SND_MAT_CARPET = 14,
    SND_MAT_QSAND = 15,
    SND_MAT_WOOD = 16,
    SND_MAT_DEATH = 17,

    SND_MAT_MAX = 18,
};

const char *getHitEffectName(u32 polyAttr0);

#endif
