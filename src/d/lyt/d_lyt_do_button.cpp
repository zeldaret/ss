#include "d/lyt/d_lyt_do_button.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/math/math_types.h"
#include "sized_string.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/small_sound_mgr.h"

struct DoButtonClass {
    DoButtonClass();
    virtual ~DoButtonClass();

    u8 field_0x04;
    u8 field_0x05;
    u8 field_0x06;
    u8 field_0x07;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
    u8 field_0x14;
    u8 field_0x15;
    u8 field_0x16;
};

static DoButtonClass sDoButtonClass;

DoButtonClass::DoButtonClass() {
    field_0x04 = 0;
    field_0x05 = 0x29;
    field_0x06 = 0x5E;
    field_0x07 = 0;
    field_0x08 = 0.0f;
    field_0x0C = 0.0f;
    field_0x14 = 0xFF;
    field_0x10 = 76.0f;
    field_0x15 = 0;
    field_0x16 = 0;
}
DoButtonClass::~DoButtonClass() {}

dLytDobutton_c *dLytDobutton_c::sInstance;

STATE_DEFINE(dLytDobutton_c, InvisibleWait);
STATE_DEFINE(dLytDobutton_c, InvisibleTimeCnt);
STATE_DEFINE(dLytDobutton_c, In);
STATE_DEFINE(dLytDobutton_c, Wait);
STATE_DEFINE(dLytDobutton_c, Out);

#define DO_BUTTON_ANIM_IN 0
#define DO_BUTTON_ANIM_BASIC_INFO 1
#define DO_BUTTON_ANIM_LOOP_BTN 2
#define DO_BUTTON_ANIM_LOOP_BTN_Z 3
#define DO_BUTTON_ANIM_LOOP_NUN 4
#define DO_BUTTON_ANIM_LOOP_REMO_CON 5
#define DO_BUTTON_ANIM_LOOP_RECOVER 6
#define DO_BUTTON_ANIM_LOOP_ROTATE 7
#define DO_BUTTON_ANIM_LOOP_MOVE_INFO 8
#define DO_BUTTON_ANIM_MOVE_ALPHA 9
#define DO_BUTTON_ANIM_MOVE_OUT 10
#define DO_BUTTON_ANIM_LOOP_BG 11

#define DO_BUTTON_NUM_ANIMS 12

static const d2d::LytBrlanMapping brlanMap[] = {
    {         "basicInfo_00_in.brlan",       "G_inOut_00"},
    {  "basicInfo_00_basicInfo.brlan",   "G_basicInfo_00"},
    {    "basicInfo_00_loopBtn.brlan",     "G_loopBtn_00"},
    {    "basicInfo_00_loopBtn.brlan",    "G_loopBtnZ_00"},
    {    "basicInfo_00_loopNun.brlan",     "G_loopNun_00"},
    {"basicInfo_00_loopRemoCon.brlan", "G_loopRemoCon_00"},
    {"basicInfo_00_loopRecover.brlan", "G_loopRecover_00"},
    { "basicInfo_00_loopRotate.brlan",     "G_loopRotate"},
    {   "basicInfo_00_moveInfo.brlan",    "G_moveInfo_00"},
    {  "basicInfo_00_moveAlpha.brlan",   "G_moveAlpha_00"},
    {        "basicInfo_00_out.brlan",       "G_inOut_00"},
    {     "basicInfo_00_loopBg.brlan",      "G_loopBg_00"},
};

static const char *sPaneNames[] = {
    "N_infoAll_00",  "N_infoAll_01",   "N_bg_00",    "W_bgP_00",    "W_bgP_01",      "N_infoText_00", "N_aBtn_00",
    "N_aBtn_01",     "N_bBtn_00",      "N_cBtn_00",  "N_cBtn_01",   "N_nunchaku_01", "N_nunchaku_02", "N_nunchaku_03",
    "N_upSwing_00",  "N_downSwing_00", "N_swing_00", "N_rotate_00", "N_nunRimo_00",  "N_nunDrag_00",  "N_aBtnDown_00",
    "N_aBtnDown_01", "N_draw_00",      "N_play_00",  "N_swing_01",  "N_swing_03",    "N_upset_00",    "N_stab_00",
    "N_pullOut_01",  "N_bBtn_01",      "N_twist_00", "N_zBtn_00",   "N_zBtn_01",     "N_crossBtn_00", "N_plusAll_00",
    "N_upSwing_01",  "N_aBtnDown_02",
};

#define DO_BUTTON_PANE_INFOALL_00 0
#define DO_BUTTON_PANE_INFOALL_01 1
#define DO_BUTTON_PANE_BG_00 2
#define DO_BUTTON_PANE_W_BGP_00 3
#define DO_BUTTON_PANE_W_BGP_01 4
#define DO_BUTTON_PANE_INFOTEXT_00 5
#define DO_BUTTON_PANE_A_BTN_00 6
#define DO_BUTTON_PANE_A_BTN_01 7
#define DO_BUTTON_PANE_B_BTN_00 8
#define DO_BUTTON_PANE_C_BTN_00 9
#define DO_BUTTON_PANE_C_BTN_01 10
#define DO_BUTTON_PANE_NUNCHAKU_01 11
#define DO_BUTTON_PANE_NUNCHAKU_02 12
#define DO_BUTTON_PANE_NUNCHAKU_03 13
#define DO_BUTTON_PANE_UP_SWING_00 14
#define DO_BUTTON_PANE_DOWN_SWING_00 15
#define DO_BUTTON_PANE_SWING_00 16
#define DO_BUTTON_PANE_ROTATE_00 17
#define DO_BUTTON_PANE_NUN_RIMO_00 18
#define DO_BUTTON_PANE_NUN_DRAG_00 19
#define DO_BUTTON_PANE_A_BTN_DOWN_00 20
#define DO_BUTTON_PANE_A_BTN_DOWN_01 21
#define DO_BUTTON_PANE_DRAW_00 22
#define DO_BUTTON_PANE_PLAY_00 23
#define DO_BUTTON_PANE_SWING_01 24
#define DO_BUTTON_PANE_SWING_03 25
#define DO_BUTTON_PANE_UPSET_00 26
#define DO_BUTTON_PANE_STAB_00 27
#define DO_BUTTON_PANE_PULL_OUT_01 28
#define DO_BUTTON_PANE_B_BTN_01 29
#define DO_BUTTON_PANE_TWIST_00 30
#define DO_BUTTON_PANE_ZBTN_00 31
#define DO_BUTTON_PANE_ZBTN_01 32
#define DO_BUTTON_PANE_CROSS_BTN_00 33
#define DO_BUTTON_PANE_PLUS_ALL_00 34
#define DO_BUTTON_PANE_UP_SWING_01 35
#define DO_BUTTON_PANE_A_BTN_DOWN_02 36

#define DO_BUTTON_PANE_NONE 37
#define DO_BUTTON_NUM_PANES 37

static const char *sTextBoxNames[] = {
    "T_infoText_00",
    "T_infoTextS_00",
};

#define DO_BUTTON_NUM_TEXT_BOXES 2

void dLytDobutton_c::initializeState_InvisibleWait() {}
void dLytDobutton_c::executeState_InvisibleWait() {
    if (field_0x488 == field_0x474 && mNextDoActionToShow == mDoActionToShow) {
        return;
    }

    field_0x478 = field_0x474;
    field_0x474 = field_0x488;
    mDoActionToShow = mNextDoActionToShow;
    if (field_0x488 == ICON_NONE) {
        return;
    }

    if (mNextDoActionToShow == ACT_DO_INVALID) {
        return;
    }
    mStateMgr.changeState(StateID_InvisibleTimeCnt);
}
void dLytDobutton_c::finalizeState_InvisibleWait() {}

void dLytDobutton_c::initializeState_InvisibleTimeCnt() {
    field_0x48C = 0;
}

void dLytDobutton_c::executeState_InvisibleTimeCnt() {
    if (field_0x488 != field_0x474 || mNextDoActionToShow != mDoActionToShow) {
        field_0x478 = field_0x474;
        field_0x474 = field_0x488;
        mDoActionToShow = mNextDoActionToShow;
        if (field_0x488 == ICON_NONE || mNextDoActionToShow == ACT_DO_INVALID) {
            mStateMgr.changeState(StateID_InvisibleWait);
        } else {
            field_0x48C = 0;
        }
    } else {
        if (field_0x48C < sDoButtonClass.field_0x15) {
            field_0x48C++;
        } else {
            realize();
            mStateMgr.changeState(StateID_In);
        }
    }
}
void dLytDobutton_c::finalizeState_InvisibleTimeCnt() {}

void dLytDobutton_c::initializeState_In() {
    mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setAnimEnable(true);
    mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setFrame(0.0f);
    mLyt.calc();
    mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setAnimEnable(false);

    mAnmGroups[DO_BUTTON_ANIM_IN].setAnimEnable(true);
    mAnmGroups[DO_BUTTON_ANIM_IN].setFrame(0.0f);
}
void dLytDobutton_c::executeState_In() {
    if (field_0x488 == ICON_NONE || mNextDoActionToShow == ACT_DO_INVALID) {
        mAnmGroups[DO_BUTTON_ANIM_IN].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setAnimEnable(true);
        mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setToEnd();
        mLyt.calc();
        mAnmGroups[DO_BUTTON_ANIM_IN].setAnimEnable(false);
        mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_InvisibleWait);
    } else if (mAnmGroups[DO_BUTTON_ANIM_IN].isEndReached()) {
        mAnmGroups[DO_BUTTON_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytDobutton_c::finalizeState_In() {}

void dLytDobutton_c::initializeState_Wait() {}
void dLytDobutton_c::executeState_Wait() {
    if (field_0x488 != field_0x474 || mNextDoActionToShow != mDoActionToShow) {
        field_0x478 = field_0x474;
        field_0x474 = field_0x488;
        mDoActionToShow = mNextDoActionToShow;
        if (field_0x488 == ICON_NONE || mNextDoActionToShow == ACT_DO_INVALID) {
            mStateMgr.changeState(StateID_Out);
            field_0x48C = 0;
        } else {
            realize();
        }
    }
}
void dLytDobutton_c::finalizeState_Wait() {}

void dLytDobutton_c::initializeState_Out() {
    mAnmGroups[DO_BUTTON_ANIM_IN].setAnimEnable(true);
    mAnmGroups[DO_BUTTON_ANIM_IN].setFrame(0.0f);
    mLyt.calc();
    mAnmGroups[DO_BUTTON_ANIM_IN].setAnimEnable(false);

    mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setAnimEnable(true);
    mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setFrame(0.0f);
}
void dLytDobutton_c::executeState_Out() {
    if (mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].isEndReached()) {
        mLyt.calc();
        mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_InvisibleWait);
    }
}
void dLytDobutton_c::finalizeState_Out() {}

bool dLytDobutton_c::build(m2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("basicInfo_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < DO_BUTTON_NUM_ANIMS; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].bind(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    for (int i = 0; i < DO_BUTTON_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sPaneNames[i]);
    }

    for (int i = 0; i < DO_BUTTON_NUM_TEXT_BOXES; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(sTextBoxNames[i]);
    }

    mDoActionToShow = ACT_DO_INVALID;
    field_0x474 = ICON_NONE;
    field_0x478 = ICON_NONE;
    mNextDoActionToShow = ACT_DO_INVALID;
    field_0x488 = ICON_NONE;
    field_0x490 = 0;
    field_0x47C = ACT_DO_INVALID;
    field_0x480 = ICON_NONE;
    field_0x491 = 0;
    mSavedIsInEvent = false;

    mAnmGroups[DO_BUTTON_ANIM_LOOP_NUN].setAnimEnable(true);
    mAnmGroups[DO_BUTTON_ANIM_LOOP_BG].setAnimEnable(true);
    mAnmGroups[DO_BUTTON_ANIM_IN].setAnimEnable(true);
    mAnmGroups[DO_BUTTON_ANIM_IN].setFrame(0.0f);
    mAnmGroups[DO_BUTTON_ANIM_MOVE_OUT].setAnimEnable(false);
    mLyt.calc();
    mAnmGroups[DO_BUTTON_ANIM_IN].setAnimEnable(false);

    mStateMgr.changeState(StateID_InvisibleWait);
    field_0x48C = 0;
    return true;
}

bool dLytDobutton_c::remove() {
    for (int i = 0; i < DO_BUTTON_NUM_ANIMS; i++) {
        mAnmGroups[i].remove();
    }
    return true;
}

bool dLytDobutton_c::execute() {
    bool isInEvent = EventManager::isInEvent();
    bool b1 = true;
    if (dAcPy_c::isInBambooCuttingMinigame()) {
        setActionTextStuff(ICON_12, ACT_DO_70, false);
    }
    if (field_0x490 == 0) {
        if (isInEvent != mSavedIsInEvent) {
            if (isInEvent && field_0x488 == field_0x480 && mNextDoActionToShow == field_0x47C) {
                setActionTextStuff(ICON_NONE, ACT_DO_INVALID, false);
                b1 = false;
            }
            mSavedIsInEvent = isInEvent;
        } else if (isInEvent && field_0x490 == 0 && field_0x488 == field_0x480 && mNextDoActionToShow == field_0x47C) {
            setActionTextStuff(ICON_NONE, ACT_DO_INVALID, false);
            b1 = false;
        }
    }

    if (b1) {
        set_0x47C_0x480(mNextDoActionToShow, field_0x488);
    }

    if ((isInEvent && !field_0x490) || dLytMeter_c::getItemSelect0x75A2() != 0 ||
        dLytMeter_c::getMinusBtnFn800F7600()) {
        setActionTextStuffInternal(41, ACT_DO_INVALID, false);
    }

    for (int i = 0; i < DO_BUTTON_NUM_ANIMS; i++) {
        if (i != DO_BUTTON_ANIM_BASIC_INFO && i != DO_BUTTON_ANIM_LOOP_MOVE_INFO && i != DO_BUTTON_ANIM_MOVE_ALPHA) {
            if (mAnmGroups[i].isEnabled()) {
                mAnmGroups[i].play();
            }
        }
    }

    if (mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].isEnabled() || mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].isEnabled()) {
        bool b2 = false;
        if (mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].isStop2()) {
            if (((field_0x478 == 26 && field_0x474 == 11) || (field_0x478 == 27 && field_0x474 == 11) ||
                 (field_0x478 == 28 && field_0x474 == 11) || (field_0x478 == 29 && field_0x474 == 11) ||
                 (field_0x478 == 30 && field_0x474 == 8) || (field_0x478 == 31 && field_0x474 == 11) ||
                 (field_0x478 == 32 && field_0x474 == 36) || (field_0x478 == 33 && field_0x474 == 11) ||
                 (field_0x478 == 34 && field_0x474 == 11) || (field_0x478 == 35 && field_0x474 == 11)) &&
                mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].getFrame() == 0.0f) {
                mAnmGroups[DO_BUTTON_ANIM_BASIC_INFO].setAnimEnable(true);
                mAnmGroups[DO_BUTTON_ANIM_BASIC_INFO].setFrame(field_0x474);
            }
            mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setAnimEnable(false);
        } else {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].play();
            b2 = true;
        }
        if (mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].isStop2()) {
            mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setAnimEnable(false);
        } else {
            mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].play();
            b2 = true;
        }

        if (b2) {
            mLyt.calc();
        }
        fn_8010E3D0(mDoActionToShow != ACT_DO_NONE);
    }

    mStateMgr.executeState();

    if (((field_0x474 == 1 && mDoActionToShow == ACT_DO_27) ||
         (field_0x474 == 1 && mDoActionToShow == ACT_DO_BREAK_FREE)) &&
        field_0x491) {
        if (!mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].isEnabled()) {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setFrame(0.0f);
            mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setRate(3.0f);
            mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setAnimEnable(true);
        }
        if (mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].isEnabled()) {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setRate(1.0f);
            mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setAnimEnable(false);
        }
        if (mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].isEnabled()) {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setRate(1.0f);
            mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setAnimEnable(false);
        }
    } else if ((field_0x474 == 6 && field_0x491) || (field_0x478 == 6 && *mStateMgr.getStateID() == StateID_Out)) {
        // Same code as above, copypasted
        if (!mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].isEnabled()) {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setFrame(0.0f);
            mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setRate(3.0f);
            mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setAnimEnable(true);
        }
        if (mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].isEnabled()) {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setRate(1.0f);
            mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setAnimEnable(false);
        }
        if (mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].isEnabled()) {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setRate(1.0f);
            mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setAnimEnable(false);
        }
    } else {
        if (mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].isEnabled()) {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setRate(1.0f);
            mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setAnimEnable(false);
        }

        if (field_0x474 == 4 || field_0x474 == 28 || field_0x474 == 29) {
            if (mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].isEnabled()) {
                mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setRate(1.0f);
                mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setAnimEnable(false);
            }
            if (!mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].isEnabled()) {
                mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setFrame(0.0f);
                mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setRate(1.0f);
                mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setAnimEnable(true);
            }
        } else if (field_0x474 == 12 && mDoActionToShow == ACT_DO_70) {
            if (!mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].isEnabled()) {
                mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setFrame(0.0f);
                mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setAnimEnable(true);
            }

            if (mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].getRate() != 2.0f) {
                mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setRate(2.0f);
            }

            if (mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].isEnabled()) {
                mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setRate(1.0f);
                mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setAnimEnable(false);
            }
        } else {
            if (!mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].isEnabled()) {
                mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setFrame(0.0f);
                mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setAnimEnable(true);
            }
            if (mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].getRate() != 1.0f) {
                mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setRate(1.0f);
            }
            if (mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].isEnabled()) {
                mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setRate(1.0f);
                mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setAnimEnable(false);
            }
        }
    }

    mLyt.calc();

    if (field_0x474 >= 26 && field_0x474 < 36) {
        if (!mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].isEnabled()) {
            if (mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN].isEnabled()) {
                if (mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN].getFrame() == 0.0f) {
                    mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN].setAnimEnable(false);
                }
            }
            if (mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN_Z].isEnabled()) {
                if (mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN_Z].getFrame() == 0.0f) {
                    mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN_Z].setAnimEnable(false);
                }
            }
        }
    } else {
        if (!mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN].isEnabled()) {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN].setAnimEnable(true);
        }

        if (!mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN_Z].isEnabled()) {
            mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN_Z].setAnimEnable(true);
        }
    }

    if (mAnmGroups[DO_BUTTON_ANIM_BASIC_INFO].isEnabled()) {
        mAnmGroups[DO_BUTTON_ANIM_BASIC_INFO].setAnimEnable(false);
    }

    setActionTextStuffInternal(41, ACT_DO_INVALID, false);
    return true;
}

bool dLytDobutton_c::draw() {
    mLyt.addToDrawList();
    return true;
}

static const s32 sActDoIds[] = {
    1,  // ACT_DO_CLIMB_UP
    2,  // ACT_DO_JUMP
    3,  // ACT_DO_DROP_DOWN
    4,  // ACT_DO_LET_GO
    5,  // ACT_DO_OPEN
    6,  // ACT_DO_PUT_DOWN
    7,  // ACT_DO_THROW
    8,  // ACT_DO_BURROW
    9,  // ACT_DO_ROLL
    10, // ACT_DO_9
    11, // ACT_DO_MOVE
    12, // ACT_DO_GRAB
    13, // ACT_DO_SHOOT
    14, // ACT_DO_13
    15, // ACT_DO_14
    16, // ACT_DO_15
    17, // ACT_DO_TALK
    18, // ACT_DO_EXAMINE
    19, // ACT_DO_DRAW
    20, // ACT_DO_19
    21, // ACT_DO_BLOW
    22, // ACT_DO_21
    23, // ACT_DO_22
    24, // ACT_DO_SWIM
    25, // ACT_DO_LEAP
    26, // ACT_DO_DASH
    27, // ACT_DO_GET_OUT
    28, // ACT_DO_27
    29, // ACT_DO_PICK_UP
    30, // ACT_DO_SIT
    31, // ACT_DO_STAND
    32, // ACT_DO_DIG
    33, // ACT_DO_32
    34, // ACT_DO_33
    35, // ACT_DO_BREAK_FREE
    36, // ACT_DO_35
    37, // ACT_DO_36
    38, // ACT_DO_PLANT
    39, // ACT_DO_CATCH
    40, // ACT_DO_SPEED_UP
    41, // ACT_DO_SAILCLOTH
    42, // ACT_DO_PUT_AWAY
    43, // ACT_DO_EMERGE
    44, // ACT_DO_INSERT
    45, // ACT_DO_44
    46, // ACT_DO_TURN
    47, // ACT_DO_PUSH_IN
    48, // ACT_DO_47
    49, // ACT_DO_DRAW_BOW
    50, // ACT_DO_49
    51, // ACT_DO_50
    52, // ACT_DO_51
    53, // ACT_DO_SWING
    54, // ACT_DO_BALANCE
    55, // ACT_DO_JOSTLE
    56, // ACT_DO_55
    57, // ACT_DO_SLEEP
    58, // ACT_DO_DROP
    59, // ACT_DO_LAUNCH
    60, // ACT_DO_READ
    61, // ACT_DO_WHIP
    62, // ACT_DO_PULL
    63, // ACT_DO_DRINK
    64, // ACT_DO_63
    65, // ACT_DO_GET_IN
    66, // ACT_DO_FIRE
    67, // ACT_DO_ATTACK
    68, // ACT_DO_FATAL_BLOW
    69, // ACT_DO_JUMP_DOWN
    70, // ACT_DO_STRUM
    71, // ACT_DO_70
    72, // ACT_DO_RELEASE
    73, // ACT_DO_GRAB_2
    74, // ACT_DO_READY_STANCE
    75, // ACT_DO_REMOVE
    76, // ACT_DO_READY_SWORD
    77, // ACT_DO_THRUST_SWORD
    78, // ACT_DO_USE
    79, // ACT_DO_SCOOP
    80, // ACT_DO_79
    81, // ACT_DO_80
    82, // ACT_DO_LOOK
    83, // ACT_DO_DOWSE
    84, // ACT_DO_PILOT
    85, // ACT_DO_LOOK_DOWN
    86, // ACT_DO_DIVE
    87, // ACT_DO_SELECT
    88, // ACT_DO_GRAB_3
    89, // ACT_DO_MOVE_2
    90, // ACT_DO_ACCELERATE
    91, // ACT_DO_CHARGE
    92, // ACT_DO_LEAN
    93, // ACT_DO_PRESS
    -1,
};

void dLytDobutton_c::realize() {
    // TODO: what?
    s32 i1 = field_0x478;
    s32 i2 = field_0x474;
    if ((i1 == 11 && i2 == 26) || (i1 == 11 && i2 == 27) || (i1 == 11 && i2 == 28) || (i1 == 11 && i2 == 29) ||
        (i1 == 8 && i2 == 30) || (i1 == 8 && i2 == 31) || (i1 == 36 && i2 == 32) || (i1 == 11 && i2 == 33) ||
        (i1 == 11 && i2 == 34) || (i1 == 11 && i2 == 35)) {
        mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN_Z].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_NUN].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setForwardOnce();
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setAnimEnable(true);
        mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setForwardOnce();
        mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setAnimEnable(true);
    } else if ((i1 == 26 && i2 == 11) || (i1 == 27 && i2 == 11) || (i1 == 28 && i2 == 11) || (i1 == 29 && i2 == 11) ||
               (i1 == 30 && i2 == 8) || (i1 == 31 && i2 == /* this one is changed 8 -> 11 */ 11) ||
               (i1 == 32 && i2 == 36) || (i1 == 33 && i2 == 11) || (i1 == 34 && i2 == 11) || (i1 == 35 && i2 == 11)) {
        // Same conditions as above, but inverted (with one exception. Is that exception a bug?)
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setBackwardsOnce();
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setAnimEnable(true);
        mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setBackwardsOnce();
        mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setAnimEnable(true);
        i2 = field_0x478;
    } else if (i2 >= 26 && i2 < 36) {
        mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_BTN_Z].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_NUN].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_REMO_CON].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_RECOVER].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_ROTATE].setFrame(0.0f);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setForwardOnce();
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setAnimEnable(true);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setToEnd2();
        mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setForwardOnce();
        mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setAnimEnable(true);
        mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setToEnd2();
    } else {
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setBackwardsOnce();
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setAnimEnable(true);
        mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].setToEnd2();
        if (field_0x474 == 11 || field_0x474 == 8) {
            mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setBackwardsOnce();
        } else {
            mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setForwardOnce();
        }
        mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setAnimEnable(true);
        mAnmGroups[DO_BUTTON_ANIM_MOVE_ALPHA].setToEnd2();
    }

    mAnmGroups[DO_BUTTON_ANIM_BASIC_INFO].setAnimEnable(true);
    mAnmGroups[DO_BUTTON_ANIM_BASIC_INFO].setFrame(i2);
    if (mDoActionToShow != ACT_DO_NONE) {
        SizedString<16> buf;
        buf.sprintf("ACT_DO_%03d", sActDoIds[mDoActionToShow]);
        mpTextBoxes[0]->setMessageWithGlobalTextProcessor2(buf, nullptr);
        mpTextBoxes[1]->setMessageWithGlobalTextProcessor2(buf, nullptr);
        fn_8010E3D0(true);
    } else {
        wchar_t buf1[2];
        buf1[0] = buf1[1] = 0;
        mpTextBoxes[0]->setTextWithGlobalTextProcessor(buf1, nullptr);
        mpTextBoxes[1]->setTextWithGlobalTextProcessor(buf1, nullptr);
        fn_8010E3D0(false);
    }

    if (mDoActionToShow == ACT_DO_CATCH) {
        SmallSoundManager::GetInstance()->playSound(SE_S_CALL_BIRD_CATCH);
    }
}

static const s32 sPaneIdxes1[] = {
    DO_BUTTON_PANE_A_BTN_00,      // ICON_0
    DO_BUTTON_PANE_SWING_00,      // ICON_1
    DO_BUTTON_PANE_UP_SWING_00,   // ICON_2
    DO_BUTTON_PANE_DOWN_SWING_00, // ICON_3
    DO_BUTTON_PANE_ROTATE_00,     // ICON_4
    DO_BUTTON_PANE_NONE,          // ICON_5
    DO_BUTTON_PANE_NUN_RIMO_00,   // ICON_6
    DO_BUTTON_PANE_A_BTN_00,      // ICON_7
    DO_BUTTON_PANE_C_BTN_00,      // ICON_8
    DO_BUTTON_PANE_NUN_DRAG_00,   // ICON_9
    DO_BUTTON_PANE_C_BTN_01,      // ICON_10
    DO_BUTTON_PANE_A_BTN_DOWN_00, // ICON_11
    DO_BUTTON_PANE_SWING_01,      // ICON_12
    DO_BUTTON_PANE_DRAW_00,       // ICON_13
    DO_BUTTON_PANE_PLAY_00,       // ICON_14
    DO_BUTTON_PANE_NONE,          // ICON_15
    DO_BUTTON_PANE_NUNCHAKU_01,   // ICON_16
    DO_BUTTON_PANE_UPSET_00,      // ICON_17
    DO_BUTTON_PANE_STAB_00,       // ICON_18
    DO_BUTTON_PANE_NONE,          // ICON_19
    DO_BUTTON_PANE_PULL_OUT_01,   // ICON_20
    DO_BUTTON_PANE_A_BTN_01,      // ICON_21
    DO_BUTTON_PANE_SWING_03,      // ICON_22
    DO_BUTTON_PANE_B_BTN_00,      // ICON_23
    DO_BUTTON_PANE_B_BTN_01,      // ICON_24
    DO_BUTTON_PANE_TWIST_00,      // ICON_25
    DO_BUTTON_PANE_A_BTN_DOWN_00, // ICON_26
    DO_BUTTON_PANE_A_BTN_DOWN_00, // ICON_27
    DO_BUTTON_PANE_A_BTN_DOWN_01, // ICON_28
    DO_BUTTON_PANE_A_BTN_DOWN_01, // ICON_29
    DO_BUTTON_PANE_C_BTN_00,      // ICON_30
    DO_BUTTON_PANE_ZBTN_00,       // ICON_31
    DO_BUTTON_PANE_ZBTN_01,       // ICON_32
    DO_BUTTON_PANE_A_BTN_DOWN_01, // ICON_33
    DO_BUTTON_PANE_A_BTN_DOWN_00, // ICON_34
    DO_BUTTON_PANE_A_BTN_DOWN_00, // ICON_35
    DO_BUTTON_PANE_ZBTN_00,       // ICON_36
    DO_BUTTON_PANE_CROSS_BTN_00,  // ICON_37
    DO_BUTTON_PANE_UP_SWING_01,   // ICON_38
    DO_BUTTON_PANE_A_BTN_DOWN_02, // ICON_39
    DO_BUTTON_PANE_A_BTN_DOWN_02, // ICON_40

};

static const s32 sPaneIdxes2[] = {
    DO_BUTTON_PANE_NONE,        // ICON_0
    DO_BUTTON_PANE_NONE,        // ICON_1
    DO_BUTTON_PANE_NONE,        // ICON_2
    DO_BUTTON_PANE_NONE,        // ICON_3
    DO_BUTTON_PANE_NONE,        // ICON_4
    DO_BUTTON_PANE_NONE,        // ICON_5
    DO_BUTTON_PANE_NONE,        // ICON_6
    DO_BUTTON_PANE_NONE,        // ICON_7
    DO_BUTTON_PANE_NONE,        // ICON_8
    DO_BUTTON_PANE_NONE,        // ICON_9
    DO_BUTTON_PANE_NONE,        // ICON_10
    DO_BUTTON_PANE_NONE,        // ICON_11
    DO_BUTTON_PANE_NONE,        // ICON_12
    DO_BUTTON_PANE_NONE,        // ICON_13
    DO_BUTTON_PANE_NONE,        // ICON_14
    DO_BUTTON_PANE_NONE,        // ICON_15
    DO_BUTTON_PANE_NONE,        // ICON_16
    DO_BUTTON_PANE_NONE,        // ICON_17
    DO_BUTTON_PANE_NONE,        // ICON_18
    DO_BUTTON_PANE_NONE,        // ICON_19
    DO_BUTTON_PANE_NONE,        // ICON_20
    DO_BUTTON_PANE_NONE,        // ICON_21
    DO_BUTTON_PANE_NONE,        // ICON_22
    DO_BUTTON_PANE_NONE,        // ICON_23
    DO_BUTTON_PANE_NONE,        // ICON_24
    DO_BUTTON_PANE_NONE,        // ICON_25
    DO_BUTTON_PANE_NUNCHAKU_02, // ICON_26
    DO_BUTTON_PANE_NUNCHAKU_03, // ICON_27
    DO_BUTTON_PANE_ROTATE_00,   // ICON_28
    DO_BUTTON_PANE_ROTATE_00,   // ICON_29
    DO_BUTTON_PANE_NUN_DRAG_00, // ICON_30
    DO_BUTTON_PANE_A_BTN_00,    // ICON_31
    DO_BUTTON_PANE_UP_SWING_00, // ICON_32
    DO_BUTTON_PANE_PLAY_00,     // ICON_33
    DO_BUTTON_PANE_PULL_OUT_01, // ICON_34
    DO_BUTTON_PANE_STAB_00,     // ICON_35
    DO_BUTTON_PANE_NONE,        // ICON_36
    DO_BUTTON_PANE_NONE,        // ICON_37
    DO_BUTTON_PANE_NONE,        // ICON_38
    DO_BUTTON_PANE_STAB_00,     // ICON_39
    DO_BUTTON_PANE_PULL_OUT_01, // ICON_40

};

void dLytDobutton_c::fn_8010E3D0(bool b) {
    s32 i1 = field_0x474;
    if (mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].isEnabled()) {
        if (mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].isPlayingForwardsOnce()) {
            if (mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].getFrame() <
                mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].getAnimDuration() * 0.5f) {
                i1 = field_0x478;
            }
        } else {
            if (mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].getFrame() >
                mAnmGroups[DO_BUTTON_ANIM_LOOP_MOVE_INFO].getAnimDuration() * 0.5f) {
                i1 = field_0x478;
            }
        }
    }

    s32 idx1 = sPaneIdxes1[i1];
    f32 w1 = mpPanes[idx1]->GetSize().width;
    f32 x1 = mpPanes[idx1]->GetTranslate().x;
    f32 left = x1 - w1 / 2.0f;
    f32 right = x1 + w1 / 2.0f;
    f32 xBase = mpPanes[5]->GetTranslate().x;

    s32 idx2 = sPaneIdxes2[i1];
    if (idx2 != 37) {
        f32 x2 = mpPanes[idx2]->GetTranslate().x;
        f32 w2 = mpPanes[idx2]->GetSize().width;
        f32 left2 = x2 - w2 / 2.0f;
        f32 right2 = x2 + w2 / 2.0f;
        if (left > left2) {
            left = left2;
        }
        if (right < right2) {
            right = right2;
        }
        w1 = right - left;
    }

    nw4r::lyt::Size sz3 = mpPanes[3]->GetSize();
    nw4r::lyt::Size sz4 = mpPanes[4]->GetSize();
    if (b) {
        f32 tmp = (xBase - left);
        w1 = mpTextBoxes[0]->GetLineWidth(nullptr) + tmp;
    }
    f32 w = w1 + sDoButtonClass.field_0x10;
    sz3.width = w;
    sz4.width = w;
    mpPanes[3]->SetSize(sz3);
    mpPanes[4]->SetSize(sz4);
    nw4r::math::VEC3 v = mpPanes[1]->GetTranslate();
    v.x = -left - w1 / 2.0f;
    mpPanes[1]->SetTranslate(v);
}

void dLytDobutton_c::setActionTextStuffInternal(s32 a1, s32 a2, bool b) {
    field_0x488 = a1;
    mNextDoActionToShow = a2;
    field_0x490 = b;
}

s32 dLytDobutton_c::getActionInternal() const {
    return mDoActionToShow;
}

s32 dLytDobutton_c::fn_8010E5E0() const {
    if (mDoActionToShow != ACT_DO_INVALID) {
        return field_0x474;
    }
    return ICON_NONE;
}
