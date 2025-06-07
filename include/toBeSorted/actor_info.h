#ifndef D_ACTOR_INFO_H
#define D_ACTOR_INFO_H

#include "common.h"

// TODO: Expand when we figure out what the other entries have in common
enum SoundSourceCategory_e {
    SND_SOURCE_PLAYER = 0,
    SND_SOURCE_PLAYER_HEAD = 1,
    SND_SOURCE_NET = 2,
    SND_SOURCE_BLADE = 3,
    SND_SOURCE_SHIELD = 4,
    SND_SOURCE_WHIP = 5,

    SND_SOURCE_ARROW = 7,

    SND_SOURCE_HOOKSHOT = 9,

    SND_SOURCE_BULLET = 29,
    SND_SOURCE_OBJECT = 32,
    SND_SOURCE_ITEM = 35,
    SND_SOURCE_TERRY_SHOP = 36,
    SND_SOURCE_TIME_STONE = 37,
    SND_SOURCE_CLEF = 38,
    SND_SOURCE_SHUTTER = 39,
    SND_SOURCE_KENSEI = 44,
    SND_SOURCE_PLAYER_BIRD = 45,
    SND_SOURCE_INSECT = 49,
    SND_SOURCE_NPC_NRM = 51,
    SND_SOURCE_NPC_DRAGON = 52,
    SND_SOURCE_TG_SOUND = 53,
    SND_SOURCE_SW_HARP = 56,
};

/**
 * profileId and profileId2 are always the same.
 */
struct ActorInfo {
    /* 0x00 */ const char *name;
    /* 0x04 */ u16 profileId;
    /* 0x06 */ u16 profileId2;
    /* 0x08 */ u16 fiTextEntryId; // also used for kill counters
    /* 0x0A */ s8 soundSourceCategory;
    /* 0x0B */ u8 subtype;
};

const ActorInfo *getActorInfoByName(const char *name);
u16 getProfileIdForName(const char *name);
u16 getProfileId2ForName(const char *name);
const char *getNameForProfileId(u32 profileId);
const char *getActorName(const ActorInfo *actorInfo);
const ActorInfo *getActorInfoByProfileAndSubtype(u32 profileId, u32 subtype);
s32 getSoundSourceCategoryForName(const char *name);

#endif
