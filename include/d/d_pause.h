#ifndef D_PAUSE_H
#define D_PAUSE_H

#include "common.h"

// reference to NSMBW PauseManage_c (im adding back the d)
class dPauseManager_c {
public:
    static dPauseManager_c *GetInstance() {
        return sInstance;
    }
    bool getField_0x25() const {
        return field_0x25;
    }

private:
    /* 0x00 */ u8 _00[0x25 - 0x00];
    /* 0x25 */ bool field_0x25;

    static dPauseManager_c *sInstance;
};

#endif
