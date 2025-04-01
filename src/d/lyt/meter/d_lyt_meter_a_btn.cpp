#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "d/lyt/meter/d_lyt_meter_a_btn.h"


STATE_DEFINE(dLytMeterABtn_c, InvisibleWait);
STATE_DEFINE(dLytMeterABtn_c, In);
STATE_DEFINE(dLytMeterABtn_c, Out);
STATE_DEFINE(dLytMeterABtn_c, Wait);
STATE_DEFINE(dLytMeterABtn_c, On);
STATE_DEFINE(dLytMeterABtn_c, Active);
STATE_DEFINE(dLytMeterABtn_c, Off);

void dLytMeterABtn_c::initializeState_InvisibleWait() {}
void dLytMeterABtn_c::executeState_InvisibleWait() {
    if (field_0x1C8 == 0) {
        return;
    }

    if (field_0x1CA == 0) {
        return;
    }

    mStateMgr.changeState(StateID_In);
}
void dLytMeterABtn_c::finalizeState_InvisibleWait() {}

void dLytMeterABtn_c::initializeState_In() {
    mpContainerAnmGroup1->setToStart();
    mpContainerAnmGroup1->setAnimEnable(true);
    mAnmGroups[0].setBackwardsOnce();
    mAnmGroups[0].setToStart();
}
void dLytMeterABtn_c::executeState_In() {
    if (mpContainerAnmGroup1->isEndReached()) {
        mpContainerAnmGroup1->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
    if (mpContainerAnmGroup1->isEnabled()) {
        mpContainerAnmGroup1->play();
    }
}
void dLytMeterABtn_c::finalizeState_In() {}

void dLytMeterABtn_c::initializeState_Out() {
    mpContainerAnmGroup2->setToStart();
    mpContainerAnmGroup2->setAnimEnable(true);
}
void dLytMeterABtn_c::executeState_Out() {
    if (mpContainerAnmGroup2->isEndReached()) {
        mpContainerAnmGroup2->setAnimEnable(false);
        mStateMgr.changeState(StateID_InvisibleWait);
    }
    if (mpContainerAnmGroup2->isEnabled()) {
        mpContainerAnmGroup2->play();
    }
}
void dLytMeterABtn_c::finalizeState_Out() {}

void dLytMeterABtn_c::initializeState_Wait() {}
void dLytMeterABtn_c::executeState_Wait() {}
void dLytMeterABtn_c::finalizeState_Wait() {}

void dLytMeterABtn_c::initializeState_On() {}
void dLytMeterABtn_c::executeState_On() {}
void dLytMeterABtn_c::finalizeState_On() {}

void dLytMeterABtn_c::initializeState_Active() {}
void dLytMeterABtn_c::executeState_Active() {}
void dLytMeterABtn_c::finalizeState_Active() {}

void dLytMeterABtn_c::initializeState_Off() {}
void dLytMeterABtn_c::executeState_Off() {}
void dLytMeterABtn_c::finalizeState_Off() {}

static const d2d::LytBrlanMapping brlanMap[] = {
    {"remoConBtn_06_input.brlan", "G_input_00"},
    { "remoConBtn_06_text.brlan",  "G_text_00"},
    { "remoConBtn_06_loop.brlan",  "G_loop_00"},
};

#define A_BTN_ANIM_INPUT 0
#define A_BTN_ANIM_TEXT 1
#define A_BTN_ANIM_LOOP 2

static const char *sTextBoxes[] = {
    "T_aBtn_00",
    "T_aBtnS_00",
};

bool dLytMeterABtn_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_06.brlyt", nullptr);

    for (int i = 0; i < 3; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].setDirection(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    static const char *sPane = "N_all_0";
    mpPane = mLyt.findPane(sPane);

    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(sTextBoxes[i]);
    }

    static const char *sWindow = "W_bg_P_00";
    mpWindow = mLyt.getWindow(sWindow);
    mpSizeBox = mLyt.getSizeBoxInWindow(sWindow);

    field_0x1C4 = 0x5F;
    field_0x1B8 = 0x5F;
    field_0x1BC = 0x5F;
    field_0x1C8 = 0;
    field_0x1CA = 0;
    field_0x1C9 = 0;
    field_0x1C0 = 1;
    field_0x1CB = 0;
    field_0x1A4 = 0;
    mAnmGroups[A_BTN_ANIM_TEXT].setAnimEnable(true);
    fn_800E0870(field_0x1B8);
    mAnmGroups[A_BTN_ANIM_TEXT].setFrame(0.0f);
    mAnmGroups[A_BTN_ANIM_INPUT].setToEnd();
    mAnmGroups[A_BTN_ANIM_INPUT].setAnimEnable(true);
    mAnmGroups[A_BTN_ANIM_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnmGroups[A_BTN_ANIM_INPUT].setAnimEnable(false);
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterABtn_c::remove() {
    for (int i = 0; i < 3; i++) {
        mAnmGroups[i].unbind();
        mAnmGroups[i].afterUnbind();
    }
    return true;
}
