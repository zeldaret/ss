#ifndef D_LYT_UNKNOWNS_H
#define D_LYT_UNKNOWNS_H

#include <common.h>
#include <d/lyt/d2d.h>

class LytDoButtonRelated {
public:
    LytDoButtonRelated();
    virtual ~LytDoButtonRelated();
    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();

    static LytDoButtonRelated *sInstance;

private:
    u8 field_0x04[0x40 - 0x04];
};

// size 0x27C
class LytBirdButtonRelated {
public:
    LytBirdButtonRelated() {
        sInstance = this;
    }
    ~LytBirdButtonRelated() {
        sInstance = nullptr;
    }
    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();

    static LytBirdButtonRelated *sInstance;


private:
    d2d::LytBase_c mLyt;
    d2d::AnmGroup_c mAnmGroups[7];
    /* 0x250 */ u8 field_0x250[0x27C - 0x250];
};

#endif
