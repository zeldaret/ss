#include "d/lyt/d_lyt_deposit.h"

#include "common.h"
#include "d/d_cs_base.h"
#include "d/d_cs_game.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_message.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/d_pouch.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_deposit_box_cursor.h"
#include "d/lyt/d_lyt_deposit_stock.h"
#include "d/lyt/d_lyt_util_items.h"
#include "d/lyt/d_textbox.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "m/m_pad.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/file_manager.h"

struct DepositSellValues {
    /* 0x00 */ u8 itemId;
    /* 0x02 */ u16 sellValue; //< 0 -> unsellable
};

static const DepositSellValues sItemData[] = {
    {                 ITEM_HEART_POTION,   0},
    {            ITEM_HEART_POTION_PLUS,   0},
    {       ITEM_HEART_POTION_PLUS_PLUS,   0},

    {                   ITEM_AIR_POTION,   0},
    {              ITEM_AIR_POTION_PLUS,   0},

    {               ITEM_STAMINA_POTION,   0},
    {          ITEM_STAMINA_POTION_PLUS,   0},

    {              ITEM_GUARDIAN_POTION,   0},
    {         ITEM_GUARDIAN_POTION_PLUS,   0},

    {          ITEM_REVITALIZING_POTION,   0},
    {     ITEM_REVITALIZING_POTION_PLUS,   0},
    {ITEM_REVITALIZING_POTION_PLUS_PLUS,   0},

    {                        ITEM_FAIRY,   0},
    {                        ITEM_WATER,   0},
    {                 ITEM_SACRED_WATER,   0},
    {              ITEM_MUSHROOM_SPORES,   0},
    {            ITEM_GLITTERING_SPORES,   0},
    {                     ITEM_HOT_SOUP,   0},
    {                    ITEM_COLD_SOUP,   0},
    {                       ITEM_BOTTLE,   0},

    {                ITEM_WOODEN_SHIELD,  20},
    {                ITEM_BANDED_SHIELD,  50},
    {                ITEM_BRACED_SHIELD, 100},

    {                  ITEM_IRON_SHIELD,  50},
    {            ITEM_REINFORCED_SHIELD,  80},
    {             ITEM_FORTIFIED_SHIELD, 150},

    {                ITEM_SACRED_SHIELD, 300},
    {                ITEM_DIVINE_SHIELD, 400},
    {               ITEM_GODDESS_SHIELD, 500},

    {                ITEM_HYLIAN_SHIELD,   0},

    {           ITEM_SMALL_SEED_SATCHEL,  50},
    {          ITEM_MEDIUM_SEED_SATCHEL,  80},
    {           ITEM_LARGE_SEED_SATCHEL, 150},

    {                 ITEM_SMALL_QUIVER,  50},
    {                ITEM_MEDIUM_QUIVER,  80},
    {                 ITEM_LARGE_QUIVER, 150},

    {               ITEM_SMALL_BOMB_BAG,  50},
    {              ITEM_MEDIUM_BOMB_BAG,  80},
    {               ITEM_LARGE_BOMB_BAG, 150},

    {             ITEM_RUPEE_MEDAL_ALSO, 100},
    {                  ITEM_HEART_MEDAL,   0},
    {                  ITEM_RUPEE_MEDAL,   0},
    {               ITEM_TREASURE_MEDAL,   0},
    {                 ITEM_POTION_MEDAL,   0},
    {                   ITEM_LIFE_MEDAL,   0},
    {                 ITEM_CURSED_MEDAL,   0},
    {                    ITEM_BUG_MEDAL,   0},
};

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
#define DEPOSIT_ANIM_CHANGE 2
#define DEPOSIT_ANIM_SELECT_ON_BOX 3
#define DEPOSIT_ANIM_SELECT_ON_POUCH 4
#define DEPOSIT_ANIM_FLASH_OFFSET 5
#define DEPOSIT_ANIM_SCROLL_R 13
#define DEPOSIT_ANIM_SCROLL_L 14
#define DEPOSIT_ANIM_BTN_DECIDE_00 15
#define DEPOSIT_ANIM_BTN_DECIDE_01 16
#define DEPOSIT_ANIM_BTN_DECIDE_02 17
#define DEPOSIT_ANIM_OUT 18
#define DEPOSIT_ANIM_BTN_ON_OFF_00 19
#define DEPOSIT_ANIM_BTN_ON_OFF_01 20
#define DEPOSIT_ANIM_RECYCLE 21
#define DEPOSIT_ANIM_INPUT_00 22
#define DEPOSIT_ANIM_INPUT_01 23
#define DEPOSIT_ANIM_ARROW_LOOP 24
#define DEPOSIT_ANIM_BTN_LOOP 25
#define DEPOSIT_ANIM_LOOP 26
#define DEPOSIT_ANIM_HAVE_OFFSET 27
#define DEPOSIT_ANIM_KETA 35

#define DEPOSIT_NUM_ANIMS 36

void dLytDepositMain_c::initializeState_ModeNone() {}
void dLytDepositMain_c::executeState_ModeNone() {
    mIsIdle = true;
}
void dLytDepositMain_c::finalizeState_ModeNone() {}

void dLytDepositMain_c::initializeState_ModeIn() {}
void dLytDepositMain_c::executeState_ModeIn() {
    if (mStateStep == 0) {
        if (!mIsIdle) {
            if (mAnm[DEPOSIT_ANIM_IN].isEndReached()) {
                mAnm[DEPOSIT_ANIM_IN].setAnimEnable(false);
                mpBoundingBox->SetVisible(true);
                mpBoundingChoices->SetVisible(true);
                mpBoundingItem12->SetVisible(true);
                mpBoundingCleanUp->SetVisible(false);
                setPouchItemsVisible(true);
                mStock[STOCK_ACTIVE].navigateToItem();
                mStock[STOCK_ACTIVE].fn_80156530(false);
                mCurrentPouchNavTarget = -RING_NUM_ITEMS;
                dCsBase_c::GetInstance()->setCursorStickTargetPane(mpRingBoundings[0]);
                field_0x1954C = 0;
                mIsIdle = true;
            }
        }
    } else {
        if (!mIsIdle) {
            if (mAnm[DEPOSIT_ANIM_IN_BOX].isEndReached() && mArrow.isChangingState()) {
                mAnm[DEPOSIT_ANIM_IN_BOX].setAnimEnable(false);
                mpBoundingBox->SetVisible(false);
                mpBoundingChoices->SetVisible(true);
                mpBoundingItem12->SetVisible(true);
                mpBoundingCleanUp->SetVisible(true);
                setPouchItemsVisible(false);
                mStock[STOCK_ACTIVE].navigateToItem();
                mStock[STOCK_ACTIVE].fn_80156530(true);
                s32 target = mStock[STOCK_ACTIVE].getCurrentNavTarget();
                if (target < 0) {
                    target += NUM_ICONS_PER_PAGE;
                }
                dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[target]);
                field_0x1954C = 1;
                mIsIdle = true;
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
    checkForItemPickupOrDrop();
    fn_802ABB60();
    mIsIdle = true;
}
void dLytDepositMain_c::finalizeState_ModeCheck() {}

void dLytDepositMain_c::initializeState_ModeChange() {
    field_0x19521 = false;
    field_0x19522 = false;
}
void dLytDepositMain_c::executeState_ModeChange() {
    if (mIsIdle) {
        fn_802AC3C0();
        fn_802AB080();
        if (field_0x19550 != 0) {
            field_0x19550--;
        } else {
            mStock[STOCK_ACTIVE].handleNavOrPoint();
            if (!mStock[STOCK_ACTIVE].isModeSort()) {
                checkForItemPickupOrDrop();
            }
        }

        if (!dPadNav::isPointerVisible()) {
            if (!mCursor.isVisible() && mCurrentlyHoldingItemSlot >= 0) {
                mCursor.setVisible(true);
            }

            if (mStock[STOCK_ACTIVE].isModePouch()) {
                mStock[STOCK_ACTIVE].disableModePouch();
                field_0x19522 = true;
            }
        }

        if (mStock[STOCK_ACTIVE].getCurrentNavTarget() >= 0) {
            dCsBase_c::GetInstance()->setCursorStickTargetPane(
                mpItemBoundings[mStock[STOCK_ACTIVE].getCurrentNavTarget()]
            );
            fn_802AD4C0();
        }
        s32 slot = mStock[STOCK_ACTIVE].getCurrentSlot();
        s32 v;
        if (mCurrentlyHoldingItemSlot >= 8 && slot == mCurrentlyHoldingItemSlot - 8) {
            v = -1;
        } else if (mStock[STOCK_ACTIVE].getCurrentNavTarget() < 0) {
            v = -1;
        } else {
            s32 item = mStock[STOCK_ACTIVE].getItem(mStock[STOCK_ACTIVE].getCurrentNavTarget());
            if (item != 12 && item != 50 && slot >= 0 && slot < NUM_ICONS_PER_PAGE * NUM_PAGES) {
                v = FileManager::GetInstance()->getItemCheckItem(slot);
                if (v == 0) {
                    v = -1;
                }
            } else {
                v = -1;
            }
        }
        loadItemText(v, true);
        fn_802ABB60();
        fn_802AC670();
    } else {
        if (mAnm[DEPOSIT_ANIM_CHANGE].isStop() && mArrow.isChangingState()) {
            mAnm[DEPOSIT_ANIM_CHANGE].setAnimEnable(false);
            mAnm[DEPOSIT_ANIM_SELECT_ON_BOX].setAnimEnable(false);
            mpBoundingBox->SetVisible(false);
            mpBoundingCleanUp->SetVisible(true);
            mStock[STOCK_ACTIVE].fn_80156530(true);
            setPouchItemsVisible(false);
            field_0x1954C = 1;
            mStock[STOCK_ACTIVE].returnToNormalMode();
            s32 target = mStock[STOCK_ACTIVE].getCurrentNavTarget();
            if (target < 0) {
                target += NUM_ICONS_PER_PAGE;
            }
            dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[target]);
            if (!dPadNav::isPointerVisible()) {
                fn_802AC290(target);
            }
            fn_802AD460();
            fn_802AD4C0();
            field_0x19550 = 1;
            mIsIdle = true;
        }
    }
}
void dLytDepositMain_c::finalizeState_ModeChange() {}

void dLytDepositMain_c::initializeState_ModeArrangement() {}
void dLytDepositMain_c::executeState_ModeArrangement() {
    if (!mIsIdle) {
        if (mAnm[DEPOSIT_ANIM_BTN_DECIDE_01].isEndReached()) {
            mAnm[DEPOSIT_ANIM_BTN_DECIDE_01].setAnimEnable(false);
            loadItems(mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET);
            mStock[STOCK_ACTIVE].fn_80156530(true);
            field_0x1954C = 1;
            mIsIdle = true;
        }
    }
}
void dLytDepositMain_c::finalizeState_ModeArrangement() {}

void dLytDepositMain_c::initializeState_ModeReverseChange() {}
void dLytDepositMain_c::executeState_ModeReverseChange() {
    if (mStateStep == 0) {
        if (!mIsIdle && mAnm[DEPOSIT_ANIM_CHANGE].isStop() && mArrow.isChangingState()) {
            mAnm[DEPOSIT_ANIM_CHANGE].setAnimEnable(false);
            mAnm[DEPOSIT_ANIM_SELECT_ON_POUCH].setAnimEnable(false);
            mpBoundingBox->SetVisible(true);
            mpBoundingCleanUp->SetVisible(false);
            setPouchItemsVisible(true);
            mStock[STOCK_ACTIVE].fn_80156530(false);
            field_0x1954C = 0;
            mStateStep = 1;
        }
    } else {
        fn_802AD460();
        fn_802AC0E0(4);
        mIsIdle = true;
    }
}
void dLytDepositMain_c::finalizeState_ModeReverseChange() {}

void dLytDepositMain_c::initializeState_ModeScroll() {}
void dLytDepositMain_c::executeState_ModeScroll() {
    s32 currentlyHeldSlot = mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET;
    dPadNav::scrollRelated();
    if (mAnm[DEPOSIT_ANIM_SCROLL_R].isEnabled()) {
        if (!mIsIdle && mAnm[DEPOSIT_ANIM_SCROLL_R].isEndReached()) {
            mAnm[DEPOSIT_ANIM_SCROLL_R].setFrame(0.0f);
            mLyt.calc();
            mAnm[DEPOSIT_ANIM_SCROLL_R].setAnimEnable(false);
            for (int i = 0; i < NUM_STOCKS; i++) {
                mStock[i].nextPage(field_0x1952C);
                mStock[i].loadItems(currentlyHeldSlot);
            }
            mStock[STOCK_ACTIVE].fn_80156530(true);
            field_0x1954C = 1;
            fn_802AD370();
            fn_802AD460();
            if (mStock[STOCK_ACTIVE].getArrowDirection() >= dLytDepositStock_c::ARROW_NONE) {
                fn_802AD4C0();
            }
            field_0x1952C = true;
            mIsIdle = true;
        } else if (!mHasScrolledOtherStocks && mAnm[DEPOSIT_ANIM_SCROLL_R].getFrame() >= 2.0f) {
            mHasScrolledOtherStocks = true;
            u8 oldPage = mStock[2].getPage();
            mStock[2].setPage(mStock[1].getPage());
            mStock[2].nextPage(false);
            mStock[2].loadItems(currentlyHeldSlot);
            mStock[2].setPage(oldPage);
        }
    } else {
        if (!mIsIdle && mAnm[DEPOSIT_ANIM_SCROLL_L].isEndReached()) {
            mAnm[DEPOSIT_ANIM_SCROLL_L].setFrame(0.0f);
            mLyt.calc();
            mAnm[DEPOSIT_ANIM_SCROLL_L].setAnimEnable(false);
            for (int i = 0; i < NUM_STOCKS; i++) {
                mStock[i].prevPage(field_0x1952C);
                mStock[i].loadItems(currentlyHeldSlot);
            }
            mStock[STOCK_ACTIVE].fn_80156530(true);
            field_0x1954C = 1;
            fn_802AD370();
            fn_802AD460();
            if (mStock[STOCK_ACTIVE].getArrowDirection() >= dLytDepositStock_c::ARROW_NONE) {
                fn_802AD4C0();
            }
            field_0x1952C = true;
            mIsIdle = true;
        } else if (!mHasScrolledOtherStocks && mAnm[DEPOSIT_ANIM_SCROLL_L].getFrame() >= 2.0f) {
            mHasScrolledOtherStocks = true;
            u8 oldPage = mStock[1].getPage();
            mStock[1].setPage(mStock[2].getPage());
            mStock[1].prevPage(false);
            mStock[1].loadItems(currentlyHeldSlot);
            mStock[1].setPage(oldPage);
        }
    }
}
void dLytDepositMain_c::finalizeState_ModeScroll() {}

void dLytDepositMain_c::initializeState_ModeRecycle() {}
void dLytDepositMain_c::executeState_ModeRecycle() {
    if (mStateStep == 0) {
        if (mAnm[DEPOSIT_ANIM_BTN_DECIDE_02].isEndReached()) {
            mAnm[DEPOSIT_ANIM_BTN_DECIDE_02].setAnimEnable(false);
            mAnm[DEPOSIT_ANIM_OUT].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_OUT].setFrame(0.0f);
            mLyt.calc();
            dCsBase_c::GetInstance()->setVisible(false);
            dPadNav::setNavEnabled(false, false);
            if (field_0x1954C != 0) {
                mArrow.requestOut();
                mStateStep = 2;
            } else {
                mStateStep = 1;
            }
        }
    } else {
        bool b = mStateStep != 2 || mArrow.isChangingState();
        if (mAnm[DEPOSIT_ANIM_OUT].isEndReached() && b) {
            mAnm[DEPOSIT_ANIM_OUT].setAnimEnable(false);
            mAnm[DEPOSIT_ANIM_RECYCLE].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_RECYCLE].setFrame(0.0f);
            mLyt.calc();
            mAnm[DEPOSIT_ANIM_RECYCLE].setAnimEnable(false);
            mCurrentlyHoldingItemSlot = -1;
            mIsIdle = true;
        }
    }
}
void dLytDepositMain_c::finalizeState_ModeRecycle() {}

void dLytDepositMain_c::initializeState_ModeArrangementOut() {}
void dLytDepositMain_c::executeState_ModeArrangementOut() {
    if (mStateStep == 0) {
        if (mAnm[DEPOSIT_ANIM_BTN_DECIDE_01].isEndReached()) {
            mAnm[DEPOSIT_ANIM_BTN_DECIDE_01].setAnimEnable(false);
            mAnm[DEPOSIT_ANIM_OUT].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_OUT].setFrame(0.0f);
            mLyt.calc();
            dCsBase_c::GetInstance()->setVisible(false);
            dPadNav::setNavEnabled(false, false);
            mArrow.requestOut();
            mStateStep = 1;
        }
    } else {
        if (mAnm[DEPOSIT_ANIM_OUT].isEndReached() && mArrow.isChangingState()) {
            mAnm[DEPOSIT_ANIM_OUT].setAnimEnable(false);
            mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setFrame(0.0f);
            mLyt.calc();
            mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setAnimEnable(false);
            field_0x19528 = true;
            mStock[STOCK_ACTIVE].disableModeSort();
            mStock[STOCK_ACTIVE].navigateToItem();
            mIsIdle = true;
        }
    }
}
void dLytDepositMain_c::finalizeState_ModeArrangementOut() {}

void dLytDepositMain_c::initializeState_ModeOut() {}
void dLytDepositMain_c::executeState_ModeOut() {
    if (mAnm[DEPOSIT_ANIM_BTN_DECIDE_00].isEndReached()) {
        mAnm[DEPOSIT_ANIM_BTN_DECIDE_00].setAnimEnable(false);
        mIsIdle = true;
    }
}
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

    mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_INPUT_00].setForwardOnce();
    mAnm[DEPOSIT_ANIM_INPUT_00].setFrame(mAnm[DEPOSIT_ANIM_INPUT_00].getLastFrame());

    for (int i = 0; i < RING_NUM_ITEMS; i++) {
        mAnm[i + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(true);
        mAnm[i + DEPOSIT_ANIM_FLASH_OFFSET].setToEnd();
    }

    mLyt.calc();

    mAnm[DEPOSIT_ANIM_IN].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_ARROW_LOOP].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(false);

    for (int i = 0; i < RING_NUM_ITEMS; i++) {
        mAnm[i + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(false);
    }

    fn_802AA7E0();
    mCsHitCheck.init(mLyt.getLayout()->GetRootPane(), 0x1, 0, 0);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitCheck);
    fn_802AD370();

    mCursor.build(resAcc);

    mArrow.build();
    mArrow.setState(1);

    setModeNone();

    field_0x19521 = false;
    field_0x19522 = false;
    field_0x19523 = false;
    field_0x19524 = false;
    field_0x19525 = false;
    field_0x19526 = false;
    field_0x19527 = false;
    field_0x19528 = false;
    mHasScrolledOtherStocks = false;
    field_0x1952A = true;
    field_0x1952B = false;
    field_0x1952C = true;

    mCurrentPouchNavTarget = -1;
    mCurrentlyHoldingItemSlot = -1;
    mStateStep = 0;
    mCursorLocation = dLytDepositBoxCursor_c::LOC_POUCH;
    field_0x19540 = 0;
    mItemSellValue = 0;
    field_0x19548 = -1;
    field_0x1954C = 0;
    field_0x19550 = 0;
    field_0x19554 = 0;

    return true;
}

bool dLytDepositMain_c::execute() {
    field_0x1952B = false;
    mCsHitCheck.resetCachedHitboxes();
    mCsHitCheck.execute();

    for (int i = 0; i < NUM_STOCKS; i++) {
        mStockNodes[i].mpLytPane->execute();
    }

    mStock[STOCK_ACTIVE].setSellBlocked(mItemSellValue <= 0);
    mStock[STOCK_ACTIVE].setSortBlocked(mCurrentlyHoldingItemSlot >= 0);

    mStateMgr.executeState();
    mCursor.execute();

    for (int i = 0; i < RING_NUM_ITEMS; i++) {
        if (mPouchItemIds[i] < 50) {
            mRingNodes[i].mpLytPane->execute();
        }
    }

    for (int i = 0; i < DEPOSIT_ANIM_HAVE_OFFSET; i++) {
        if (mAnm[i].isEnabled()) {
            if (i >= 19 && i <= 23) {
                if (mAnm[i].isStop()) {
                    mAnm[i].setAnimEnable(false);
                }
            }
            mAnm[i].play();
        }
    }

    mLyt.calc();

    nw4r::math::VEC3 t = mpPaneCommonArrow->GetTranslate();
    mVec2_c v;
    v.x = t.x;
    v.y = t.y;
    mArrow.setTranslate(&v);
    mArrow.execute();

    field_0x1952A = dPadNav::isPointerVisible();

    return true;
}

bool dLytDepositMain_c::draw() {
    mLyt.addToDrawList();
    mCursor.draw();
    mArrow.draw();
    return true;
}

bool dLytDepositMain_c::remove() {
    dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitCheck);

    // Okay this one does need temps to avoid regswaps, the others probably should use temps too for consistency
    d2d::SubPaneList::Iterator it = mStockList.GetBeginIter();
    d2d::SubPaneList::Iterator end = mStockList.GetEndIter();
    for (; it != end; ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < NUM_STOCKS; i++) {
        mStockNodes[i].mpLytPane->remove();
    }

    it = mRingList.GetBeginIter();
    end = mRingList.GetEndIter();
    for (; it != end; ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < RING_NUM_ITEMS; i++) {
        mRingNodes[i].mpLytPane->remove();
    }

    mLyt.unbindAnims();

    for (int i = 0; i < DEPOSIT_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }

    mCursor.remove();

    mArrow.remove();

    return true;
}

void dLytDepositMain_c::setModeNone() {
    mStateMgr.changeState(StateID_ModeNone);
    mIsIdle = false;
}

void dLytDepositMain_c::setModeIn() {
    s32 loc;
    if (field_0x19527) {
        loc = mCursorLocation;
    } else {
        loc = dLytDepositBoxCursor_c::LOC_POUCH;
    }
    fn_802AAB30();

    if (loc == dLytDepositBoxCursor_c::LOC_POUCH) {
        mAnm[DEPOSIT_ANIM_IN].setAnimEnable(true);

        mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(true);
        mAnm[DEPOSIT_ANIM_INPUT_00].setForwardOnce();
        mAnm[DEPOSIT_ANIM_INPUT_00].setFrame(mAnm[DEPOSIT_ANIM_INPUT_00].getLastFrame());
        mLyt.calc();
        mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(false);

        mStateStep = 0;
    } else {
        mStock[STOCK_ACTIVE].fn_80156530(true);
        mAnm[DEPOSIT_ANIM_IN_BOX].setAnimEnable(true);
        mAnm[DEPOSIT_ANIM_IN_BOX].setFrame(0.0f);

        mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(true);
        mAnm[DEPOSIT_ANIM_INPUT_00].setForwardOnce();
        mAnm[DEPOSIT_ANIM_INPUT_00].setFrame(mAnm[DEPOSIT_ANIM_INPUT_00].getLastFrame());
        mLyt.calc();
        mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(false);

        mArrow.requestIn();
        mStateStep = 1;
    }

    dPadNav::setNavEnabled(false, false);
    mStateMgr.changeState(StateID_ModeIn);
    mIsIdle = false;
}

void dLytDepositMain_c::setModeCheck() {
    s32 slot = mCurrentPouchNavTarget;
    if (slot < 0) {
        slot += RING_NUM_ITEMS;
    }
    dCsBase_c::GetInstance()->setCursorStickTargetPane(mpRingBoundings[slot]);
    fn_802AD460();
    mStateMgr.changeState(StateID_ModeCheck);
    mIsIdle = false;
}

void dLytDepositMain_c::setModeChange(bool unk) {
    if (unk) {
        mAnm[DEPOSIT_ANIM_CHANGE].setAnimEnable(true);
        mAnm[DEPOSIT_ANIM_CHANGE].setFrame(0.0f);
        mAnm[DEPOSIT_ANIM_CHANGE].setForwardOnce();

        mAnm[DEPOSIT_ANIM_SELECT_ON_BOX].setAnimEnable(true);
        mAnm[DEPOSIT_ANIM_SELECT_ON_BOX].setFrame(0.0f);
        mAnm[DEPOSIT_ANIM_SELECT_ON_BOX].setForwardOnce();
        mLyt.calc();

        mIsIdle = false;
        fn_802AD400();
        mArrow.requestIn();
        mStateStep = 0;
    }
    mStateMgr.changeState(StateID_ModeChange);
}

void dLytDepositMain_c::setModeArrangement() {
    mAnm[DEPOSIT_ANIM_BTN_DECIDE_01].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_BTN_DECIDE_01].setFrame(0.0f);
    mLyt.calc();
    mIsIdle = false;
    dPadNav::setNavEnabled(true, false);
    mStateMgr.changeState(StateID_ModeArrangement);
}

void dLytDepositMain_c::loadItemText(s32 itemId, s32 where) {
    dTextBox_c *box1, *box2;

    if (where == 0) {
        box1 = mLyt.getTextBox("T_message_08");
        box2 = mLyt.getTextBox("T_messageS_08");
    } else {
        box1 = mLyt.getTextBox("T_nameSpace_00");
        box2 = mLyt.getTextBox("T_nameSpaceS_00");
    }

    if (itemId < 0) {
        static wchar_t empty[] = L"";
        box1->setTextWithGlobalTextProcessor(empty);
        box2->setTextWithGlobalTextProcessor(empty);
    } else {
        const wchar_t *str;
        if (where == 0) {
            SizedString<32> msg;
            msg.sprintf("TRUSTEE_ITEM_%03d", itemId);
            str = dMessage_c::getTextMessageByLabel(msg, true, nullptr, 0);
        } else {
            // TODO check if we can avoid this cast...
            // getItemText has a bunch of other users as well
            str = getItemText((u16)itemId);
        }
        box1->setTextWithGlobalTextProcessor(str);
        box2->setTextWithGlobalTextProcessor(str);
    }
}

void dLytDepositMain_c::checkForItemPickupOrDrop() {
    bool anyPickup = false;

    if (dPad::getDownTrigA()) {
        s32 item;
        if (mCurrentlyHoldingItemSlot < 0) {
            // Not holding an item...
            if (mCurrentPouchNavTarget >= 0 && mPouchItemIds[mCurrentPouchNavTarget] != 12 &&
                // ... and picking up an item from the pouch
                mPouchItemIds[mCurrentPouchNavTarget] != 50) {
                mCursorLocation = dLytDepositBoxCursor_c::LOC_POUCH;
                field_0x19540 = convertLytPouchSlot2(mCurrentPouchNavTarget);
                item = getPouchItemIdForIndex(mCurrentPouchNavTarget, false);
                mCurrentlyHoldingItemSlot = mCurrentPouchNavTarget;
                mCursor.init(mCursorLocation, mCurrentPouchNavTarget, mPouchItemIds[mCurrentPouchNavTarget], false);
                mRingNodes[mCurrentPouchNavTarget].mpPane->SetVisible(false);
                mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(true);
                mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setFrame(0.0f);
                mLyt.calc();
                mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(false);

                mPouchItems[mCurrentPouchNavTarget].setOff();
                loadItemText(-1, false);
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POUCH_CHACH_ITEM);
                anyPickup = true;
            } else if (mStock[STOCK_ACTIVE].getCurrentNavTarget() >= 0) {
                s32 activeItem = mStock[STOCK_ACTIVE].getItem(mStock[STOCK_ACTIVE].getCurrentNavTarget());
                if (activeItem != 12) {
                    // ... and picking up an item from the stock
                    s32 stockSlot = mStock[STOCK_ACTIVE].getCurrentSlot();
                    mCursorLocation = dLytDepositBoxCursor_c::LOC_STOCK;
                    field_0x19540 = stockSlot;
                    item = FileManager::GetInstance()->getItemCheckItem(stockSlot);
                    mCurrentlyHoldingItemSlot = stockSlot + SLOT_STOCK_OFFSET;
                    mCursor.init(
                        mCursorLocation, stockSlot,
                        mStock[STOCK_ACTIVE].getItem(mStock[STOCK_ACTIVE].getCurrentNavTarget()), false
                    );
                    mStock[STOCK_ACTIVE].pickUpItem(stockSlot, 0);
                    loadItemText(-1, true);
                    field_0x19554 = 0;
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_CHACH_ITEM);
                    anyPickup = true;
                }
            }

            if (!anyPickup) {
                // didn't pick anything up
                return;
            }

            mAnm[DEPOSIT_ANIM_ARROW_LOOP].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_ARROW_LOOP].setFrame(0.0f);

            for (int i = 0; i < (s32)ARRAY_LENGTH(sItemData); i++) {
                if (item == sItemData[i].itemId) {
                    mItemSellValue = sItemData[i].sellValue;
                    if (mItemSellValue == 0) {
                        mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(true);
                        mAnm[DEPOSIT_ANIM_INPUT_00].setForwardOnce();
                    } else {
                        s32 frame = getItemSellValueFrame(mItemSellValue);
                        mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(true);
                        mAnm[DEPOSIT_ANIM_INPUT_00].setBackwardsOnce();
                        mAnm[DEPOSIT_ANIM_KETA].setAnimEnable(true);
                        mAnm[DEPOSIT_ANIM_KETA].setFrame(frame);
                    }
                    break;
                }
            }

            mAnm[DEPOSIT_ANIM_INPUT_01].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_INPUT_01].setForwardOnce();
        } else {
            // Currently holding an item...
            bool b2 = false;
            if (mCurrentPouchNavTarget >= 0 && mPouchItemIds[mCurrentPouchNavTarget] != 50) {
                // ... and dropping the item onto the pouch
                s32 oldItem = mPouchItemIds[mCurrentPouchNavTarget];
                if (mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) {
                    // Source item is from pouch, swap pouch items
                    mPouchItemIds[mCurrentPouchNavTarget] = mPouchItemIds[mCurrentlyHoldingItemSlot];
                    mPouchItemIds[mCurrentlyHoldingItemSlot] = oldItem;
                    swapAdventurePouchItems(
                        convertLytPouchSlot(mCurrentPouchNavTarget), convertLytPouchSlot(mCurrentlyHoldingItemSlot)
                    );
                    fn_802AAEC0(mCurrentPouchNavTarget);
                    mPouchItems[mCurrentlyHoldingItemSlot].reset();
                    fn_802AAEC0(mCurrentlyHoldingItemSlot);
                    if (mCurrentlyHoldingItemSlot == mCurrentPouchNavTarget) {
                        b2 = true;
                    }
                } else {
                    // Source item is from stock, move item from stock to pouch
                    s32 actualSlot = mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET;
                    mPouchItemIds[mCurrentPouchNavTarget] = getDepositItemForSlot(actualSlot);
                    swapStockAndPouchItems(convertLytPouchSlot(mCurrentPouchNavTarget), actualSlot);
                    loadItems(-1);
                    fn_802AAEC0(mCurrentPouchNavTarget);

                    mStock[STOCK_ACTIVE].field_0x684E = false;
                    for (int i = 0; i < NUM_STOCKS; i++) {
                        mStock[i].field_0x684E = false;
                    }
                }

                if (oldItem != 12 && !b2) {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POUCH_RELEASE_ITEM_CHANGE);
                } else {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POUCH_RELEASE_ITEM);
                }
                mItemSellValue = 0;
                anyPickup = true;
            } else if (mStock[STOCK_ACTIVE].getCurrentNavTarget() >= 0 && mStock[STOCK_ACTIVE].getActiveItem() != 50) {
                // ... and dropping the item into the stock
                s32 slot = mStock[STOCK_ACTIVE].getCurrentSlot();
                field_0x19554 = slot;
                s32 item = mStock[STOCK_ACTIVE].getActiveItem();
                if (mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) {
                    // Source item is from pouch, so move item from pouch to stock
                    swapStockAndPouchItems(convertLytPouchSlot(mCurrentlyHoldingItemSlot), field_0x19554);
                    mStock[STOCK_ACTIVE].setItem(
                        mStock[STOCK_ACTIVE].getCurrentNavTarget(), mPouchItemIds[mCurrentlyHoldingItemSlot]
                    );
                    mPouchItemIds[mCurrentlyHoldingItemSlot] = item;
                    mPouchItems[mCurrentlyHoldingItemSlot].reset();
                    fn_802AAEC0(mCurrentlyHoldingItemSlot);
                } else {
                    // Source item is from stock, so swap stock items
                    swapStockItems(mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET, slot);
                    loadItems(-1);
                    if (!dPadNav::isPointerVisible()) {
                        mStock[STOCK_ACTIVE].field_0x684B = true;
                    }
                    mStock[STOCK_ACTIVE].fn_80156530(true);
                    field_0x1954C = 1;
                    if (field_0x19554 == mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET) {
                        b2 = true;
                    }
                }

                for (int i = 0; i < NUM_STOCKS; i++) {
                    mStock[i].field_0x684E = false;
                }

                if (item != 12 && !b2) {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_RELEASE_ITEM_CHANGE);
                } else {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_RELEASE_ITEM);
                }
                mItemSellValue = 0;
                anyPickup = true;
            }
            if (anyPickup) {
                if (mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) {
                    mRingNodes[mCurrentlyHoldingItemSlot].mpPane->SetVisible(true);
                }
                mCurrentlyHoldingItemSlot = -1;
                mAnm[DEPOSIT_ANIM_ARROW_LOOP].setAnimEnable(false);
                mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(true);
                mAnm[DEPOSIT_ANIM_INPUT_00].setForwardOnce();
                mAnm[DEPOSIT_ANIM_INPUT_01].setAnimEnable(true);
                mAnm[DEPOSIT_ANIM_INPUT_01].setBackwardsOnce();
                mCursor.setVisible(false);
            }
        }

    } else {
        if (dPad::getDownTrigB() && mCurrentlyHoldingItemSlot >= 0) {
            if (mStock[STOCK_ACTIVE].isModeSell()) {
                // cancel while item over sell button
                mAnm[DEPOSIT_ANIM_RECYCLE].setAnimEnable(true);
                mAnm[DEPOSIT_ANIM_RECYCLE].setBackwardsOnce();
                mStock[STOCK_ACTIVE].disableModeSell();
                field_0x19524 = false;
                if (field_0x1954C == 0) {
                    // TODO: Why is this not >= ...
                    if (mCurrentlyHoldingItemSlot > SLOT_STOCK_OFFSET) {
                        fn_802AC0E0(0);
                    } else {
                        fn_802AC0E0(mCurrentlyHoldingItemSlot);
                    }
                } else {
                    if ((mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) ||
                        ((mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET) / 12) != mStock[STOCK_ACTIVE].getPage()) {
                        fn_802AC290(0);
                    } else {
                        fn_802AC290((mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET) % 12);
                    }
                }
            }
            if (mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) {
                mRingNodes[mCurrentlyHoldingItemSlot].mpPane->SetVisible(true);
            } else {
                mStock[STOCK_ACTIVE].pickUpItem(mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET, true);
                loadItems(-1);
                mStock[STOCK_ACTIVE].field_0x684B = true;
                mStock[STOCK_ACTIVE].fn_80156530(true);
            }
            mCurrentlyHoldingItemSlot = -1;
            mAnm[DEPOSIT_ANIM_ARROW_LOOP].setAnimEnable(false);
            mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_INPUT_00].setForwardOnce();
            mAnm[DEPOSIT_ANIM_INPUT_01].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_INPUT_01].setBackwardsOnce();
            mCursor.setVisible(false);
            mItemSellValue = 0;
            field_0x1952B = 1;
        }
    }
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

void dLytDeposit_c::initializeState_None() {}
void dLytDeposit_c::executeState_None() {
    if (field_0x19998) {
        mMain.setModeIn();
        mStateMgr.changeState(StateID_In);
        field_0x19998 = false;
    }
}
void dLytDeposit_c::finalizeState_None() {}

void dLytDeposit_c::initializeState_In() {}
void dLytDeposit_c::executeState_In() {
    if (mMain.isIdle()) {
        if (mMain.getStateStep() == 0) {
            mMain.setModeCheck();
            mStateMgr.changeState(StateID_Check);
        } else {
            mMain.setModeChange(false);
            mStateMgr.changeState(StateID_Change);
        }

        dCsBase_c::GetInstance()->setVisible(true);
        dPadNav::setNavEnabled(true, false);
        dCsBase_c::GetInstance()->setCursorStickPriority(0x86);
    }
}
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

SPECIAL_BASE_PROFILE(LYT_DEPOSIT, dLytDeposit_c, fProfile::LYT_DEPOSIT, 0x2B9, 0);

bool dLytDeposit_c::build() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("Shop");
    mResAcc.attach(data, "");
    mMain.build(&mResAcc);
    return true;
}

int dLytDeposit_c::create() {
    sInstance = this;
    allocateLytWork1Heap("dLytDeposit_c::m_allocator", false);
    mStateMgr.changeState(StateID_None);
    field_0x19998 = false;
    field_0x19999 = false;
    field_0x1999A = false;
    return SUCCEEDED;
}

int dLytDeposit_c::execute() {
    if (*mStateMgr.getStateID() != StateID_None) {
        if (dPad::getDownTrigDown()) {
            dPad::ex_c::getInstance()->centerCursor(mPad::getCurrentCoreID(), true);
        }
        if (dCsBase_c::GetInstance()->isVisible()) {
            dCsGame_c::GetInstance()->pointerDefSet();
        }
        mMain.execute();
    }
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dLytDeposit_c::draw() {
    if (*mStateMgr.getStateID() != StateID_None || field_0x19999) {
        mMain.draw();
    }
    return SUCCEEDED;
}

int dLytDeposit_c::doDelete() {
    dCsBase_c::GetInstance()->setCursorStickPriority2(0x8A);
    mMain.remove();
    mResAcc.detach();
    sInstance = nullptr;
    return SUCCEEDED;
}

bool dLytDeposit_c::checkForNavRight() {
    if (dPad::getDownTrigRight()) {
        if (!mMain.isSellableItem()) {
            field_0x1999A = false;
            return true;
        }
    }

    if (dPad::getDownTrigA() && mMain.getArrowDirection() == dLytDepositStock_c::ARROW_RIGHT) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_SELECT_TURN_PAGE_RIGHT);
        field_0x1999A = true;
        return true;
    } else if (dPadNav::isPointerVisible() && dPadNav::isMplsNavRightGesture()) {
        mMain.setField_0x1952C(false);
        field_0x1999A = false;
        return true;
    }

    return false;
}

bool dLytDeposit_c::checkForNavLeft() {
    if (dPad::getDownTrigLeft()) {
        if (!mMain.isSellableItem()) {
            field_0x1999A = false;
            return true;
        }
    }

    if (dPad::getDownTrigA() && mMain.getArrowDirection() == dLytDepositStock_c::ARROW_LEFT) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_SELECT_TURN_PAGE_LEFT);
        field_0x1999A = true;
        return true;
    } else if (dPadNav::isPointerVisible() && !mMain.isSellableItem() && dPadNav::isMplsNavLeftGesture()) {
        mMain.setField_0x1952C(false);
        field_0x1999A = false;
        return true;
    }

    return false;
}
