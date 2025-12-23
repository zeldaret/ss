#include "d/lyt/d_lyt_deposit.h"

#include "common.h"
#include "d/d_cs_base.h"
#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"

STATE_DEFINE(dLytDepositMain_c, ModeNone);
STATE_DEFINE(dLytDepositMain_c, ModeIn);
STATE_DEFINE(dLytDepositMain_c, ModeCheck);
STATE_DEFINE(dLytDepositMain_c, ModeChange);
STATE_DEFINE(dLytDepositMain_c, ModeArrangement);
STATE_DEFINE(dLytDepositMain_c, ModeReverseChange);
STATE_DEFINE(dLytDepositMain_c, ModeScroll);
STATE_DEFINE(dLytDepositMain_c, ModeRecycle);
STATE_DEFINE(dLytDepositMain_c, ModeArrangementOut);
STATE_DEFINE(dLytDepositMain_c, ModeOut);

#define DEPOSIT_ANIM_IN 0
#define DEPOSIT_ANIM_IN_BOX 1
#define DEPOSIT_ANIM_FLASH_OFFSET 5
#define DEPOSIT_ANIM_INPUT 22
#define DEPOSIT_ANIM_ARROW_LOOP 24
#define DEPOSIT_ANIM_BTN_LOOP 25
#define DEPOSIT_ANIM_LOOP 26

#define DEPOSIT_NUM_RING_ITEMS 8

void dLytDepositMain_c::initializeState_ModeNone() {}
void dLytDepositMain_c::executeState_ModeNone() {
    field_0x19520 = true;
}
void dLytDepositMain_c::finalizeState_ModeNone() {}

void dLytDepositMain_c::initializeState_ModeIn() {}
void dLytDepositMain_c::executeState_ModeIn() {
    if (field_0x19538 == 0) {
        if (!field_0x19520) {
            if (mAnm[DEPOSIT_ANIM_IN].isEndReached()) {
                mAnm[DEPOSIT_ANIM_IN].setAnimEnable(false);
                mpBoundingBox->SetVisible(true);
                mpBoundingChoices->SetVisible(true);
                mpBoundingItem12->SetVisible(true);
                mpBoundingCleanUp->SetVisible(false);
                setPouchItemsVisible(true);
                mStock[0].navigateToItem();
                mStock[0].fn_80156530(false);
                field_0x19530 = -DEPOSIT_NUM_RING_ITEMS;
                dCsBase_c::GetInstance()->setCursorStickTargetPane(mpRingBoundings[0]);
                field_0x1954C = 0;
                field_0x19520 = true;
            }
        }
    } else {
        if (!field_0x19520) {
            if (mAnm[DEPOSIT_ANIM_IN_BOX].isEndReached() && mArrow.isChangingState()) {
                mAnm[DEPOSIT_ANIM_IN_BOX].setAnimEnable(false);
                mpBoundingBox->SetVisible(false);
                mpBoundingChoices->SetVisible(true);
                mpBoundingItem12->SetVisible(true);
                mpBoundingCleanUp->SetVisible(true);
                setPouchItemsVisible(false);
                mStock[0].navigateToItem();
                mStock[0].fn_80156530(true);
                s32 target = mStock[0].getCurrentNavTarget();
                if (target < 0) {
                    target += 12;
                }
                dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[target]);
                field_0x1954C = 1;
                field_0x19520 = true;
            }
        }
    }
}
void dLytDepositMain_c::finalizeState_ModeIn() {}

void dLytDepositMain_c::initializeState_ModeCheck() {
    field_0x19521 = false;
    field_0x19522 = false;
}
void dLytDepositMain_c::executeState_ModeCheck() {
    fn_802AAFC0();
    fn_802AB210();
    fn_802ACAF0();
    fn_802ABB60();
    field_0x19520 = true;
}
void dLytDepositMain_c::finalizeState_ModeCheck() {}

void dLytDepositMain_c::initializeState_ModeChange() {
    field_0x19521 = false;
    field_0x19522 = false;
}
void dLytDepositMain_c::executeState_ModeChange() {}
void dLytDepositMain_c::finalizeState_ModeChange() {}

void dLytDepositMain_c::initializeState_ModeArrangement() {}
void dLytDepositMain_c::executeState_ModeArrangement() {}
void dLytDepositMain_c::finalizeState_ModeArrangement() {}

void dLytDepositMain_c::initializeState_ModeReverseChange() {}
void dLytDepositMain_c::executeState_ModeReverseChange() {}
void dLytDepositMain_c::finalizeState_ModeReverseChange() {}

void dLytDepositMain_c::initializeState_ModeScroll() {}
void dLytDepositMain_c::executeState_ModeScroll() {}
void dLytDepositMain_c::finalizeState_ModeScroll() {}

void dLytDepositMain_c::initializeState_ModeRecycle() {}
void dLytDepositMain_c::executeState_ModeRecycle() {}
void dLytDepositMain_c::finalizeState_ModeRecycle() {}

void dLytDepositMain_c::initializeState_ModeArrangementOut() {}
void dLytDepositMain_c::executeState_ModeArrangementOut() {}
void dLytDepositMain_c::finalizeState_ModeArrangementOut() {}

void dLytDepositMain_c::initializeState_ModeOut() {}
void dLytDepositMain_c::executeState_ModeOut() {}
void dLytDepositMain_c::finalizeState_ModeOut() {}

dLytDepositMain_c::dLytDepositMain_c() : mStateMgr(*this) {}

static const d2d::LytBrlanMapping brlanMap[] = {
    {       "depositBox_00_in.brlan",      "G_inOut_00"},
    {    "depositBox_00_inBox.brlan",      "G_inOut_01"},
    {   "depositBox_00_change.brlan",     "G_change_00"},
    { "depositBox_00_selectOn.brlan",   "G_flashBox_00"},
    { "depositBox_00_selectOn.brlan", "G_flashPoach_00"},
    {    "depositBox_00_flash.brlan", "G_poachFlash_00"},
    {    "depositBox_00_flash.brlan", "G_poachFlash_01"},
    {    "depositBox_00_flash.brlan", "G_poachFlash_02"},
    {    "depositBox_00_flash.brlan", "G_poachFlash_03"},
    {    "depositBox_00_flash.brlan", "G_poachFlash_04"},
    {    "depositBox_00_flash.brlan", "G_poachFlash_05"},
    {    "depositBox_00_flash.brlan", "G_poachFlash_06"},
    {    "depositBox_00_flash.brlan", "G_poachFlash_07"},
    {  "depositBox_00_scrollR.brlan",     "G_scroll_00"},
    {  "depositBox_00_scrollL.brlan",     "G_scroll_00"},
    {"depositBox_00_btnDecide.brlan",        "G_btn_00"},
    {"depositBox_00_btnDecide.brlan",        "G_btn_01"},
    {"depositBox_00_btnDecide.brlan",        "G_btn_02"},
    {      "depositBox_00_out.brlan",      "G_inOut_00"},
    { "depositBox_00_btnOnOff.brlan",        "G_btn_00"},
    { "depositBox_00_btnOnOff.brlan",        "G_btn_01"},
    {  "depositBox_00_recycle.brlan",    "G_recycle_00"},
    {    "depositBox_00_input.brlan",      "G_input_00"},
    {    "depositBox_00_input.brlan",      "G_input_01"},
    {"depositBox_00_arrowLoop.brlan",  "G_arrowLoop_00"},
    {  "depositBox_00_btnLoop.brlan",    "G_btnLoop_00"},
    {     "depositBox_00_loop.brlan",       "G_loop_00"},
    {     "depositBox_00_have.brlan",      "G_itemA_00"},
    {     "depositBox_00_have.brlan",      "G_itemB_00"},
    {     "depositBox_00_have.brlan",      "G_itemC_00"},
    {     "depositBox_00_have.brlan",      "G_itemD_00"},
    {     "depositBox_00_have.brlan",      "G_itemE_00"},
    {     "depositBox_00_have.brlan",      "G_itemF_00"},
    {     "depositBox_00_have.brlan",      "G_itemG_00"},
    {     "depositBox_00_have.brlan",      "G_itemH_00"},
    {     "depositBox_00_keta.brlan",       "G_keta_00"},
};

#define DEPOSIT_NUM_ANIMS 36

static const char *sRingBoundingNames[] = {
    "B_ringA_00", "B_ringB_00", "B_ringC_00", "B_ringD_00", "B_ringE_00", "B_ringF_00", "B_ringG_00", "B_ringH_00",
};

#define DEPOSIT_NUM_RING_BOUNDINGS 8

static const char *sItemBoundingNames[] = {
    "B_item_00", "B_item_01", "B_item_02", "B_item_03", "B_item_04", "B_item_05",
    "B_item_06", "B_item_07", "B_item_08", "B_item_09", "B_item_10", "B_item_11",
};

#define DEPOSIT_NUM_ITEM_BOUNDINGS 12

static const char *sWindowNames[] = {
    "W_bgP_00",
    "W_bgP_01",
};

static const char *sGroupName = "G_ref_00";

bool dLytDepositMain_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("depositBox_00.brlyt", nullptr);

    for (int i = 0; i < DEPOSIT_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mpBoundingBox = mLyt.findBounding("B_box_00");
    mpBoundingChoices = mLyt.findBounding("B_choices_00");
    mpBoundingChoices->SetVisible(true);
    mpBoundingCleanUp = mLyt.findBounding("B_cleanUp_00");
    mpBoundingCleanUp->SetVisible(true);
    mpBoundingItem12 = mLyt.findBounding("B_item_12");

    mpPaneABtn = mLyt.findPane("N_aBtn_00");
    mpPanePrice = mLyt.findPane("N_price_00");
    mpPaneCommonArrow = mLyt.findPane("N_commonArrow_00");

    for (int i = 0; i < DEPOSIT_NUM_RING_BOUNDINGS; i++) {
        mpRingBoundings[i] = mLyt.findBounding(sRingBoundingNames[i]);
        mpRingBoundings[i]->SetVisible(false);
    }

    for (int i = 0; i < DEPOSIT_NUM_ITEM_BOUNDINGS; i++) {
        mpItemBoundings[i] = mLyt.findBounding(sItemBoundingNames[i]);
        mpItemBoundings[i]->SetVisible(false);
    }

    for (int i = 0; i < 2; i++) {
        mpWindows[i] = mLyt.getWindow(sWindowNames[i]);
        mpSizeBoxes[i] = mLyt.getSizeBoxInWindow(sWindowNames[i]);
        mpWindows[i]->UpdateSize(mpSizeBoxes[i], 32.0f);
    }

    mAnm[DEPOSIT_ANIM_IN].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_IN].setFrame(mAnm[DEPOSIT_ANIM_IN].getLastFrame());

    mAnm[DEPOSIT_ANIM_ARROW_LOOP].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_ARROW_LOOP].setFrame(0.0f);

    mAnm[DEPOSIT_ANIM_BTN_LOOP].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_BTN_LOOP].setFrame(0.0f);

    mAnm[DEPOSIT_ANIM_LOOP].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_LOOP].setFrame(0.0f);

    mAnm[DEPOSIT_ANIM_INPUT].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_INPUT].setForwardOnce();
    mAnm[DEPOSIT_ANIM_INPUT].setFrame(mAnm[DEPOSIT_ANIM_INPUT].getLastFrame());

    for (int i = 0; i < DEPOSIT_NUM_RING_ITEMS; i++) {
        mAnm[i + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(true);
        mAnm[i + DEPOSIT_ANIM_FLASH_OFFSET].setToEnd();
    }

    mLyt.calc();

    mAnm[DEPOSIT_ANIM_IN].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_ARROW_LOOP].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_INPUT].setAnimEnable(false);

    for (int i = 0; i < DEPOSIT_NUM_RING_ITEMS; i++) {
        mAnm[i + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(false);
    }

    fn_802AA7E0();
    mCsHitCheck.init(mLyt.getLayout()->GetRootPane(), 0x1, 0, 0);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitCheck);
    fn_802AD370();

    mCursor.build(resAcc);

    mArrow.build();
    mArrow.setState(1);

    fn_802A9E80();

    field_0x19521 = false;
    field_0x19522 = false;
    field_0x19523 = false;
    field_0x19524 = false;
    field_0x19525 = false;
    field_0x19526 = false;
    field_0x19527 = false;
    field_0x19528 = false;
    field_0x19529 = false;
    field_0x1952A = true;
    field_0x1952B = false;
    field_0x1952C = true;

    field_0x19530 = -1;
    field_0x19534 = -1;
    field_0x19538 = 0;
    field_0x1953C = 0;
    field_0x19540 = 0;
    field_0x19544 = 0;
    field_0x19548 = -1;
    field_0x1954C = 0;
    field_0x19550 = 0;
    field_0x19554 = 0;

    return true;
}

STATE_DEFINE(dLytDeposit_c, None);
STATE_DEFINE(dLytDeposit_c, In);
STATE_DEFINE(dLytDeposit_c, Check);
STATE_DEFINE(dLytDeposit_c, Change);
STATE_DEFINE(dLytDeposit_c, Arrangement);
STATE_DEFINE(dLytDeposit_c, ReverseChange);
STATE_DEFINE(dLytDeposit_c, Scroll);
STATE_DEFINE(dLytDeposit_c, Recycle);
STATE_DEFINE(dLytDeposit_c, ArrangementOut);
STATE_DEFINE(dLytDeposit_c, Out);

SPECIAL_BASE_PROFILE(LYT_DEPOSIT, dLytDeposit_c, fProfile::LYT_DEPOSIT, 0x2B9, 0);

void dLytDeposit_c::initializeState_None() {}
void dLytDeposit_c::executeState_None() {}
void dLytDeposit_c::finalizeState_None() {}

void dLytDeposit_c::initializeState_In() {}
void dLytDeposit_c::executeState_In() {}
void dLytDeposit_c::finalizeState_In() {}

void dLytDeposit_c::initializeState_Check() {}
void dLytDeposit_c::executeState_Check() {}
void dLytDeposit_c::finalizeState_Check() {}

void dLytDeposit_c::initializeState_Change() {}
void dLytDeposit_c::executeState_Change() {}
void dLytDeposit_c::finalizeState_Change() {}

void dLytDeposit_c::initializeState_Arrangement() {}
void dLytDeposit_c::executeState_Arrangement() {}
void dLytDeposit_c::finalizeState_Arrangement() {}

void dLytDeposit_c::initializeState_ReverseChange() {}
void dLytDeposit_c::executeState_ReverseChange() {}
void dLytDeposit_c::finalizeState_ReverseChange() {}

void dLytDeposit_c::initializeState_Scroll() {}
void dLytDeposit_c::executeState_Scroll() {}
void dLytDeposit_c::finalizeState_Scroll() {}

void dLytDeposit_c::initializeState_Recycle() {}
void dLytDeposit_c::executeState_Recycle() {}
void dLytDeposit_c::finalizeState_Recycle() {}

void dLytDeposit_c::initializeState_ArrangementOut() {}
void dLytDeposit_c::executeState_ArrangementOut() {}
void dLytDeposit_c::finalizeState_ArrangementOut() {}

void dLytDeposit_c::initializeState_Out() {}
void dLytDeposit_c::executeState_Out() {}
void dLytDeposit_c::finalizeState_Out() {}
