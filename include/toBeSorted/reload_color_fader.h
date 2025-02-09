#ifndef RELOAD_COLOR_FADER_H
#define RELOAD_COLOR_FADER_H

#include "common.h"
class ReloadColorFader {
public:
    static ReloadColorFader *GetInstance();
    void fn_80067ED0();
    void fn_80067F00();
    void fn_80067F50();

    u8 field_0x00[0x0C - 0x00];
    s32 field_0x0C;
};

#endif
