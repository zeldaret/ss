#include "d/lyt/msg_window/d_lyt_msg_window_select_btn.h"

#include "common.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/lyt/d2d.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"

STATE_DEFINE(dLytMsgWindowSelectBtnParts_c, Wait);
STATE_DEFINE(dLytMsgWindowSelectBtnParts_c, On);
STATE_DEFINE(dLytMsgWindowSelectBtnParts_c, Select);
STATE_DEFINE(dLytMsgWindowSelectBtnParts_c, Off);

STATE_DEFINE(dLytMsgWindowSelectBtn_c, Wait);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, In);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, WaitSelect);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, CursorInOut);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, WaitDecide);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, WaitCancel);
STATE_DEFINE(dLytMsgWindowSelectBtn_c, Out);

SelectBtnHelper::SelectBtnHelper() {
    field_0x4C = -1;
    field_0x50 = -1;
    field_0x44 = field_0x48 = 0;
    field_0x34 = field_0x3C = 0.0f;
    field_0x38 = field_0x40 = 0.0f;
    field_0x52 = 0;
    field_0x51 = 1;

    panes[0] = 0;
    panes[1] = 0;
    panes[2] = 0;
    panes[3] = 0;
    panes[4] = 0;
    panes[5] = 0;
    panes[6] = 0;
    panes[7] = 0;
    panes[8] = 0;
}
SelectBtnHelper::~SelectBtnHelper() {}

void dLytMsgWindowSelectBtnParts_c::initializeState_Wait() {}
void dLytMsgWindowSelectBtnParts_c::executeState_Wait() {
    if (field_0x48 != 0) {
        for (int i = 0; i <= 1; i++) {
            mpAnms[i]->setForwardOnce();
            mpAnms[i]->setFrame(0.0f);
            mpAnms[i]->setAnimEnable(true);
        }
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMsgWindowSelectBtnParts_c::finalizeState_Wait() {}

void dLytMsgWindowSelectBtnParts_c::initializeState_On() {}
void dLytMsgWindowSelectBtnParts_c::executeState_On() {
    if (mpAnms[0]->isEndReached() && mpAnms[1]->isEndReached()) {
        mpAnms[0]->setAnimEnable(false);
        mpAnms[1]->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }
    for (int i = 0; i <= 1; i++) {
        if (mpAnms[i]->isEnabled()) {
            mpAnms[i]->play();
        }
    }
}
void dLytMsgWindowSelectBtnParts_c::finalizeState_On() {}

void dLytMsgWindowSelectBtnParts_c::initializeState_Select() {}
void dLytMsgWindowSelectBtnParts_c::executeState_Select() {
    if (field_0x48 == 0) {
        for (int i = 0; i <= 1; i++) {
            mpAnms[i]->setBackwardsOnce();
            mpAnms[i]->setToStart();
            mpAnms[i]->setAnimEnable(true);
        }
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMsgWindowSelectBtnParts_c::finalizeState_Select() {}

void dLytMsgWindowSelectBtnParts_c::initializeState_Off() {}
void dLytMsgWindowSelectBtnParts_c::executeState_Off() {
    if (mpAnms[0]->isStop2() && mpAnms[1]->isStop2()) {
        mpAnms[0]->setAnimEnable(false);
        mpAnms[1]->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
    for (int i = 0; i <= 1; i++) {
        if (mpAnms[i]->isEnabled()) {
            mpAnms[i]->play();
        }
    }
}
void dLytMsgWindowSelectBtnParts_c::finalizeState_Off() {}

void dLytMsgWindowSelectBtnParts_c::init() {
    mStateMgr.changeState(StateID_Wait);
    field_0x48 = 0;
    field_0x50 = 0;
    field_0x4C = 3;
}

void dLytMsgWindowSelectBtnParts_c::execute() {
    mStateMgr.executeState();
    mpAnms[2]->play();
}

#define SELECT_BTN_ANIM_IN 0
#define SELECT_BTN_ANIM_LOOP_REMOCON 1
#define SELECT_BTN_ANIM_INOUT_CURSOR 2
#define SELECT_BTN_ANIM_MESSAGE_BTN 3
#define SELECT_BTN_ANIM_ITEM_ARROW_0 4
#define SELECT_BTN_ANIM_ITEM_ARROW_1 5
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_0 6
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_1 7
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_2 8
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_3 9
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0 10
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_1 11
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_2 12
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_3 13
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_0 14
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_1 15
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_2 16
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_3 17
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_0 18
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_1 19
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_2 20
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_3 21
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_0 22
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_1 23
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_2 24
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_3 25
#define SELECT_BTN_ANIM_OUT 26

#define SELECT_BTN_NUM_ANIMS 27
#define SELECT_BTN_NUM_BTNS 4
#define SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_0
#define SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_0
#define SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_0
#define SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_OFFSET SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_0

static const d2d::LytBrlanMapping brlanMap[] = {
    {         "messageBtn_00_in.brlan",       "G_inOut_00"},
    {"messageBtn_00_loopRemocon.brlan", "G_loopRemocon_00"},
    {"messageBtn_00_inOutCursor.brlan",  "G_inOutAlpha_00"},
    { "messageBtn_00_messageBtn.brlan",  "G_messageBtn_00"},
    {  "messageBtn_00_itemArrow.brlan",   "G_itemArrow_00"},
    {  "messageBtn_00_itemArrow.brlan",   "G_itemArrow_01"},
    {       "messageBtn_00_loop.brlan",         "G_btn_00"},
    {       "messageBtn_00_loop.brlan",         "G_btn_01"},
    {       "messageBtn_00_loop.brlan",         "G_btn_02"},
    {       "messageBtn_00_loop.brlan",         "G_btn_03"},
    {     "messageBtn_00_cancel.brlan",      "G_cancel_00"},
    {     "messageBtn_00_cancel.brlan",      "G_cancel_01"},
    {     "messageBtn_00_cancel.brlan",      "G_cancel_02"},
    {     "messageBtn_00_cancel.brlan",      "G_cancel_03"},
    {     "messageBtn_00_onOffB.brlan",      "G_onOffB_00"},
    {     "messageBtn_00_onOffB.brlan",      "G_onOffB_01"},
    {     "messageBtn_00_onOffB.brlan",      "G_onOffB_02"},
    {     "messageBtn_00_onOffB.brlan",      "G_onOffB_03"},
    {     "messageBtn_00_onOffA.brlan",         "G_btn_00"},
    {     "messageBtn_00_onOffA.brlan",         "G_btn_01"},
    {     "messageBtn_00_onOffA.brlan",         "G_btn_02"},
    {     "messageBtn_00_onOffA.brlan",         "G_btn_03"},
    {     "messageBtn_00_decide.brlan",         "G_btn_00"},
    {     "messageBtn_00_decide.brlan",         "G_btn_01"},
    {     "messageBtn_00_decide.brlan",         "G_btn_02"},
    {     "messageBtn_00_decide.brlan",         "G_btn_03"},
    {        "messageBtn_00_out.brlan",       "G_inOut_00"}
};

static const char *sSelectTextBoxes[SELECT_BTN_NUM_BTNS][2] = {
    {"T_selectS_00", "T_select_00"},
    {"T_selectS_01", "T_select_01"},
    {"T_selectS_02", "T_select_02"},
    {"T_selectS_03", "T_select_03"},
};

static const char *sDecideTextBoxes[2] = {
    "T_decide_00",
    "T_decideS_00",
};

static const char *sWindowName = "W_bgP_01";

static const char *sBoundings[SELECT_BTN_NUM_BTNS] = {
    "B_btn_00",
    "B_btn_01",
    "B_btn_02",
    "B_btn_03",
};

static const char *sSelectPanes[7] = {
    "N_itemArrow_00",  "N_arrowHand_00",  "N_aBtn_00",       "N_messageBtn_00",
    "N_messageBtn_01", "N_messageBtn_02", "N_messageBtn_03",
};

void dLytMsgWindowSelectBtn_c::initializeState_Wait() {
    field_0x9A4 = 0;
    field_0x9B8 = -1;
    field_0x9BC = -1;

    field_0x9CC = 0xFF;
    field_0x9C0 = 10;
    field_0x9CD = 1;
}
void dLytMsgWindowSelectBtn_c::executeState_Wait() {}
void dLytMsgWindowSelectBtn_c::finalizeState_Wait() {}

void dLytMsgWindowSelectBtn_c::initializeState_In() {
    field_0x9A4 = 1;
    field_0x9B4 = -1;
    for (int i = 0; i < 2; i++) {
        // "Select"
        mLyt.loadTextVariant(mpDecideTextBoxes[i], 1);
    }

    mpWindow->UpdateSize(mpSizeBox, 32.0f);

    for (int i = SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0; i <= SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_3; i++) {
        if (field_0x9BC == i - SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0) {
            mAnm[i].setFrame(1.0f);
        } else {
            mAnm[i].setFrame(0.0f);
        }
    }

    mBtnHelper.field_0x50 = -1;
    mBtnHelper.field_0x51 = 1;
    // ??????
    f32 v = mBtnHelper.fn_8011D690(mBtnHelper.field_0x50);

    mLyt.findPane("N_allBtn_00")->SetVisible(true);
    mLyt.findPane("N_arrowIn_00")->SetVisible(true);

    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setForwardOnce();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setToStart();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(true);

    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setFrame(0.0f);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setFrame(0.0f);

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mParts[i].init();
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET].setAnimEnable(true);
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET].setFrame(0.0f);
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET].setAnimEnable(true);
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET].setFrame(0.0f);
    }

    mLyt.calc();

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET].setAnimEnable(false);
        mAnm[i + SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET].setAnimEnable(false);
    }

    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(false);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setAnimEnable(false);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setAnimEnable(false);

    mLyt.findPane("N_allBtn_00")->SetVisible(false);
    mLyt.findPane("N_arrowIn_00")->SetVisible(false);

    mAnm[SELECT_BTN_ANIM_IN].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_IN].setForwardOnce();

    if (field_0x9D1 != 0) {
        mAnm[SELECT_BTN_ANIM_IN].setToEnd();
        field_0x9D1 = false;
    } else {
        if (field_0x9CF != 0) {
            if (field_0x9A0 == 1) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CHOICE_START);
            } else if (field_0x9A0 == 2) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CHOICE_START_GAMEOVER);
            } else {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CHOICE_START);
            }
        }
        mAnm[SELECT_BTN_ANIM_IN].setFrame(0.0f);
    }
}
void dLytMsgWindowSelectBtn_c::executeState_In() {
    mAnm[SELECT_BTN_ANIM_IN].play();
    if (mAnm[SELECT_BTN_ANIM_IN].isEndReached()) {
        mStateMgr.changeState(StateID_WaitSelect);
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_In() {
    mAnm[SELECT_BTN_ANIM_IN].setAnimEnable(false);
}

void dLytMsgWindowSelectBtn_c::initializeState_WaitSelect() {
    if (field_0x9D0 == 0) {
        field_0x9D0 = true;
        dSndPlayerMgr_c::GetInstance()->enterMsgWait();
    }

    mBtnHelper.fn_8011C970();
    mBtnHelper.field_0x48 = fn_8011FE50();
}

void dLytMsgWindowSelectBtn_c::executeState_WaitSelect() {
    u8 v = mBtnHelper.fn_8011CAC0();
    if (v == 1) {
        mStateMgr.changeState(StateID_CursorInOut);
        return;
    } else if (v == 2) {
        f32 f = mBtnHelper.fn_8011D690(mBtnHelper.field_0x50);
        // TODO
        for (int i = 0; i < 2; i++) {
            // "Confirm"
            mLyt.loadTextVariant(mpDecideTextBoxes[i], 0);
        }

        mpWindow->UpdateSize(mpSizeBox, 32.0f);
    } else if (v == 3) {
        field_0x9D1 = 1;
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_POINTER_RESET);
        mStateMgr.changeState(StateID_In);
        return;
    }

    if (mBtnHelper.field_0x50 >= 0 && dPad::getDownTrigA()) {
        field_0x9D0 = 0;
        field_0x9B0 = mBtnHelper.field_0x50;
        mStateMgr.changeState(StateID_WaitDecide);
        // TODO
        if (mpTagProcessor != nullptr) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR_CANCEL);
        } else {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR_OK);
        }
        dSndPlayerMgr_c::GetInstance()->leaveMsgWait();
    } else if (dPad::getDownTrigB()) {
        f32 f = mBtnHelper.fn_8011D690(field_0x9BC);
        // TODO
        field_0x9CC = field_0x9BC;
        field_0x9D0 = 0;
        mStateMgr.changeState(StateID_WaitCancel);
        // TODO
        if (mpTagProcessor != nullptr) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR_CANCEL);
        } else {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_TALK_CURSOR_OK);
        }
        dSndPlayerMgr_c::GetInstance()->leaveMsgWait();
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_WaitSelect() {}

void dLytMsgWindowSelectBtn_c::initializeState_CursorInOut() {}
void dLytMsgWindowSelectBtn_c::executeState_CursorInOut() {
    if (mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].isStop2()) {
        mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(false);
        mStateMgr.changeState(StateID_WaitSelect);
    }
    if (mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].isEnabled()) {
        mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].play();
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_CursorInOut() {}

void dLytMsgWindowSelectBtn_c::initializeState_WaitDecide() {
    s32 decideAnm = field_0x9B0 + SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_OFFSET;
    mAnm[decideAnm].setAnimEnable(true);
    mAnm[decideAnm].setFrame(0.0f);
    mAnm[decideAnm].setForwardOnce();
}
void dLytMsgWindowSelectBtn_c::executeState_WaitDecide() {
    s32 decideAnm = field_0x9B0 + SELECT_BTN_ANIM_MESSAGE_BTN_DECIDE_OFFSET;
    mAnm[decideAnm].play();
    if (mAnm[decideAnm].isStop2()) {
        mAnm[decideAnm].setAnimEnable(false);
        field_0x9B4 = field_0x9B0;
        if (field_0x99C == 1) {
            mStateMgr.changeState(StateID_Wait);
        } else {
            mStateMgr.changeState(StateID_Out);
        }
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_WaitDecide() {}

void dLytMsgWindowSelectBtn_c::initializeState_WaitCancel() {
    field_0x9C0 = 10;
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setForwardOnce();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setToEnd2();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(true);
    mLyt.calc();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(false);
    mpSelectPanes[0]->SetAlpha(0xFF);
}
void dLytMsgWindowSelectBtn_c::executeState_WaitCancel() {
    if (--field_0x9C0 <= 0) {
        field_0x9B0 = field_0x9CC;
        mStateMgr.changeState(StateID_WaitDecide);
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_WaitCancel() {}

void dLytMsgWindowSelectBtn_c::initializeState_Out() {
    mAnm[SELECT_BTN_ANIM_OUT].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_OUT].setFrame(0.0f);
    mAnm[SELECT_BTN_ANIM_OUT].setForwardOnce();
    field_0x9C8 = 0;
}
void dLytMsgWindowSelectBtn_c::executeState_Out() {
    switch (field_0x9C8) {
        case 0:
            mAnm[SELECT_BTN_ANIM_OUT].play();
            if (mAnm[SELECT_BTN_ANIM_OUT].isStop2()) {
                field_0x9A4 = 0;
                field_0x9C8 = 1;
            }
            break;
        case 1:
            mAnm[SELECT_BTN_ANIM_OUT].setAnimEnable(false);
            field_0x9B4 = field_0x9B0;
            mStateMgr.changeState(StateID_Wait);
            break;
    }
}
void dLytMsgWindowSelectBtn_c::finalizeState_Out() {}

bool dLytMsgWindowSelectBtn_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("messageBtn_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < SELECT_BTN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_0; i <= SELECT_BTN_ANIM_MESSAGE_BTN_CANCEL_3; i++) {
        mAnm[i].setAnimEnable(true);
    }

    mAnm[SELECT_BTN_ANIM_LOOP_REMOCON].setAnimEnable(true);

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mParts[i].mpAnms[0] = &mAnm[SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_A_OFFSET + i];
        mParts[i].mpAnms[1] = &mAnm[SELECT_BTN_ANIM_MESSAGE_BTN_ONOFF_B_OFFSET + i];
        mParts[i].mpAnms[2] = &mAnm[SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_OFFSET + i];
        mParts[i].init();
        mAnm[SELECT_BTN_ANIM_MESSAGE_BTN_LOOP_OFFSET + i].setAnimEnable(true);
    }

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        for (int j = 0; j < 2; j++) {
            mpSelectTextBoxes[i][j] = mLyt.getTextBox(sSelectTextBoxes[i][j]);
        }
    }

    for (int i = 0; i < 2; i++) {
        mpDecideTextBoxes[i] = mLyt.getTextBox(sDecideTextBoxes[i]);
    }

    mpWindow = mLyt.getWindow(sWindowName);
    mpSizeBox = mLyt.getSizeBoxInWindow(sWindowName);

    mpWindow->UpdateSize(mpSizeBox, 32.0f);

    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mpBoundings[i] = mLyt.findPane(sBoundings[i]);
    }

    mpTagProcessor = nullptr;

    for (int i = 0; i < 7; i++) {
        mpSelectPanes[i] = mLyt.findPane(sSelectPanes[i]);
    }

    mBtnHelper.panes[0] = mpSelectPanes[0];
    mBtnHelper.panes[1] = mpSelectPanes[3];
    mBtnHelper.panes[2] = mpSelectPanes[4];
    mBtnHelper.panes[3] = mpSelectPanes[5];
    mBtnHelper.panes[4] = mpSelectPanes[6];
    mBtnHelper.panes[5] = mpBoundings[0];
    mBtnHelper.panes[6] = mpBoundings[1];
    mBtnHelper.panes[7] = mpBoundings[2];
    mBtnHelper.panes[8] = mpBoundings[3];

    field_0x9C4 = 0;

    mLyt.findPane("N_arrowIn_00")->SetVisible(true);

    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setForwardOnce();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setToStart();
    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setFrame(0.0f);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setAnimEnable(true);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setFrame(0.0f);

    mLyt.calc();

    mAnm[SELECT_BTN_ANIM_INOUT_CURSOR].setAnimEnable(false);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].setAnimEnable(false);
    mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].setAnimEnable(false);

    mLyt.findPane("N_arrowIn_00")->SetVisible(false);

    mStateMgr.changeState(StateID_Wait);

    field_0x9A8 = -1;
    field_0x9B0 = -1;

    field_0x9D0 = 0;
    field_0x9D1 = 0;
    field_0x99C = 0;
    field_0x9A0 = 0;
    field_0x9CE = 0;
    field_0x9CF = 1;

    return true;
}

bool dLytMsgWindowSelectBtn_c::remove() {
    for (int i = 0; i < SELECT_BTN_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    if (mBtnHelper.field_0x52 != 0) {
        mBtnHelper.field_0x52 = 0;
        dPadNav::setNavEnabled(false, false);
    }

    return true;
}

bool dLytMsgWindowSelectBtn_c::execute() {
    if (*mStateMgr.getStateID() != StateID_Wait) {
        // TODO
    }

    mStateMgr.executeState();
    for (int i = 0; i < SELECT_BTN_NUM_BTNS; i++) {
        mParts[i].execute();
    }

    if (mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].isEnabled()) {
        mAnm[SELECT_BTN_ANIM_ITEM_ARROW_0].play();
    }

    if (mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].isEnabled()) {
        mAnm[SELECT_BTN_ANIM_ITEM_ARROW_1].play();
    }

    if (mAnm[SELECT_BTN_ANIM_LOOP_REMOCON].isEnabled()) {
        mAnm[SELECT_BTN_ANIM_LOOP_REMOCON].play();
    }

    mLyt.calc();

    return true;
}

bool dLytMsgWindowSelectBtn_c::draw() {
    if (field_0x9A4 != 0) {
        mLyt.addToDrawList();
    }
    return true;
}
