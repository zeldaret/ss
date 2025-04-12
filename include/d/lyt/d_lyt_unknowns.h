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
        DO_DASH = 0x5F,
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
        DO_BUTTON_A = 0,
        DO_BUTTON_B = 1,

        DO_BUTTON_Z = 3,
        DO_BUTTON_NUN_STK = 4,
        DO_BUTTON_REMOCON_BG = 5,
        DO_BUTTON_NUN_BG = 6,

        DO_BUTTON_CROSS_T = 7,
        DO_BUTTON_CROSS_D = 8,
        DO_BUTTON_CROSS_L = 9,
        DO_BUTTON_CROSS_R = 10,

        DO_BUTTON_PLUS = 11,
    };

    static void set(DoButton_e button, Action_e value) {
        LytDoButtonRelated *b = sInstance;
        if (b != nullptr) {
            b->field_0x04[button] = value;
            b->field_0x34[button] = true;
        }
    }

    static Action_e get(DoButton_e button) {
        if (sInstance != nullptr) {
            return sInstance->field_0x04[button];
        } else {
            return DO_NONE;
        }
    }

    static bool getHas(DoButton_e button) {
        if (sInstance != nullptr) {
            return sInstance->field_0x34[button];
        } else {
            return false;
        }
    }


    static Action_e get0x08() {
        if (sInstance != nullptr) {
            return sInstance->field_0x04[1];
        } else {
            return DO_NONE;
        }
    }

    static Action_e convertDoButton(s32);

    static void fn_8010EC10(Action_e, bool);
    static void fn_8010ED50(Action_e, bool);
    
private:

    static LytDoButtonRelated *sInstance;
    /* 0x04 */ Action_e field_0x04[12];
    /* 0x34 */ bool field_0x34[12];
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
