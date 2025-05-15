#include "d/lyt/d_lyt_do_button.h"

#include "common.h"
#include "sized_string.h"
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

STATE_DEFINE(dLytDobutton_c, InvisibleWait);
STATE_DEFINE(dLytDobutton_c, InvisibleTimeCnt);
STATE_DEFINE(dLytDobutton_c, In);
STATE_DEFINE(dLytDobutton_c, Wait);
STATE_DEFINE(dLytDobutton_c, Out);

#define DO_BUTTON_ANIM_IN 0
#define DO_BUTTON_ANIM 1
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

#define DO_BUTTON_NUM_ANIM 12

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
    if (field_0x488 == 0x29) {
        return;
    }

    if (mNextDoActionToShow == 0x5E) {
        return;
    }
    mStateMgr.changeState(StateID_InvisibleTimeCnt);
}
void dLytDobutton_c::finalizeState_InvisibleWait() {}

void dLytDobutton_c::initializeState_InvisibleTimeCnt() {
    field_0x48C = 0;
}

extern "C" void fn_8010DF00(dLytDobutton_c *);

void dLytDobutton_c::executeState_InvisibleTimeCnt() {
    if (field_0x488 != field_0x474 || mNextDoActionToShow != mDoActionToShow) {
        field_0x478 = field_0x474;
        field_0x474 = field_0x488;
        mDoActionToShow = mNextDoActionToShow;
        if (field_0x488 == 0x29 || mNextDoActionToShow == 0x5E) {
            mStateMgr.changeState(StateID_InvisibleWait);
        } else {
            field_0x48C = 0;
        }
    } else {
        if (field_0x48C < sDoButtonClass.field_0x15) {
            field_0x48C++;
        } else {
            fn_8010DF00(this);
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
    if (field_0x488 == 0x29 || mNextDoActionToShow == 0x5E) {
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
        if (field_0x488 == 0x29 || mNextDoActionToShow == 0x5E) {
            mStateMgr.changeState(StateID_Out);
            field_0x48C = 0;
        } else {
            fn_8010DF00(this);
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
        mStateMgr.changeState(StateID_InvisibleTimeCnt);
    }
}
void dLytDobutton_c::finalizeState_Out() {}

bool dLytDobutton_c::build(m2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("basicInfo_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < DO_BUTTON_NUM_ANIM; i++) {
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

    mDoActionToShow = 0x5E;
    field_0x474 = 0x29;
    field_0x478 = 0x29;
    mNextDoActionToShow = 0x5E;
    field_0x488 = 0x29;
    field_0x490 = 0;
    field_0x47C = 0x5E;
    field_0x480 = 0x29;
    field_0x491 = 0;
    field_0x492 = 0;

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
    for (int i = 0; i < DO_BUTTON_NUM_ANIM; i++) {
        mAnmGroups[i].remove();
    }
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

    mAnmGroups[DO_BUTTON_ANIM].setAnimEnable(true);
    mAnmGroups[DO_BUTTON_ANIM].setFrame(i2);
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

s32 dLytDobutton_c::getActionInternal() const {
    return mDoActionToShow;
}
