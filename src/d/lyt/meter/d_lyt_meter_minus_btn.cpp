#include "d/lyt/meter/d_lyt_meter_minus_btn.h"

#include "common.h"
#include "d/d_pouch.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_util_items.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "m/m_angle.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/math/math_types.h"
#include "sized_string.h"
#include "toBeSorted/file_manager.h"

STATE_DEFINE(dLytMeterMinusBtnMenuIcon_c, Wait);
STATE_DEFINE(dLytMeterMinusBtnMenuIcon_c, On);
STATE_DEFINE(dLytMeterMinusBtnMenuIcon_c, Select);
STATE_DEFINE(dLytMeterMinusBtnMenuIcon_c, Off);

STATE_DEFINE(dLytMeterMinusBtnBlink_c, Wait);
STATE_DEFINE(dLytMeterMinusBtnBlink_c, On);
STATE_DEFINE(dLytMeterMinusBtnBlink_c, Select);
STATE_DEFINE(dLytMeterMinusBtnBlink_c, Off);

STATE_DEFINE(dLytMeterMinusBtnText_c, Wait);
STATE_DEFINE(dLytMeterMinusBtnText_c, On);
STATE_DEFINE(dLytMeterMinusBtnText_c, Select);
STATE_DEFINE(dLytMeterMinusBtnText_c, Off);

STATE_DEFINE(dLytMeterMinusBtn_c, Wait);
STATE_DEFINE(dLytMeterMinusBtn_c, MenuSelectingIn);
STATE_DEFINE(dLytMeterMinusBtn_c, MenuSelecting);
STATE_DEFINE(dLytMeterMinusBtn_c, MenuSelectingOut);
STATE_DEFINE(dLytMeterMinusBtn_c, ToUse);
STATE_DEFINE(dLytMeterMinusBtn_c, ToUnuse);
STATE_DEFINE(dLytMeterMinusBtn_c, Unuse);
STATE_DEFINE(dLytMeterMinusBtn_c, DemoMove);
STATE_DEFINE(dLytMeterMinusBtn_c, DemoOut);

static u32 demoRotIncrement = 0x199A;

void dLytMeterMinusBtnMenuIcon_c::initializeState_Wait() {}
void dLytMeterMinusBtnMenuIcon_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterMinusBtnMenuIcon_c::finalizeState_Wait() {}

void dLytMeterMinusBtnMenuIcon_c::initializeState_On() {}
void dLytMeterMinusBtnMenuIcon_c::executeState_On() {
    if (mShouldBeSelect == 0) {
        mpAnm->setToEnd();
    }

    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterMinusBtnMenuIcon_c::finalizeState_On() {}

void dLytMeterMinusBtnMenuIcon_c::initializeState_Select() {}
void dLytMeterMinusBtnMenuIcon_c::executeState_Select() {
    mpAnm->setBackwardsOnce();
    mpAnm->setToStart();
    mpAnm->setAnimEnable(true);
    if (mShouldBeSelect == 0) {
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterMinusBtnMenuIcon_c::finalizeState_Select() {}

void dLytMeterMinusBtnMenuIcon_c::initializeState_Off() {}
void dLytMeterMinusBtnMenuIcon_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterMinusBtnMenuIcon_c::finalizeState_Off() {}

void dLytMeterMinusBtnMenuIcon_c::init() {
    mShouldBeSelect = 0;
    mpAnm->setFrame(0.0f);
    mStateMgr.changeState(StateID_Wait);
}

void dLytMeterMinusBtnMenuIcon_c::execute() {
    mStateMgr.executeState();
}

void dLytMeterMinusBtnBlink_c::initializeState_Wait() {}
void dLytMeterMinusBtnBlink_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterMinusBtnBlink_c::finalizeState_Wait() {}

void dLytMeterMinusBtnBlink_c::initializeState_On() {}
void dLytMeterMinusBtnBlink_c::executeState_On() {
    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterMinusBtnBlink_c::finalizeState_On() {}

void dLytMeterMinusBtnBlink_c::initializeState_Select() {}
void dLytMeterMinusBtnBlink_c::executeState_Select() {
    if (mShouldBeSelect == 0) {
        mpAnm->setBackwardsOnce();
        mpAnm->setToStart();
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterMinusBtnBlink_c::finalizeState_Select() {}

void dLytMeterMinusBtnBlink_c::initializeState_Off() {}
void dLytMeterMinusBtnBlink_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterMinusBtnBlink_c::finalizeState_Off() {}

void dLytMeterMinusBtnBlink_c::init() {
    mStateMgr.changeState(StateID_Wait);
    mShouldBeSelect = 0;
}

void dLytMeterMinusBtnBlink_c::execute() {
    mStateMgr.executeState();
}

void dLytMeterMinusBtnText_c::initializeState_Wait() {}
void dLytMeterMinusBtnText_c::executeState_Wait() {
    if (mShouldBeSelect != 0) {
        mpAnm->setForwardOnce();
        mpAnm->setFrame(0.0f);
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_On);
    }
}
void dLytMeterMinusBtnText_c::finalizeState_Wait() {}

void dLytMeterMinusBtnText_c::initializeState_On() {}
void dLytMeterMinusBtnText_c::executeState_On() {
    if (mpAnm->isEndReached()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Select);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterMinusBtnText_c::finalizeState_On() {}

void dLytMeterMinusBtnText_c::initializeState_Select() {}
void dLytMeterMinusBtnText_c::executeState_Select() {
    if (mShouldBeSelect == 0) {
        mpAnm->setBackwardsOnce();
        mpAnm->setToStart();
        mpAnm->setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterMinusBtnText_c::finalizeState_Select() {}

void dLytMeterMinusBtnText_c::initializeState_Off() {}
void dLytMeterMinusBtnText_c::executeState_Off() {
    if (mpAnm->isStop2()) {
        mpAnm->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }

    if (mpAnm->isEnabled()) {
        mpAnm->play();
    }
}
void dLytMeterMinusBtnText_c::finalizeState_Off() {}

void dLytMeterMinusBtnText_c::init() {
    mStateMgr.changeState(StateID_Wait);
    mShouldBeSelect = 0;
}

void dLytMeterMinusBtnText_c::execute() {
    mStateMgr.executeState();
}

static const char *sPaneNames[] = {
    "N_itemArrow_00", "N_arrowHand_00", "N_bBtn_00",  "N_A_00",     "N_B_00",     "N_C_00",     "N_D_00",
    "N_E_00",         "N_F_00",         "N_G_00",     "N_H_00",     "N_iconA_00", "N_iconB_00", "N_iconC_00",
    "N_iconD_00",     "N_iconE_00",     "N_iconF_00", "N_iconG_00", "N_iconH_00", "N_textA_00", "N_textB_00",
    "N_textC_00",     "N_textD_00",     "N_textE_00", "N_textF_00", "N_textG_06", "N_textH_07", "N_UIMode_00",
};

#define MINUS_BTN_PANE_ITEM_ARROW 0
#define MINUS_BTN_PANE_ARROW_HAND 1
#define MINUS_BTN_PANE_N_BTN_0 2
#define MINUS_BTN_PANE_ITEM_OFFSET 3
#define MINUS_BTN_PANE_ICON_OFFSET 11
#define MINUS_BTN_PANE_TEXT_OFFSET 19
#define MINUS_BTN_PANE_UIMODE 27

#define MINUS_BTN_NUM_PANES 28

static const d2d::LytBrlanMapping brlanMap[] = {
    {         "remoConBtn_01_in.brlan",       "G_inOut_00"},
    {      "remoConBtn_01_onOff.brlan",       "G_itemA_00"},
    {      "remoConBtn_01_onOff.brlan",       "G_itemB_00"},
    {      "remoConBtn_01_onOff.brlan",       "G_itemC_00"},
    {      "remoConBtn_01_onOff.brlan",       "G_itemD_00"},
    {      "remoConBtn_01_onOff.brlan",       "G_itemE_00"},
    {      "remoConBtn_01_onOff.brlan",       "G_itemF_00"},
    {      "remoConBtn_01_onOff.brlan",       "G_itemG_00"},
    {      "remoConBtn_01_onOff.brlan",       "G_itemH_00"},
    {      "remoConBtn_01_onOff.brlan",        "G_text_00"},
    {      "remoConBtn_01_onOff.brlan",        "G_text_01"},
    {"remoConBtn_01_selectOnOff.brlan", "G_selectOnOff_00"},
    { "remoConBtn_01_selectLoop.brlan",  "G_selectLoop_00"},
    {       "remoConBtn_01_have.brlan",       "G_itemA_00"},
    {       "remoConBtn_01_have.brlan",       "G_itemB_00"},
    {       "remoConBtn_01_have.brlan",       "G_itemC_00"},
    {       "remoConBtn_01_have.brlan",       "G_itemD_00"},
    {       "remoConBtn_01_have.brlan",       "G_itemE_00"},
    {       "remoConBtn_01_have.brlan",       "G_itemF_00"},
    {       "remoConBtn_01_have.brlan",       "G_itemG_00"},
    {       "remoConBtn_01_have.brlan",       "G_itemH_00"},
    {  "remoConBtn_01_itemArrow.brlan",   "G_itemArrow_00"},
    {      "remoConBtn_01_equip.brlan",      "G_equipA_00"},
    {      "remoConBtn_01_equip.brlan",      "G_equipB_00"},
    {      "remoConBtn_01_equip.brlan",      "G_equipC_00"},
    {      "remoConBtn_01_equip.brlan",      "G_equipD_00"},
    {      "remoConBtn_01_equip.brlan",      "G_equipE_00"},
    {      "remoConBtn_01_equip.brlan",      "G_equipF_00"},
    {      "remoConBtn_01_equip.brlan",      "G_equipG_00"},
    {      "remoConBtn_01_equip.brlan",      "G_equipH_00"},
    {     "remoConBtn_01_decide.brlan",      "G_decide_00"},
    {      "remoConBtn_01_input.brlan",       "G_input_00"},
    {       "remoConBtn_01_call.brlan",        "G_call_00"},
};

#define MINUS_BTN_ANIM_IN 0
#define MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET 1
#define MINUS_BTN_ANIM_TEXT_ONOFF_OFFSET 9
#define MINUS_BTN_ANIM_SELECT_ONOFF 11
#define MINUS_BTN_ANIM_SELECT_LOOP 12
#define MINUS_BTN_ANIM_HAVE_OFFSET 13
#define MINUS_BTN_ANIM_ARROW 21
#define MINUS_BTN_ANIM_EQUIP_OFFSET 22
#define MINUS_BTN_ANIM_DECIDE 30
#define MINUS_BTN_ANIM_INPUT 31
#define MINUS_BTN_ANIM_CALL 32

#define MINUS_BTN_NUM_ANIMS 33

#define MINUS_BTN_NUM_ITEMS 8

static const char *sTextboxNames[] = {"T_itemName_00", "T_itemNameS_00"};

static const char *sWindowNames[] = {
    "W_bgP_01",
    "W_bgP_09",
};

static const char *sGroupName = "G_ref_00";

void dLytMeterMinusBtn_c::initializeState_Wait() {}
void dLytMeterMinusBtn_c::executeState_Wait() {
    if (field_0x4E88 == 1) {
        mStateMgr.changeState(StateID_DemoMove);
    } else if (field_0x4EB8 != 0) {
        mStateMgr.changeState(StateID_MenuSelectingIn);
    } else if (!dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_MINUS)) {
        mStateMgr.changeState(StateID_ToUnuse);
    }
}
void dLytMeterMinusBtn_c::finalizeState_Wait() {}

void dLytMeterMinusBtn_c::initializeState_MenuSelectingIn() {
    bool locked = isPouchBocoburinLocked();
    mAnm[MINUS_BTN_ANIM_DECIDE].setAnimEnable(true);
    mAnm[MINUS_BTN_ANIM_DECIDE].setFrame(0.0f);

    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        mpPanes[i + MINUS_BTN_PANE_ICON_OFFSET]->SetVisible(true);
        mAnm[i + MINUS_BTN_ANIM_HAVE_OFFSET].setAnimEnable(true);
        mAnm[i + MINUS_BTN_ANIM_EQUIP_OFFSET].setAnimEnable(true);

        field_0x4EBB[i] = 0;
        field_0x4E98[i] = getPouchItemForSlot(i, true);
        if (hasPouchSlot(i, true)) {
            mAnm[i + MINUS_BTN_ANIM_HAVE_OFFSET].setFrame(0.0f);
            if (field_0x4E98[i] != 0x32) {
                s32 amount = getPouchItemAmount(i, true);
                if (locked) {
                    mItemIcons[i].setBocoburinLocked(true);
                } else {
                    mItemIcons[i].setBocoburinLocked(false);
                }

                if (amount >= 0 && !locked) {
                    mItemIcons[i].setHasNumber(true);
                    mItemIcons[i].setNumber(amount);
                    mItemIcons[i].setItem(field_0x4E98[i]);
                    mItemIcons[i].setNumberColor(getPouchItemNumberColor(i, true));
                } else {
                    mItemIcons[i].setHasNumber(false);
                    mItemIcons[i].setItem(field_0x4E98[i]);
                }

                if (getPouchShieldDurability(i, true) >= 0.0f) {
                    mItemIcons[i].setShieldDurability(getPouchShieldDurability(i, true));
                    if (i == convertFilePouchSlot(FileManager::GetInstance()->getShieldPouchSlot())) {
                        mItemIcons[i].setShieldOnOff(false);
                    } else {
                        mItemIcons[i].setShieldOnOff(true);
                        field_0x4EBB[i] = 1;
                    }
                } else {
                    mItemIcons[i].setShieldOnOff(false);
                }
            } else {
                mpPanes[i + MINUS_BTN_PANE_ICON_OFFSET]->SetVisible(false);
            }
        } else {
            mAnm[i + MINUS_BTN_ANIM_HAVE_OFFSET].setFrame(1.0f);
        }

        mAnm[i + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setForwardOnce();
        mAnm[i + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(true);
        mAnm[i + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setFrame(0.0f);
        mIcon[i].setShouldBeSelect(false);
    }

    mLyt.calc();
    mAnm[MINUS_BTN_ANIM_DECIDE].setAnimEnable(false);
    mAnm[MINUS_BTN_ANIM_IN].setForwardOnce();
    mAnm[MINUS_BTN_ANIM_IN].setFrame(0.0f);
    mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(true);

    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        mAnm[i + MINUS_BTN_ANIM_HAVE_OFFSET].setAnimEnable(false);
        mAnm[i + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(false);
    }
}
void dLytMeterMinusBtn_c::executeState_MenuSelectingIn() {
    realizeShieldDurability();
    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        mpPanes[i + MINUS_BTN_PANE_TEXT_OFFSET]->SetVisible(true);
        s32 item = getPouchItemForSlot(i, true);
        if (i == convertFilePouchSlot(FileManager::GetInstance()->getShieldPouchSlot())) {
            mAnm[i + MINUS_BTN_ANIM_EQUIP_OFFSET].setFrame(1.0f);
            mpPanes[i + MINUS_BTN_PANE_TEXT_OFFSET]->SetVisible(true);
        } else {
            if (isUnequippable(item)) {
                mAnm[i + MINUS_BTN_ANIM_EQUIP_OFFSET].setFrame(1.0f);
            } else {
                mAnm[i + MINUS_BTN_ANIM_EQUIP_OFFSET].setFrame(0.0f);
            }
            mpPanes[i + MINUS_BTN_PANE_TEXT_OFFSET]->SetVisible(false);
        }
    }

    if (mAnm[MINUS_BTN_ANIM_IN].isEndReached()) {
        for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
            mAnm[i + MINUS_BTN_ANIM_EQUIP_OFFSET].setAnimEnable(false);
        }
        mStateMgr.changeState(StateID_MenuSelecting);
    }
    if (mAnm[MINUS_BTN_ANIM_IN].isEnabled()) {
        mAnm[MINUS_BTN_ANIM_IN].play();
    }
}
void dLytMeterMinusBtn_c::finalizeState_MenuSelectingIn() {
    mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(false);
}

void dLytMeterMinusBtn_c::initializeState_MenuSelecting() {}
void dLytMeterMinusBtn_c::executeState_MenuSelecting() {
    realizeShieldDurability();
    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        s32 item = getPouchItemForSlot(i, true);
        // If our soup turned cold while the menu is open, fix the icon and text
        if (hasPouchSlot(i, true) && field_0x4E98[i] == LYT_CMN_PouchPumpkinSoup &&
            item == LYT_CMN_PouchPumpkinSoupCold) {
            field_0x4E98[i] = item;
            mItemIcons[i].setItem(item);
            mItemIcons[i].setNumberColor(getPouchItemNumberColor(i, true));
            u32 fileSlot = convertLytPouchSlot(i);
            setItemText(FileManager::GetInstance()->getPouchItem(fileSlot));
        }
    }

    if (field_0x4EB8 == 0) {
        mStateMgr.changeState(StateID_MenuSelectingOut);
    }
}
void dLytMeterMinusBtn_c::finalizeState_MenuSelecting() {}

void dLytMeterMinusBtn_c::initializeState_MenuSelectingOut() {
    mAnm[MINUS_BTN_ANIM_IN].setBackwardsOnce();
    mAnm[MINUS_BTN_ANIM_IN].setToStart();
    mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(true);
}
void dLytMeterMinusBtn_c::executeState_MenuSelectingOut() {
    realizeShieldDurability();
    if (mAnm[MINUS_BTN_ANIM_IN].isStop2()) {
        mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
    if (mAnm[MINUS_BTN_ANIM_IN].isEnabled()) {
        mAnm[MINUS_BTN_ANIM_IN].play();
    }
}
void dLytMeterMinusBtn_c::finalizeState_MenuSelectingOut() {}

void dLytMeterMinusBtn_c::initializeState_ToUse() {
    mAnm[MINUS_BTN_ANIM_INPUT].setBackwardsOnce();
    mAnm[MINUS_BTN_ANIM_INPUT].setToStart();
    mAnm[MINUS_BTN_ANIM_INPUT].setAnimEnable(true);
}
void dLytMeterMinusBtn_c::executeState_ToUse() {
    if (field_0x4EB8 != 0) {
        mAnm[MINUS_BTN_ANIM_INPUT].setBackwardsOnce();
        mAnm[MINUS_BTN_ANIM_INPUT].setToEnd2();
        mLyt.calc();
        mStateMgr.changeState(StateID_MenuSelectingIn);
    } else {
        if (mAnm[MINUS_BTN_ANIM_INPUT].isStop2()) {
            mStateMgr.changeState(StateID_Wait);
        }
        if (mAnm[MINUS_BTN_ANIM_INPUT].isEnabled()) {
            mAnm[MINUS_BTN_ANIM_INPUT].play();
        }
    }
}
void dLytMeterMinusBtn_c::finalizeState_ToUse() {
    mAnm[MINUS_BTN_ANIM_INPUT].setAnimEnable(false);
}

void dLytMeterMinusBtn_c::initializeState_ToUnuse() {
    mAnm[MINUS_BTN_ANIM_INPUT].setForwardOnce();
    mAnm[MINUS_BTN_ANIM_INPUT].setToStart();
    mAnm[MINUS_BTN_ANIM_INPUT].setAnimEnable(true);
}
void dLytMeterMinusBtn_c::executeState_ToUnuse() {
    if (field_0x4EB8 != 0) {
        mAnm[MINUS_BTN_ANIM_INPUT].setFrame(0.0f);
        mLyt.calc();
        mStateMgr.changeState(StateID_MenuSelectingIn);
    } else {
        if (mAnm[MINUS_BTN_ANIM_INPUT].isStop2()) {
            mStateMgr.changeState(StateID_Unuse);
        }
        if (mAnm[MINUS_BTN_ANIM_INPUT].isEnabled()) {
            mAnm[MINUS_BTN_ANIM_INPUT].play();
        }
    }
}
void dLytMeterMinusBtn_c::finalizeState_ToUnuse() {
    mAnm[MINUS_BTN_ANIM_INPUT].setAnimEnable(false);
}

void dLytMeterMinusBtn_c::initializeState_Unuse() {
    field_0x4E84 = 15;
}
void dLytMeterMinusBtn_c::executeState_Unuse() {
    if (field_0x4E88 == 1) {
        mAnm[MINUS_BTN_ANIM_INPUT].setAnimEnable(true);
        mAnm[MINUS_BTN_ANIM_INPUT].setFrame(0.0f);
        mLyt.calc();
        mAnm[MINUS_BTN_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_DemoMove);
    } else if (field_0x4EB8 != 0) {
        mAnm[MINUS_BTN_ANIM_INPUT].setAnimEnable(true);
        mAnm[MINUS_BTN_ANIM_INPUT].setFrame(0.0f);
        mLyt.calc();
        mAnm[MINUS_BTN_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_MenuSelectingIn);
    } else {
        if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_MINUS)) {
            if (field_0x4E84 <= 0) {
                mStateMgr.changeState(StateID_ToUse);
            } else {
                field_0x4E84--;
            }
        } else {
            field_0x4E84 = 15;
        }
    }
}
void dLytMeterMinusBtn_c::finalizeState_Unuse() {}

void dLytMeterMinusBtn_c::initializeState_DemoMove() {
    mDemoFrame = 0;
    mSlotToDemo = -1;
    field_0x4EC3 = 1;
    mpPanes[MINUS_BTN_PANE_UIMODE]->SetVisible(false);
    isPouchBocoburinLocked(); // @bug (?) return value ignored
    s32 count = getPouchSlotCount(true);

    u8 slotConfig[8];
    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        if (hasPouchSlot(i, true)) {
            if (field_0x4E8C == 0) {
                slotConfig[i] = 2;
            } else {
                slotConfig[i] = 1;
            }
        } else {
            slotConfig[i] = 0;
            if (field_0x4E8C == 1 && i == convertFilePouchSlot(count)) {
                mSlotToDemo = i;
                field_0x4E8C = 2;
                mDemoFrame = -15;
            }
        }
    }

    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        field_0x4E98[i] = getPouchItemForSlot(i, true);
        mAnm[i + MINUS_BTN_ANIM_HAVE_OFFSET].setAnimEnable(true);
        field_0x4EBB[i] = 0;
        if (slotConfig[i] != 0) {
            mAnm[i + MINUS_BTN_ANIM_HAVE_OFFSET].setFrame(0.0f);
            if (field_0x4E98[i] != 0x32) {
                mpPanes[i + MINUS_BTN_PANE_ICON_OFFSET]->SetVisible(true);
                s32 amount = getPouchItemAmount(i, true);
                bool locked = isPouchBocoburinLocked();
                if (locked) {
                    mItemIcons[i].setBocoburinLocked(true);
                } else {
                    mItemIcons[i].setBocoburinLocked(false);
                }

                if (amount >= 0 && !locked) {
                    mItemIcons[i].setHasNumber(true);
                    mItemIcons[i].setNumber(amount);
                    mItemIcons[i].setItem(field_0x4E98[i]);
                    mItemIcons[i].setNumberColor(getPouchItemNumberColor(i, true));
                } else {
                    mItemIcons[i].setHasNumber(false);
                    mItemIcons[i].setItem(field_0x4E98[i]);
                }

                if (getPouchShieldDurability(i, true) >= 0.0f) {
                    mItemIcons[i].setShieldDurability(getPouchShieldDurability(i, true));
                    if (i == convertFilePouchSlot(FileManager::GetInstance()->getShieldPouchSlot())) {
                        mItemIcons[i].setShieldOnOff(false);
                    } else {
                        mItemIcons[i].setShieldOnOff(true);
                        field_0x4EBB[i] = 1;
                    }
                } else {
                    mItemIcons[i].setShieldOnOff(false);
                }
            } else {
                mpPanes[i + MINUS_BTN_PANE_ICON_OFFSET]->SetVisible(false);
            }
        } else {
            mAnm[i + MINUS_BTN_ANIM_HAVE_OFFSET].setFrame(1.0f);
            mpPanes[i + MINUS_BTN_PANE_ICON_OFFSET]->SetVisible(true);
        }

        if (i == convertFilePouchSlot(FileManager::GetInstance()->getShieldPouchSlot())) {
            mAnm[i + MINUS_BTN_ANIM_EQUIP_OFFSET].setFrame(1.0f);
            mpPanes[i + MINUS_BTN_PANE_TEXT_OFFSET]->SetVisible(true);
        } else {
            if (isUnequippable(field_0x4E98[i])) {
                mAnm[i + MINUS_BTN_ANIM_EQUIP_OFFSET].setFrame(1.0f);
            } else {
                mAnm[i + MINUS_BTN_ANIM_EQUIP_OFFSET].setFrame(0.0f);
            }
            mpPanes[i + MINUS_BTN_PANE_TEXT_OFFSET]->SetVisible(false);
        }

        mAnm[i + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setForwardOnce();
        mAnm[i + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(true);
        mAnm[i + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setFrame(0.0f);
        mIcon[i].setShouldBeSelect(false);
    }

    mAnm[MINUS_BTN_ANIM_IN].setForwardOnce();
    mAnm[MINUS_BTN_ANIM_IN].setToEnd2();
    mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(false);

    int numPlayingEffects = 0;
    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        mAnm[i + MINUS_BTN_ANIM_HAVE_OFFSET].setAnimEnable(false);
        mAnm[i + MINUS_BTN_ANIM_EQUIP_OFFSET].setAnimEnable(false);
        mAnm[i + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(false);

        if (slotConfig[i] == 2 && numPlayingEffects < 4) {
            nw4r::math::MTX34 mtx = mpPanes[i + MINUS_BTN_PANE_ITEM_OFFSET]->GetGlobalMtx();
            mVec3_c pos(mtx._03, mtx._13, 0.0f);
            mSlotForEffect[numPlayingEffects] = i;
            mEffects[numPlayingEffects].startUIEffect(
                PARTICLE_RESOURCE_ID_MAPPING_971_, pos, &mEffectsRot[numPlayingEffects], nullptr, nullptr, nullptr
            );
            numPlayingEffects++;
        }
    }

    if (field_0x4E8C == 0) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_DOGU_PORCH_ADD_FIRST);
    } else {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_DOGU_PORCH_ADD);
    }
}
void dLytMeterMinusBtn_c::executeState_DemoMove() {
    mDemoFrame++;
    if (mDemoFrame == 0) {
        if (mSlotToDemo >= 0) {
            mAnm[mSlotToDemo + MINUS_BTN_ANIM_HAVE_OFFSET].setAnimEnable(true);
            mAnm[mSlotToDemo + MINUS_BTN_ANIM_HAVE_OFFSET].setFrame(0.0f);
            getPaneByIndex(mSlotToDemo + MINUS_BTN_PANE_ICON_OFFSET)->SetVisible(false);

            mAnm[mSlotToDemo + MINUS_BTN_ANIM_EQUIP_OFFSET].setAnimEnable(true);
            mAnm[mSlotToDemo + MINUS_BTN_ANIM_EQUIP_OFFSET].setFrame(0.0f);
            getPaneByIndex(mSlotToDemo + MINUS_BTN_PANE_TEXT_OFFSET)->SetVisible(false);

            mAnm[mSlotToDemo + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setForwardOnce();
            mAnm[mSlotToDemo + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(true);
            mAnm[mSlotToDemo + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setFrame(0.0f);

            mIcon[mSlotToDemo].setShouldBeSelect(false);
            mLyt.calc();

            mAnm[mSlotToDemo + MINUS_BTN_ANIM_HAVE_OFFSET].setAnimEnable(false);
            mAnm[mSlotToDemo + MINUS_BTN_ANIM_EQUIP_OFFSET].setAnimEnable(false);
            mAnm[mSlotToDemo + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET].setAnimEnable(false);

            nw4r::math::MTX34 mtx = getPaneByIndex(mSlotToDemo + MINUS_BTN_PANE_ITEM_OFFSET)->GetGlobalMtx();
            mVec3_c pos(mtx._03, mtx._13, 0.0f);
            mSlotForEffect[0] = mSlotToDemo;
            mEffects[0].startUIEffect(
                PARTICLE_RESOURCE_ID_MAPPING_971_, pos, &mEffectsRot[0], nullptr, nullptr, nullptr
            );
        }
    } else if (mDemoFrame > 0) {
        for (int i = 0; i < 4; i++) {
            s32 slot = mSlotForEffect[i];
            if (slot >= 0) {
                mEffectsRot[i].z += demoRotIncrement;
                nw4r::math::MTX34 mtx = getPaneByIndex(slot + MINUS_BTN_PANE_ITEM_OFFSET)->GetGlobalMtx();
                mVec3_c pos(mtx._03, mtx._13, 0.0f);
                mEffects[i].setPosRotScaleWithScreenScale(pos, &mEffectsRot[i], nullptr);
            }
        }
    }

    if (mDemoFrame >= 65) {
        mDemoFrame = 0;
        mSlotToDemo = -1;
        mStateMgr.changeState(StateID_DemoOut);
    }
}
void dLytMeterMinusBtn_c::finalizeState_DemoMove() {}

void dLytMeterMinusBtn_c::initializeState_DemoOut() {
    mAnm[MINUS_BTN_ANIM_IN].setBackwardsOnce();
    mAnm[MINUS_BTN_ANIM_IN].setToStart();
    mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(true);
}
void dLytMeterMinusBtn_c::executeState_DemoOut() {
    if (mAnm[MINUS_BTN_ANIM_IN].isStop2()) {
        mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(false);
        field_0x4E88 = 4;
        mStateMgr.changeState(StateID_Wait);
    }
    if (mAnm[MINUS_BTN_ANIM_IN].isEnabled()) {
        mAnm[MINUS_BTN_ANIM_IN].play();
    }
}
void dLytMeterMinusBtn_c::finalizeState_DemoOut() {}

bool dLytMeterMinusBtn_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_01.brlyt", nullptr);

    for (int i = 0; i < MINUS_BTN_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sPaneNames[i]);
    }

    for (int i = 0; i < MINUS_BTN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[MINUS_BTN_ANIM_SELECT_LOOP].setAnimEnable(true);

    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        mIcon[i].setAnm(&mAnm[i + MINUS_BTN_ANIM_ITEM_ONOFF_OFFSET]);
        mIcon[i].init();
    }

    mBlink.setAnm(&mAnm[MINUS_BTN_ANIM_SELECT_ONOFF]);
    mBlink.init();

    for (int i = 0; i < 2; i++) {
        mText[i].setAnm(&mAnm[i + MINUS_BTN_ANIM_TEXT_ONOFF_OFFSET]);
        mText[i].init();
    }

    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(sTextboxNames[i]);
    }

    for (int i = 0; i < 2; i++) {
        mpWindows[i] = mLyt.getWindow(sWindowNames[i]);
        mpSizeBoxes[i] = mLyt.getSizeBoxInWindow(sWindowNames[i]);
        mpWindows[i]->UpdateSize(mpSizeBoxes[i], 32.0f);
    }

    for (int i = 0; i < 8; i++) {
        mSubpanes[i].mpLytPane = &mItemIcons[i];
        mItemIcons[i].build(resAcc, dLytCommonIconItem_c::POUCH);
        mSubpaneList.PushBack(&mSubpanes[i]);
    }

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            linkMeters(g, &mSubpaneList);
        }
    }

    for (int i = 0; i < 8; i++) {
        mItemIcons[i].reset();
        mItemIcons[i].setVisible(false);
        mItemIcons[i].setSize(true);
        field_0x4EBB[i] = 0;
        field_0x4E98[i] = 0x32;
    }

    for (s32 i = 0; i < 4; i++) {
        mEffectsRot[i] = 0;
        mSlotForEffect[i] = -1;
        mEffects[i].init(dLytControlGame_c::getInstance());
    }

    field_0x4E80 = 8;
    mArrowRotation = 0.0f;
    mArrowLength = 0.0f;

    mpOwnerPane = nullptr;

    field_0x4E88 = 0;
    field_0x4E8C = -1;
    mDemoFrame = 0;
    mSlotToDemo = -1;

    field_0x4EB8 = 0;
    field_0x4EB9 = 0;
    field_0x4EBA = 0;

    field_0x4EC3 = 0;
    field_0x4E84 = 15;

    mAnm[MINUS_BTN_ANIM_IN].setForwardOnce();
    mAnm[MINUS_BTN_ANIM_IN].setFrame(0.0f);
    mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(true);
    mAnm[MINUS_BTN_ANIM_CALL].setToEnd();
    mAnm[MINUS_BTN_ANIM_CALL].setAnimEnable(true);

    mLyt.calc();

    mAnm[MINUS_BTN_ANIM_IN].setAnimEnable(false);
    mAnm[MINUS_BTN_ANIM_CALL].setAnimEnable(false);

    mStateMgr.changeState(StateID_Wait);

    return true;
}

bool dLytMeterMinusBtn_c::remove() {
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        mSubpanes[i].mpLytPane->remove();
    }

    for (int i = 0; i < MINUS_BTN_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    // Not removing our emitters I guess?
    return true;
}

bool dLytMeterMinusBtn_c::execute() {
    bool btnShown = true;
    if (dLytMeter_c::GetMain()->getUiMode() > 1 && !shouldCall() && !fn_800F75E0()) {
        btnShown = false;
    }

    if (field_0x4EC3 == 0) {
        if (!btnShown) {
            mpPanes[MINUS_BTN_PANE_UIMODE]->SetVisible(false);
        } else {
            mpPanes[MINUS_BTN_PANE_UIMODE]->SetVisible(true);
        }
    }

    if (dLytControlGame_c::getInstance()->isStateNormal() && mpPanes[MINUS_BTN_PANE_UIMODE]->IsVisible()) {
        if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_MINUS_BTN_NOTICE) != 0) {
            if (field_0x4EB8 != 0) {
                field_0x4EB9 = 0;
                StoryflagManager::sInstance->unsetFlag(STORYFLAG_MINUS_BTN_NOTICE);
            } else {
                field_0x4EB9 = 1;
            }
        }
    } else {
        field_0x4EB9 = 0;
    }

    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() != StateID_DemoMove && *mStateMgr.getStateID() != StateID_DemoOut &&
            field_0x4EC3 != 0) {
            if (btnShown) {
                mpPanes[MINUS_BTN_PANE_UIMODE]->SetVisible(true);
            }
            field_0x4EC3 = false;
        }

        return true;
    }

    if (!getPane()->IsVisible()) {
        if (*mStateMgr.getStateID() == StateID_ToUnuse) {
            mAnm[MINUS_BTN_ANIM_INPUT].setToEnd2();
        }
        return true;
    }

    if (!StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_POUCH_UNLOCKED)) {
        dLytMeter_c::GetInstance()->clearFlags(METER_BTN_MINUS);
    }

    fn_800F7300();

    mStateMgr.executeState();

    if (field_0x4EB9 != 0 && dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_MINUS)) {
        if (field_0x4EBA == 0) {
            mAnm[MINUS_BTN_ANIM_CALL].setToEnd();
            mAnm[MINUS_BTN_ANIM_CALL].setAnimEnable(true);
            field_0x4EBA = 1;
        }

        if (mAnm[MINUS_BTN_ANIM_CALL].isEnabled()) {
            mAnm[MINUS_BTN_ANIM_CALL].play();
            if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_MINUS) &&
                mAnm[MINUS_BTN_ANIM_CALL].getFrame() == 1.0f) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MINUS_BUTTON_BLINK);
            }
        }
    } else if (field_0x4EBA == 1) {
        mAnm[MINUS_BTN_ANIM_CALL].setToEnd();
    } else if (mAnm[MINUS_BTN_ANIM_CALL].isEnabled()) {
        mAnm[MINUS_BTN_ANIM_CALL].setAnimEnable(false);
    }

    if (!field_0x4EB9) {
        field_0x4EBA = field_0x4EB9;
    }

    field_0x4EB9 = 0;

    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        mIcon[i].execute();
    }

    mBlink.execute();

    for (int i = 0; i < 2; i++) {
        mText[i].execute();
    }

    mVec3_c t1(0.0f, 0.0f, 0.0f);
    t1.z = mArrowRotation;
    mpPanes[MINUS_BTN_PANE_ITEM_ARROW]->SetRotate(t1);
    // But rotate the button and the pointer back so that
    // they point up
    mVec3_c t2(0.0f, 0.0f, 0.0f);
    t2.z = -mArrowRotation;
    mpPanes[MINUS_BTN_PANE_ARROW_HAND]->SetRotate(t2);
    mpPanes[MINUS_BTN_PANE_N_BTN_0]->SetRotate(t2);

    f32 frame = mAnm[MINUS_BTN_ANIM_ARROW].getAnimDuration();
    if (mArrowLength < 0.0f) {
        mArrowLength = 0.0f;
    }
    if (mArrowLength > 1.0f) {
        mArrowLength = 1.0f;
    }
    mAnm[MINUS_BTN_ANIM_ARROW].setFrame(frame * mArrowLength);
    mAnm[MINUS_BTN_ANIM_ARROW].setAnimEnable(true);

    if (mAnm[MINUS_BTN_ANIM_SELECT_LOOP].isEnabled()) {
        mAnm[MINUS_BTN_ANIM_SELECT_LOOP].play();
    }

    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        mSubpanes[i].mpLytPane->execute();
    }

    return true;
}

bool dLytMeterMinusBtn_c::shouldCall() const {
    if (dLytControlGame_c::getInstance()->isStateNormal() &&
        StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_MINUS_BTN_NOTICE)) {
        return true;
    }
    return false;
}

void dLytMeterMinusBtn_c::fn_800F7300() {
    s32 itemId = -1;
    for (int i = 0; i < 8; i++) {
        int item = getPouchItemForSlot(i, true);
        if (field_0x4E80 == i && field_0x4E88 == 0 && item != 0x32) {
            mIcon[i].setShouldBeSelect(true);
            u32 slot = convertLytPouchSlot(i);
            if (slot < getPouchSlotCount(true)) {
                itemId = FileManager::GetInstance()->getPouchItem(slot);
                if (itemId == 0) {
                    itemId = -1;
                }
            }
        } else {
            mIcon[i].setShouldBeSelect(false);
        }
    }

    if (*mStateMgr.getStateID() == StateID_Wait || *mStateMgr.getStateID() == StateID_MenuSelectingIn ||
        *mStateMgr.getStateID() == StateID_ToUnuse || *mStateMgr.getStateID() == StateID_Unuse) {
        itemId = -1;
    }

    setItemText(itemId);

    if (itemId >= 0) {
        mBlink.setShouldBeSelect(true);
        if (isNotMedalOrExtraAmmo(itemId)) {
            mText[0].setShouldBeSelect(true);
        } else {
            mText[0].setShouldBeSelect(false);
        }
    } else {
        mBlink.setShouldBeSelect(false);
        mText[0].setShouldBeSelect(false);
    }
}

bool dLytMeterMinusBtn_c::demoRelated(s32 arg) {
    if (field_0x4E88 == 0) {
        field_0x4E88 = 1;
        field_0x4E8C = arg;
        mDemoFrame = 0;
        for (s32 i = 0; i < 4; i++) {
            mEffectsRot[i].set(0);
            mSlotForEffect[i] = -1;
        }
        mArrowRotation = 0.0f;
        mArrowLength = 0.0f;

        for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
            mIcon[i].init();
        }
    } else if (field_0x4E88 == 4) {
        field_0x4E88 = 0;
        return true;
    }
    return false;
}

bool dLytMeterMinusBtn_c::fn_800F75E0() const {
    return field_0x4E88 != 0;
}

bool dLytMeterMinusBtn_c::fn_800F7600() const {
    if (field_0x4EB8 != 0 || field_0x4E80 < 8) {
        if (*mStateMgr.getStateID() == StateID_ToUse || *mStateMgr.getStateID() == StateID_Wait) {
            return true;
        }
    }

    return *mStateMgr.getStateID() == StateID_MenuSelectingIn || *mStateMgr.getStateID() == StateID_MenuSelecting ||
         *mStateMgr.getStateID() == StateID_MenuSelectingOut;
}

bool dLytMeterMinusBtn_c::fn_800F7760() const {
    if (!StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_POUCH_UNLOCKED) ||
        (mpOwnerPane == nullptr || !mpOwnerPane->IsVisible() || dLytMeter_c::GetMain()->isInModeMap() ||
         dLytMeter_c::GetMain()->isInModePause() || !dLytMeter_c::GetMain()->getMinusBtnNotHiddenByAreaCaption())) {
        return false;
    }

    return true;
}

bool dLytMeterMinusBtn_c::isUnequippable(s32 item) const {
    switch (item) {
        case LYT_CMN_PouchSeedSatchelSmall:
        case LYT_CMN_PouchSeedSatchelMed:
        case LYT_CMN_PouchSeedSatchelBig:
        case LYT_CMN_PouchQuiverSmall:
        case LYT_CMN_PouchQuiverMed:
        case LYT_CMN_PouchQuiverBig:
        case LYT_CMN_PouchBombBagSmall:
        case LYT_CMN_PouchBombBagMed:
        case LYT_CMN_PouchBombBagBig:
        case LYT_CMN_PouchMedalDurable:
        case LYT_CMN_PouchMedalHeart:
        case LYT_CMN_PouchMedalLife:
        case LYT_CMN_PouchMedalRupee:
        case LYT_CMN_PouchMedalTreasure:
        case LYT_CMN_PouchMedalCursed:
        case LYT_CMN_Pouch32: // ?
        case LYT_CMN_PouchMedalBug:         return true;
        default:                            return false;
    }
}

void dLytMeterMinusBtn_c::setItemText(s32 item) {
    if (item >= 0 && field_0x4E88 == 0) {
        mText[1].setShouldBeSelect(true);
        SizedString<16> label;
        label.sprintf("NAME_ITEM_%03d", item);
        mpTextBoxes[0]->setMessageWithGlobalTextProcessor2(label, nullptr);
        mpTextBoxes[1]->setMessageWithGlobalTextProcessor2(label, nullptr);
        mpWindows[1]->UpdateSize(mpSizeBoxes[1], 32.0f);
    } else {
        mText[1].setShouldBeSelect(false);
    }
}

void dLytMeterMinusBtn_c::realizeShieldDurability() {
    for (int i = 0; i < MINUS_BTN_NUM_ITEMS; i++) {
        if (field_0x4EBB[i] != 0) {
            mItemIcons[i].setShieldDurability(getPouchShieldDurability(i, true));
        }
    }
}

bool dLytMeterMinusBtn_c::isNotMedalOrExtraAmmo(s32 item) {
    if (isItemExtraAmmo(item) || isItemMedal(item)) {
        return false;
    }
    return true;
}
