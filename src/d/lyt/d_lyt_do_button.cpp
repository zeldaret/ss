#include "d/lyt/d_lyt_do_button.h"

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

bool dLytDobutton_c::init(m2d::ResAccIf_c *resAcc) {
    return brlanMap[0].mFile[0] != '\0' && sDoButtonClass.field_0x04;
}
