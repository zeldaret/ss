#ifndef D_SND_SOURCE_NPC_SPECIAL_H
#define D_SND_SOURCE_NPC_SPECIAL_H

#include "common.h"
#include "d/snd/d_snd_source_npc.h"

class dSndSourceNpcSpecial_c : public dSndSourceNpcAnimBase_c {
public:
    dSndSourceNpcSpecial_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

private:
    /* 0x200 */ u8 _0x200[0x204 - 0x200];
};

class dSndSourceNpcDr_c : public dSndSourceNpcSpecial_c {
public:
    dSndSourceNpcDr_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

private:
    /* 0x204 */ u8 _0x204[0x20C - 0x204];
};

#endif
