#ifndef D_SND_SOURCE_E_SPARK_H
#define D_SND_SOURCE_E_SPARK_H

#include "d/snd/d_snd_source.h"

class dSndSourceESpark_c : public dSoundSource_c {
public:
    dSndSourceESpark_c(s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
        : dSoundSource_c(sourceType, ac, name, pOwnerGroup) {}

    // TODO - this one doesn't actually have an AnimSound. Maybe the name is wrong,
    // maybe something else is weird
    virtual bool hasAnimSound() override {
        return true;
    }
};

#endif
