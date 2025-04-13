#include "d/lyt/meter/d_lyt_meter_item_select.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/d_pad.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_do_button.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/math/math_types.h"
#include "s/s_Math.h"
#include "sized_string.h"
#include "toBeSorted/effects_struct.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/minigame_mgr.h"
#include "toBeSorted/small_sound_mgr.h"

STATE_DEFINE(dLytMeterItemSelectIcon_c, Wait);
STATE_DEFINE(dLytMeterItemSelectIcon_c, On);
STATE_DEFINE(dLytMeterItemSelectIcon_c, Select);
STATE_DEFINE(dLytMeterItemSelectIcon_c, Off);

STATE_DEFINE(dLytMeterItemSelectBlink_c, Wait);
STATE_DEFINE(dLytMeterItemSelectBlink_c, On);
STATE_DEFINE(dLytMeterItemSelectBlink_c, Select);
STATE_DEFINE(dLytMeterItemSelectBlink_c, Off);

STATE_DEFINE(dLytMeterItemSelectText_c, Wait);
STATE_DEFINE(dLytMeterItemSelectText_c, On);
STATE_DEFINE(dLytMeterItemSelectText_c, Select);
STATE_DEFINE(dLytMeterItemSelectText_c, Off);

STATE_DEFINE(dLytMeterItemSelectBtn_c, Invisible);
STATE_DEFINE(dLytMeterItemSelectBtn_c, On);
STATE_DEFINE(dLytMeterItemSelectBtn_c, Visible);
STATE_DEFINE(dLytMeterItemSelectBtn_c, Off);

STATE_DEFINE(dLytMeterItemSelect_c, InitWait);
STATE_DEFINE(dLytMeterItemSelect_c, Wait);
STATE_DEFINE(dLytMeterItemSelect_c, SelectIn);
STATE_DEFINE(dLytMeterItemSelect_c, Select);
STATE_DEFINE(dLytMeterItemSelect_c, SetIn);
STATE_DEFINE(dLytMeterItemSelect_c, SetSpecialItemIn);
STATE_DEFINE(dLytMeterItemSelect_c, Set);
STATE_DEFINE(dLytMeterItemSelect_c, SetOut);
STATE_DEFINE(dLytMeterItemSelect_c, SetNone);
STATE_DEFINE(dLytMeterItemSelect_c, SelectOut);
STATE_DEFINE(dLytMeterItemSelect_c, ResetIn);
STATE_DEFINE(dLytMeterItemSelect_c, Reset);
STATE_DEFINE(dLytMeterItemSelect_c, ResetOut);
STATE_DEFINE(dLytMeterItemSelect_c, ToUse);
STATE_DEFINE(dLytMeterItemSelect_c, ToUnuse);
STATE_DEFINE(dLytMeterItemSelect_c, Unuse);
STATE_DEFINE(dLytMeterItemSelect_c, DemoMove);
STATE_DEFINE(dLytMeterItemSelect_c, DemoOut0);
STATE_DEFINE(dLytMeterItemSelect_c, DemoOut1);
STATE_DEFINE(dLytMeterItemSelect_c, FrameOffIn);
STATE_DEFINE(dLytMeterItemSelect_c, FrameOffMove);
STATE_DEFINE(dLytMeterItemSelect_c, FrameOffOut);

void dLytMeterItemSelectIcon_c::initializeState_Wait() {}
void dLytMeterItemSelectIcon_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterItemSelectIcon_c::finalizeState_Wait() {}

void dLytMeterItemSelectIcon_c::initializeState_On() {}
void dLytMeterItemSelectIcon_c::executeState_On() {
    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterItemSelectIcon_c::finalizeState_On() {}

void dLytMeterItemSelectIcon_c::initializeState_Select() {}
void dLytMeterItemSelectIcon_c::executeState_Select() {
    mpAnm->setBackwardsOnce();
    mpAnm->setToStart();
    mpAnm->setAnimEnable(true);
    if (mShouldBeSelect == 0) {
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterItemSelectIcon_c::finalizeState_Select() {}

void dLytMeterItemSelectIcon_c::initializeState_Off() {}
void dLytMeterItemSelectIcon_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterItemSelectIcon_c::finalizeState_Off() {}

void dLytMeterItemSelectIcon_c::init() {
    mShouldBeSelect = 0;
    mpAnm->setFrame(0.0f);
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterItemSelectIcon_c::execute() {
    mStateMgr.executeState();
}

void dLytMeterItemSelectBlink_c::initializeState_Wait() {}
void dLytMeterItemSelectBlink_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterItemSelectBlink_c::finalizeState_Wait() {}

void dLytMeterItemSelectBlink_c::initializeState_On() {}
void dLytMeterItemSelectBlink_c::executeState_On() {
    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterItemSelectBlink_c::finalizeState_On() {}

void dLytMeterItemSelectBlink_c::initializeState_Select() {}
void dLytMeterItemSelectBlink_c::executeState_Select() {
    if (mShouldBeSelect == 0) {
        mpAnm->setBackwardsOnce();
        mpAnm->setToStart();
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterItemSelectBlink_c::finalizeState_Select() {}

void dLytMeterItemSelectBlink_c::initializeState_Off() {}
void dLytMeterItemSelectBlink_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterItemSelectBlink_c::finalizeState_Off() {}

void dLytMeterItemSelectBlink_c::init() {
    mShouldBeSelect = 0;
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterItemSelectBlink_c::execute() {
    mStateMgr.executeState();
}

void dLytMeterItemSelectText_c::initializeState_Wait() {}
void dLytMeterItemSelectText_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterItemSelectText_c::finalizeState_Wait() {}

void dLytMeterItemSelectText_c::initializeState_On() {}
void dLytMeterItemSelectText_c::executeState_On() {
    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterItemSelectText_c::finalizeState_On() {}

void dLytMeterItemSelectText_c::initializeState_Select() {}
void dLytMeterItemSelectText_c::executeState_Select() {
    if (mShouldBeSelect == 0) {
        mpAnm->setBackwardsOnce();
        mpAnm->setToStart();
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterItemSelectText_c::finalizeState_Select() {}

void dLytMeterItemSelectText_c::initializeState_Off() {}
void dLytMeterItemSelectText_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterItemSelectText_c::finalizeState_Off() {}

void dLytMeterItemSelectText_c::init() {
    mShouldBeSelect = 0;
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterItemSelectText_c::execute() {
    mStateMgr.executeState();
}

void dLytMeterItemSelectBtn_c::initializeState_Invisible() {
    mDelay = 15;
}
void dLytMeterItemSelectBtn_c::executeState_Invisible() {
    if (mpAnm->isEnabled()) {
        mpAnm->setAnimEnable(false);
    }

    if (mShouldBeVisible) {
        if (mDelay <= 0) {
            mStateMgr.changeState(StateID_On);
        } else {
            mDelay--;
        }
    } else {
        mDelay = 15;
    }
}
void dLytMeterItemSelectBtn_c::finalizeState_Invisible() {}

void dLytMeterItemSelectBtn_c::initializeState_On() {
    mpAnm->setBackwardsOnce();

    if (mInstant) {
        mpAnm->setToEnd2();
        mInstant = false;
    }

    mpAnm->setAnimEnable(true);
}
void dLytMeterItemSelectBtn_c::executeState_On() {
    if (!mShouldBeVisible) {
        mStateMgr.changeState(StateID_Off);
    } else {
        if (mpAnm->isStop2()) {
            mpAnm->setAnimEnable(false);
            mStateMgr.changeState(StateID_Visible);
        } else if (mInstant) {
            mpAnm->setToEnd2();
            mInstant = false;
            mStateMgr.changeState(StateID_Visible);
        }
        if (mpAnm->isEnabled()) {
            mpAnm->play();
        }
    }
}
void dLytMeterItemSelectBtn_c::finalizeState_On() {}

void dLytMeterItemSelectBtn_c::initializeState_Visible() {}
void dLytMeterItemSelectBtn_c::executeState_Visible() {
    if (mpAnm->isEnabled()) {
        mpAnm->setAnimEnable(false);
    }
    if (!mShouldBeVisible) {
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterItemSelectBtn_c::finalizeState_Visible() {}

void dLytMeterItemSelectBtn_c::initializeState_Off() {
    mpAnm->setForwardOnce();

    if (mInstant) {
        mpAnm->setToEnd2();
        mInstant = false;
    }

    mpAnm->setAnimEnable(true);
}
void dLytMeterItemSelectBtn_c::executeState_Off() {
    if (mShouldBeVisible) {
        mStateMgr.changeState(StateID_On);
    } else {
        if (mpAnm->isStop2()) {
            mpAnm->setAnimEnable(false);
            mStateMgr.changeState(StateID_Invisible);
        } else if (mInstant) {
            mpAnm->setToEnd2();
            mInstant = false;
            mStateMgr.changeState(StateID_Invisible);
        }
        if (mpAnm->isEnabled()) {
            mpAnm->play();
        }
    }
}
void dLytMeterItemSelectBtn_c::finalizeState_Off() {}

void dLytMeterItemSelectBtn_c::init(s32 i, d2d::dLytSub *lyt, nw4r::lyt::Pane *pane) {
    mpLyt = lyt;
    mIdx = i;
    mpPane1 = pane;
    mpPane2 = nullptr;
    mDelay = 15;
    mShouldBeVisible = 0;
    mInstant = 0;
    field_0x57 = 0;
    mStateMgr.changeState(StateID_Invisible);
}

void dLytMeterItemSelectBtn_c::execute() {
    if (mpPane2 != nullptr && mpPane2->IsVisible() && mpPane1->IsVisible()) {
        mStateMgr.executeState();
    }
}

void dLytMeterItemSelectBtn_c::setVisible(bool bVisible, bool bInstant) {
    mShouldBeVisible = bVisible;
    mInstant = bInstant;
    field_0x57 = 1;
    if (bInstant) {
        mDelay = 0;
    }
}

void dLytMeterItemSelectBtn_c::setVisible(bool bVisible) {
    if (mpAnm->isEnabled()) {
        setVisible(bVisible, true);
    } else {
        mpAnm->setAnimEnable(true);
        mShouldBeVisible = bVisible;
        if (bVisible) {
            mpAnm->setFrame(0.0f);
            mStateMgr.changeState(StateID_Visible);
        } else {
            mpAnm->setToEnd();
            mStateMgr.changeState(StateID_Invisible);
        }
        mpLyt->getLayout()->Animate(0);
        mpLyt->calc();
        mpAnm->setAnimEnable(false);
    }
}

bool dLytMeterItemSelectBtn_c::isSettled() const {
    return !mpAnm->isEnabled();
}

void dLytMeterItemSelectBtn_c::setVisible2(bool bVisible) {
    field_0x57 = 1;
    if (bVisible) {
        mDelay = 15;
        mShouldBeVisible = true;
        mInstant = false;
        mpAnm->setToEnd();
        mStateMgr.changeState(StateID_Visible);
    } else {
        mDelay = 15;
        mShouldBeVisible = false;
        mInstant = false;
        mpAnm->setFrame(0.0f);
        mStateMgr.changeState(StateID_Invisible);
    }
}

static const char *sPaneNames[] = {
    "N_itemArrow_00", "N_arrowHand_00", "N_bBtn_00",  "N_itemSelect_00", "N_setBtn_00", "N_bBtn_01",
    "N_btnBg_00",     "N_btn_00",       "N_input_00", "N_iconA_00",      "N_iconB_00",  "N_iconC_00",
    "N_iconD_00",     "N_iconE_00",     "N_iconF_00", "N_iconG_00",      "N_iconH_00",
};

#define ITEM_SELECT_PANE_ITEM_ARROW 0
#define ITEM_SELECT_PANE_ARROW_HAND 1
#define ITEM_SELECT_PANE_N_BTN_0 2
#define ITEM_SELECT_PANE_ITEM_SELECT 3
#define ITEM_SELECT_PANE_SET_BTN 4
#define ITEM_SELECT_PANE_B_BTN_1 5
#define ITEM_SELECT_PANE_BTN_BG 6
#define ITEM_SELECT_PANE_BTN 7
#define ITEM_SELECT_PANE_INPUT 8
#define ITEM_SELECT_PANE_ICON_OFFSET 9

#define ITEM_SELECT_NUM_PANES 17

static const int sDemoMoveIntsUnk[] = {4, 2, 3, 1, 7, 6, 5, 0};

static const d2d::LytBrlanMapping brlanMap[] = {
    {         "remoConBtn_00_in.brlan",       "G_inOut_00"},
    {      "remoConBtn_00_onOff.brlan",       "G_itemA_00"},
    {      "remoConBtn_00_onOff.brlan",       "G_itemB_00"},
    {      "remoConBtn_00_onOff.brlan",       "G_itemC_00"},
    {      "remoConBtn_00_onOff.brlan",       "G_itemD_00"},
    {      "remoConBtn_00_onOff.brlan",       "G_itemE_00"},
    {      "remoConBtn_00_onOff.brlan",       "G_itemF_00"},
    {      "remoConBtn_00_onOff.brlan",       "G_itemG_00"},
    {      "remoConBtn_00_onOff.brlan",       "G_itemH_00"},
    {      "remoConBtn_00_onOff.brlan",        "G_text_00"},
    {      "remoConBtn_00_onOff.brlan",        "G_text_01"},
    {       "remoConBtn_00_have.brlan",       "G_itemA_00"},
    {       "remoConBtn_00_have.brlan",       "G_itemB_00"},
    {       "remoConBtn_00_have.brlan",       "G_itemC_00"},
    {       "remoConBtn_00_have.brlan",       "G_itemD_00"},
    {       "remoConBtn_00_have.brlan",       "G_itemE_00"},
    {       "remoConBtn_00_have.brlan",       "G_itemF_00"},
    {       "remoConBtn_00_have.brlan",       "G_itemG_00"},
    {       "remoConBtn_00_have.brlan",       "G_itemH_00"},
    {"remoConBtn_00_selectOnOff.brlan", "G_selectOnOff_00"},
    { "remoConBtn_00_selectLoop.brlan",  "G_selectLoop_00"},
    {  "remoConBtn_00_itemArrow.brlan",   "G_itemArrow_00"},
    {     "remoConBtn_00_decide.brlan",      "G_decide_00"},
    {    "remoConBtn_00_numberV.brlan",     "G_numberV_00"},
    {        "remoConBtn_00_out.brlan",       "G_inOut_00"},
    {      "remoConBtn_00_reset.brlan",       "G_inOut_00"},
    {  "remoConBtn_00_textColor.brlan",   "G_textColor_00"},
    {     "remoConBtn_00_enough.brlan",      "G_enough_00"},
    {       "remoConBtn_00_full.brlan",        "G_full_00"},
    {       "remoConBtn_00_call.brlan",        "G_call_00"},
    {      "remoConBtn_00_input.brlan",       "G_input_00"},
    {      "remoConBtn_00_input.brlan",       "G_input_01"},
    {      "remoConBtn_00_input.brlan",       "G_input_02"},
    {      "remoConBtn_00_input.brlan",       "G_input_03"},
};

#define ITEM_SELECT_ANIM_IN 0
#define ITEM_SELECT_ANIM_ITEM_ONOFF_OFFSET 1
#define ITEM_SELECT_ANIM_TEXT_ONOFF_OFFSET 9
#define ITEM_SELECT_ANIM_HAVE_OFFSET 11
#define ITEM_SELECT_ANIM_SELECT_ONOFF 19
#define ITEM_SELECT_ANIM_SELECT_LOOP 20
#define ITEM_SELECT_ANIM_ARROW 21
#define ITEM_SELECT_ANIM_DECIDE 22
#define ITEM_SELECT_ANIM_NUMBERV 23
#define ITEM_SELECT_ANIM_OUT 24
#define ITEM_SELECT_ANIM_RESET 25
#define ITEM_SELECT_ANIM_TEXTCOLOR 26
#define ITEM_SELECT_ANIM_ENOUGH 27
#define ITEM_SELECT_ANIM_FULL 28
#define ITEM_SELECT_ANIM_CALL 29
#define ITEM_SELECT_ANIM_INPUT_OFFSET 30

#define ITEM_SELECT_NUM_ANIMS 34

#define ITEM_SELECT_NUM_ITEMS 8

static const char *sTextboxNames[] = {
    "T_number_00", "T_numberS_00",  "T_numberS_01",   "T_numberS_02", "T_back_00",
    "T_backS_00",  "T_itemName_00", "T_itemNameS_00", "T_item_00",    "T_itemS_00",
};

static const char *sWindowNames[] = {
    "W_bgP_00",
    "W_bgP_01",
    "W_bgP_02",
    "W_bgP_04",
};

static const char *sGroupName = "G_ref_00";

dLytMeterItemSelect_c::dLytMeterItemSelect_c() : mStateMgr(*this, sStateID::null) {
    field_0x574C = I_INVALID;
    field_0x5750 = I_INVALID;
    field_0x5754 = 0;
    field_0x5758 = -1;
    field_0x575C = 0;
    mDemoMoveTimer = 0;
    field_0x5764 = -1;
    field_0x5768 = 2;

    field_0x57B5 = 0;
    field_0x57B6 = 0;
    field_0x57B7 = 0;
    field_0x57B8 = 0;

    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        field_0x57A3[i] = false;
        mIsBocoburinLocked[i] = false;
    }
}

void dLytMeterItemSelect_c::initializeState_InitWait() {}
void dLytMeterItemSelect_c::executeState_InitWait() {
    if (field_0x57B5 == 0) {
        return;
    }
    if (field_0x5794 == I_SAILCLOTH || field_0x5794 == I_BOAT_CANNON || field_0x5794 == I_HARP) {
        field_0x579A = field_0x5794;
    }

    bool blocked = isWheelBlockedByCurrentAction();
    if ((field_0x574C != I_INVALID && field_0x5794 != I_INVALID && !blocked) || field_0x579A != I_INVALID) {
        realizeSelectedWheelItem();
        fn_800EF8C0(true);
        if (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10)) {
            for (int i = 0; i < 4; i++) {
                if (i == 2) {
                    mBtn[i].setVisible2(false);
                } else {
                    mBtn[i].setVisible2(true);
                }
            }

            mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(true);

            for (int i = 0; i < 4; i++) {
                mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setAnimEnable(true);
                if (i == 2) {
                    mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setToEnd();
                } else {
                    mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setFrame(0.0f);
                }
            }

            mLyt.calc();

            for (int i = 0; i < 4; i++) {
                mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setAnimEnable(false);
            }

            mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(false);

            mAnm[ITEM_SELECT_ANIM_OUT].setToEnd();
            mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
            mLyt.calc();
            mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
            mAnm[ITEM_SELECT_ANIM_SELECT_LOOP].setAnimEnable(true);
            mStateMgr.changeState(StateID_Wait);
        } else {
            for (int i = 0; i < 4; i++) {
                mBtn[i].setVisible2(false);
            }

            mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(true);

            for (int i = 0; i < 4; i++) {
                mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setAnimEnable(true);
                mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setToEnd();
            }

            mLyt.calc();

            for (int i = 0; i < 4; i++) {
                mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setAnimEnable(false);
            }

            mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(false);

            mAnm[ITEM_SELECT_ANIM_OUT].setToEnd();
            mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
            mLyt.calc();
            mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
            mBtn[1].setVisible(false);
            mAnm[ITEM_SELECT_ANIM_SELECT_LOOP].setAnimEnable(true);
            mStateMgr.changeState(StateID_Unuse);
        }
    } else {
        if (field_0x5754 != 1) {
            mAnm[ITEM_SELECT_ANIM_IN].setFrame(0.0f);
            mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
            mLyt.calc();
            mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
            mpPanes[ITEM_SELECT_PANE_ITEM_SELECT]->SetVisible(false);
        }
        mAnm[ITEM_SELECT_ANIM_SELECT_LOOP].setAnimEnable(true);
        if (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10)) {
            for (int i = 0; i < 4; i++) {
                if (i == 2 && blocked) {
                    mBtn[i].setVisible2(false);
                } else {
                    mBtn[i].setVisible2(true);
                }
            }

            mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(true);

            for (int i = 0; i < 4; i++) {
                mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setAnimEnable(true);
                if (i == 2 && blocked) {
                    mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setToEnd();
                } else {
                    mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setFrame(0.0f);
                }
            }

            mLyt.calc();

            for (int i = 0; i < 4; i++) {
                mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setAnimEnable(false);
            }

            mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(false);

            if (blocked) {
                field_0x5768 = 0;
                mStateMgr.changeState(StateID_FrameOffIn);
            } else {
                mStateMgr.changeState(StateID_Wait);
            }
        } else {
            for (int i = 0; i < 4; i++) {
                if (i != 2 || blocked) {
                    mBtn[i].setVisible2(false);
                } else {
                    mBtn[i].setVisible2(true);
                }
            }

            mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(true);

            for (int i = 0; i < 4; i++) {
                mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setAnimEnable(true);
                if (i != 2 || blocked) {
                    mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setToEnd();
                } else {
                    mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setFrame(0.0f);
                }
            }

            mLyt.calc();

            for (int i = 0; i < 4; i++) {
                mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET].setAnimEnable(false);
            }

            mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(false);
            mStateMgr.changeState(StateID_Unuse);
        }
    }
}
void dLytMeterItemSelect_c::finalizeState_InitWait() {}

void dLytMeterItemSelect_c::initializeState_Wait() {
    if (mpPanes[ITEM_SELECT_PANE_ICON_OFFSET + 2]->GetAlpha() == 64) {
        mpPanes[ITEM_SELECT_PANE_ICON_OFFSET + 2]->SetAlpha(255);
    }
}
void dLytMeterItemSelect_c::executeState_Wait() {
    if (field_0x57B3 == 1 && (field_0x5794 == I_INVALID || LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) ==
                                                               LytDoButtonRelated::DO_STOP)) {
        mStateMgr.changeState(StateID_SetNone);
    } else if (field_0x57B3 == 0 && field_0x5794 == I_INVALID &&
               (field_0x579A == I_SAILCLOTH || field_0x579A == I_BOAT_CANNON || field_0x579A == I_HARP)) {
        mStateMgr.changeState(StateID_SetNone);
    } else if (field_0x5754 == 1) {
        mStateMgr.changeState(StateID_DemoMove);
    } else if (field_0x57A0 != 0) {
        field_0x5780 = mAnm[ITEM_SELECT_ANIM_DECIDE].getAnimDuration();
        mStateMgr.changeState(StateID_SetIn);
    } else if (field_0x57A2 != 0) {
        mStateMgr.changeState(StateID_SelectIn);
    } else if (isWheelBlockedByCurrentAction()) {
        field_0x5768 = 0;
        mStateMgr.changeState(StateID_FrameOffIn);
    } else if ((field_0x5794 == I_SAILCLOTH || (field_0x5794 == I_BOAT_CANNON || field_0x5794 == I_HARP)) &&
               field_0x579A == I_INVALID) {
        mStateMgr.changeState(StateID_SetSpecialItemIn);
    } else if (!dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10)) {
        mStateMgr.changeState(StateID_ToUnuse);
    } else if (!dLytMeterContainer_c::GetMeter()->fn_800D5650() && !dLytMeterContainer_c::GetMeter()->fn_800D5680() &&
               LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) != LytDoButtonRelated::DO_ITEMS &&
               LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) != LytDoButtonRelated::DO_RETURN &&
               LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) != LytDoButtonRelated::DO_DONE &&
               LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) != LytDoButtonRelated::DO_STOP &&
               (field_0x5794 != I_BOAT_CANNON && field_0x5794 != I_HARP && field_0x5794 != I_INVALID &&
                field_0x5794 != I_SAILCLOTH)) {
        mStateMgr.changeState(StateID_ResetIn);
    }
}
void dLytMeterItemSelect_c::finalizeState_Wait() {}

void dLytMeterItemSelect_c::initializeState_SelectIn() {
    if (dAcPy_c::GetLink()->checkFlags0x340(0x400)) {
        field_0x57BD = 1;
        field_0x5788 = 0;
        field_0x579B = 0;
        mEffects.fn_80027320(0);
        mpPanes[ITEM_SELECT_PANE_ICON_OFFSET + 2]->SetInfluencedAlpha(true);
        mpPanes[ITEM_SELECT_PANE_ICON_OFFSET + 2]->SetAlpha(64);
    }

    if (mAnm[ITEM_SELECT_ANIM_FULL].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_FULL].setToEnd();
        mLyt.calc();
        mAnm[ITEM_SELECT_ANIM_FULL].setAnimEnable(false);
    }

    mAnm[ITEM_SELECT_ANIM_DECIDE].setAnimEnable(true);
    mAnm[ITEM_SELECT_ANIM_DECIDE].setFrame(0.0f);

    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        if (fn_800F01B0(i)) {
            mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setFrame(0.0f);
            if (fn_800F01E0(i)) {
                mIsBocoburinLocked[i] = true;
            }
        } else {
            mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setFrame(1.0f);
        }
        mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setAnimEnable(true);
    }

    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        mAnm[i + ITEM_SELECT_ANIM_ITEM_ONOFF_OFFSET].setForwardOnce();
        mAnm[i + ITEM_SELECT_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(true);
        mAnm[i + ITEM_SELECT_ANIM_ITEM_ONOFF_OFFSET].setFrame(0.0f);
        mIcon[i].setShouldBeSelect(0);
    }

    mLyt.calc();

    mAnm[ITEM_SELECT_ANIM_DECIDE].setAnimEnable(false);

    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setAnimEnable(false);
    }

    mpPanes[ITEM_SELECT_PANE_ITEM_SELECT]->SetVisible(true);

    mAnm[ITEM_SELECT_ANIM_IN].setForwardOnce();
    mAnm[ITEM_SELECT_ANIM_IN].setFrame(0.0f);
    mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
    realizeWheelItems();
}
void dLytMeterItemSelect_c::executeState_SelectIn() {
    bool bAllAnimsSettled = true;
    if (field_0x57BD != 0) {
        if (field_0x5788 > 0) {
            field_0x5788--;
            bAllAnimsSettled = false;
        } else {
            field_0x57BE = 1;
            bool settled0x57BE = true;
            bool settled0x5784 = true;
            if (field_0x579B != 0xFF) {
                settled0x57BE = sLib::chaseUC(&field_0x579B, 0xFF, 0x55);
            }
            if (field_0x5784 != 1.0f) {
                settled0x5784 = sLib::chase(&field_0x5784, 1.0f, 0.06666666f);
            }
            mEffects.fn_80027320(field_0x579B);
            if (!settled0x57BE || !settled0x5784) {
                bAllAnimsSettled = false;
            }
        }
    }

    if (bAllAnimsSettled) {
        if (mAnm[ITEM_SELECT_ANIM_IN].isEndReached()) {
            mStateMgr.changeState(StateID_Select);
        }
    }

    if (mAnm[ITEM_SELECT_ANIM_IN].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_IN].play();
    }
}
void dLytMeterItemSelect_c::finalizeState_SelectIn() {
    mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
}

void dLytMeterItemSelect_c::initializeState_Select() {}
void dLytMeterItemSelect_c::executeState_Select() {
    if (dLytMeterContainer_c::getCrossBtn0x7BF8() == 0 && dPad::checkButtonDpadDownPressed()) {
        if (field_0x5794 != I_INVALID) {
            field_0x5780 = 0.0f;
            mStateMgr.changeState(StateID_SetIn);
        } else {
            mStateMgr.changeState(StateID_SelectOut);
        }
    } else if (field_0x57A2 == 0) {
        if (field_0x57A0 != 0 || field_0x5794 != I_INVALID) {
            field_0x5780 = 0.0f;
            mStateMgr.changeState(StateID_SetIn);
        } else {
            mStateMgr.changeState(StateID_SelectOut);
        }
    }
}
void dLytMeterItemSelect_c::finalizeState_Select() {}

void dLytMeterItemSelect_c::initializeState_SetIn() {
    if (mAnm[ITEM_SELECT_ANIM_FULL].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_FULL].setToEnd();
        mLyt.calc();
        mAnm[ITEM_SELECT_ANIM_FULL].setAnimEnable(false);
    }

    if (field_0x5780 == mAnm[ITEM_SELECT_ANIM_DECIDE].getAnimDuration()) {
        mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
        mAnm[ITEM_SELECT_ANIM_IN].setFrame(mAnm[ITEM_SELECT_ANIM_IN].getAnimDuration());
    }

    mAnm[ITEM_SELECT_ANIM_DECIDE].setAnimEnable(true);
    mAnm[ITEM_SELECT_ANIM_DECIDE].setFrame(field_0x5780);
    mBtn[0].setVisible(true, false);
    mBtn[2].setVisible(false, true);
    mLyt.calc();
    field_0x57B3 = 1;
    field_0x578C = 3;
    if (field_0x57BD != 0) {
        field_0x5788 = 0;
    }
}
void dLytMeterItemSelect_c::executeState_SetIn() {
    bool bAllAnimsSettled = true;
    if (field_0x57BD != 0) {
        if (field_0x5788 > 0) {
            field_0x5788--;
            bAllAnimsSettled = false;
        } else {
            bool settled0x57BE = true;
            bool settled0x5784 = true;
            if (field_0x579B != 0) {
                settled0x57BE = sLib::chaseUC(&field_0x579B, 0, 0x55);
            }
            if (field_0x5784 != 0.8f) {
                settled0x5784 = sLib::chase(&field_0x5784, 0.8f, 0.06666666f);
            }
            mEffects.fn_80027320(field_0x579B);
            if (settled0x57BE && settled0x5784) {
                field_0x57BD = 0;
                field_0x57BE = 0;
                mpPanes[ITEM_SELECT_PANE_ICON_OFFSET + 2]->SetAlpha(0xFF);
            } else {
                bAllAnimsSettled = false;
            }
        }
    }

    if (mAnm[ITEM_SELECT_ANIM_IN].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
    }

    if (bAllAnimsSettled) {
        if (mAnm[ITEM_SELECT_ANIM_DECIDE].isEndReached()) {
            if (!mBtn[1].getShouldBeVisible()) {
                mBtn[1].setVisible(true, true);
            }
            mAnm[ITEM_SELECT_ANIM_DECIDE].setAnimEnable(false);
            mStateMgr.changeState(StateID_Set);
        }
    }

    if (mAnm[ITEM_SELECT_ANIM_DECIDE].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_DECIDE].play();
    }
}
void dLytMeterItemSelect_c::finalizeState_SetIn() {}

void dLytMeterItemSelect_c::initializeState_SetSpecialItemIn() {
    mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
    mAnm[ITEM_SELECT_ANIM_IN].setToEnd();

    mLyt.calc();

    mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);

    mBtn[0].setVisible(true);
    mBtn[1].setVisible(true);
    mBtn[3].setVisible(true);

    mAnm[ITEM_SELECT_ANIM_OUT].setForwardOnce();

    if (field_0x57B3 != 0) {
        mAnm[ITEM_SELECT_ANIM_OUT].setToStart();
    } else {
        mAnm[ITEM_SELECT_ANIM_OUT].setToEnd2();
        if (field_0x5798 == I_INVALID) {
            mAnm[ITEM_SELECT_ANIM_RESET].setBackwardsOnce();
            mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
            mBtn[2].setVisible(false);
            mBtn[0].setVisible(true);
            mBtn[1].setVisible(true);
        } else {
            mBtn[2].setVisible(false, false);
            mBtn[0].setVisible(true, false);
            mBtn[1].setVisible(true, false);
        }
    }
    mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
    field_0x579A = field_0x5794;
}
void dLytMeterItemSelect_c::executeState_SetSpecialItemIn() {
    if (field_0x57B4 != 0 && !mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_RESET].setForwardOnce();
        mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
    }

    if (mAnm[ITEM_SELECT_ANIM_OUT].isStop2() && mBtn[0].isSettled() && mBtn[1].isSettled() && mBtn[2].isSettled()) {
        mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
        if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
            if (mAnm[ITEM_SELECT_ANIM_RESET].isStop2()) {
                mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
                mStateMgr.changeState(StateID_Wait);
            }
        } else {
            mStateMgr.changeState(StateID_Wait);
        }
    }

    if (mAnm[ITEM_SELECT_ANIM_OUT].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_OUT].play();
    }

    if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_RESET].play();
    }
}
void dLytMeterItemSelect_c::finalizeState_SetSpecialItemIn() {}

void dLytMeterItemSelect_c::initializeState_Set() {
    if (field_0x5794 == I_HARP && field_0x5790 <= 0) {
        field_0x5790 = 10;
    }
    if (field_0x5794 == I_SAILCLOTH || field_0x5794 == I_BOAT_CANNON || field_0x5794 == I_HARP) {
        field_0x579A = field_0x5794;
    }
}
void dLytMeterItemSelect_c::executeState_Set() {
    if (field_0x57A0 == 0) {
        if (field_0x5794 == I_INVALID) {
            mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
            mAnm[ITEM_SELECT_ANIM_IN].setFrame(0.0f);
            mLyt.calc();
            mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
            mpPanes[ITEM_SELECT_PANE_ITEM_SELECT]->SetVisible(false);
            if (!dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10)) {
                mStateMgr.changeState(StateID_ToUnuse);
            } else {
                mStateMgr.changeState(StateID_SetNone);
            }
        } else {
            mAnm[ITEM_SELECT_ANIM_OUT].setForwardOnce();
            mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
            mAnm[ITEM_SELECT_ANIM_OUT].setToStart();
            mStateMgr.changeState(StateID_SetOut);
        }
    } else {
        if ((field_0x5794 == I_SAILCLOTH || field_0x5794 == I_BOAT_CANNON || field_0x5794 == I_HARP) &&
            field_0x579A != field_0x5794) {
            mStateMgr.changeState(StateID_SetSpecialItemIn);
        } else if (!dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) && field_0x5790 <= 0 &&
                   (field_0x5794 == I_BOAT_CANNON || field_0x5794 == I_HARP)) {
            mStateMgr.changeState(StateID_ToUnuse);
        } else if (field_0x57A2 != 0) {
            mStateMgr.changeState(StateID_SelectIn);
        }
    }
}
void dLytMeterItemSelect_c::finalizeState_Set() {}

void dLytMeterItemSelect_c::initializeState_SetOut() {
    field_0x57B3 = 0;
    field_0x5790 = 0;
}
void dLytMeterItemSelect_c::executeState_SetOut() {
    if (mAnm[ITEM_SELECT_ANIM_OUT].isStop2()) {
        mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
        field_0x57B3 = 0;
        field_0x579A = I_INVALID;
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnm[ITEM_SELECT_ANIM_OUT].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_OUT].play();
    }
}
void dLytMeterItemSelect_c::finalizeState_SetOut() {}

void dLytMeterItemSelect_c::initializeState_SetNone() {
    mAnm[ITEM_SELECT_ANIM_RESET].setForwardOnce();
    mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
    if (field_0x5795 == I_SAILCLOTH || field_0x5795 == I_BOAT_CANNON || field_0x5795 == I_HARP) {
        // "instant"
        mAnm[ITEM_SELECT_ANIM_RESET].setToEnd2();
        mBtn[0].setVisible(true, true);
        mBtn[2].setVisible(true, true);
    } else {
        mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
        mBtn[0].setVisible(true, false);
        mBtn[2].setVisible(true, false);
    }
}
void dLytMeterItemSelect_c::executeState_SetNone() {
    if (field_0x5754 == 1) {
        mAnm[ITEM_SELECT_ANIM_RESET].setBackwardsOnce();
        mAnm[ITEM_SELECT_ANIM_RESET].setToEnd2();
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
        mLyt.calc();
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
        fn_800F0310();
    } else {
        if (mAnm[ITEM_SELECT_ANIM_RESET].isStop2() && mBtn[0].isSettled()) {
            mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
            field_0x57B3 = 0;
            field_0x579A = I_INVALID;
            mStateMgr.changeState(StateID_Wait);
        }

        if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_RESET].play();
        }
    }
}
void dLytMeterItemSelect_c::finalizeState_SetNone() {}

void dLytMeterItemSelect_c::initializeState_SelectOut() {
    mAnm[ITEM_SELECT_ANIM_IN].setBackwardsOnce();
    mAnm[ITEM_SELECT_ANIM_IN].setToStart();
    mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
    field_0x57B3 = 0;
    if (field_0x5796 != I_INVALID) {
        mBtn[2].setVisible(false, false);
    } else {
        mBtn[2].setVisible(true, false);
    }
    if (field_0x57BD != 0) {
        field_0x5788 = 0;
    }
}
void dLytMeterItemSelect_c::executeState_SelectOut() {
    bool bAllAnimsSettled = true;
    if (field_0x57BD != 0) {
        if (field_0x5788 > 0) {
            field_0x5788--;
            bAllAnimsSettled = false;
        } else {
            bool settled0x57BE = true;
            bool settled0x5784 = true;
            if (field_0x579B != 0) {
                settled0x57BE = sLib::chaseUC(&field_0x579B, 0, 0x55);
            }
            if (field_0x5784 != 0.8f) {
                settled0x5784 = sLib::chase(&field_0x5784, 0.8f, 0.06666666f);
            }
            mEffects.fn_80027320(field_0x579B);
            if (settled0x57BE && settled0x5784) {
                field_0x57BD = 0;
                field_0x57BE = 0;
                mpPanes[ITEM_SELECT_PANE_ICON_OFFSET + 2]->SetAlpha(0xFF);
            } else {
                bAllAnimsSettled = false;
            }
        }
    }

    if (bAllAnimsSettled) {
        if (mAnm[ITEM_SELECT_ANIM_IN].isStop2() && mBtn[2].isSettled()) {
            mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
            mpPanes[ITEM_SELECT_PANE_ITEM_SELECT]->SetVisible(false);
            mStateMgr.changeState(StateID_Wait);
        }
    }

    if (mAnm[ITEM_SELECT_ANIM_IN].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_IN].play();
    }
}
void dLytMeterItemSelect_c::finalizeState_SelectOut() {}

void dLytMeterItemSelect_c::initializeState_ResetIn() {
    mAnm[ITEM_SELECT_ANIM_RESET].setForwardOnce();
    mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
    mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
    mBtn[2].setVisible(true);
    setBtnText(mLastDoButtonRelatedAction);
}
void dLytMeterItemSelect_c::executeState_ResetIn() {
    if (mAnm[ITEM_SELECT_ANIM_RESET].isStop2()) {
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
        field_0x57B3 = 0;
        mStateMgr.changeState(StateID_Reset);
    }

    if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_RESET].play();
    }
}
void dLytMeterItemSelect_c::finalizeState_ResetIn() {}

void dLytMeterItemSelect_c::initializeState_Reset() {}
void dLytMeterItemSelect_c::executeState_Reset() {
    if (field_0x57A2 != 0 || field_0x5754 == 1 ||
        (field_0x57A0 != 0 && (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) ||
                               (field_0x5794 != I_HARP && field_0x5794 != I_BOAT_CANNON)))) {
        mAnm[ITEM_SELECT_ANIM_RESET].setBackwardsOnce();
        mAnm[ITEM_SELECT_ANIM_RESET].setToEnd2();
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
        mLyt.calc();
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
        fn_800F0310();
    } else if (field_0x5794 == I_SAILCLOTH || field_0x5794 == I_BOAT_CANNON || field_0x5794 == I_HARP) {
        mStateMgr.changeState(StateID_SetSpecialItemIn);
    } else if (LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) == LytDoButtonRelated::DO_ITEMS ||
               LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) == LytDoButtonRelated::DO_RETURN ||
               LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) == LytDoButtonRelated::DO_DONE ||
               LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) == LytDoButtonRelated::DO_STOP ||
               field_0x5794 == I_INVALID || !dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) ||
               field_0x5754 == 1) {
        mStateMgr.changeState(StateID_ResetOut);
    }
}
void dLytMeterItemSelect_c::finalizeState_Reset() {}

void dLytMeterItemSelect_c::initializeState_ResetOut() {
    mAnm[ITEM_SELECT_ANIM_RESET].setBackwardsOnce();
    mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
    mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
    mBtn[2].setVisible(false);
}
void dLytMeterItemSelect_c::executeState_ResetOut() {
    if (field_0x57A2 != 0 || field_0x5754 == 1 ||
        (field_0x57A0 != 0 && (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) ||
                               (field_0x5794 != I_HARP && field_0x5794 != I_BOAT_CANNON)))) {
        mAnm[ITEM_SELECT_ANIM_RESET].setToEnd2();
        mBtn[2].setVisible(false, true);
        mLyt.calc();
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
        fn_800F0310();
    } else {
        if (mAnm[ITEM_SELECT_ANIM_RESET].isStop2() && mBtn[2].isSettled()) {
            mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
            mStateMgr.changeState(StateID_Wait);
        }

        if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_RESET].play();
        }
    }
}
void dLytMeterItemSelect_c::finalizeState_ResetOut() {}

void dLytMeterItemSelect_c::initializeState_ToUse() {
    if (field_0x5796 != I_INVALID) {
        field_0x57B4 = 0;
    }
    field_0x57C4 = 0;
    mBtn[1].setVisible(true, false);
    mBtn[0].setVisible(true, false);

    if (field_0x57B3 == 0) {
        if (field_0x5796 != I_INVALID) {
            mBtn[2].setVisible(false, false);
        } else {
            mBtn[2].setVisible(true, false);
        }

        if (field_0x57B4 != 0) {
            mAnm[ITEM_SELECT_ANIM_RESET].setForwardOnce();
            mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
            mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
        }
    } else if (field_0x5794 != I_INVALID && (field_0x5795 == I_BOAT_CANNON || field_0x5795 == I_HARP)) {
        field_0x57B3 = 0;
        field_0x57C4 = 1;
        mAnm[ITEM_SELECT_ANIM_OUT].setForwardOnce();
        mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
        mAnm[ITEM_SELECT_ANIM_OUT].setToStart();
    }
}
void dLytMeterItemSelect_c::executeState_ToUse() {
    if (field_0x57A2 != 0 || field_0x5754 == 1 ||
        (field_0x57A0 != 0 && (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) ||
                               (field_0x5794 != I_HARP && field_0x5794 != I_BOAT_CANNON)))) {
        if (field_0x57C4 != 0 && mAnm[ITEM_SELECT_ANIM_OUT].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_OUT].setToEnd2();
            mLyt.calc();
            mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
        }

        fn_800F0310();
    } else {
        bool b = false;
        if (field_0x57C4 != 0) {
            if (mAnm[ITEM_SELECT_ANIM_OUT].isEnabled()) {
                if (mAnm[ITEM_SELECT_ANIM_OUT].isStop2()) {
                    mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
                    b = true;
                }
            } else {
                b = true;
            }
        } else {
            b = true;
        }

        if (mBtn[1].isSettled() && b) {
            if (field_0x57B3 == 0) {
                if (mBtn[0].isSettled() && mBtn[2].isSettled()) {
                    if (field_0x57B4 != 0) {
                        if (mAnm[ITEM_SELECT_ANIM_RESET].isStop2()) {
                            mStateMgr.changeState(StateID_Wait);
                        }
                    } else {
                        mStateMgr.changeState(StateID_Wait);
                    }
                }
            } else {
                mStateMgr.changeState(StateID_Wait);
            }
        }

        if (field_0x57B3 == 0 && field_0x57B4 != 0 && mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_RESET].play();
        }

        if (field_0x57C4 != 0 && mAnm[ITEM_SELECT_ANIM_OUT].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_OUT].play();
        }
    }
}
void dLytMeterItemSelect_c::finalizeState_ToUse() {
    if (field_0x57B3 == 0 && field_0x57B4 != 0) {
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
        field_0x57B4 = 0;
    }
}

void dLytMeterItemSelect_c::initializeState_ToUnuse() {
    mBtn[1].setVisible(false, false);
    mBtn[0].setVisible(false, false);
    if (field_0x57B3 == 0) {
        if (field_0x5794 != I_INVALID) {
            mBtn[2].setVisible(false, false);
        } else {
            mBtn[2].setVisible(true, false);
        }
    }

    field_0x57A1 = field_0x57A0;
}
void dLytMeterItemSelect_c::executeState_ToUnuse() {
    if (field_0x57B3 == 0 && field_0x5794 == I_INVALID &&
        (field_0x579A == I_SAILCLOTH || field_0x579A == I_BOAT_CANNON || field_0x579A == I_HARP)) {
        mStateMgr.changeState(StateID_SetNone);
    } else if (field_0x57A2 != 0 || field_0x5754 == 1 ||
               (field_0x57A0 != 0 && (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) ||
                                      (field_0x5794 != I_HARP && field_0x5794 != I_BOAT_CANNON)))) {
        fn_800F0310();
        return;
    }

    if (field_0x57A0 == 0 && field_0x57A1 != 0) {
        field_0x57A1 = field_0x57A0;
        if (field_0x5794 == I_INVALID) {
            mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
            mAnm[ITEM_SELECT_ANIM_IN].setFrame(0.0f);
            mLyt.calc();
            mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
            mBtn[1].setVisible(false);
            mBtn[2].setVisible(true);
        } else {
            mAnm[ITEM_SELECT_ANIM_OUT].setForwardOnce();
            mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
            mAnm[ITEM_SELECT_ANIM_OUT].setToEnd2();
            mLyt.calc();
            mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
            mBtn[2].setVisible(false);
            field_0x57B3 = 0;
        }
    }

    if (mBtn[1].isSettled() && mBtn[0].isSettled()) {
        mBtn[1].setVisible(false);
        mBtn[0].setVisible(false);
        if (field_0x57B3 == 0) {
            if (mBtn[2].isSettled()) {
                if (field_0x5794 != I_INVALID) {
                    mBtn[2].setVisible(false);
                } else {
                    mBtn[2].setVisible(true);
                }
                mStateMgr.changeState(StateID_Unuse);
            }
        } else {
            mStateMgr.changeState(StateID_Unuse);
        }
    }
}
void dLytMeterItemSelect_c::finalizeState_ToUnuse() {}

void dLytMeterItemSelect_c::initializeState_Unuse() {}
void dLytMeterItemSelect_c::executeState_Unuse() {
    if (isWheelBlockedByCurrentAction()) {
        field_0x5768 = 1;
        mBtn[2].setVisible(false);
        mStateMgr.changeState(StateID_FrameOffIn);
    } else if (field_0x57A2 != 0 || field_0x5754 == 1 ||
               (field_0x57A0 != 0 && (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) ||
                                      (field_0x5794 != I_HARP && field_0x5794 != I_BOAT_CANNON)))) {
        fn_800F0310();
    } else if (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) || field_0x5754 == 1) {
        if (field_0x5794 != I_INVALID) {
            if (field_0x5794 == I_SAILCLOTH || field_0x5794 == I_BOAT_CANNON || field_0x5794 == I_HARP) {
                mStateMgr.changeState(StateID_SetSpecialItemIn);
                return;
            }
        } else {
            field_0x57B4 = 0;
        }
        mStateMgr.changeState(StateID_ToUse);
    }
}
void dLytMeterItemSelect_c::finalizeState_Unuse() {}

void dLytMeterItemSelect_c::initializeState_DemoMove() {
    mDemoMoveTimer = 0;
    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        if (field_0x5758 < 8) {
            if (i == sDemoMoveIntsUnk[field_0x5758]) {
                field_0x57A3[i] = false;
                field_0x575C++;
            } else {
                field_0x57A3[i] = true;
            }
        } else if ((i == 0 || i == 2 || i == 3) && fn_800F01B0(i)) {
            field_0x57A3[i] = false;
            field_0x575C++;
        } else {
            field_0x57A3[i] = true;
        }
        mIsBocoburinLocked[i] = false;
    }

    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        if (fn_800F01B0(i)) {
            if (fn_800F01E0(i) || field_0x57A3[i] == 0) {
                if (isSlotBocoburinLocked(i)) {
                    mIsBocoburinLocked[i] = true;
                    mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setFrame(0.0f);
                } else {
                    mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setFrame(1.0f);
                }
            } else {
                mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setFrame(0.0f);
            }
        } else {
            mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setFrame(1.0f);
        }

        mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setAnimEnable(true);
    }

    realizeWheelItems();
    mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(true);
    mpPanes[ITEM_SELECT_PANE_B_BTN_1]->SetVisible(true);
    mpPanes[ITEM_SELECT_PANE_ITEM_SELECT]->SetVisible(true);
    field_0x57C5 = 1;
    mpPanes[ITEM_SELECT_PANE_BTN_BG]->SetVisible(false);
    mpPanes[ITEM_SELECT_PANE_BTN]->SetVisible(false);
    mpPanes[ITEM_SELECT_PANE_INPUT]->SetVisible(false);

    mAnm[ITEM_SELECT_ANIM_IN].setForwardOnce();
    mAnm[ITEM_SELECT_ANIM_IN].setToEnd2();
    mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);

    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setAnimEnable(false);
    }

    SmallSoundManager::GetInstance()->playSound(SE_S_GET_ITEM_SET);
}
extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_740_;
void dLytMeterItemSelect_c::executeState_DemoMove() {
    mDemoMoveTimer++;
    if (mDemoMoveTimer == 15) {
        for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
            if (fn_800F01B0(i) && !field_0x57A3[i]) {
                mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setFrame(0.0f);
                field_0x57A3[i] = true;

                field_0x575C--;

                mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setAnimEnable(true);
                mLyt.calc();
                mAnm[i + ITEM_SELECT_ANIM_HAVE_OFFSET].setAnimEnable(false);

                nw4r::math::MTX34 mtx = mpPanes[i + ITEM_SELECT_PANE_ICON_OFFSET]->GetGlobalMtx();
                mVec3_c pos(mtx._03, mtx._13, 0.0f);
                EffectsStruct::fn_800298C0(PARTICLE_RESOURCE_ID_MAPPING_740_, &pos, nullptr, nullptr, nullptr, nullptr);

                if (mIsBocoburinLocked[i]) {
                    mIsBocoburinLocked[i] = false;
                    field_0x5799 = I_INVALID;
                } else {
                    field_0x5799 = getInternalBaseItemForSlot(i);
                    field_0x5794 = field_0x5799;
                }
            }
        }
        realizeWheelItems();
    }

    if (mDemoMoveTimer > 80) {
        mDemoMoveTimer = 0;
        if (field_0x575C <= 0) {
            mStateMgr.changeState(StateID_DemoOut0);
        }
    }
}
void dLytMeterItemSelect_c::finalizeState_DemoMove() {}

void dLytMeterItemSelect_c::initializeState_DemoOut0() {
    if (field_0x5799 != I_INVALID) {
        mAnm[ITEM_SELECT_ANIM_DECIDE].setAnimEnable(true);
        mAnm[ITEM_SELECT_ANIM_DECIDE].setFrame(0.0f);
    } else {
        mAnm[ITEM_SELECT_ANIM_IN].setBackwardsOnce();
        mAnm[ITEM_SELECT_ANIM_IN].setToStart();
        mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
    }
}
void dLytMeterItemSelect_c::executeState_DemoOut0() {
    if (field_0x5799 != I_INVALID) {
        if (mAnm[ITEM_SELECT_ANIM_DECIDE].isEndReached()) {
            if (!mBtn[1].getShouldBeVisible()) {
                mBtn[1].setVisible(true, true);
            }
            mAnm[ITEM_SELECT_ANIM_DECIDE].setAnimEnable(false);
            mStateMgr.changeState(StateID_DemoOut1);
        }

        if (mAnm[ITEM_SELECT_ANIM_DECIDE].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_DECIDE].play();
        }
    } else {
        if (mAnm[ITEM_SELECT_ANIM_IN].isStop2()) {
            mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
            field_0x5754 = 4;
            mStateMgr.changeState(StateID_Wait);
        }

        if (mAnm[ITEM_SELECT_ANIM_IN].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_IN].play();
        }
    }
}
void dLytMeterItemSelect_c::finalizeState_DemoOut0() {}

void dLytMeterItemSelect_c::initializeState_DemoOut1() {
    mAnm[ITEM_SELECT_ANIM_OUT].setForwardOnce();
    mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
    mAnm[ITEM_SELECT_ANIM_OUT].setFrame(0.0f);
}
void dLytMeterItemSelect_c::executeState_DemoOut1() {
    if (mAnm[ITEM_SELECT_ANIM_OUT].isEndReached()) {
        mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
        field_0x57B3 = 1;
        field_0x5754 = 4;
        field_0x5799 = I_INVALID;
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnm[ITEM_SELECT_ANIM_OUT].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_OUT].play();
    }
}
void dLytMeterItemSelect_c::finalizeState_DemoOut1() {}

void dLytMeterItemSelect_c::initializeState_FrameOffIn() {
    field_0x57C2 = 0;
    if ((field_0x57B3 == 0 || (field_0x57B3 == 1 && field_0x57B4 == 1)) && field_0x5768 == 1) {
        mBtn[0].setVisible(true, false);
        mBtn[1].setVisible(true, false);
    }

    if (field_0x5768 == 1) {
        mBtn[0].setVisible(true, false);
    }

    if (field_0x57B3 == 0) {
        if (field_0x5794 == I_INVALID) {
            mBtn[2].setVisible(false, false);
        } else if (field_0x57B4 == 0) {
            field_0x57C2 = 1;
            mBtn[2].setVisible(false, true);
        }
    }

    if (field_0x57B3 == 0 && field_0x5794 != I_INVALID) {
        mAnm[ITEM_SELECT_ANIM_RESET].setForwardOnce();
        mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
    }
}
void dLytMeterItemSelect_c::executeState_FrameOffIn() {
    bool isDone = true;
    if (!mBtn[0].isSettled()) {
        isDone = false;
    }
    if (!mBtn[1].isSettled()) {
        isDone = false;
    }
    if (!mBtn[2].isSettled()) {
        isDone = false;
    }
    if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
        if (mAnm[ITEM_SELECT_ANIM_RESET].isStop2()) {
            mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
        } else {
            isDone = false;
        }
    }

    if (isDone) {
        mStateMgr.changeState(StateID_FrameOffMove);
    }

    if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_RESET].play();
    }
}
void dLytMeterItemSelect_c::finalizeState_FrameOffIn() {}

void dLytMeterItemSelect_c::initializeState_FrameOffMove() {}
void dLytMeterItemSelect_c::executeState_FrameOffMove() {
    if (!dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10)) {
        mBtn[1].setVisible(false, false);
        mBtn[0].setVisible(false, false);
    } else {
        mBtn[1].setVisible(true, false);
        mBtn[0].setVisible(true, false);
    }

    if (!isWheelBlockedByCurrentAction()) {
        mStateMgr.changeState(StateID_FrameOffOut);
    }
}
void dLytMeterItemSelect_c::finalizeState_FrameOffMove() {}

void dLytMeterItemSelect_c::initializeState_FrameOffOut() {
    if (!mpPanes[ITEM_SELECT_PANE_SET_BTN]->IsVisible()) {
        mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(true);
    }

    if (field_0x57B3 == 0 && field_0x5794 != I_INVALID) {
        if (field_0x57A0 != 0) {
            field_0x57B3 = 1;
            mAnm[ITEM_SELECT_ANIM_DECIDE].setAnimEnable(true);
            mAnm[ITEM_SELECT_ANIM_DECIDE].setToEnd();
            mLyt.calc();
            mAnm[ITEM_SELECT_ANIM_DECIDE].setAnimEnable(false);
        } else {
            mAnm[ITEM_SELECT_ANIM_RESET].setBackwardsOnce();
            mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
            mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
        }

        if (!dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10)) {
            mBtn[0].setVisible(false);
            mBtn[1].setVisible(false);
            mBtn[2].setVisible(false);
        } else {
            mBtn[0].setVisible(true, false);
            mBtn[1].setVisible(true, false);
            mBtn[3].setVisible(true);
            mBtn[2].setVisible(false, false);
        }
    } else {
        mBtn[0].setVisible(true, false);
        mBtn[1].setVisible(true, false);
        mBtn[3].setVisible(true);
        if (field_0x5794 != I_INVALID) {
            mBtn[2].setVisible(false, false);
        } else {
            mBtn[2].setVisible(true, false);
        }
    }
}
void dLytMeterItemSelect_c::executeState_FrameOffOut() {
    bool isDone = true;
    if (!mBtn[0].isSettled()) {
        isDone = false;
    }
    if (!mBtn[1].isSettled()) {
        isDone = false;
    }
    if (!mBtn[2].isSettled()) {
        isDone = false;
    }

    if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
        if (mAnm[ITEM_SELECT_ANIM_RESET].isStop2()) {
            mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
        } else {
            isDone = false;
        }
    }

    if (isDone) {
        if (field_0x57C2 != 0) {
            field_0x57C2 = 0;
        }

        if (field_0x5768 == 1) {
            mBtn[1].setVisible(false);
            mStateMgr.changeState(StateID_Unuse);
        } else if (!dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10)) {
            mBtn[1].setVisible(false);
            mStateMgr.changeState(StateID_Unuse);
        } else {
            mBtn[0].setVisible(true);
            mBtn[1].setVisible(true);
            mBtn[3].setVisible(true);
            if (field_0x5794 != I_INVALID) {
                mBtn[2].setVisible(false);
            } else {
                mBtn[2].setVisible(true);
            }
            mStateMgr.changeState(StateID_Wait);
        }
    }

    if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_RESET].play();
    }
}
void dLytMeterItemSelect_c::finalizeState_FrameOffOut() {}

bool dLytMeterItemSelect_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_00.brlyt", nullptr);

    for (int i = 0; i < ITEM_SELECT_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sPaneNames[i]);
    }

    for (int i = 0; i < ITEM_SELECT_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        mIcon[i].setAnm(&mAnm[i + ITEM_SELECT_ANIM_ITEM_ONOFF_OFFSET]);
        mIcon[i].init();
    }

    mBlink.setAnm(&mAnm[ITEM_SELECT_ANIM_SELECT_ONOFF]);
    mBlink.init();

    field_0x578C = 0;
    field_0x5790 = 0;
    field_0x5784 = 0.8f;
    field_0x5788 = 0;
    field_0x579B = 0;
    field_0x57BD = 0;
    field_0x57BE = 0;
    field_0x57BF = 0;
    field_0x57C0 = 1;
    field_0x57C1 = 0;
    field_0x57C2 = 0;
    field_0x57C3 = 0;
    field_0x57C4 = 0;
    field_0x57C5 = 0;

    mEffects.init(dLytControlGame_c::getInstance());

    for (int i = 0; i < 2; i++) {
        mText[i].setAnm(&mAnm[i + ITEM_SELECT_ANIM_TEXT_ONOFF_OFFSET]);
        mText[i].init();
    }

    for (int i = 0; i < 4; i++) {
        mBtn[i].setAnm(&mAnm[i + ITEM_SELECT_ANIM_INPUT_OFFSET]);
        mBtn[i].init(i, &mLyt, mpPanes[5]);
    }

    for (int i = 0; i < 10; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(sTextboxNames[i]);
    }

    for (int i = 0; i < 4; i++) {
        mpWindows[i] = mLyt.getWindow(sWindowNames[i]);
        mpSizeBoxes[i] = mLyt.getSizeBoxInWindow(sWindowNames[i]);
        mpWindows[i]->UpdateSize(mpSizeBoxes[i], 32.0f);
    }

    for (int i = 0; i < 9; i++) {
        mSubpanes[i].mpLytPane = &mItemIcons[i];
        mItemIcons[i].build(resAcc, dLytCommonIconItem_c::B_WHEEL);
        mSubpaneList.PushBack(&mSubpanes[i]);
    }

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            linkMeters(g, &mSubpaneList);
        }
    }

    for (int i = 0; i < 9; i++) {
        mItemIcons[i].reset();
        mItemIcons[i].setVisible(false);
        mItemIcons[i].setSize(true);
    }

    field_0x5794 = I_INVALID;
    field_0x5795 = I_INVALID;
    field_0x5796 = I_INVALID;
    field_0x5797 = I_INVALID;
    field_0x5798 = I_INVALID;
    field_0x5799 = I_INVALID;
    field_0x579A = I_INVALID;

    mStoredBowVariant = baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_BOW));
    mStoredSlingshotVariant = baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_SLINGSHOT));
    mStoredBeetleVariant = baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_BEETLE));
    mStoredBugNetVariant = baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_BUG_NET));

    field_0x57B3 = 0;
    field_0x57B4 = 0;

    realizeWheelItems();
    setBtnItem(field_0x5796);

    mAnm[ITEM_SELECT_ANIM_NUMBERV].setAnimEnable(true);
    mAnm[ITEM_SELECT_ANIM_CALL].setToEnd();
    mAnm[ITEM_SELECT_ANIM_CALL].setAnimEnable(true);

    mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(true);

    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 0].setAnimEnable(true);
    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 0].setToEnd();

    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 1].setAnimEnable(true);
    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 1].setToEnd();

    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 2].setAnimEnable(true);
    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 2].setToEnd();

    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 3].setAnimEnable(true);
    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 3].setToEnd();

    mLyt.calc();

    mAnm[ITEM_SELECT_ANIM_CALL].setAnimEnable(false);

    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 0].setAnimEnable(false);
    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 1].setAnimEnable(false);
    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 2].setAnimEnable(false);
    mAnm[ITEM_SELECT_ANIM_INPUT_OFFSET + 3].setAnimEnable(false);

    mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(false);

    field_0x57A0 = 0;
    field_0x57A1 = 0;
    mArrowRotation = 0.0f;
    mArrowLength = 0.0f;
    field_0x5780 = 0.0f;

    mpOwnerPane = nullptr;

    field_0x57B9 = 0;
    field_0x57BA = 0;
    field_0x57BB = 0;
    field_0x57BC = 0;
    field_0x5774 = 0;
    mLastDoButtonRelatedAction = LytDoButtonRelated::DO_NONE;
    field_0x5770 = 5;

    setBtnText(LytDoButtonRelated::DO_NONE);

    mStateMgr.changeState(StateID_InitWait);

    return true;
}

bool dLytMeterItemSelect_c::remove() {
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        if (!it->mpLytPane->LytMeter0x24()) {
            continue;
        }
        // @bug checking nullptr after invoking virtual function on it
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < 9; i++) {
        mSubpanes[i].mpLytPane->remove();
    }

    for (int i = 0; i < ITEM_SELECT_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    mEffects.remove(true);
    return true;
}

bool dLytMeterItemSelect_c::execute() {
    if (StoryflagManager::sInstance->getCounterOrFlag(569) != 0) {
        if (field_0x57A2 != 0 || (field_0x57A0 != 0 && field_0x5794 != I_SAILCLOTH && field_0x5794 != I_BOAT_CANNON &&
                                  field_0x5794 != I_HARP)) {
            field_0x57B9 = 0;
            StoryflagManager::sInstance->unsetFlag(569);
        } else {
            field_0x57B9 = 1;
        }
    }

    field_0x57BF = field_0x57BE;
    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() != StateID_DemoMove && *mStateMgr.getStateID() != StateID_DemoOut0 &&
            *mStateMgr.getStateID() != StateID_DemoOut1 && field_0x57C5 != 0) {
            mpPanes[ITEM_SELECT_PANE_BTN_BG]->SetVisible(true);
            mpPanes[ITEM_SELECT_PANE_BTN]->SetVisible(true);
            mpPanes[ITEM_SELECT_PANE_INPUT]->SetVisible(true);
            field_0x57C5 = 0;
        }
        if ((*mStateMgr.getStateID() == StateID_SetIn) || (*mStateMgr.getStateID() == StateID_Set) ||
            (*mStateMgr.getStateID() == StateID_SetOut) || (*mStateMgr.getStateID() == StateID_ToUnuse)) {
            if (mAnm[ITEM_SELECT_ANIM_IN].isEnabled()) {
                mAnm[ITEM_SELECT_ANIM_IN].setToEnd2();
            }

            if (mAnm[ITEM_SELECT_ANIM_DECIDE].isEnabled()) {
                mAnm[ITEM_SELECT_ANIM_DECIDE].setToEnd2();
            }

            if (field_0x5798 != I_INVALID) {
                if (field_0x5794 != I_INVALID) {
                    mAnm[ITEM_SELECT_ANIM_OUT].setForwardOnce();
                    mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
                    mAnm[ITEM_SELECT_ANIM_OUT].setToEnd2();
                } else {
                    mAnm[ITEM_SELECT_ANIM_RESET].setForwardOnce();
                    mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
                    mAnm[ITEM_SELECT_ANIM_RESET].setToEnd2();
                }
            } else if (field_0x5794 != I_INVALID) {
                mAnm[ITEM_SELECT_ANIM_OUT].setForwardOnce();
                mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
                mAnm[ITEM_SELECT_ANIM_OUT].setToEnd2();
            } else {
                mAnm[ITEM_SELECT_ANIM_RESET].setForwardOnce();
                mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
                mAnm[ITEM_SELECT_ANIM_RESET].setToEnd2();
            }

            mLyt.getLayout()->Animate(0);
            mLyt.calc();

            if (mAnm[ITEM_SELECT_ANIM_IN].isEnabled()) {
                mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
            }

            if (mAnm[ITEM_SELECT_ANIM_DECIDE].isEnabled()) {
                mAnm[ITEM_SELECT_ANIM_DECIDE].setAnimEnable(false);
            }

            if (mAnm[ITEM_SELECT_ANIM_OUT].isEnabled()) {
                mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
            }

            if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
                mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
            }

            mBtn[1].setVisible(true, true);
            mBtn[0].setVisible(true, true);

            field_0x57B3 = 0;

            if (field_0x5798 != I_INVALID) {
                if (field_0x5794 != I_INVALID) {
                    mBtn[2].setVisible(false, true);
                } else {
                    mBtn[2].setVisible(true, true);
                }
            } else if (field_0x5794 != I_INVALID) {
                mBtn[2].setVisible(false, true);
            } else {
                mBtn[2].setVisible(true, true);
            }

            mStateMgr.changeState(StateID_Wait);
        } else if (*mStateMgr.getStateID() == StateID_SelectOut) {
            if (mAnm[ITEM_SELECT_ANIM_IN].isEnabled()) {
                mAnm[ITEM_SELECT_ANIM_IN].setToEnd2();
            }

            if (field_0x57BD != 0) {
                field_0x579B = 0;
                field_0x5784 = 0.8f;
                mEffects.fn_80027320(0);
                field_0x57BD = 0;
                field_0x57BE = 0;
                mpPanes[ITEM_SELECT_PANE_ICON_OFFSET + 2]->SetAlpha(0xFF);
            }

            mLyt.getLayout()->Animate(0);
            mLyt.calc();

            if (mAnm[ITEM_SELECT_ANIM_IN].isEnabled()) {
                mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
            }

            mStateMgr.changeState(StateID_Wait);
        }

        // LAB_800ee8ec:
        if (*mStateMgr.getStateID() == StateID_SetNone) {
            mAnm[ITEM_SELECT_ANIM_RESET].setToEnd2();
            mBtn[0].setVisible(true);
            mBtn[2].setVisible(true);

            mLyt.getLayout()->Animate(0);
            mLyt.calc();

            mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
            field_0x57B3 = 0;
            field_0x579A = I_INVALID;
            mStateMgr.changeState(StateID_Wait);
        }
        if (*mStateMgr.getStateID() == StateID_SetSpecialItemIn) {
            mBtn[0].setVisible(true, true);
            if (mpOwnerPane->GetAlpha() != 0xFF) {
                if (mAnm[ITEM_SELECT_ANIM_OUT].isEnabled()) {
                    mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
                }

                if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
                    mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
                }

                mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(true);

                mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
                mAnm[ITEM_SELECT_ANIM_IN].setFrame(0.0f);

                mBtn[1].setVisible(true, true);
                mBtn[2].setVisible(true, true);
                mLyt.calc();

                mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
                mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
                mpPanes[ITEM_SELECT_PANE_SET_BTN]->SetVisible(false);
                field_0x57B3 = 0;
                field_0x579A = I_INVALID;
                mStateMgr.changeState(StateID_Wait);
            }
        }

        if (dLytDobutton_c::getField0x47C() == 0x28) {
            if (field_0x5774 == 0) {
                SmallSoundManager::GetInstance()->playSound(SE_S_PARACHUTE_CALL);
            }
            field_0x5774++;
            if (field_0x5774 >= mAnm[ITEM_SELECT_ANIM_CALL].getAnimDuration() / 2.0f) {
                field_0x5774 = 0;
            }
        } else {
            field_0x5774 = 0;
        }

        return true;
    }

    bool a = false;
    bool b = false;
    if (field_0x5790 > 0) {
        field_0x5790--;
    }

    if (field_0x5794 != field_0x5796) {
        field_0x5796 = field_0x5794;
        a = true;
        if (field_0x5794 != I_INVALID) {
            field_0x5797 = field_0x5794;
            field_0x57B4 = 0;
        } else {
            field_0x57B4 = 1;
        }
        setBtnItem(field_0x5796);
        b = true;
        // The conditions below check if you just upgraded a certain item. If the current item differs from
        // what we recorded last, this automatically switches the item.
    } else if (mStoredBowVariant != baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_BOW))) {
        mStoredBowVariant = baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_BOW));
        setBtnItem(field_0x5796);
        b = true;
    } else if (mStoredSlingshotVariant !=
               baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_SLINGSHOT))) {
        mStoredSlingshotVariant = baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_SLINGSHOT));
        setBtnItem(field_0x5796);
        b = true;
    } else if (mStoredBeetleVariant != baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_BEETLE))) {
        mStoredBeetleVariant = baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_BEETLE));
        setBtnItem(field_0x5796);
        b = true;
    } else if (mStoredBugNetVariant != baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_BUG_NET))) {
        mStoredBugNetVariant = baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(I_BUG_NET));
        setBtnItem(field_0x5796);
        b = true;
    }

    if (field_0x5797 != I_INVALID) {
        realizeBtnNumberForLytIndex(
            baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(field_0x5797)), a
        );
    }

    if (field_0x57B6) {
        if (!mAnm[ITEM_SELECT_ANIM_ENOUGH].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_ENOUGH].setFrame(0.0f);
            mAnm[ITEM_SELECT_ANIM_ENOUGH].setAnimEnable(true);
        }
        field_0x57B6 = false;
    }

    if (field_0x57B7) {
        if (!mAnm[ITEM_SELECT_ANIM_FULL].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_FULL].setFrame(0.0f);
            mAnm[ITEM_SELECT_ANIM_FULL].setAnimEnable(true);
            if (field_0x5797 == I_BOMB_BAG) {
                SmallSoundManager::GetInstance()->playSound(SE_S_BM_MAX);
            } else if (field_0x5797 == I_BOW) {
                SmallSoundManager::GetInstance()->playSound(SE_S_AW_MAX);
            } else if (field_0x5797 == I_SLINGSHOT) {
                SmallSoundManager::GetInstance()->playSound(SE_S_PC_MAX);
            }
        }
        field_0x57B7 = false;
    }

    realizeSelectedWheelItem();
    mStateMgr.executeState();
    fn_800EF8C0(false);

    if (dLytDobutton_c::getField0x47C() == 0x28) {
        if (field_0x57C3 != 0) {
            field_0x57BB = 1;
        }
    } else {
        field_0x57C3 = 0;
    }

    bool b2 = false;
    if (field_0x57B9 != 0 && dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) &&
        mLastDoButtonRelatedAction == 0x85) {
        if (field_0x57BA == 0) {
            mAnm[ITEM_SELECT_ANIM_CALL].setToEnd();
            mAnm[ITEM_SELECT_ANIM_CALL].setRate(1.0f);
            mAnm[ITEM_SELECT_ANIM_CALL].setAnimEnable(true);
            field_0x57BA = 1;
        }

        if (mAnm[ITEM_SELECT_ANIM_CALL].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_CALL].play();
            if (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) &&
                mAnm[ITEM_SELECT_ANIM_CALL].getFrame() == 1.0f) {
                SmallSoundManager::GetInstance()->playSound(SE_S_B_BUTTON_BLINK);
            }
        }
        b2 = true;
    } else {
        if (field_0x57BA == 1) {
            mAnm[ITEM_SELECT_ANIM_CALL].setToEnd();
            b2 = true;
        }

        if (field_0x57BB != 0) {
            if (field_0x57BC == 0) {
                mAnm[ITEM_SELECT_ANIM_CALL].setToEnd();
                mAnm[ITEM_SELECT_ANIM_CALL].setRate(2.0f);
                mAnm[ITEM_SELECT_ANIM_CALL].setAnimEnable(true);
            }

            if (mAnm[ITEM_SELECT_ANIM_CALL].isEnabled()) {
                mAnm[ITEM_SELECT_ANIM_CALL].play();
                if (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) &&
                    mAnm[ITEM_SELECT_ANIM_CALL].getFrame() == 1.0f) {
                    SmallSoundManager::GetInstance()->playSound(SE_S_PARACHUTE_CALL);
                }
            }
            b2 = true;
        } else if (field_0x57BC == 1) {
            mAnm[ITEM_SELECT_ANIM_CALL].setToEnd();
            b2 = true;
        }
    }

    if (!b2 && mAnm[ITEM_SELECT_ANIM_CALL].isEnabled()) {
        mAnm[ITEM_SELECT_ANIM_CALL].setAnimEnable(false);
    }

    if (field_0x57B9 == 0) {
        field_0x57BA = field_0x57B9;
    }

    field_0x57B9 = 0;
    field_0x57BC = field_0x57BB;
    field_0x57BB = 0;

    if (*mStateMgr.getStateID() != StateID_InitWait) {
        for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
            mIcon[i].execute();
        }
        mBlink.execute();
        for (int i = 0; i < 2; i++) {
            mText[i].execute();
        }

        for (int i = 0; i < 4; i++) {
            mBtn[i].execute();
            if (mBtn[i].getField_0x57() != 0) {
                mBtn[i].setField_0x57(0);
                b = true;
            }
        }

        if (b) {
            mLyt.getLayout()->Animate(0);
        }

        // Rotate the arrow
        mVec3_c t1(0.0f, 0.0f, 0.0f);
        t1.z = mArrowRotation;
        mpPanes[ITEM_SELECT_PANE_ITEM_ARROW]->SetRotate(t1);
        // But rotate the button and the pointer back so that
        // they point up
        mVec3_c t2(0.0f, 0.0f, 0.0f);
        t2.z = -mArrowRotation;
        mpPanes[ITEM_SELECT_PANE_ARROW_HAND]->SetRotate(t2);
        mpPanes[ITEM_SELECT_PANE_N_BTN_0]->SetRotate(t2);

        f32 frame = mAnm[ITEM_SELECT_ANIM_ARROW].getAnimDuration();
        if (mArrowLength < 0.0f) {
            mArrowLength = 0.0f;
        }
        if (mArrowLength > 1.0f) {
            mArrowLength = 1.0f;
        }
        mAnm[ITEM_SELECT_ANIM_ARROW].setFrame(frame * mArrowLength);
        mAnm[ITEM_SELECT_ANIM_ARROW].setAnimEnable(true);

        if (mAnm[ITEM_SELECT_ANIM_SELECT_LOOP].isEnabled()) {
            mAnm[ITEM_SELECT_ANIM_SELECT_LOOP].play();
        }

        if (mAnm[ITEM_SELECT_ANIM_ENOUGH].isEnabled()) {
            if (mAnm[ITEM_SELECT_ANIM_ENOUGH].isEndReached()) {
                mAnm[ITEM_SELECT_ANIM_ENOUGH].setFrame(0.0f);
                mAnm[ITEM_SELECT_ANIM_ENOUGH].setAnimEnable(false);
            } else {
                mAnm[ITEM_SELECT_ANIM_ENOUGH].play();
            }
        }

        if (mAnm[ITEM_SELECT_ANIM_FULL].isEnabled()) {
            if (mAnm[ITEM_SELECT_ANIM_FULL].isEndReached()) {
                mAnm[ITEM_SELECT_ANIM_FULL].setFrame(0.0f);
                mAnm[ITEM_SELECT_ANIM_FULL].setAnimEnable(false);
            } else {
                mAnm[ITEM_SELECT_ANIM_FULL].play();
            }
        }

        for (int i = 0; i < 9; i++) {
            mSubpanes[i].mpLytPane->execute();
        }

        if (field_0x5794 == I_SAILCLOTH) {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_B, LytDoButtonRelated::DO_SAILCLOTH);
        } else if (field_0x5794 == I_BOAT_CANNON) {
            if (field_0x57B3 != 0) {
                LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_B, LytDoButtonRelated::DO_STOW_CANNON);
            } else {
                LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_B, LytDoButtonRelated::DO_READY_CANNON);
            }
        }

        if (field_0x578C <= 0 && *mStateMgr.getStateID() != StateID_SetIn &&
            *mStateMgr.getStateID() != StateID_SetOut && *mStateMgr.getStateID() != StateID_ToUnuse &&
            *mStateMgr.getStateID() != StateID_Unuse) {
            if (mLastDoButtonRelatedAction != LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B)) {
                mLastDoButtonRelatedAction = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B);
                setBtnText(mLastDoButtonRelatedAction);
            }
        } else {
            if (mLastDoButtonRelatedAction != LytDoButtonRelated::DO_NONE) {
                mLastDoButtonRelatedAction = LytDoButtonRelated::DO_NONE;
                setBtnText(mLastDoButtonRelatedAction);
            }

            if (field_0x578C > 0) {
                field_0x578C--;
            }
        }

        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_B, LytDoButtonRelated::DO_NONE);
    }

    field_0x57C1 = isWheelBlockedByCurrentAction();
    return true;
}

extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_717_;
void dLytMeterItemSelect_c::fn_800EF580() {
    nw4r::math::MTX34 mtx = mpPanes[11]->GetGlobalMtx();
    mVec3_c pos(mtx._03, mtx._13, 0.0f);
    mVec3_c scale(field_0x5784, field_0x5784, field_0x5784);
    mEffects.fn_80029A70(PARTICLE_RESOURCE_ID_MAPPING_717_, &pos, nullptr, &scale, nullptr, nullptr);

    if (field_0x57C0 != 0) {
        mEffects.fn_80026ED0();
        field_0x57C0 = 0;
    } else {
        if (field_0x57BE != field_0x57BF) {
            if (field_0x57BE != 0) {
                mEffects.fn_80026F10();
            } else {
                mEffects.fn_80026ED0();
            }
        }
    }
}

void dLytMeterItemSelect_c::fn_800EF6B0(u8 arg) {
    if (field_0x5794 != I_SAILCLOTH && field_0x5794 != I_BOAT_CANNON && field_0x5794 != I_HARP) {
        field_0x5798 = field_0x5794;
    }

    if (field_0x5794 != arg) {
        field_0x5795 = field_0x5794;
    }
    field_0x5794 = arg;
}

void dLytMeterItemSelect_c::setOwnerPane(nw4r::lyt::Pane *p) {
    mpOwnerPane = p;
    for (int i = 0; i < 4; i++) {
        mBtn[i].setOwnerPane(p);
    }
}

void dLytMeterItemSelect_c::fn_800EF710(u8 arg) {
    field_0x57A0 = arg;
}

void dLytMeterItemSelect_c::fn_800EF720(InternalItem_e arg) {
    if (field_0x57B5 == 0 || *mStateMgr.getStateID() != StateID_InitWait) {
        field_0x57B5 = 1;
        field_0x574C = arg;
    }
}

void dLytMeterItemSelect_c::fn_800EF7A0(bool a, s32 b) {
    if (field_0x5796 == b) {
        field_0x57B6 = a;
    }
}

void dLytMeterItemSelect_c::realizeSelectedWheelItem() {
    s32 selectedSlot = -1;
    if (field_0x5754 == 0) {
        for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
            if (field_0x574C == fn_800F0170(i)) {
                selectedSlot = i;
                break;
            }
        }
    }

    ITEM_ID id = static_cast<ITEM_ID>(-1);
    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        if (selectedSlot == i) {
            id = getBaseItemForBWheelSlot(selectedSlot);
            mIcon[i].setShouldBeSelect(true);
        } else {
            mIcon[i].setShouldBeSelect(false);
        }
    }

    setCurrentItemText(baseItemIdToCurrentVariant(id));
    if (selectedSlot != -1 && field_0x5754 == 0) {
        mBlink.setShouldBeSelect(true);
        mText[0].setShouldBeSelect(true);
    } else {
        mBlink.setShouldBeSelect(false);
        mText[0].setShouldBeSelect(false);
    }
}

void dLytMeterItemSelect_c::fn_800EF8C0(bool bInstant) {
    bool visible = true;
    if (dAcPy_c::GetLink2()->fn_80202D90(false)) {
        mpPanes[10]->SetInfluencedAlpha(true);
        mpPanes[10]->SetAlpha(64);
        if (field_0x5794 == 0) {
            visible = false;
        }
    } else {
        mpPanes[10]->SetInfluencedAlpha(true);
        mpPanes[10]->SetAlpha(0xFF);
    }

    if (!mBtn[1].getShouldBeVisible() && dLytMeterContainer_c::GetInstance()->getMeterField_0x13770() != 1 &&
        dLytMeterContainer_c::GetInstance()->getMeterField_0x13770() != 2) {
        visible = false;
    }
    if (bInstant == false) {
        bInstant = mBtn[1].getInstant();
    }

    if (visible) {
        mBtn[3].setVisible(true, bInstant);
    } else {
        mBtn[3].setVisible(false, bInstant);
    }
}

void dLytMeterItemSelect_c::realizeWheelNumberForLytIndex(s32 iconIdx, s32 lytItemIdx) {
    if (lytItemIdx == LYT_CMN_ItemBombBag || lytItemIdx == LYT_CMN_ItemBow || lytItemIdx == LYT_CMN_ItemSlingshot ||
        lytItemIdx == LYT_CMN_ItemIronBow || lytItemIdx == LYT_CMN_ItemSacredBow ||
        lytItemIdx == LYT_CMN_ItemScattershot) {
        s32 num = 0;
        s32 max = 0;
        switch (lytItemIdx) {
            case LYT_CMN_ItemBombBag:
                num = dAcItem_c::getTotalBombCount();
                max = dAcItem_c::getTotalBombCapacity();
                break;
            case LYT_CMN_ItemBow:
            case LYT_CMN_ItemIronBow:
            case LYT_CMN_ItemSacredBow:
                num = dAcItem_c::getTotalArrowCount();
                max = dAcItem_c::getTotalArrowCapacity();
                break;
            case LYT_CMN_ItemSlingshot:
            case LYT_CMN_ItemScattershot:
                num = dAcItem_c::getTotalSeedCount();
                max = dAcItem_c::getTotalSeedCapacity();
                break;
        }

        s32 color = 0;
        if (num == 0) {
            color = 1;
        } else if (num == max) {
            color = 2;
        }
        mItemIcons[iconIdx].setNumber(num);
        mItemIcons[iconIdx].setNumberColor(color);
    }
}

void dLytMeterItemSelect_c::realizeBtnNumberForLytIndex(s32 lytItemIdx, bool b) {
    if (lytItemIdx == LYT_CMN_ItemBombBag || lytItemIdx == LYT_CMN_ItemBow || lytItemIdx == LYT_CMN_ItemSlingshot ||
        lytItemIdx == LYT_CMN_ItemIronBow || lytItemIdx == LYT_CMN_ItemSacredBow ||
        lytItemIdx == LYT_CMN_ItemScattershot) {
        s32 num = 0;
        s32 max = 0;
        switch (lytItemIdx) {
            case LYT_CMN_ItemBombBag:
                num = dAcItem_c::getTotalBombCount();
                max = dAcItem_c::getTotalBombCapacity();
                break;
            case LYT_CMN_ItemBow:
            case LYT_CMN_ItemIronBow:
            case LYT_CMN_ItemSacredBow:
                num = dAcItem_c::getTotalArrowCount();
                max = dAcItem_c::getTotalArrowCapacity();
                break;
            case LYT_CMN_ItemSlingshot:
            case LYT_CMN_ItemScattershot:
                num = dAcItem_c::getTotalSeedCount();
                max = dAcItem_c::getTotalSeedCapacity();
                break;
        }

        SizedWString<32> numberText;
        numberText.sprintf(L"%d", num);
        mpTextBoxes[0]->setTextWithGlobalTextProcessor(numberText, nullptr);
        mpTextBoxes[1]->setTextWithGlobalTextProcessor(numberText, nullptr);
        mpTextBoxes[2]->setTextWithGlobalTextProcessor(numberText, nullptr);
        mpTextBoxes[3]->setTextWithGlobalTextProcessor(numberText, nullptr);

        s32 color = 0;
        if (num == 0) {
            color = 1;
        } else if (num == max) {
            color = 2;
        }

        if (color != field_0x5764) {
            if (!b && color == 2) {
                field_0x57B7 = 1;
            }
            field_0x5764 = color;
        }

        mAnm[ITEM_SELECT_ANIM_TEXTCOLOR].setFrame(color);
        mAnm[ITEM_SELECT_ANIM_TEXTCOLOR].setAnimEnable(true);
        mLyt.calc();
        mAnm[ITEM_SELECT_ANIM_TEXTCOLOR].setAnimEnable(false);
        mAnm[ITEM_SELECT_ANIM_NUMBERV].setFrame(1.0f);
    } else {
        mAnm[ITEM_SELECT_ANIM_NUMBERV].setFrame(0.0f);
    }
}

void dLytMeterItemSelect_c::realizeWheelNumberVForLytIndex(s32 iconIdx, s32 lytItemIdx) {
    if (!mIsBocoburinLocked[iconIdx] &&
        (lytItemIdx == LYT_CMN_ItemBombBag || lytItemIdx == LYT_CMN_ItemBow || lytItemIdx == LYT_CMN_ItemSlingshot ||
         lytItemIdx == LYT_CMN_ItemIronBow || lytItemIdx == LYT_CMN_ItemSacredBow ||
         lytItemIdx == LYT_CMN_ItemScattershot)) {
        mItemIcons[iconIdx].setHasNumber(true);
    } else {
        mItemIcons[iconIdx].setHasNumber(false);
    }
}

bool dLytMeterItemSelect_c::fn_800EFDF0(bool b) const {
    if (!mpOwnerPane->IsVisible()) {
        return false;
    }

    if (field_0x57A2 != 0) {
        if (*mStateMgr.getStateID() == StateID_ToUse || *mStateMgr.getStateID() == StateID_InitWait ||
            *mStateMgr.getStateID() == StateID_Wait) {
            return true;
        }
    }

    if (*mStateMgr.getStateID() == StateID_SelectIn || *mStateMgr.getStateID() == StateID_Select ||
        *mStateMgr.getStateID() == StateID_SelectOut) {
        return true;
    }

    if (!b && (*mStateMgr.getStateID() == StateID_SetIn || *mStateMgr.getStateID() == StateID_Set ||
               *mStateMgr.getStateID() == StateID_SetOut)) {
        return true;
    }
    return false;
}

bool dLytMeterItemSelect_c::fn_800F0030() const {
    if (!StoryflagManager::sInstance->getCounterOrFlag(58) &&
            !(EventManager::isInEvent() && EventManager::isCurrentEvent("ItemGetGorgeous")) ||
        (mpOwnerPane == nullptr || !mpOwnerPane->IsVisible() || dLytMeterContainer_c::GetMeter()->fn_800D5650() ||
         dLytMeterContainer_c::GetMeter()->fn_800D5680() ||
         dLytMeterContainer_c::GetMeter()->getField_0x13792() == 0)) {
        // If we don't have a B-Wheel and we're not currently in the ItemGetGorgeous event,
        // or (random other conditions), return false
        return false;
    }

    return true;
}

static const ITEM_ID sSlotToBaseItem[] = {
    ITEM_BOW, ITEM_BOMB_BAG, ITEM_BEETLE, ITEM_BUG_NET, ITEM_SLINGSHOT, ITEM_CLAWSHOTS, ITEM_WHIP, ITEM_GUST_BELLOWS,
};

ITEM_ID dLytMeterItemSelect_c::getBaseItemForBWheelSlot(s32 idx) {
    return sSlotToBaseItem[idx];
}

// What is this for? This is not an internal item id...
static s32 sUnkArray1[] = {1, 0, 3, 8, 4, 2, 6, 5};

s32 dLytMeterItemSelect_c::fn_800F0170(s32 slot) const {
    return sUnkArray1[slot];
}

u8 dLytMeterItemSelect_c::getInternalBaseItemForSlot(s32 slot) const {
    static s32 sSlotToInternalItem[] = {I_BOW,       I_BOMB_BAG,  I_BEETLE, I_BUG_NET,
                                        I_SLINGSHOT, I_CLAWSHOTS, I_WHIP,   I_GUST_BELLOWS};

    return sSlotToInternalItem[slot];
}

extern "C" s32 fn_801673B0(s32);

bool dLytMeterItemSelect_c::fn_800F01B0(s32 arg) const {
    return fn_801673B0(arg) != 0;
}

extern "C" bool fn_80167780(s32, bool);

bool dLytMeterItemSelect_c::fn_800F01E0(s32 arg) const {
    return fn_80167780(arg, true);
}

bool dLytMeterItemSelect_c::isSlotBocoburinLocked(s32 slot) {
    return dAcPy_c::isItemRestrictedByBokoBase(getBaseItemForBWheelSlot(slot));
}

bool dLytMeterItemSelect_c::fn_800F0220(s32 arg) {
    if (field_0x5754 == 0) {
        field_0x5754 = 1;

        field_0x5758 = arg;
        field_0x575C = 0;
        mArrowRotation = 0.0f;
        mArrowLength = 0.0f;
        mpPanes[4]->SetVisible(false);
        mpPanes[5]->SetVisible(false);
        mpPanes[3]->SetVisible(false);
        for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
            mIcon[i].init();
        }
    } else if (field_0x5754 == 4) {
        field_0x5754 = 0;
        return true;
    }
    return false;
}

bool dLytMeterItemSelect_c::fn_800F02F0() const {
    return field_0x5754 != 0;
}

void dLytMeterItemSelect_c::fn_800F0310() {
    mBtn[1].setVisible(false, true);
    if (field_0x57B3 == 0) {
        mBtn[0].setVisible(false, true);
        mBtn[2].setVisible(false, true);
    }

    mLyt.calc();

    if (field_0x57B3 == 0 && field_0x57B4 != 0) {
        mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
        field_0x57B4 = 0;
    }

    if (field_0x5754 == 1) {
        mStateMgr.changeState(StateID_DemoMove);
    } else if (field_0x57A0 != 0) {
        field_0x5780 = mAnm[ITEM_SELECT_ANIM_DECIDE].getAnimDuration();
        mStateMgr.changeState(StateID_SetIn);
    } else {
        mStateMgr.changeState(StateID_SelectIn);
    }
}

#include "d/lyt/meter/d_lyt_meter_action_table.inc"

void dLytMeterItemSelect_c::setBtnText(s32 unkId) {
    SizedString<16> id;
    if (unkId < LytDoButtonRelated::DO_NONE) {
        mpTextBoxes[8]->SetVisible(true);
        mpTextBoxes[9]->SetVisible(true);
        mpWindows[0]->SetVisible(true);
        mpTextBoxes[4]->SetVisible(true);
        mpTextBoxes[5]->SetVisible(true);
        mpWindows[3]->SetVisible(true);
        if (unkId < 0x5D) {
            id.sprintf("ACT_INFO_%03d", sActIds[unkId]);
        } else {
            unkId -= 0x5E;
            id.sprintf("ACT_ETC_%03d", sActIds[unkId]);
        }
        mpTextBoxes[8]->setMessageWithGlobalTextProcessor2(id, nullptr);
        mpTextBoxes[9]->setMessageWithGlobalTextProcessor2(id, nullptr);
        mpWindows[0]->UpdateSize(mpSizeBoxes[0], 32.0f);
        mpTextBoxes[4]->setMessageWithGlobalTextProcessor2(id, nullptr);
        mpTextBoxes[5]->setMessageWithGlobalTextProcessor2(id, nullptr);
        mpWindows[3]->UpdateSize(mpSizeBoxes[3], 32.0f);
    } else {
        const wchar_t *empty = L"";
        mpTextBoxes[8]->setTextWithGlobalTextProcessor(empty, nullptr);
        mpTextBoxes[9]->setTextWithGlobalTextProcessor(empty, nullptr);
        mpTextBoxes[8]->SetVisible(false);
        mpTextBoxes[9]->SetVisible(false);
        mpWindows[0]->SetVisible(false);
        mpTextBoxes[4]->setTextWithGlobalTextProcessor(empty, nullptr);
        mpTextBoxes[5]->setTextWithGlobalTextProcessor(empty, nullptr);
        mpTextBoxes[4]->SetVisible(false);
        mpTextBoxes[5]->SetVisible(false);
        mpWindows[3]->SetVisible(false);
    }
}

void dLytMeterItemSelect_c::setBtnItem(s32 item) {
    if (item == I_SAILCLOTH || item == I_BOAT_CANNON || item == I_HARP) {
        field_0x57B8 = 1;
    } else {
        field_0x57B8 = 0;
    }

    if (item != I_INVALID) {
        mItemIcons[8].setItem(baseItemLytIndexToCurrentVariant(getBaseItemLytIndexforInternalId(item)));
    }
}

static const s32 sSlotToBaseItemLytIdx[] = {
    LYT_CMN_ItemBow,       LYT_CMN_ItemBombBag,   LYT_CMN_ItemBeetle, LYT_CMN_ItemBugnet,
    LYT_CMN_ItemSlingshot, LYT_CMN_ItemClawshots, LYT_CMN_ItemWhip,   LYT_CMN_ItemGustBellows,
};

void dLytMeterItemSelect_c::realizeWheelItems() {
    for (int i = 0; i < ITEM_SELECT_NUM_ITEMS; i++) {
        s32 lytItem = baseItemLytIndexToCurrentVariant(sSlotToBaseItemLytIdx[i]);
        if (mIsBocoburinLocked[i]) {
            mItemIcons[i].setBocoburinLocked(true);
        } else {
            mItemIcons[i].setBocoburinLocked(false);
        }
        mItemIcons[i].setUnk(false);
        realizeWheelNumberVForLytIndex(i, lytItem);
        realizeWheelNumberForLytIndex(i, lytItem);
        mItemIcons[i].setItem(lytItem);
    }
}

s32 dLytMeterItemSelect_c::baseItemLytIndexToCurrentVariant(s32 baseItem) const {
    if (baseItem == LYT_CMN_ItemBow) {
        switch (dAcPy_c::getCurrentBowType()) {
            case 2: return LYT_CMN_ItemIronBow;
            case 3: return LYT_CMN_ItemSacredBow;
        }
    } else if (baseItem == LYT_CMN_ItemSlingshot) {
        switch (dAcPy_c::getCurrentSlingshotType()) {
            case 2: return LYT_CMN_ItemScattershot;
        }
    } else if (baseItem == LYT_CMN_ItemBeetle) {
        switch (dAcPy_c::getCurrentBeetleType()) {
            case 2: return LYT_CMN_ItemHookBeetle;
            case 3: return LYT_CMN_ItemQuickBeetle;
            case 4: return LYT_CMN_ItemToughBeetle;
        }
    } else if (baseItem == LYT_CMN_ItemBugnet) {
        switch (dAcPy_c::getCurrentBugNetType()) {
            case 2: return LYT_CMN_ItemBigBugnet;
        }
    }
    return baseItem;
}

ITEM_ID dLytMeterItemSelect_c::baseItemIdToCurrentVariant(ITEM_ID baseItem) const {
    if (baseItem == ITEM_BOW) {
        switch (dAcPy_c::getCurrentBowType()) {
            case 2: return ITEM_IRON_BOW;
            case 3: return ITEM_SACRED_BOW;
        }
    } else if (baseItem == ITEM_SLINGSHOT) {
        switch (dAcPy_c::getCurrentSlingshotType()) {
            case 2: return ITEM_MIGHTY_SCATTERSHOT;
        }
    } else if (baseItem == ITEM_BEETLE) {
        switch (dAcPy_c::getCurrentBeetleType()) {
            case 2: return ITEM_HOOK_BEETLE;
            case 3: return ITEM_QUICK_BEETLE;
            case 4: return ITEM_TOUGH_BEETLE;
        }
    } else if (baseItem == ITEM_BUG_NET) {
        switch (dAcPy_c::getCurrentBugNetType()) {
            case 2: return ITEM_BIG_BUG_NET;
        }
    }
    return baseItem;
}

static const s32 sInternalItemToLytIndex[] = {
    LYT_CMN_ItemBombBag, LYT_CMN_ItemBow,        LYT_CMN_ItemSailcloth,   LYT_CMN_ItemClawshots,
    LYT_CMN_ItemBeetle,  LYT_CMN_ItemSlingshot,  LYT_CMN_ItemGustBellows, LYT_CMN_ItemBugnet,
    LYT_CMN_ItemWhip,    LYT_CMN_ItemBoatCannon, LYT_CMN_ItemHarp,
};

s32 dLytMeterItemSelect_c::getBaseItemLytIndexforInternalId(s32 idx) const {
    if (idx < 11) {
        return sInternalItemToLytIndex[idx];
    } else if (field_0x5797 != I_INVALID) {
        return sInternalItemToLytIndex[field_0x5797];
    }

    return LYT_CMN_ItemInvalid;
}

extern "C" bool checkIsInSkykeepPuzzle();

bool dLytMeterItemSelect_c::isWheelBlockedByCurrentAction() {
    if (dAcPy_c::LINK->getRidingActorType() == 2 || dAcPy_c::LINK->checkActionFlags(dAcPy_c::FLG0_SWING_ROPE) ||
        dAcPy_c::LINK->checkCurrentAction(0x9C) || dAcPy_c::LINK->getRidingActorType() == 1 ||
        dAcPy_c::LINK->checkCurrentAction(0x95) || isInSpiralChargeTutorialMinigame() || isInRollercoasterMinigame() ||
        dAcPy_c::LINK->checkCurrentAction(0x8C) || dLytMeterContainer_c::GetMeter()->getField_0x1377E() != 0 ||
        checkIsInSkykeepPuzzle() || dLytMeterContainer_c::GetInstance()->getMeterField_0x13770() == 0) {
        return true;
    }

    return false;
}

bool dLytMeterItemSelect_c::isInSpiralChargeTutorialMinigame() {
    return MinigameManager::isInMinigameState(MinigameManager::SPIRAL_CHARGE_TUTORIAL);
}

bool dLytMeterItemSelect_c::isInRollercoasterMinigame() {
    return MinigameManager::isInMinigameState(MinigameManager::ROLLERCOASTER);
}

void dLytMeterItemSelect_c::setCurrentItemText(ITEM_ID item) {
    if (item >= ITEM_NONE && field_0x5754 == 0) {
        mText[1].setShouldBeSelect(true);
        SizedString<16> msg;
        msg.sprintf("NAME_ITEM_%03d", item);
        mpTextBoxes[6]->setMessageWithGlobalTextProcessor2(msg, nullptr);
        mpTextBoxes[7]->setMessageWithGlobalTextProcessor2(msg, nullptr);
        mpWindows[2]->UpdateSize(mpSizeBoxes[2], 32.0f);
    } else {
        mText[1].setShouldBeSelect(false);
    }
}
