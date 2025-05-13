#ifndef RELOAD_COLOR_FADER_H
#define RELOAD_COLOR_FADER_H

#include "common.h"
class ReloadColorFader {
public:
    static ReloadColorFader *GetInstance();
    void fn_80067ED0();
    void fn_80067EF0(s32);
    void fn_80067F00();
    void fn_80067F50();
    void fn_80067FE0();
    bool fn_80068E80();
    bool fn_80067F60();
    void fn_80067DD0(bool);

    bool is1Or5() const {
        return field_0x0C == 1 || field_0x0C == 5;
    }

    u8 field_0x00[0x0C - 0x00];
    s32 field_0x0C;
};

#endif
