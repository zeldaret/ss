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

    enum Action_e {
        DO_RETURN = 0x7F,
        DO_STOP = 0x84,
        DO_ITEMS = 0x85,
        DO_DONE = 0x87,
        DO_READY_CANNON = 0x8F,
        DO_STOW_CANNON = 0x90,
        DO_SAILCLOTH = 0x91,
        DO_NONE = 0x98,
    };

    enum DoButton_e {
        DO_BUTTON_B = 1
    };

    void set(DoButton_e button, Action_e value) {
        field_0x04[button] = value;
        field_0x34[button] = 1;
    }

    static Action_e get(DoButton_e button) {
        if (sInstance != nullptr) {
            return sInstance->field_0x04[button];
        } else {
            return DO_NONE;
        }
    }


    static Action_e get0x08() {
        if (sInstance != nullptr) {
            return sInstance->field_0x04[1];
        } else {
            return DO_NONE;
        }
    }
    
private:

    static LytDoButtonRelated *sInstance;
    /* 0x04 */ Action_e field_0x04[12];
    /* 0x34 */ u8 field_0x34[12];
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
