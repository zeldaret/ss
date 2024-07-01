#ifndef SPAWN_STRUCT_H
#define SPAWN_STRUCT_H

#include <common.h>

struct SpawnStruct {
    /* 0x00 */ char mName[32]; // Could be SizedString<32>
    /* 0x20 */ u16 mTransitionFadeFrames;
    /* 0x22 */ s8 mRoom;
    /* 0x23 */ s8 mLayer;
    /* 0x24 */ s8 mEntrance;
    /* 0x25 */ bool mNight;
    /* 0x26 */ bool mTrial;
    /* 0x27 */ bool mTransitionType;
    /* 0x28 */ u8 UNK_0x28[0x2C - 0x28];

    static SpawnStruct sInstance;
};

#endif
