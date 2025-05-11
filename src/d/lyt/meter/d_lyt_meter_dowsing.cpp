#include "d/lyt/meter/d_lyt_meter_dowsing.h"

#include "common.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_area_caption.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_do_button.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/d_lyt_util_items.h"
#include "d/lyt/d_window.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/math/math_types.h"
#include "sized_string.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/small_sound_mgr.h"

STATE_DEFINE(dLytMeterDowsingMenuIcon_c, Wait);
STATE_DEFINE(dLytMeterDowsingMenuIcon_c, On);
STATE_DEFINE(dLytMeterDowsingMenuIcon_c, Select);
STATE_DEFINE(dLytMeterDowsingMenuIcon_c, Off);

STATE_DEFINE(dLytMeterDowsingBlink_c, Wait);
STATE_DEFINE(dLytMeterDowsingBlink_c, On);
STATE_DEFINE(dLytMeterDowsingBlink_c, Select);
STATE_DEFINE(dLytMeterDowsingBlink_c, Off);

STATE_DEFINE(dLytMeterDowsingInput_c, Wait);
STATE_DEFINE(dLytMeterDowsingInput_c, On);
STATE_DEFINE(dLytMeterDowsingInput_c, Select);
STATE_DEFINE(dLytMeterDowsingInput_c, Off);

STATE_DEFINE(dLytMeterDowsingText_c, Wait);
STATE_DEFINE(dLytMeterDowsingText_c, On);
STATE_DEFINE(dLytMeterDowsingText_c, Select);
STATE_DEFINE(dLytMeterDowsingText_c, Off);

STATE_DEFINE(dLytMeterDowsing_c, Invisible);
STATE_DEFINE(dLytMeterDowsing_c, Wait);
STATE_DEFINE(dLytMeterDowsing_c, MenuSelectingIn);
STATE_DEFINE(dLytMeterDowsing_c, MenuSelecting);
STATE_DEFINE(dLytMeterDowsing_c, MenuSelectingOut);
STATE_DEFINE(dLytMeterDowsing_c, Reset);
STATE_DEFINE(dLytMeterDowsing_c, ToUse);
STATE_DEFINE(dLytMeterDowsing_c, ToUnuse);
STATE_DEFINE(dLytMeterDowsing_c, Unuse);
STATE_DEFINE(dLytMeterDowsing_c, DemoMove);
STATE_DEFINE(dLytMeterDowsing_c, DemoOut);

void dLytMeterDowsingMenuIcon_c::initializeState_Wait() {}
void dLytMeterDowsingMenuIcon_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterDowsingMenuIcon_c::finalizeState_Wait() {}

void dLytMeterDowsingMenuIcon_c::initializeState_On() {}
void dLytMeterDowsingMenuIcon_c::executeState_On() {
    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterDowsingMenuIcon_c::finalizeState_On() {}

void dLytMeterDowsingMenuIcon_c::initializeState_Select() {}
void dLytMeterDowsingMenuIcon_c::executeState_Select() {
    mpAnm->setBackwardsOnce();
    mpAnm->setToStart();
    mpAnm->setAnimEnable(true);
    if (mShouldBeSelect == 0) {
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterDowsingMenuIcon_c::finalizeState_Select() {}

void dLytMeterDowsingMenuIcon_c::initializeState_Off() {}
void dLytMeterDowsingMenuIcon_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterDowsingMenuIcon_c::finalizeState_Off() {}

void dLytMeterDowsingMenuIcon_c::init() {
    mShouldBeSelect = 0;
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterDowsingMenuIcon_c::execute() {
    mStateMgr.executeState();
}

void dLytMeterDowsingMenuIcon_c::reset() {
    mShouldBeSelect = 0;
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterDowsingBlink_c::initializeState_Wait() {}
void dLytMeterDowsingBlink_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterDowsingBlink_c::finalizeState_Wait() {}

void dLytMeterDowsingBlink_c::initializeState_On() {}
void dLytMeterDowsingBlink_c::executeState_On() {
    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterDowsingBlink_c::finalizeState_On() {}

void dLytMeterDowsingBlink_c::initializeState_Select() {}
void dLytMeterDowsingBlink_c::executeState_Select() {
    if (mShouldBeSelect == 0) {
        mpAnm->setBackwardsOnce();
        mpAnm->setToStart();
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterDowsingBlink_c::finalizeState_Select() {}

void dLytMeterDowsingBlink_c::initializeState_Off() {}
void dLytMeterDowsingBlink_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterDowsingBlink_c::finalizeState_Off() {}

void dLytMeterDowsingBlink_c::init() {
    mShouldBeSelect = 0;
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterDowsingBlink_c::execute() {
    mStateMgr.executeState();
}

void dLytMeterDowsingInput_c::initializeState_Wait() {}
void dLytMeterDowsingInput_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setBackwardsOnce();
        mpAnm->setToStart();
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterDowsingInput_c::finalizeState_Wait() {}

void dLytMeterDowsingInput_c::initializeState_On() {}
void dLytMeterDowsingInput_c::executeState_On() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterDowsingInput_c::finalizeState_On() {}

void dLytMeterDowsingInput_c::initializeState_Select() {}
void dLytMeterDowsingInput_c::executeState_Select() {
    if (mShouldBeSelect == 0) {
        mpAnm->setForwardOnce();
        mpAnm->setToStart();
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterDowsingInput_c::finalizeState_Select() {}

void dLytMeterDowsingInput_c::initializeState_Off() {}
void dLytMeterDowsingInput_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterDowsingInput_c::finalizeState_Off() {}

void dLytMeterDowsingInput_c::init() {
    mShouldBeSelect = 0;
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterDowsingInput_c::execute() {
    mStateMgr.executeState();
}

void dLytMeterDowsingText_c::initializeState_Wait() {}
void dLytMeterDowsingText_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterDowsingText_c::finalizeState_Wait() {}

void dLytMeterDowsingText_c::initializeState_On() {}
void dLytMeterDowsingText_c::executeState_On() {
    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterDowsingText_c::finalizeState_On() {}

void dLytMeterDowsingText_c::initializeState_Select() {}
void dLytMeterDowsingText_c::executeState_Select() {
    if (mShouldBeSelect == 0) {
        mpAnm->setBackwardsOnce();
        mpAnm->setToStart();
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterDowsingText_c::finalizeState_Select() {}

void dLytMeterDowsingText_c::initializeState_Off() {}
void dLytMeterDowsingText_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterDowsingText_c::finalizeState_Off() {}

void dLytMeterDowsingText_c::init() {
    mShouldBeSelect = 0;
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterDowsingText_c::execute() {
    mStateMgr.executeState();
}

static const char *sPaneNames[] = {
    "N_itemArrow_00", "N_arrowHand_00", "N_bBtn_00",  "N_A_00",     "N_B_00",     "N_C_00",
    "N_D_00",         "N_E_00",         "N_F_00",     "N_G_00",     "N_H_00",     "N_iconA_00",
    "N_iconB_00",     "N_iconC_00",     "N_iconD_00", "N_iconE_00", "N_iconF_00", "N_iconG_00",
    "N_iconH_00",     "N_btnBg_01",     "N_input_00", "N_input_01",
};

#define DOWSING_PANE_ITEM_ARROW 0
#define DOWSING_PANE_ARROW_HAND 1
#define DOWSING_PANE_N_BTN_0 2
#define DOWSING_PANE_ITEM_OFFSET 3
#define DOWSING_PANE_ICON_OFFSET 11
#define DOWSING_PANE_BTN_BG_01 19
#define DOWSING_PANE_INPUT_00 20
#define DOWSING_PANE_INPUT_01 21

#define DOWSING_NUM_PANES 22

static const d2d::LytBrlanMapping brlanMap[] = {
    {         "nunBtn_03_in.brlan",       "G_inOut_00"},
    {      "nunBtn_03_onOff.brlan",       "G_itemA_00"},
    {      "nunBtn_03_onOff.brlan",       "G_itemB_00"},
    {      "nunBtn_03_onOff.brlan",       "G_itemC_00"},
    {      "nunBtn_03_onOff.brlan",       "G_itemD_00"},
    {      "nunBtn_03_onOff.brlan",       "G_itemE_00"},
    {      "nunBtn_03_onOff.brlan",       "G_itemF_00"},
    {      "nunBtn_03_onOff.brlan",       "G_itemG_00"},
    {      "nunBtn_03_onOff.brlan",       "G_itemH_00"},
    {      "nunBtn_03_onOff.brlan",        "G_text_00"},
    {      "nunBtn_03_onOff.brlan",        "G_text_01"},
    {"nunBtn_03_selectOnOff.brlan", "G_selectOnOff_00"},
    { "nunBtn_03_selectLoop.brlan",  "G_selectLoop_00"},
    {       "nunBtn_03_have.brlan",       "G_itemA_00"},
    {       "nunBtn_03_have.brlan",       "G_itemB_00"},
    {       "nunBtn_03_have.brlan",       "G_itemC_00"},
    {       "nunBtn_03_have.brlan",       "G_itemD_00"},
    {       "nunBtn_03_have.brlan",       "G_itemF_00"},
    {       "nunBtn_03_have.brlan",       "G_itemG_00"},
    {       "nunBtn_03_have.brlan",       "G_itemH_00"},
    {  "nunBtn_03_itemArrow.brlan",   "G_itemArrow_00"},
    {     "nunBtn_03_decide.brlan",      "G_decide_00"},
    {      "nunBtn_03_reset.brlan",       "G_inOut_00"},
    {      "nunBtn_03_input.brlan",       "G_input_00"},
    {      "nunBtn_03_input.brlan",       "G_input_01"},
    {       "nunBtn_03_type.brlan",        "G_type_00"},
    {       "nunBtn_03_call.brlan",        "G_call_00"},
    {       "nunBtn_03_text.brlan",        "G_text_02"},
    {       "nunBtn_03_loop.brlan",        "G_loop_00"},
};

#define DOWSING_ANIM_IN 0
#define DOWSING_ANIM_ITEM_ONOFF_OFFSET 1
#define DOWSING_ANIM_TEXT_ONOFF_OFFSET 9
#define DOWSING_ANIM_SELECT_ONOFF 11
#define DOWSING_ANIM_SELECT_LOOP 12
#define DOWSING_ANIM_HAVE_OFFSET 13
#define DOWSING_ANIM_ARROW 20
#define DOWSING_ANIM_DECIDE 21
#define DOWSING_ANIM_RESET 22
#define DOWSING_ANIM_INPUT_0 23
#define DOWSING_ANIM_INPUT_1 24
#define DOWSING_ANIM_TYPE 25
#define DOWSING_ANIM_CALL 26
#define DOWSING_ANIM_TEXT 27
#define DOWSING_ANIM_LOOP 28

#define DOWSING_NUM_ANIMS 29

#define DOWSING_NUM_ITEMS 8

static const char *sTextboxNames[] = {"T_itemName_00", "T_itemNameS_00", "T_dawsing_00", "T_dawsingS_00"};

static const char *sWindowNames[] = {
    "W_bgP_01",
    "W_bgP_02",
    "W_bgP_00",
};

static const char *sGroupName = "G_ref_00";

void dLytMeterDowsing_c::initializeState_Invisible() {}
void dLytMeterDowsing_c::executeState_Invisible() {
    if (getPane()->IsVisible()) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMeterDowsing_c::finalizeState_Invisible() {
    if (getSelectedDowsingSlot() != DowsingTarget::SLOT_NONE &&
        LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52) {
        mAnm[DOWSING_ANIM_DECIDE].setToEnd();
        mAnm[DOWSING_ANIM_DECIDE].setAnimEnable(true);
        mLyt.calc();
        mAnm[DOWSING_ANIM_DECIDE].setAnimEnable(false);
    }
}

void dLytMeterDowsing_c::initializeState_Wait() {
    fn_800FE220();
}
void dLytMeterDowsing_c::executeState_Wait() {
    if (field_0x5507 != 0) {
        fn_800FE220();
        field_0x5507 = 0;
    }

    if (!getPane()->IsVisible()) {
        mStateMgr.changeState(StateID_Invisible);
    } else if (field_0x54F4 == 1) {
        mStateMgr.changeState(StateID_DemoMove);
    } else if (field_0x5505 != 0) {
        mStateMgr.changeState(StateID_MenuSelectingIn);
    } else if (field_0x550A != 0 && (!dLytMeter_c::GetMain()->fn_800D5380(0) || dLytMeter_c::GetMain()->fn_800D5650() ||
                                     dLytMeter_c::GetMain()->fn_800D5680())) {
        mStateMgr.changeState(StateID_Reset);
    } else if (!dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_C)) {
        mStateMgr.changeState(StateID_ToUnuse);
    } else if (dLytMeter_c::GetInstance()->getMeterField_0x1377F() != 0 && field_0x550A == 0 &&
               dLytMeter_c::GetMain()->fn_800D5380(0) && !dLytMeter_c::GetMain()->fn_800D5650() &&
               !dLytMeter_c::GetMain()->fn_800D5680() && getSelectedDowsingSlot() != DowsingTarget::SLOT_LOOK &&
               getSelectedDowsingSlot() != DowsingTarget::SLOT_NONE &&
               LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52) {
        mAnm[DOWSING_ANIM_IN].setBackwardsOnce();
        mAnm[DOWSING_ANIM_IN].setToEnd2();
        mAnm[DOWSING_ANIM_IN].setAnimEnable(true);
        mAnm[DOWSING_ANIM_DECIDE].setToEnd();
        mAnm[DOWSING_ANIM_DECIDE].setAnimEnable(true);
        mItemIcons[0].setItem(convertToLytIndex(getSelectedDowsingSlot()));
        mLyt.calc();
        mAnm[DOWSING_ANIM_IN].setAnimEnable(false);
        mAnm[DOWSING_ANIM_DECIDE].setAnimEnable(false);
        field_0x550A = 1;
    }
}
void dLytMeterDowsing_c::finalizeState_Wait() {}

void dLytMeterDowsing_c::initializeState_MenuSelectingIn() {
    mAnm[DOWSING_ANIM_DECIDE].setAnimEnable(true);
    mAnm[DOWSING_ANIM_DECIDE].setFrame(0.0f);

    for (int i = 0; i < DOWSING_NUM_ITEMS; i++) {
        if (i != 4) {
            // The bottom slot is always enabled?
            mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setAnimEnable(true);
        }

        DowsingTarget::DowsingSlot slot = dowsingSlotForIndex(i);
        if (slot != DowsingTarget::SLOT_NONE && DowsingTarget::hasDowsingInSlot(slot)) {
            mpPanes[i + DOWSING_PANE_ICON_OFFSET]->SetVisible(true);
            if (i != 4) {
                mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setFrame(0.0f);
            }
            mItemIcons[i + 1].setItem(getLytIndexForDowsingIndex(i));
        } else {
            mpPanes[i + DOWSING_PANE_ICON_OFFSET]->SetVisible(false);
            if (i != 4) {
                mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setFrame(1.0f);
            }
        }

        mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET].setForwardOnce();
        mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(true);
        mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET].setFrame(0.0f);

        mIcon[i].reset();
    }
    mAnm[DOWSING_ANIM_IN].setForwardOnce();
    mAnm[DOWSING_ANIM_IN].setFrame(0.0f);
    mAnm[DOWSING_ANIM_IN].setAnimEnable(true);
    if (StoryflagManager::sInstance->getCounterOrFlag(583)) {
        mAnm[DOWSING_ANIM_TYPE].setFrame(1.0f);
    } else {
        mAnm[DOWSING_ANIM_TYPE].setFrame(0.0f);
    }

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[DOWSING_ANIM_DECIDE].setAnimEnable(false);

    for (int i = 0; i < DOWSING_NUM_ITEMS; i++) {
        if (i != 4) {
            mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setAnimEnable(false);
        }
        mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(false);
    }
}
void dLytMeterDowsing_c::executeState_MenuSelectingIn() {
    if (mAnm[DOWSING_ANIM_IN].isEndReached()) {
        mStateMgr.changeState(StateID_MenuSelecting);
    }

    if (mAnm[DOWSING_ANIM_IN].isEnabled()) {
        mAnm[DOWSING_ANIM_IN].play();
    }
}
void dLytMeterDowsing_c::finalizeState_MenuSelectingIn() {
    mAnm[DOWSING_ANIM_IN].setAnimEnable(false);
}

void dLytMeterDowsing_c::initializeState_MenuSelecting() {}
void dLytMeterDowsing_c::executeState_MenuSelecting() {
    if (field_0x5505 == 0) {
        mStateMgr.changeState(StateID_MenuSelectingOut);
    }
}
void dLytMeterDowsing_c::finalizeState_MenuSelecting() {}

void dLytMeterDowsing_c::initializeState_MenuSelectingOut() {
    fn_800FE220();
    mAnm[DOWSING_ANIM_IN].setBackwardsOnce();
    mAnm[DOWSING_ANIM_IN].setToStart();
    mAnm[DOWSING_ANIM_IN].setAnimEnable(true);
    field_0x550A = 0;
    if (getSelectedDowsingSlot() != DowsingTarget::SLOT_NONE &&
        LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52) {
        if (getSelectedDowsingSlot() != DowsingTarget::SLOT_LOOK) {
            field_0x550A = 1;
        }
        mAnm[DOWSING_ANIM_DECIDE].setAnimEnable(true);
    }
}
void dLytMeterDowsing_c::executeState_MenuSelectingOut() {
    if (mAnm[DOWSING_ANIM_IN].isStop2()) {
        mAnm[DOWSING_ANIM_IN].setAnimEnable(false);
        if (mAnm[DOWSING_ANIM_DECIDE].isEnabled()) {
            if (mAnm[DOWSING_ANIM_DECIDE].isEndReached()) {
                mAnm[DOWSING_ANIM_DECIDE].setAnimEnable(false);
                mStateMgr.changeState(StateID_Wait);
            }
        } else {
            mStateMgr.changeState(StateID_Wait);
        }
    }

    if (mAnm[DOWSING_ANIM_IN].isEnabled()) {
        mAnm[DOWSING_ANIM_IN].play();
    }

    if (mAnm[DOWSING_ANIM_DECIDE].isEnabled()) {
        mAnm[DOWSING_ANIM_DECIDE].play();
    }
}
void dLytMeterDowsing_c::finalizeState_MenuSelectingOut() {}

void dLytMeterDowsing_c::initializeState_Reset() {
    mAnm[DOWSING_ANIM_RESET].setForwardOnce();
    mAnm[DOWSING_ANIM_RESET].setToStart();
    mAnm[DOWSING_ANIM_RESET].setAnimEnable(true);
}
void dLytMeterDowsing_c::executeState_Reset() {
    if (mAnm[DOWSING_ANIM_RESET].isStop2()) {
        field_0x550A = 0;
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnm[DOWSING_ANIM_RESET].isEnabled()) {
        mAnm[DOWSING_ANIM_RESET].play();
    }
}
void dLytMeterDowsing_c::finalizeState_Reset() {}

void dLytMeterDowsing_c::initializeState_ToUse() {
    mAnm[DOWSING_ANIM_INPUT_0].setBackwardsOnce();
    mAnm[DOWSING_ANIM_INPUT_0].setToStart();
    mAnm[DOWSING_ANIM_INPUT_0].setAnimEnable(true);

    if (FileManager::GetInstance()->getDowsingSlotIdx() != 8 &&
        LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52 && dLytMeter_c::GetMain()->fn_800D5380(0) &&
        !dLytMeter_c::GetMain()->fn_800D5650() && !dLytMeter_c::GetMain()->fn_800D5680()) {
        mAnm[DOWSING_ANIM_RESET].setAnimEnable(true);
        mAnm[DOWSING_ANIM_RESET].setBackwardsOnce();
        mAnm[DOWSING_ANIM_RESET].setToStart();
    }
}
void dLytMeterDowsing_c::executeState_ToUse() {
    if (FileManager::GetInstance()->getDowsingSlotIdx() != 8 &&
        LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52 && dLytMeter_c::GetMain()->fn_800D5380(0) &&
        !dLytMeter_c::GetMain()->fn_800D5650() && !dLytMeter_c::GetMain()->fn_800D5680()) {
        if (mAnm[DOWSING_ANIM_INPUT_0].isStop2()) {
            if (!mAnm[DOWSING_ANIM_RESET].isEnabled() || mAnm[DOWSING_ANIM_RESET].isStop2()) {
                mStateMgr.changeState(StateID_Wait);
            }
        }
    } else if (mAnm[DOWSING_ANIM_INPUT_0].isStop2()) {
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnm[DOWSING_ANIM_INPUT_0].isEnabled()) {
        mAnm[DOWSING_ANIM_INPUT_0].play();
    }

    if (mAnm[DOWSING_ANIM_RESET].isEnabled()) {
        mAnm[DOWSING_ANIM_RESET].play();
    }
}
void dLytMeterDowsing_c::finalizeState_ToUse() {
    mAnm[DOWSING_ANIM_RESET].setAnimEnable(false);
    mAnm[DOWSING_ANIM_INPUT_0].setAnimEnable(false);
}

void dLytMeterDowsing_c::initializeState_ToUnuse() {
    mAnm[DOWSING_ANIM_INPUT_0].setForwardOnce();
    if (dLytMeter_c::GetMain()->fn_800D5650()) {
        mAnm[DOWSING_ANIM_INPUT_0].setToEnd2();
    } else {
        mAnm[DOWSING_ANIM_INPUT_0].setToStart();
    }
    mAnm[DOWSING_ANIM_INPUT_0].setAnimEnable(true);

    if (FileManager::GetInstance()->getDowsingSlotIdx() != 8 &&
        LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52 && dLytMeter_c::GetMain()->fn_800D5380(0) &&
        !dLytMeter_c::GetMain()->fn_800D5650() && !dLytMeter_c::GetMain()->fn_800D5680()) {
        mAnm[DOWSING_ANIM_RESET].setAnimEnable(true);
        mAnm[DOWSING_ANIM_RESET].setForwardOnce();
        mAnm[DOWSING_ANIM_RESET].setToStart();
    }
}
void dLytMeterDowsing_c::executeState_ToUnuse() {
    if (mAnm[DOWSING_ANIM_INPUT_0].isStop2()) {
        if (!mAnm[DOWSING_ANIM_RESET].isEnabled() || mAnm[DOWSING_ANIM_RESET].isStop2()) {
            mStateMgr.changeState(StateID_Unuse);
        }
    }

    if (mAnm[DOWSING_ANIM_INPUT_0].isEnabled()) {
        mAnm[DOWSING_ANIM_INPUT_0].play();
    }

    if (mAnm[DOWSING_ANIM_RESET].isEnabled()) {
        mAnm[DOWSING_ANIM_RESET].play();
    }
}
void dLytMeterDowsing_c::finalizeState_ToUnuse() {
    mAnm[DOWSING_ANIM_RESET].setAnimEnable(false);
    mAnm[DOWSING_ANIM_INPUT_0].setAnimEnable(false);
}

void dLytMeterDowsing_c::initializeState_Unuse() {
    field_0x54D8 = 15;
}
void dLytMeterDowsing_c::executeState_Unuse() {
    if (field_0x54F4 == 1) {
        mAnm[DOWSING_ANIM_INPUT_0].setAnimEnable(true);
        mAnm[DOWSING_ANIM_INPUT_0].setFrame(0.0f);
        mLyt.calc();
        mAnm[DOWSING_ANIM_INPUT_0].setAnimEnable(false);
        mStateMgr.changeState(StateID_DemoMove);
    } else if (!dLytMeter_c::GetMain()->fn_800D5680() && dLytMeter_c::GetInstance()->getMeterField_0x13774() == 0 &&
               dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_C)) {
        if (field_0x54D8 <= 0) {
            mStateMgr.changeState(StateID_ToUse);
        } else {
            field_0x54D8--;
        }
    } else {
        field_0x54D8 = 15;
    }
}
void dLytMeterDowsing_c::finalizeState_Unuse() {}

void dLytMeterDowsing_c::initializeState_DemoMove() {
    field_0x550B = 1;
    mDemoFrame = 0;

    for (int i = 0; i < DOWSING_NUM_ITEMS; i++) {
        if (i != 4) {
            // The bottom slot is always enabled?
            mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setAnimEnable(true);
        }

        DowsingTarget::DowsingSlot slot = dowsingSlotForIndex(i);
        bool condition = slot != DowsingTarget::SLOT_NONE && DowsingTarget::hasDowsingInSlot(slot);
        if (i == fn_800FE9C0(field_0x54F8)) {
            condition = false;
        }
        if (condition) {
            mpPanes[i + DOWSING_PANE_ICON_OFFSET]->SetVisible(true);
            if (i != 4) {
                mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setFrame(0.0f);
            }
            mItemIcons[i + 1].setItem(getLytIndexForDowsingIndex(i));
        } else {
            mpPanes[i + DOWSING_PANE_ICON_OFFSET]->SetVisible(false);
            if (i != 4) {
                mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setFrame(1.0f);
            }
        }

        mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET].setForwardOnce();
        mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(true);
        mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET].setFrame(0.0f);

        mIcon[i].reset();
    }

    mAnm[DOWSING_ANIM_IN].setForwardOnce();
    mAnm[DOWSING_ANIM_IN].setToEnd2();
    mAnm[DOWSING_ANIM_IN].setAnimEnable(true);

    if (StoryflagManager::sInstance->getCounterOrFlag(583)) {
        mAnm[DOWSING_ANIM_TYPE].setFrame(1.0f);
    } else {
        mAnm[DOWSING_ANIM_TYPE].setFrame(0.0f);
    }

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[DOWSING_ANIM_IN].setAnimEnable(false);

    for (int i = 0; i < DOWSING_NUM_ITEMS; i++) {
        if (i != 4) {
            mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setAnimEnable(false);
        }
        mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(false);
    }
}

void dLytMeterDowsing_c::executeState_DemoMove() {
    mDemoFrame++;
    if (mDemoFrame == 15) {
        if (field_0x54F8 <= 21) {
            s32 i = fn_800FE9C0(field_0x54F8);
            s32 idx = i + DOWSING_PANE_ICON_OFFSET;
            mpPanes[idx]->SetVisible(true);
            mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setAnimEnable(true);
            mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setFrame(0.0f);
            mItemIcons[i + 1].setItem(getLytIndexForDowsingIndex(i));

            idx = i + DOWSING_PANE_ITEM_OFFSET;
            nw4r::math::MTX34 mtx = mpPanes[idx]->GetGlobalMtx();
            mVec3_c pos(mtx._03, mtx._13, 0.0f);
            dJEffManager_c::spawnUIEffect(PARTICLE_RESOURCE_ID_MAPPING_970_, pos, nullptr, nullptr, nullptr, nullptr);
            SmallSoundManager::GetInstance()->playSound(SE_S_DOWSING_ADD);
        }
    } else if (mDemoFrame == 16) {
        for (int i = 0; i < DOWSING_NUM_ITEMS; i++) {
            if (i != 4 && mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].isEnabled()) {
                mAnm[convertSlotIndexToAnm(i) + DOWSING_ANIM_HAVE_OFFSET].setAnimEnable(false);
            }
        }
    }

    if (mDemoFrame > 80) {
        mDemoFrame = 0;
        mStateMgr.changeState(StateID_DemoOut);
    }
}
void dLytMeterDowsing_c::finalizeState_DemoMove() {}

void dLytMeterDowsing_c::initializeState_DemoOut() {
    mAnm[DOWSING_ANIM_IN].setBackwardsOnce();
    mAnm[DOWSING_ANIM_IN].setToStart();
    mAnm[DOWSING_ANIM_IN].setAnimEnable(true);
}
void dLytMeterDowsing_c::executeState_DemoOut() {
    if (mAnm[DOWSING_ANIM_IN].isStop2()) {
        mAnm[DOWSING_ANIM_IN].setAnimEnable(false);
        field_0x54F4 = 4;
        mStateMgr.changeState(StateID_Wait);
    }
    if (mAnm[DOWSING_ANIM_IN].isEnabled()) {
        mAnm[DOWSING_ANIM_IN].play();
    }
}
void dLytMeterDowsing_c::finalizeState_DemoOut() {}

bool dLytMeterDowsing_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("nunBtn_03.brlyt", nullptr);

    for (int i = 0; i < DOWSING_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sPaneNames[i]);
    }

    for (int i = 0; i < DOWSING_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[DOWSING_ANIM_SELECT_LOOP].setAnimEnable(true);
    mAnm[DOWSING_ANIM_TYPE].setAnimEnable(true);

    for (int i = 0; i < DOWSING_NUM_ITEMS; i++) {
        mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET].setFrame(0.0f);
        mIcon[i].setAnm(&mAnm[i + DOWSING_ANIM_ITEM_ONOFF_OFFSET]);
        mIcon[i].init();
    }

    mBlink.setAnm(&mAnm[DOWSING_ANIM_SELECT_ONOFF]);
    mBlink.init();
    mInput.setAnm(&mAnm[DOWSING_ANIM_INPUT_1]);
    mInput.init();

    for (int i = 0; i < 2; i++) {
        mText[i].setAnm(&mAnm[i + DOWSING_ANIM_TEXT_ONOFF_OFFSET]);
        mText[i].init();
    }

    for (int i = 0; i < 4; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(sTextboxNames[i]);
    }

    for (int i = 0; i < 3; i++) {
        mpWindows[i] = mLyt.getWindow(sWindowNames[i]);
        mpSizeBoxes[i] = mLyt.getSizeBoxInWindow(sWindowNames[i]);
    }
    mpWindows[0]->UpdateSize(mpSizeBoxes[0], 32.0f);

    for (int i = 0; i < 9; i++) {
        mSubpanes[i].mpLytPane = &mItemIcons[i];
        mItemIcons[i].build(resAcc, dLytCommonIconItem_c::DOWSING);
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

    field_0x54E0 = 8;

    mArrowRotation = 0.0f;
    mArrowLength = 0.0f;

    mpOwnerPane = nullptr;

    field_0x54F4 = 0;
    field_0x54F8 = -1;
    mDemoFrame = 0;
    field_0x5500 = 0;
    field_0x54F0 = LytDoButtonRelated::DO_NONE;
    field_0x54E4 = LytDoButtonRelated::DO_NONE;
    field_0x54E8 = LytDoButtonRelated::DO_NONE;
    field_0x54D8 = 15;
    field_0x5505 = 0;
    field_0x54EC = 1;
    field_0x5504 = 0;
    field_0x5508 = 0;
    field_0x5509 = 0;
    field_0x550A = 0;

    field_0x5506 = getPane()->IsVisible();
    field_0x5507 = 0;
    field_0x550B = 0;

    mAnm[DOWSING_ANIM_IN].setForwardOnce();
    mAnm[DOWSING_ANIM_IN].setFrame(0.0f);
    mAnm[DOWSING_ANIM_IN].setAnimEnable(true);

    mAnm[DOWSING_ANIM_CALL].setToEnd();
    mAnm[DOWSING_ANIM_CALL].setAnimEnable(true);

    mAnm[DOWSING_ANIM_INPUT_1].setForwardOnce();
    mAnm[DOWSING_ANIM_INPUT_1].setToEnd2();
    mAnm[DOWSING_ANIM_INPUT_1].setAnimEnable(true);
    mAnm[DOWSING_ANIM_TEXT].setAnimEnable(true);

    setMessage(field_0x54E4);

    mAnm[DOWSING_ANIM_TEXT].setFrame(0.0f);

    mLyt.calc();

    mAnm[DOWSING_ANIM_IN].setAnimEnable(false);
    mAnm[DOWSING_ANIM_CALL].setAnimEnable(false);
    mAnm[DOWSING_ANIM_INPUT_1].setAnimEnable(false);
    mAnm[DOWSING_ANIM_LOOP].setAnimEnable(true);

    mStateMgr.changeState(StateID_Invisible);

    return true;
}

bool dLytMeterDowsing_c::remove() {
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

    for (int i = 0; i < DOWSING_NUM_ITEMS + 1; i++) {
        mSubpanes[i].mpLytPane->remove();
    }

    for (int i = 0; i < DOWSING_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    return true;
}

bool dLytMeterDowsing_c::execute() {
    if (dLytControlGame_c::getInstance()->isStateNormal()) {
        if (StoryflagManager::sInstance->getCounterOrFlag(566) &&
            LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) != LytDoButtonRelated::DO_NONE) {
            if (dLytMeter_c::GetMain()->fn_800D5380(0)) {
                field_0x5508 = 0;
                StoryflagManager::sInstance->unsetFlag(566);
                if (getSelectedDowsingSlot() != DowsingTarget::SLOT_NONE) {
                    if (LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52) {
                        StoryflagManager::sInstance->unsetFlag(818);
                    }
                }
            } else {
                field_0x5508 = 1;
            }
        } else if (StoryflagManager::sInstance->getCounterOrFlag(818) &&
                   LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52) {
            if (field_0x5505 != 0 || field_0x550A != 0) {
                field_0x5508 = 0;
                StoryflagManager::sInstance->unsetFlag(818);
            } else {
                field_0x5508 = 1;
            }
        }
    } else {
        field_0x5508 = 0;
    }

    bool newVisible = getPane()->IsVisible();
    if (field_0x5506 != newVisible) {
        field_0x5506 = newVisible;
        if (!newVisible) {
            field_0x5507 = 1;
        }
    }

    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() != StateID_DemoMove && *mStateMgr.getStateID() != StateID_DemoOut &&
            field_0x550B != 0) {
            mpPanes[DOWSING_PANE_INPUT_00]->SetVisible(true);
            mpPanes[DOWSING_PANE_INPUT_01]->SetVisible(true);
            if (mAnm[DOWSING_ANIM_INPUT_0].isEnabled()) {
                mAnm[DOWSING_ANIM_INPUT_0].setFrame(0.0f);
                mLyt.calc();
                mAnm[DOWSING_ANIM_INPUT_0].setAnimEnable(false);
            }
            field_0x550B = 0;
            mStateMgr.changeState(StateID_Reset);
        }

        if (*mStateMgr.getStateID() == StateID_MenuSelectingIn || *mStateMgr.getStateID() == StateID_MenuSelecting ||
            *mStateMgr.getStateID() == StateID_MenuSelectingOut) {
            mStateMgr.changeState(StateID_Reset);
        }

        if (*mStateMgr.getStateID() == StateID_Wait) {
            field_0x5507 = 1;
        }
        return true;
    }

    if (field_0x5506 == 0) {
        return true;
    }

    fn_800FE110();
    bool state = false;
    s32 action = dLytDobutton_c::getFn0x8010E5E0();
    if (action == 8 || action == 10 || action == 30) {
        LytDoButtonRelated::Action_e action = LytDoButtonRelated::convertDoButton(dLytDobutton_c::getFn0x8010E5D0());
        if (action >= 0) {
            state = true;
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_C, action);
        }
    }
    field_0x54E8 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C);
    field_0x54EC = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_C) &&
                   dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_C);
    if (*mStateMgr.getStateID() != StateID_ToUnuse &&
        (dLytMeter_c::GetMain()->getUiMode() == 0 || field_0x54E8 != 0x51)) {
        if (field_0x54E8 != field_0x54E4) {
            if (field_0x5500 != 0) {
                field_0x54E4 = field_0x54E8;
                setMessage(field_0x54E8);
            } else {
                field_0x5500 += 1;
            }
        } else {
            field_0x5500 = 0;
        }
    } else {
        field_0x5500 = 0;
    }

    mStateMgr.executeState();

    if (field_0x5504 != state) {
        if (state) {
            mAnm[DOWSING_ANIM_TEXT].setFrame(1.0f);
        } else {
            mAnm[DOWSING_ANIM_TEXT].setFrame(0.0f);
        }
        field_0x5504 = state;
    }

    if (field_0x5508 != 0) {
        if (field_0x5509 == 0) {
            mAnm[DOWSING_ANIM_CALL].setToEnd();
            mAnm[DOWSING_ANIM_CALL].setAnimEnable(true);
            field_0x5509 = 1;
        }
        if (mAnm[DOWSING_ANIM_CALL].isEnabled()) {
            mAnm[DOWSING_ANIM_CALL].play();
            if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_C) && mAnm[DOWSING_ANIM_CALL].getFrame() == 1.0f) {
                SmallSoundManager::GetInstance()->playSound(SE_S_C_BUTTON_BLINK);
            }
        }
    } else if (field_0x5509 == 1) {
        mAnm[DOWSING_ANIM_CALL].setToEnd();
    } else if (mAnm[DOWSING_ANIM_CALL].isEnabled()) {
        mAnm[DOWSING_ANIM_CALL].setAnimEnable(false);
    }

    if (field_0x5508 == 0) {
        field_0x5509 = field_0x5508;
    }
    field_0x5508 = 0;

    for (int i = 0; i < DOWSING_NUM_ITEMS; i++) {
        mIcon[i].execute();
    }

    if (mLyt.findPane("N_setBtn_00")->IsVisible()) {
        mBlink.execute();
        mInput.execute();

        if (mAnm[DOWSING_ANIM_INPUT_1].isEnabled()) {
            mLyt.getLayout()->Animate(0);
            mLyt.calc();
        }
    }

    for (int i = 0; i < 2; i++) {
        mText[i].execute();
    }

    // Rotate the arrow
    mVec3_c t1(0.0f, 0.0f, 0.0f);
    t1.z = mArrowRotation;
    mpPanes[DOWSING_PANE_ITEM_ARROW]->SetRotate(t1);
    // But rotate the button and the pointer back so that
    // they point up
    mVec3_c t2(0.0f, 0.0f, 0.0f);
    t2.z = -mArrowRotation;
    mpPanes[DOWSING_PANE_ARROW_HAND]->SetRotate(t2);
    mpPanes[DOWSING_PANE_N_BTN_0]->SetRotate(t2);

    f32 frame = mAnm[DOWSING_ANIM_ARROW].getAnimDuration();
    if (mArrowLength < 0.0f) {
        mArrowLength = 0.0f;
    }
    if (mArrowLength > 1.0f) {
        mArrowLength = 1.0f;
    }
    mAnm[DOWSING_ANIM_ARROW].setFrame(frame * mArrowLength);
    mAnm[DOWSING_ANIM_ARROW].setAnimEnable(true);

    if (mAnm[DOWSING_ANIM_SELECT_LOOP].isEnabled()) {
        mAnm[DOWSING_ANIM_SELECT_LOOP].play();
    }

    if (mAnm[DOWSING_ANIM_LOOP].isEnabled()) {
        mAnm[DOWSING_ANIM_LOOP].play();
    }

    for (int i = 0; i < DOWSING_NUM_ITEMS + 1; i++) {
        mSubpanes[i].mpLytPane->execute();
    }

    return true;
}

void dLytMeterDowsing_c::fn_800FE110() {
    bool anySelected = false;
    s32 selectedIdx = -1;
    for (int i = 0; i < DOWSING_NUM_ITEMS; i++) {
        DowsingTarget::DowsingSlot slot = dowsingSlotForIndex(i);
        if (field_0x54E0 == slot && field_0x54F4 == 0 && slot != DowsingTarget::SLOT_NONE &&
            DowsingTarget::hasDowsingInSlot(slot)) {
            mIcon[i].setShouldBeSelect(true);
            anySelected = true;
            selectedIdx = i;
        } else {
            mIcon[i].setShouldBeSelect(false);
        }
    }

    setItemName(selectedIdx);

    if (anySelected) {
        mBlink.setShouldBeSelect(true);
        mText[0].setShouldBeSelect(true);
    } else {
        mBlink.setShouldBeSelect(false);
        mText[0].setShouldBeSelect(false);
    }

    if (dLytMeter_c::GetInstance()->getMeterField_0x1377F() != 0 && field_0x550A == 0) {
        mInput.setShouldBeSelect(true);
    } else {
        mInput.setShouldBeSelect(false);
    }
}

void dLytMeterDowsing_c::fn_800FE220() {
    if (getSelectedDowsingSlot() != DowsingTarget::SLOT_NONE &&
        LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52 && dLytMeter_c::GetMain()->fn_800D5380(0) &&
        !dLytMeter_c::GetMain()->fn_800D5650() && !dLytMeter_c::GetMain()->fn_800D5680()) {
        mAnm[DOWSING_ANIM_RESET].setAnimEnable(true);
        mAnm[DOWSING_ANIM_RESET].setForwardOnce();
        mAnm[DOWSING_ANIM_RESET].setToStart();
        mItemIcons[0].setItem(convertToLytIndex(getSelectedDowsingSlot()));
        mLyt.calc();
        mAnm[DOWSING_ANIM_RESET].setAnimEnable(false);
        mLyt.findPane("N_bBtnText_00")->SetVisible(true);
        mLyt.findPane("N_bBtnText_00")->SetAlpha(255);
    } else {
        mAnm[DOWSING_ANIM_RESET].setAnimEnable(true);
        mAnm[DOWSING_ANIM_RESET].setForwardOnce();
        mAnm[DOWSING_ANIM_RESET].setToEnd2();
        mLyt.calc();
        mAnm[DOWSING_ANIM_RESET].setAnimEnable(false);
    }
}

bool dLytMeterDowsing_c::fn_800FE3C0(s32 arg) {
    if (field_0x54F4 == 0) {
        field_0x54F4 = 1;
        field_0x54F8 = arg;
        mDemoFrame = 0;
        mArrowRotation = 0.0f;
        mArrowLength = 0.0f;
        for (int i = 0; i < DOWSING_NUM_ITEMS; i++) {
            mIcon[i].init();
        }
        mpPanes[DOWSING_PANE_INPUT_00]->SetVisible(false);
        mpPanes[DOWSING_PANE_INPUT_01]->SetVisible(false);
    } else if (field_0x54F4 == 4) {
        field_0x54F4 = 0;
        return true;
    }
    return false;
}

bool dLytMeterDowsing_c::fn_800FE490() const {
    return field_0x54F4 != 0;
}

bool dLytMeterDowsing_c::fn_800FE4B0() const {
    if (field_0x5505 != 0 || field_0x54E0 < 8) {
        if (*mStateMgr.getStateID() == StateID_ToUse || *mStateMgr.getStateID() == StateID_Wait) {
            return true;
        }
    }

    return *mStateMgr.getStateID() == StateID_MenuSelectingIn || *mStateMgr.getStateID() == StateID_MenuSelecting ||
         *mStateMgr.getStateID() == StateID_MenuSelectingOut;
}

bool dLytMeterDowsing_c::fn_800FE610() {
    if ((!dLytMeter_c::GetMain()->getDowsingNotHiddenByAreaCaption() && !dLytAreaCaption_c::getVisible()) ||
        !getPane()->IsVisible() || dLytMeter_c::GetMain()->fn_800D5680()) {
        return false;
    }
    return true;
}

static const s32 lytIndexes[] = {
    LYT_CMN_DowsingInvalid, LYT_CMN_DowsingTreasure,         LYT_CMN_DowsingHeart,   LYT_CMN_DowsingGoddessCube,
    LYT_CMN_DowsingLook,    LYT_CMN_DowsingGratitudeCrystal, LYT_CMN_DowsingInvalid, LYT_CMN_DowsingRupee,
};

s32 dLytMeterDowsing_c::convertToLytIndex(s32 slot) const {
    if (slot == DowsingTarget::SLOT_STORY_EVENT) {
        return DowsingTarget::getTopDowsingIcon();
    } else if (slot == DowsingTarget::SLOT_QUEST) {
        return DowsingTarget::getLeftDowsingIcon();
    } else {
        for (s32 i = 0; i < DOWSING_NUM_ITEMS; i++) {
            if (slot == dowsingSlotForIndex(i)) {
                return lytIndexes[i];
            }
        }
        return LYT_CMN_DowsingInvalid;
    }
}

u8 dLytMeterDowsing_c::getSelectedDowsingSlot() const {
    if (dLytMeter_c::GetMain()->fn_800D5650()) {
        return DowsingTarget::SLOT_NONE;
    } else {
        return FileManager::GetInstance()->getDowsingSlotIdx();
    }
}

void dLytMeterDowsing_c::setItemName(s32 idx) {
    if (idx >= 0 && field_0x54F4 == 0) {
        mText[1].setShouldBeSelect(true);
        SizedString<32> label;
        label.sprintf("NAME_DOWSING_%03d_R", getDowsingItemIdForIndex(idx));
        mpTextBoxes[0]->setMessageWithGlobalTextProcessor2(label, nullptr);
        mpTextBoxes[1]->setMessageWithGlobalTextProcessor2(label, nullptr);
        mpWindows[1]->UpdateSize(mpSizeBoxes[1], 32.0f);
    } else {
        mText[1].setShouldBeSelect(false);
    }
}

#include "d/lyt/meter/d_lyt_meter_action_table.inc"

void dLytMeterDowsing_c::setMessage(s32 id) {
    char buf[11];
    for (int i = 0; i < 11; i++) {
        buf[i] = 0;
    }

    if (id < LytDoButtonRelated::DO_NONE) {
        if (id < 0x5D) {
            sprintf(buf, "ACT_INFO_%03d", sActIds[id]);
        } else {
            s32 id2 = id - 0x5E;
            sprintf(buf, "ACT_ETC_%03d", sActIds[id2]);
        }

        for (int i = 2; i <= 3; i++) {
            mpTextBoxes[i]->SetVisible(true);
            mpTextBoxes[i]->setMessageWithGlobalTextProcessor2(buf, nullptr);
        }
        mpWindows[2]->SetVisible(true);
        mpWindows[2]->UpdateSize(mpSizeBoxes[2], 32.0f);
        field_0x54F0 = id;
    } else {
        mpTextBoxes[2]->SetVisible(false);
        mpTextBoxes[3]->SetVisible(false);
        mpWindows[2]->SetVisible(false);
    }
}

s32 dLytMeterDowsing_c::convertSlotIndexToAnm(s32 idx) const {
    if (idx > 4) {
        return idx - 1;
    } else {
        return idx;
    }
}

static const s32 unkTable[] = {0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 6, 6, 6, 6, 1, 5, 7, 3};

s32 dLytMeterDowsing_c::fn_800FE9C0(s32 unkId) const {
    return unkTable[unkId];
}

bool dLytMeterDowsing_c::shouldCall() const {
    if (dLytControlGame_c::getInstance()->isStateNormal() &&
        ((StoryflagManager::sInstance->getCounterOrFlag(566) &&
          LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) != LytDoButtonRelated::DO_NONE) ||
         (StoryflagManager::sInstance->getCounterOrFlag(818) &&
          LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == 0x52))) {
        return true;
    }

    return false;
}
