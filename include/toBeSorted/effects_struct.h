#ifndef EFFECTS_STRUCT_H
#define EFFECTS_STRUCT_H

#include "common.h"
#include "d/d_base.h"

class EffectsStruct {
private:
    u8 field_0x00[0x1C - 0x00];

public:
    // vt at 0x1C
    EffectsStruct();
    EffectsStruct(dBase_c *);
    virtual ~EffectsStruct();

    inline void init(dBase_c *owner) {
        mpOwner = owner;
    }

private:
    u8 field_0x20[0x28 - 0x20];
    /* 0x28 */ dBase_c *mpOwner;
    u8 field_0x2C[0x34 - 0x2C];
};

#endif
