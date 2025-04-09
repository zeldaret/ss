#ifndef D_LYT_UNKNOWNS_H
#define D_LYT_UNKNOWNS_H

#include "common.h"
#include "d/lyt/d2d.h"

class LytDoButtonRelated {
public:
    LytDoButtonRelated();
    virtual ~LytDoButtonRelated();
    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();

    static LytDoButtonRelated *GetInstance() {
        return sInstance;
    }

    void set0x08(s32 value) {
        field_0x08 = value;
        field_0x35 = 1;
    }

    static s32 get0x08() {
        if (sInstance != nullptr) {
            return sInstance->field_0x08;
        } else {
            return 0x98;
        }
    }
    
private:

    static LytDoButtonRelated *sInstance;
    /* 0x04 */ u8 field_0x04[0x08 - 0x04];
    /* 0x08 */ s32 field_0x08;
    /* 0x0C */ u8 _0x08[0x35 - 0x0C];
    /* 0x35 */ u8 field_0x35;
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
    bool draw();

    static LytBirdButtonRelated *sInstance;


private:
    d2d::LytBase_c mLyt;
    d2d::AnmGroup_c mAnmGroups[7];
    /* 0x250 */ u8 field_0x250[0x27C - 0x250];
};

#endif
