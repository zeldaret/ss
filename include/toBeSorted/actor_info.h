#ifndef D_ACTOR_INFO_H
#define D_ACTOR_INFO_H

#include "common.h"

/**
 * profileId and profileId2 are always the same.
 */
struct ActorInfo {
    /* 0x00 */ const char *name;
    /* 0x04 */ u16 profileId;
    /* 0x06 */ u16 profileId2;
    /* 0x08 */ u16 fiTextEntryId; // also used for kill counters
    /* 0x0A */ s8 soundSourceType;
    /* 0x0B */ u8 subtype;
};

const ActorInfo *getActorInfoByName(const char *name);
u16 getProfileIdForName(const char *name);
u16 getProfileId2ForName(const char *name);
const char *getNameForProfileId(u32 profileId);
const char *getActorName(const ActorInfo *actorInfo);
const ActorInfo *getActorInfoByProfileAndSubtype(u32 profileId, u32 subtype);
s32 getSoundSourceTypeForName(const char *name);

#endif
