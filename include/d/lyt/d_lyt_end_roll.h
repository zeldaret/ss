#ifndef D_LYT_END_ROLL_H
#define D_LYT_END_ROLL_H

#include "common.h"

// TODO
class dLytEndRoll_c {
public:
    static bool isField_0xF705() {
        return sInstance != nullptr && sInstance->field_0xF705;
    }

private:
    static dLytEndRoll_c *sInstance;

    u8 _0x0000[0xF705 - 0x0000];
    /* 0xF705 */ bool field_0xF705;
};

#endif
