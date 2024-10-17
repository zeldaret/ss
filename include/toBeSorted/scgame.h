#ifndef SCGAME_H
#define SCGAME_H

#include "common.h"
#include "sized_string.h"

struct SpawnInfo {
    /* 0x00 */ SizedString<32> stageName;
    /* 0x20 */ s16 transitionFadeFrames;
    /* 0x22 */ s8 room;
    /* 0x23 */ u8 layer;
    /* 0x24 */ s8 entrance;
    /* 0x25 */ s8 night;
    /* 0x26 */ s8 trial;
    /* 0x27 */ s8 transitionType;
    /* 0x28 */ s32 unk;

    bool isNight() {
        return night == 1;
    }
};

class ScGame {
public:
    static SpawnInfo currentSpawnInfo;
    static SpawnInfo nextSpawnInfo;
    static ScGame *sInstance;

    static bool isCurrentStage(const char *stageName);
    void triggerExit(s32 room, u8 exitIndex, s32 forcedNight = 2, s32 forcedTrial = 2);
};

#endif
