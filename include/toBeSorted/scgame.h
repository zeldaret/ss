#ifndef SCGAME_H
#define SCGAME_H

#include <common.h>

struct SpawnInfo {
    /* 0x00 */ char stageName[32];
    /* 0x20 */ s16 transitionFadeFrames;
    /* 0x22 */ s8 room;
    /* 0x23 */ s8 layer;
    /* 0x24 */ s8 entrance;
    /* 0x25 */ s8 night;
    /* 0x26 */ s8 trial;
    /* 0x27 */ s8 transitionType;
    /* 0x28 */ s32 unk;
};

class ScGame {
public:
    static SpawnInfo currentSpawnInfo;
    static SpawnInfo nextSpawnInfo;
    static ScGame *sInstance;
};

#endif
