#ifndef D_LYT_TARGET_BIRD_H
#define D_LYT_TARGET_BIRD_H

#include "d/lyt/d2d.h"

// size 0x27C
class dLytTargetBird_c {
public:
    dLytTargetBird_c() {
        sInstance = this;
    }
    ~dLytTargetBird_c() {
        sInstance = nullptr;
    }
    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();

    static dLytTargetBird_c *sInstance;

private:
    d2d::LytBase_c mLyt;
    d2d::AnmGroup_c mAnmGroups[7];
    /* 0x250 */ u8 field_0x250[0x27C - 0x250];
};

#endif
