#ifndef D_SND_SOURCE_NPC_HEAD_H
#define D_SND_SOURCE_NPC_HEAD_H

#include "common.h"
#include "d/snd/d_snd_source.h"

class dSndSourceNpcHead_c : public dSoundSource_c {
public:
    dSndSourceNpcHead_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup);

    void setMainName(const char *name) {
        mpMainName = name;
    }

private:
    /* 0x15C */ UNKWORD field_0x15C;
    /* 0x160 */ const char *mpMainName;
};

#endif
