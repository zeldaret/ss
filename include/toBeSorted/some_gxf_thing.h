#ifndef SOME_GFX_THING_H
#define SOME_GFX_THING_H

#include "common.h"

class SomeGfxThing;
extern SomeGfxThing *SOME_GFX_THING;

class SomeGfxThing {
public:
    typedef void (*drawCallback)(void);
    static void setDrawCallback(drawCallback cb) {
        SOME_GFX_THING->mDrawCb = cb; 
    }

    static void setField0x09(u8 val) {
        SOME_GFX_THING->field_0x09 = val;
    }

private:
    u8 _0x00[0x04 - 0x00];
    drawCallback mDrawCb;
    u8 field_0x08;
    u8 field_0x09;
};


#endif
