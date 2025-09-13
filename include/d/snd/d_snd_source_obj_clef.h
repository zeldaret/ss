#ifndef D_SND_SOURCE_OBJ_CLEF_H
#define D_SND_SOURCE_OBJ_CLEF_H

#include "d/snd/d_snd_source_obj.h"

class dSndSourceObjClef_c : public dSndSourceObj_c {
public:
    dSndSourceObjClef_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSndSourceObj_c(sourceType, ac, name, pOwnerGroup) {}

    bool startTadtoneSound(u32 groupIdx, u32 noteIdx);
};

#endif
