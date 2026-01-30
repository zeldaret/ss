#ifndef D_LYT_DO_BUTTON_H
#define D_LYT_DO_BUTTON_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytDobutton_c {
public:
    // TODO: figure out that these mean. The two pane arrays in d_lyt_do_button.cpp might help
    enum Icon_e {
        ICON_0 = 0,
        ICON_1 = 1,
        ICON_2 = 2,
        ICON_3 = 3,
        ICON_4 = 4,
        ICON_5 = 5,
        ICON_6 = 6,
        ICON_7 = 7,
        ICON_8 = 8,
        ICON_9 = 9,
        ICON_10 = 10,
        ICON_11 = 11,
        ICON_12 = 12,
        ICON_13 = 13,
        ICON_14 = 14,
        ICON_15 = 15,
        ICON_16 = 16,
        ICON_17 = 17,
        ICON_18 = 18,
        ICON_19 = 19,
        ICON_20 = 20,
        ICON_21 = 21,
        ICON_22 = 22,
        ICON_23 = 23,
        ICON_24 = 24,
        ICON_25 = 25,
        ICON_26 = 26,
        ICON_27 = 27,
        ICON_28 = 28,
        ICON_29 = 29,
        ICON_30 = 30,
        ICON_31 = 31,
        ICON_32 = 32,
        ICON_33 = 33,
        ICON_34 = 34,
        ICON_35 = 35,
        ICON_36 = 36,
        ICON_37 = 37,
        ICON_38 = 38,
        ICON_39 = 39,
        ICON_40 = 40,

        ICON_NONE = 41,
    };

    // 001-Action.msbt - Enum for ACT_DO
    // This is actually just the first part of ACT_INFO, but
    // LytDoButtonRelated has a full conversion table
    // from Act_IE_e -> Act_Do_e

    enum Act_Do_e {
        ACT_DO_CLIMB_UP = 0,
        ACT_DO_JUMP = 1,
        ACT_DO_DROP_DOWN = 2,
        ACT_DO_LET_GO = 3,
        ACT_DO_OPEN = 4,
        ACT_DO_PUT_DOWN = 5,
        ACT_DO_THROW = 6,
        ACT_DO_BURROW = 7,
        ACT_DO_ROLL = 8,
        ACT_DO_9 = 9,
        ACT_DO_MOVE = 10,
        ACT_DO_GRAB = 11,
        ACT_DO_SHOOT = 12,
        ACT_DO_13 = 13,
        ACT_DO_14 = 14,
        ACT_DO_15 = 15,
        ACT_DO_TALK = 16,
        ACT_DO_EXAMINE = 17,
        ACT_DO_DRAW = 18,
        ACT_DO_19 = 19,
        ACT_DO_BLOW = 20,
        ACT_DO_21 = 21,
        ACT_DO_22 = 22,
        ACT_DO_SWIM = 23,
        ACT_DO_LEAP = 24,
        ACT_DO_DASH = 25,
        ACT_DO_GET_OUT = 26,
        ACT_DO_27 = 27,
        ACT_DO_PICK_UP = 28,
        ACT_DO_SIT = 29,
        ACT_DO_STAND = 30,
        ACT_DO_DIG = 31,
        ACT_DO_32 = 32,
        ACT_DO_33 = 33,
        ACT_DO_BREAK_FREE = 34,
        ACT_DO_35 = 35,
        ACT_DO_36 = 36,
        ACT_DO_PLANT = 37,
        ACT_DO_CATCH = 38,
        ACT_DO_SPEED_UP = 39,
        ACT_DO_SAILCLOTH = 40,
        ACT_DO_PUT_AWAY = 41,
        ACT_DO_EMERGE = 42,
        ACT_DO_INSERT = 43,
        ACT_DO_44 = 44,
        ACT_DO_TURN = 45,
        ACT_DO_PUSH_IN = 46,
        ACT_DO_47 = 47,
        ACT_DO_DRAW_BOW = 48,
        ACT_DO_49 = 49,
        ACT_DO_50 = 50,
        ACT_DO_51 = 51,
        ACT_DO_SWING = 52,
        ACT_DO_BALANCE = 53,
        ACT_DO_JOSTLE = 54,
        ACT_DO_55 = 55,
        ACT_DO_SLEEP = 56,
        ACT_DO_DROP = 57,
        ACT_DO_LAUNCH = 58,
        ACT_DO_READ = 59,
        ACT_DO_WHIP = 60,
        ACT_DO_PULL = 61,
        ACT_DO_DRINK = 62,
        ACT_DO_63 = 63,
        ACT_DO_GET_IN = 64,
        ACT_DO_FIRE = 65,
        ACT_DO_ATTACK = 66,
        ACT_DO_FATAL_BLOW = 67,
        ACT_DO_JUMP_DOWN = 68,
        ACT_DO_STRUM = 69,
        ACT_DO_70 = 70,
        ACT_DO_RELEASE = 71,
        ACT_DO_GRAB_2 = 72,
        ACT_DO_READY_STANCE = 73,
        ACT_DO_REMOVE = 74,
        ACT_DO_READY_SWORD = 75,
        ACT_DO_THRUST_SWORD = 76,
        ACT_DO_USE = 77,
        ACT_DO_SCOOP = 78,
        ACT_DO_79 = 79,
        ACT_DO_80 = 80,
        ACT_DO_LOOK = 81,
        ACT_DO_DOWSE = 82,
        ACT_DO_PILOT = 83,
        ACT_DO_LOOK_DOWN = 84,
        ACT_DO_DIVE = 85,
        ACT_DO_SELECT = 86,
        ACT_DO_GRAB_3 = 87,
        ACT_DO_MOVE_2 = 88,
        ACT_DO_ACCELERATE = 89,
        ACT_DO_CHARGE = 90,
        ACT_DO_LEAN = 91,
        ACT_DO_PRESS = 92,

        // why two of them?
        ACT_DO_NONE = 93,
        ACT_DO_INVALID = 94,
    };

    dLytDobutton_c() : mStateMgr(*this) {
        sInstance = this;
    }
    virtual ~dLytDobutton_c() {
        sInstance = nullptr;
    }

    bool build(m2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();

    static dLytDobutton_c *GetInstance() {
        return sInstance;
    }

    static Act_Do_e getField0x47C() {
        if (sInstance != nullptr) {
            return sInstance->field_0x47C;
        } else {
            return ACT_DO_INVALID;
        }
    }

    static s32 getField0x480() {
        if (sInstance != nullptr) {
            return sInstance->field_0x480;
        } else {
            return ICON_NONE;
        }
    }

    static Act_Do_e getNextActionToShow() {
        if (sInstance != nullptr) {
            return sInstance->mNextDoActionToShow;
        } else {
            // @bug (?) should this be ACT_DO_INVALID?
            return (Act_Do_e)ICON_NONE;
        }
    }

    static s32 getFn0x8010E5E0() {
        if (sInstance != nullptr) {
            return sInstance->fn_8010E5E0();
        } else {
            return ICON_NONE;
        }
    }

    static Act_Do_e getAction() {
        if (sInstance != nullptr) {
            return sInstance->getActionInternal();
        } else {
            return ACT_DO_INVALID;
        }
    }

    static void setActionTextStuff(s32 p1, Act_Do_e p2, bool p3) {
        if (sInstance != nullptr) {
            sInstance->setActionTextStuffInternal(p1, p2, p3);
        }
    }

private:
    void setActionTextStuffInternal(s32, Act_Do_e, bool);
    Act_Do_e getActionInternal() const;
    s32 fn_8010E5E0() const;
    void fn_8010E3D0(bool);
    void realize();

    void set_0x47C_0x480(Act_Do_e v1, s32 v2) {
        field_0x47C = v1;
        field_0x480 = v2;
    }

    static dLytDobutton_c *sInstance;

    STATE_FUNC_DECLARE(dLytDobutton_c, InvisibleWait);
    STATE_FUNC_DECLARE(dLytDobutton_c, InvisibleTimeCnt);
    STATE_FUNC_DECLARE(dLytDobutton_c, In);
    STATE_FUNC_DECLARE(dLytDobutton_c, Wait);
    STATE_FUNC_DECLARE(dLytDobutton_c, Out);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytDobutton_c);

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytDobutton_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnmGroups[12];
    /* 0x3D4 */ nw4r::lyt::Pane *mpPanes[37];
    /* 0x468 */ dTextBox_c *mpTextBoxes[2];
    /* 0x470 */ Act_Do_e mDoActionToShow;
    /* 0x474 */ s32 field_0x474;
    /* 0x478 */ s32 field_0x478;
    /* 0x47C */ Act_Do_e field_0x47C;
    /* 0x480 */ s32 field_0x480;
    /* 0x484 */ Act_Do_e mNextDoActionToShow;
    /* 0x488 */ s32 field_0x488;
    /* 0x48C */ s32 field_0x48C;
    /* 0x490 */ u8 field_0x490;
    /* 0x491 */ u8 field_0x491;
    /* 0x492 */ bool mSavedIsInEvent;
};

#endif
