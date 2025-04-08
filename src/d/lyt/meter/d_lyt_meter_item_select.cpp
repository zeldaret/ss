#include "d/lyt/meter/d_lyt_meter_item_select.h"

#include "common.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_lyt_do_button.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/math/math_types.h"
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
void dLytMeterItemSelectIcon_c::executeState_Wait() {}
void dLytMeterItemSelectIcon_c::finalizeState_Wait() {}

void dLytMeterItemSelectIcon_c::initializeState_On() {}
void dLytMeterItemSelectIcon_c::executeState_On() {}
void dLytMeterItemSelectIcon_c::finalizeState_On() {}

void dLytMeterItemSelectIcon_c::initializeState_Select() {}
void dLytMeterItemSelectIcon_c::executeState_Select() {}
void dLytMeterItemSelectIcon_c::finalizeState_Select() {}

void dLytMeterItemSelectIcon_c::initializeState_Off() {}
void dLytMeterItemSelectIcon_c::executeState_Off() {}
void dLytMeterItemSelectIcon_c::finalizeState_Off() {}

void dLytMeterItemSelectBlink_c::initializeState_Wait() {}
void dLytMeterItemSelectBlink_c::executeState_Wait() {}
void dLytMeterItemSelectBlink_c::finalizeState_Wait() {}

void dLytMeterItemSelectBlink_c::initializeState_On() {}
void dLytMeterItemSelectBlink_c::executeState_On() {}
void dLytMeterItemSelectBlink_c::finalizeState_On() {}

void dLytMeterItemSelectBlink_c::initializeState_Select() {}
void dLytMeterItemSelectBlink_c::executeState_Select() {}
void dLytMeterItemSelectBlink_c::finalizeState_Select() {}

void dLytMeterItemSelectBlink_c::initializeState_Off() {}
void dLytMeterItemSelectBlink_c::executeState_Off() {}
void dLytMeterItemSelectBlink_c::finalizeState_Off() {}

void dLytMeterItemSelectText_c::initializeState_Wait() {}
void dLytMeterItemSelectText_c::executeState_Wait() {}
void dLytMeterItemSelectText_c::finalizeState_Wait() {}

void dLytMeterItemSelectText_c::initializeState_On() {}
void dLytMeterItemSelectText_c::executeState_On() {}
void dLytMeterItemSelectText_c::finalizeState_On() {}

void dLytMeterItemSelectText_c::initializeState_Select() {}
void dLytMeterItemSelectText_c::executeState_Select() {}
void dLytMeterItemSelectText_c::finalizeState_Select() {}

void dLytMeterItemSelectText_c::initializeState_Off() {}
void dLytMeterItemSelectText_c::executeState_Off() {}
void dLytMeterItemSelectText_c::finalizeState_Off() {}

void dLytMeterItemSelectBtn_c::initializeState_Invisible() {}
void dLytMeterItemSelectBtn_c::executeState_Invisible() {}
void dLytMeterItemSelectBtn_c::finalizeState_Invisible() {}

void dLytMeterItemSelectBtn_c::initializeState_On() {}
void dLytMeterItemSelectBtn_c::executeState_On() {}
void dLytMeterItemSelectBtn_c::finalizeState_On() {}

void dLytMeterItemSelectBtn_c::initializeState_Visible() {}
void dLytMeterItemSelectBtn_c::executeState_Visible() {}
void dLytMeterItemSelectBtn_c::finalizeState_Visible() {}

void dLytMeterItemSelectBtn_c::initializeState_Off() {}
void dLytMeterItemSelectBtn_c::executeState_Off() {}
void dLytMeterItemSelectBtn_c::finalizeState_Off() {}

static const char *sPaneNames[] = {
    "N_itemArrow_00", "N_arrowHand_00", "N_bBtn_00",  "N_itemSelect_00", "N_setBtn_00", "N_bBtn_01",
    "N_btnBg_00",     "N_btn_00",       "N_input_00", "N_iconA_00",      "N_iconB_00",  "N_iconC_00",
    "N_iconD_00",     "N_iconE_00",     "N_iconF_00", "N_iconG_00",      "N_iconH_00",
};

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
#define ITEM_SELECT_ANIM_ONOFF_OFFSET 1
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

static const char *groupName = "G_ref_00";

dLytMeterItemSelect_c::dLytMeterItemSelect_c() : mStateMgr(*this, sStateID::null) {
    field_0x574C = 11;
    field_0x5750 = 11;
    field_0x5754 = 0;
    field_0x5758 = -1;
    field_0x575C = 0;
    field_0x5760 = 0;
    field_0x5764 = -1;
    field_0x5768 = 2;
}

void dLytMeterItemSelect_c::initializeState_InitWait() {}
void dLytMeterItemSelect_c::executeState_InitWait() {}
void dLytMeterItemSelect_c::finalizeState_InitWait() {}

void dLytMeterItemSelect_c::initializeState_Wait() {}
void dLytMeterItemSelect_c::executeState_Wait() {}
void dLytMeterItemSelect_c::finalizeState_Wait() {}

void dLytMeterItemSelect_c::initializeState_SelectIn() {}
void dLytMeterItemSelect_c::executeState_SelectIn() {}
void dLytMeterItemSelect_c::finalizeState_SelectIn() {}

void dLytMeterItemSelect_c::initializeState_Select() {}
void dLytMeterItemSelect_c::executeState_Select() {}
void dLytMeterItemSelect_c::finalizeState_Select() {}

void dLytMeterItemSelect_c::initializeState_SetIn() {}
void dLytMeterItemSelect_c::executeState_SetIn() {}
void dLytMeterItemSelect_c::finalizeState_SetIn() {}

void dLytMeterItemSelect_c::initializeState_SetSpecialItemIn() {}
void dLytMeterItemSelect_c::executeState_SetSpecialItemIn() {}
void dLytMeterItemSelect_c::finalizeState_SetSpecialItemIn() {}

void dLytMeterItemSelect_c::initializeState_Set() {}
void dLytMeterItemSelect_c::executeState_Set() {}
void dLytMeterItemSelect_c::finalizeState_Set() {}

void dLytMeterItemSelect_c::initializeState_SetOut() {}
void dLytMeterItemSelect_c::executeState_SetOut() {}
void dLytMeterItemSelect_c::finalizeState_SetOut() {}

void dLytMeterItemSelect_c::initializeState_SetNone() {}
void dLytMeterItemSelect_c::executeState_SetNone() {}
void dLytMeterItemSelect_c::finalizeState_SetNone() {}

void dLytMeterItemSelect_c::initializeState_SelectOut() {}
void dLytMeterItemSelect_c::executeState_SelectOut() {}
void dLytMeterItemSelect_c::finalizeState_SelectOut() {}

void dLytMeterItemSelect_c::initializeState_ResetIn() {}
void dLytMeterItemSelect_c::executeState_ResetIn() {}
void dLytMeterItemSelect_c::finalizeState_ResetIn() {}

void dLytMeterItemSelect_c::initializeState_Reset() {}
void dLytMeterItemSelect_c::executeState_Reset() {}
void dLytMeterItemSelect_c::finalizeState_Reset() {}

void dLytMeterItemSelect_c::initializeState_ResetOut() {}
void dLytMeterItemSelect_c::executeState_ResetOut() {}
void dLytMeterItemSelect_c::finalizeState_ResetOut() {}

void dLytMeterItemSelect_c::initializeState_ToUse() {}
void dLytMeterItemSelect_c::executeState_ToUse() {}
void dLytMeterItemSelect_c::finalizeState_ToUse() {}

void dLytMeterItemSelect_c::initializeState_ToUnuse() {}
void dLytMeterItemSelect_c::executeState_ToUnuse() {}
void dLytMeterItemSelect_c::finalizeState_ToUnuse() {}

void dLytMeterItemSelect_c::initializeState_Unuse() {}
void dLytMeterItemSelect_c::executeState_Unuse() {}
void dLytMeterItemSelect_c::finalizeState_Unuse() {}

void dLytMeterItemSelect_c::initializeState_DemoMove() {}
void dLytMeterItemSelect_c::executeState_DemoMove() {}
void dLytMeterItemSelect_c::finalizeState_DemoMove() {}

void dLytMeterItemSelect_c::initializeState_DemoOut0() {}
void dLytMeterItemSelect_c::executeState_DemoOut0() {}
void dLytMeterItemSelect_c::finalizeState_DemoOut0() {}

void dLytMeterItemSelect_c::initializeState_DemoOut1() {}
void dLytMeterItemSelect_c::executeState_DemoOut1() {}
void dLytMeterItemSelect_c::finalizeState_DemoOut1() {}

void dLytMeterItemSelect_c::initializeState_FrameOffIn() {}
void dLytMeterItemSelect_c::executeState_FrameOffIn() {}
void dLytMeterItemSelect_c::finalizeState_FrameOffIn() {}

void dLytMeterItemSelect_c::initializeState_FrameOffMove() {}
void dLytMeterItemSelect_c::executeState_FrameOffMove() {}
void dLytMeterItemSelect_c::finalizeState_FrameOffMove() {}

void dLytMeterItemSelect_c::initializeState_FrameOffOut() {}
void dLytMeterItemSelect_c::executeState_FrameOffOut() {}
void dLytMeterItemSelect_c::finalizeState_FrameOffOut() {}

bool dLytMeterItemSelect_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_00.brlyt", nullptr);

    for (int i = 0; i < 17; i++) {
        mpPanes[i] = mLyt.findPane(sPaneNames[i]);
    }

    for (int i = 0; i < ITEM_SELECT_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < 8; i++) {
        mIcon[i].setAnm(&mAnm[i + ITEM_SELECT_ANIM_ONOFF_OFFSET]);
        mIcon[i].init();
    }

    mBlink.setAnm(&mAnm[ITEM_SELECT_ANIM_NUMBERV]);
    mBlink.init();

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

    if (mLyt.getLayout()->GetRootPane() != nullptr) {
        nw4r::lyt::Group *g = mLyt.findGroupByName("G_ref_00");
        linkMeters(g, &mSubpaneList);
    }

    for (int i = 0; i < 9; i++) {
        mItemIcons[i].reset();
        mItemIcons[i].setVisible(false);
        mItemIcons[i].setSize(true);
    }

    mStateMgr.changeState(StateID_InitWait);

    return true;
}

bool dLytMeterItemSelect_c::remove() {
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        if (!it->mpLytPane->LytMeter0x24()) {
            continue;
        }
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
        if (field_0x57A2 != 0 || (field_0x57A0 != 0 && field_0x5794 != 2 && field_0x5794 != 9 && field_0x5794 != 10)) {
            field_0x57B9 = 0;
            StoryflagManager::sInstance->unsetFlag(569);
        } else {
            field_0x57B9 = 1;
        }
    }

    field_0x57BF = field_0x57BE;
    if (mpSomePane == nullptr) {
        return true;
    }

    if (!mpSomePane->IsVisible()) {
        if (*mStateMgr.getStateID() != StateID_DemoMove && *mStateMgr.getStateID() != StateID_DemoOut0 &&
            *mStateMgr.getStateID() != StateID_DemoOut1 && field_0x57C5 != 0) {
            mpPanes[6]->SetVisible(true);
            mpPanes[7]->SetVisible(true);
            mpPanes[8]->SetVisible(true);
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

            if (field_0x5798 != 11) {
                if (field_0x5794 != 11) {
                    mAnm[ITEM_SELECT_ANIM_OUT].setForwardOnce();
                    mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(true);
                    mAnm[ITEM_SELECT_ANIM_OUT].setToEnd2();
                } else {
                    mAnm[ITEM_SELECT_ANIM_RESET].setForwardOnce();
                    mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(true);
                    mAnm[ITEM_SELECT_ANIM_RESET].setToEnd2();
                }
            } else if (field_0x5794 != 11) {
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

            mBtn[1].fn_800E9E00(1, 1);
            mBtn[0].fn_800E9E00(1, 1);

            field_0x57B3 = 0;

            if (field_0x5798 != 11) {
                if (field_0x5794 != 11) {
                    mBtn[2].fn_800E9E00(0, 1);
                } else {
                    mBtn[2].fn_800E9E00(1, 1);
                }
            } else if (field_0x5794 != 11) {
                mBtn[2].fn_800E9E00(0, 1);
            } else {
                mBtn[2].fn_800E9E00(1, 1);
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
                mpPanes[11]->SetAlpha(0xFF);
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
            mBtn[0].fn_800E9E30(1);
            mBtn[2].fn_800E9E30(1);

            mLyt.getLayout()->Animate(0);
            mLyt.calc();

            mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
            field_0x57B3 = 0;
            field_0x579A = 11;
            mStateMgr.changeState(StateID_Wait);
        }
        if (*mStateMgr.getStateID() == StateID_SetSpecialItemIn) {
            mBtn[0].fn_800E9E00(1, 1);
            if (mpSomePane->GetAlpha() != 0xFF) {
                if (mAnm[ITEM_SELECT_ANIM_OUT].isEnabled()) {
                    mAnm[ITEM_SELECT_ANIM_OUT].setAnimEnable(false);
                }

                if (mAnm[ITEM_SELECT_ANIM_RESET].isEnabled()) {
                    mAnm[ITEM_SELECT_ANIM_RESET].setToStart();
                }

                mpPanes[4]->SetVisible(true);

                mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(true);
                mAnm[ITEM_SELECT_ANIM_IN].setFrame(0.0f);

                mBtn[1].fn_800E9E00(1, 1);
                mBtn[2].fn_800E9E00(1, 1);
                mLyt.calc();

                mAnm[ITEM_SELECT_ANIM_IN].setAnimEnable(false);
                mAnm[ITEM_SELECT_ANIM_RESET].setAnimEnable(false);
                mpPanes[4]->SetVisible(false);
                field_0x57B3 = 0;
                field_0x579A = 11;
                mStateMgr.changeState(StateID_Wait);
            }
        }
        // TODO maybe an inline
        s32 tmp;
        if (dLytDobutton_c::GetInstance() != nullptr) {
            tmp = dLytDobutton_c::GetInstance()->getField0x47C();
        } else {
            tmp = 0x5E;
        }

        if (tmp == 0x28) {
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
        if (field_0x5794 != 11) {
            field_0x5797 = field_0x5794;
            field_0x57B4 = 0;
        } else {
            field_0x57B4 = 1;
        }
        fn_800F0680(field_0x5796);
        b = true;
    } else if (field_0x579C != fn_800F07D0(fn_800F0990(1))) {
        field_0x579C = fn_800F07D0(fn_800F0990(1));
        fn_800F0680(field_0x5796);
        b = true;
    } else if (field_0x579D != fn_800F07D0(fn_800F0990(5))) {
        field_0x579D = fn_800F07D0(fn_800F0990(5));
        fn_800F0680(field_0x5796);
        b = true;
    } else if (field_0x579E != fn_800F07D0(fn_800F0990(4))) {
        field_0x579E = fn_800F07D0(fn_800F0990(4));
        fn_800F0680(field_0x5796);
        b = true;
    } else if (field_0x579F != fn_800F07D0(fn_800F0990(7))) {
        field_0x579F = fn_800F07D0(fn_800F0990(7));
        fn_800F0680(field_0x5796);
        b = true;
    }

    if (field_0x5797 != 11) {
        fn_800EFAD0(fn_800F07D0(fn_800F0990(field_0x5797)), a);
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
            if (field_0x5797 == 0) {
                SmallSoundManager::GetInstance()->playSound(SE_S_BM_MAX);
            } else if (field_0x5797 == 1) {
                SmallSoundManager::GetInstance()->playSound(SE_S_AW_MAX);
            } else if (field_0x5797 == 5) {
                SmallSoundManager::GetInstance()->playSound(SE_S_PC_MAX);
            }
        }
        field_0x57B7 = false;
    }

    fn_800EF7C0();
    mStateMgr.executeState();
    fn_800EF8C0(0);

    // TODO maybe an inline
    s32 tmp;
    if (dLytDobutton_c::GetInstance() != nullptr) {
        tmp = dLytDobutton_c::GetInstance()->getField0x47C();
    } else {
        tmp = 0x5E;
    }

    if (tmp == 0x28) {
        if (field_0x57C3 != 0) {
            field_0x57BB = 1;
        }
    } else {
        field_0x57C3 = 0;
    }

    bool b2 = false;
    if (field_0x57B9 != 0 && dLytMeterContainer_c::GetInstance()->checkAllFlags(0x10) && field_0x576C == 0x85) {
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
        for (int i = 0; i < 8; i++) {
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

        mVec3_c t1(0.0f, 0.0f, 0.0f);
        t1.z = field_0x5778;
        mpPanes[0]->SetRotate(t1);
        mVec3_c t2(0.0f, 0.0f, 0.0f);
        t2.z = -field_0x5778;
        mpPanes[1]->SetRotate(t2);
        mpPanes[2]->SetRotate(t2);

        f32 frame = mAnm[ITEM_SELECT_ANIM_ARROW].getAnimDuration();
        if (field_0x577C < 0.0f) {
            field_0x577C = 0.0f;
        }
        if (field_0x577C > 1.0f) {
            field_0x577C = 1.0f;
        }
        mAnm[ITEM_SELECT_ANIM_ARROW].setFrame(frame * field_0x577C);
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

        if (field_0x5794 == 2) {
            if (LytDoButtonRelated::GetInstance() != nullptr) {
                LytDoButtonRelated::GetInstance()->set0x08(0x91);
            }
        } else if (field_0x5794 == 9) {
            if (field_0x57B3 != 0) {
                if (LytDoButtonRelated::GetInstance() != nullptr) {
                    LytDoButtonRelated::GetInstance()->set0x08(0x90);
                }
            } else {
                if (LytDoButtonRelated::GetInstance() != nullptr) {
                    LytDoButtonRelated::GetInstance()->set0x08(0x8F);
                }
            }
        }

        if (field_0x578C <= 0 && *mStateMgr.getStateID() != StateID_SetIn &&
            *mStateMgr.getStateID() != StateID_SetOut && *mStateMgr.getStateID() != StateID_ToUnuse &&
            *mStateMgr.getStateID() != StateID_Unuse) {
            s32 tmp;
            if (LytDoButtonRelated::GetInstance() != nullptr) {
                tmp = LytDoButtonRelated::GetInstance()->get0x08();
            } else {
                tmp = 0x98;
            }
            if (field_0x576C != tmp) {
                s32 tmp;
                if (LytDoButtonRelated::GetInstance() != nullptr) {
                    tmp = LytDoButtonRelated::GetInstance()->get0x08();
                } else {
                    tmp = 0x98;
                }
                field_0x576C = tmp;
                fn_800F0440(field_0x576C);
            }
        } else {
            if (field_0x576C != 0x98) {
                field_0x576C = 0x98;
                fn_800F0440(field_0x576C);
            }

            if (field_0x578C > 0) {
                field_0x578C--;
            }
        }

        if (LytDoButtonRelated::GetInstance() != nullptr) {
            LytDoButtonRelated::GetInstance()->set0x08(0x98);
        }
    }

    field_0x57C1 = fn_800F09E0();
    return true;
}
