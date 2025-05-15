#ifndef D_LYT_UNKNOWNS_H
#define D_LYT_UNKNOWNS_H

#include "common.h"
#include "d/lyt/d2d.h"

class LytDoButtonRelated {
public:
    LytDoButtonRelated();
    virtual ~LytDoButtonRelated() {
        sInstance = nullptr;
    }
    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();

    static LytDoButtonRelated *GetInstance() {
        return sInstance;
    }

    // 001-Action.msbt - Unified enum for ACT_INFO and ACT_ETC
    enum Act_IE_e {
        ACT_IE_INFO_CLIMB_UP = 0,
        ACT_IE_INFO_JUMP = 1,
        ACT_IE_INFO_DROP_DOWN = 2,
        ACT_IE_INFO_LET_GO = 3,
        ACT_IE_INFO_OPEN = 4,
        ACT_IE_INFO_DROP = 5,
        ACT_IE_INFO_THROW = 6,
        ACT_IE_INFO_BURROW = 7,
        ACT_IE_INFO_ROLL = 8,
        ACT_IE_INFO_9 = 9,
        ACT_IE_INFO_MOVE = 10,
        ACT_IE_INFO_GRAB = 11,
        ACT_IE_INFO_SHOOT = 12,
        ACT_IE_INFO_13 = 13,
        ACT_IE_INFO_14 = 14,
        ACT_IE_INFO_15 = 15,
        ACT_IE_INFO_TALK = 16,
        ACT_IE_INFO_EXAMINE = 17,
        ACT_IE_INFO_DRAW = 18,
        ACT_IE_INFO_19 = 19,
        ACT_IE_INFO_BLOW = 20,
        ACT_IE_INFO_21 = 21,
        ACT_IE_INFO_22 = 22,
        ACT_IE_INFO_SWIM = 23,
        ACT_IE_INFO_LEAP = 24,
        ACT_IE_INFO_DASH = 25,
        ACT_IE_INFO_GET_OUT = 26,
        ACT_IE_INFO_27 = 27,
        ACT_IE_INFO_PICK_UP = 28,
        ACT_IE_INFO_SIT = 29,
        ACT_IE_INFO_STAND = 30,
        ACT_IE_INFO_DIG = 31,
        ACT_IE_INFO_32 = 32,
        ACT_IE_INFO_33 = 33,
        ACT_IE_INFO_BREAK_FREE = 34,
        ACT_IE_INFO_35 = 35,
        ACT_IE_INFO_36 = 36,
        ACT_IE_INFO_PLANT = 37,
        ACT_IE_INFO_CATCH = 38,
        ACT_IE_INFO_SPEED_UP = 39,
        ACT_IE_INFO_SAILCLOTH = 40,
        ACT_IE_INFO_PUT_AWAY = 41,
        ACT_IE_INFO_EMERGE = 42,
        ACT_IE_INFO_INSERT = 43,
        ACT_IE_INFO_44 = 44,
        ACT_IE_INFO_TURN = 45,
        ACT_IE_INFO_ALIGN = 46,
        ACT_IE_INFO_47 = 47,
        ACT_IE_INFO_DRAW_BOW = 48,
        ACT_IE_INFO_49 = 49,
        ACT_IE_INFO_50 = 50,
        ACT_IE_INFO_51 = 51,
        ACT_IE_INFO_SWING = 52,
        ACT_IE_INFO_BALANCE = 53,
        ACT_IE_INFO_JOSTLE = 54,
        ACT_IE_INFO_55 = 55,
        ACT_IE_INFO_SLEEP = 56,
        ACT_IE_INFO_DROP_2 = 57,
        ACT_IE_INFO_LAUNCH = 58,
        ACT_IE_INFO_READ = 59,
        ACT_IE_INFO_WHIP = 60,
        ACT_IE_INFO_PULL = 61,
        ACT_IE_INFO_DRINK = 62,
        ACT_IE_INFO_63 = 63,
        ACT_IE_INFO_GET_IN = 64,
        ACT_IE_INFO_FIRE = 65,
        ACT_IE_INFO_ATTACK = 66,
        ACT_IE_INFO_FATAL_BLOW = 67,
        ACT_IE_INFO_JUMP_DOWN = 68,
        ACT_IE_INFO_STRUM = 69,
        ACT_IE_INFO_70 = 70,
        ACT_IE_INFO_RELEASE = 71,
        ACT_IE_INFO_GRAB_2 = 72,
        ACT_IE_INFO_READY_STANCE = 73,
        ACT_IE_INFO_REMOVE = 74,
        ACT_IE_INFO_READY_SWORD = 75,
        ACT_IE_INFO_THRUST_SWORD = 76,
        ACT_IE_INFO_USE = 77,
        ACT_IE_INFO_SCOOP = 78,
        ACT_IE_INFO_79 = 79,
        ACT_IE_INFO_80 = 80,
        ACT_IE_INFO_LOOK = 81,
        ACT_IE_INFO_DOWSE = 82,
        ACT_IE_INFO_PILOT = 83,
        ACT_IE_INFO_LOOK_DOWN = 84,
        ACT_IE_INFO_DIVE = 85,
        ACT_IE_INFO_SELECT = 86,
        ACT_IE_INFO_GRAB_3 = 87,
        ACT_IE_INFO_MOVE_1 = 88,
        ACT_IE_INFO_ACCELERATE = 89,
        ACT_IE_INFO_CHARGE = 90,
        ACT_IE_INFO_LEAN = 91,
        ACT_IE_INFO_PRESS = 92,
        ACT_IE_SEPARATOR = 93,
        ACT_IE_ETC_LOCK_ON = 94,
        ACT_IE_ETC_DASH = 95,
        ACT_IE_ETC_RAISE_SHIELD = 96,
        ACT_IE_ETC_97 = 97,
        ACT_IE_ETC_SHIELD_BASH = 98,
        ACT_IE_ETC_99 = 99,
        ACT_IE_ETC_ROLL = 100,
        ACT_IE_ETC_DODGE = 101,
        ACT_IE_ETC_102 = 102,
        ACT_IE_ETC_VIEW_SURFACE = 103,
        ACT_IE_ETC_PUT_AWAY = 104,
        ACT_IE_ETC_105 = 105,
        ACT_IE_ETC_106 = 106,
        ACT_IE_ETC_CENTER = 107,
        ACT_IE_ETC_BACK = 108,
        ACT_IE_ETC_WORLD_MAP = 109,
        ACT_IE_ETC_ZOOM_OUT = 110,
        ACT_IE_ETC_ZOOM_IN = 111,
        ACT_IE_ETC_SCROLL = 112,
        ACT_IE_ETC_CURRENT_LOCATION = 113,
        ACT_IE_ETC_114 = 114,
        ACT_IE_ETC_115 = 115,
        ACT_IE_ETC_BACK_2 = 116,
        ACT_IE_ETC_ADD_BEACON = 117,
        ACT_IE_ETC_REMOVE_BEACON = 118,
        ACT_IE_ETC_ROTATE_MAP = 119,
        ACT_IE_ETC_ROTATE_MAP_2 = 120,
        ACT_IE_ETC_121 = 121,
        ACT_IE_ETC_122 = 122,
        ACT_IE_ETC_123 = 123,
        ACT_IE_ETC_124 = 124,
        ACT_IE_ETC_GET_OFF = 125,
        ACT_IE_ETC_STORE = 126,
        ACT_IE_ETC_RETURN = 127,
        ACT_IE_ETC_MOVE = 128,
        ACT_IE_ETC_VIEW_LOCK = 129,
        ACT_IE_ETC_RESET_POSITION = 130,
        ACT_IE_ETC_CANCEL = 131,
        ACT_IE_ETC_STOP = 132,
        ACT_IE_ETC_ITEMS = 133,
        ACT_IE_ETC_SPIN = 134,
        ACT_IE_ETC_DONE = 135,
        ACT_IE_ETC_RESET_PANELS = 136,
        ACT_IE_ETC_BACK_3 = 137,
        ACT_IE_ETC_SLOW_DOWN = 138,
        ACT_IE_ETC_DISPLAY = 139,
        ACT_IE_ETC_SELECT_FLOOR = 140,
        ACT_IE_ETC_SHOW_ISLAND_NAME = 141,
        ACT_IE_ETC_HIDE_ISLAND_NAME = 142,
        ACT_IE_ETC_READY_CANNON = 143,
        ACT_IE_ETC_STOW_CANNON = 144,
        ACT_IE_ETC_SAILCLOTH = 145,
        ACT_IE_ETC_RETURN_2 = 146,

        ACT_IE_NONE = 152,
    };

    enum DoButton_e {
        DO_BUTTON_A = 0,
        DO_BUTTON_B = 1,
        DO_BUTTON_C = 2,
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

    static void set(DoButton_e button, Act_IE_e value) {
        if (sInstance != nullptr) {
            LytDoButtonRelated *b = sInstance;
            b->field_0x04[button] = value;
            b->field_0x34[button] = true;
        }
    }

    static void reset(DoButton_e button, Act_IE_e value) {
        LytDoButtonRelated *b = sInstance;
        if (b != nullptr) {
            b->field_0x04[button] = value;
            b->field_0x34[button] = false;
        }
    }

    static Act_IE_e get(DoButton_e button) {
        if (sInstance != nullptr) {
            return sInstance->field_0x04[button];
        } else {
            return ACT_IE_NONE;
        }
    }

    static bool getHas(DoButton_e button) {
        if (sInstance != nullptr) {
            return sInstance->field_0x34[button];
        } else {
            return false;
        }
    }


    static Act_IE_e convertDoButton(s32);

    static void fn_8010EC10(Act_IE_e, bool);
    static void fn_8010ED50(Act_IE_e, bool);

private:
    static LytDoButtonRelated *sInstance;
    /* 0x04 */ Act_IE_e field_0x04[12];
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
    bool execute();
    bool draw();

    static LytBirdButtonRelated *sInstance;

private:
    d2d::LytBase_c mLyt;
    d2d::AnmGroup_c mAnmGroups[7];
    /* 0x250 */ u8 field_0x250[0x27C - 0x250];
};

#endif
