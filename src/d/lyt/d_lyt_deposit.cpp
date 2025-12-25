// clang-format off
// need this vtable order so that stripped vtable alignment gaps work out
#include "d/lyt/d2d.h"
// clang-format on

#include "d/lyt/d_lyt_deposit.h"

#include "common.h"
#include "d/d_cs_base.h"
#include "d/d_cs_game.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_message.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/d_pouch.h"
#include "d/d_rumble.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_common_arrow.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
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
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/math/math_types.h"
#include "nw4r/types_nw4r.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/file_manager.h"

#include <cwchar>

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
                mNextLocation = dLytDepositBoxCursor_c::LOC_POUCH;
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
                mNextLocation = dLytDepositBoxCursor_c::LOC_STOCK;
                mIsIdle = true;
            }
        }
    }
}
void dLytDepositMain_c::finalizeState_ModeIn() {}

void dLytDepositMain_c::initializeState_ModeCheck() {
    mIsPointingAtToStockBounding = false;
    mIsPointingAtToPouchBounding = false;
}
void dLytDepositMain_c::executeState_ModeCheck() {
    checkPointToStock();
    checkPouchItems();
    checkForItemPickupOrDrop();
    checkSellOrFinish();
    mIsIdle = true;
}
void dLytDepositMain_c::finalizeState_ModeCheck() {}

void dLytDepositMain_c::initializeState_ModeChange() {
    mIsPointingAtToStockBounding = false;
    mIsPointingAtToPouchBounding = false;
}
void dLytDepositMain_c::executeState_ModeChange() {
    if (mIsIdle) {
        checkSort();
        checkPointToPouch();
        if (mChangeCooldown != 0) {
            mChangeCooldown--;
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
                mIsPointingAtToPouchBounding = true;
            }
        }

        if (mStock[STOCK_ACTIVE].getCurrentNavTarget() >= 0) {
            dCsBase_c::GetInstance()->setCursorStickTargetPane(
                mpItemBoundings[mStock[STOCK_ACTIVE].getCurrentNavTarget()]
            );
            updateStockCursor();
        }
        s32 slot = mStock[STOCK_ACTIVE].getCurrentSlot();
        s32 v;
        if (mCurrentlyHoldingItemSlot >= 8 && slot == mCurrentlyHoldingItemSlot - 8) {
            v = -1;
        } else if (mStock[STOCK_ACTIVE].getCurrentNavTarget() < 0) {
            v = -1;
        } else {
            s32 item = mStock[STOCK_ACTIVE].getItem(mStock[STOCK_ACTIVE].getCurrentNavTarget());
            if (item != LYT_CMN_PouchBlank4 && item != LYT_CMN_PouchPotionHealthPlusPlusHalf && slot >= 0 &&
                slot < NUM_ICONS_PER_PAGE * NUM_PAGES) {
                v = FileManager::GetInstance()->getItemCheckItem(slot);
                if (v == 0) {
                    v = -1;
                }
            } else {
                v = -1;
            }
        }
        loadItemText(v, dLytDepositBoxCursor_c::LOC_STOCK);
        checkSellOrFinish();
        checkArrows();
    } else {
        if (mAnm[DEPOSIT_ANIM_CHANGE].isStop() && mArrow.isChangingState()) {
            mAnm[DEPOSIT_ANIM_CHANGE].setAnimEnable(false);
            mAnm[DEPOSIT_ANIM_SELECT_ON_BOX].setAnimEnable(false);
            mpBoundingBox->SetVisible(false);
            mpBoundingCleanUp->SetVisible(true);
            mStock[STOCK_ACTIVE].fn_80156530(true);
            setPouchItemsVisible(false);
            mNextLocation = dLytDepositBoxCursor_c::LOC_STOCK;
            mStock[STOCK_ACTIVE].returnToNoneMode();
            s32 target = mStock[STOCK_ACTIVE].getCurrentNavTarget();
            if (target < 0) {
                target += NUM_ICONS_PER_PAGE;
            }
            dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[target]);
            if (!dPadNav::isPointerVisible()) {
                returnCursorAfterCancellingSell(target);
            }
            showCursor();
            updateStockCursor();
            mChangeCooldown = 1;
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
            loadStockItems(mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET);
            mStock[STOCK_ACTIVE].fn_80156530(true);
            mNextLocation = dLytDepositBoxCursor_c::LOC_STOCK;
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
            mNextLocation = dLytDepositBoxCursor_c::LOC_POUCH;
            mStateStep = 1;
        }
    } else {
        showCursor();
        selectPouchSlot(4);
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
                mStock[i].nextPage(mPreventCancellingSelection);
                mStock[i].loadItems(currentlyHeldSlot);
            }
            mStock[STOCK_ACTIVE].fn_80156530(true);
            mNextLocation = dLytDepositBoxCursor_c::LOC_STOCK;
            loadPageText();
            showCursor();
            if (mStock[STOCK_ACTIVE].getArrowDirection() >= dLytCommonArrow_c::ARROW_NONE) {
                updateStockCursor();
            }
            mPreventCancellingSelection = true;
            mIsIdle = true;
        } else if (!mHasHandledScrollTempItems && mAnm[DEPOSIT_ANIM_SCROLL_R].getFrame() >= 2.0f) {
            mHasHandledScrollTempItems = true;
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
                mStock[i].prevPage(mPreventCancellingSelection);
                mStock[i].loadItems(currentlyHeldSlot);
            }
            mStock[STOCK_ACTIVE].fn_80156530(true);
            mNextLocation = dLytDepositBoxCursor_c::LOC_STOCK;
            loadPageText();
            showCursor();
            if (mStock[STOCK_ACTIVE].getArrowDirection() >= dLytCommonArrow_c::ARROW_NONE) {
                updateStockCursor();
            }
            mPreventCancellingSelection = true;
            mIsIdle = true;
        } else if (!mHasHandledScrollTempItems && mAnm[DEPOSIT_ANIM_SCROLL_L].getFrame() >= 2.0f) {
            mHasHandledScrollTempItems = true;
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
            if (mNextLocation != dLytDepositBoxCursor_c::LOC_POUCH) {
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
            mIsArrangementDone = true;
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

    buildSubpanes();
    mCsHitCheck.init(mLyt.getLayout()->GetRootPane(), 0x1, 0, 0);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitCheck);
    loadPageText();

    mCursor.build(resAcc);

    mArrow.build();
    mArrow.setState(1);

    setModeNone();

    mIsPointingAtToStockBounding = false;
    mIsPointingAtToPouchBounding = false;
    mIsFinishActive = false;
    mIsSellActive = false;
    mIsSortActive = false;
    mIsRecycleActive = false;
    field_0x19527 = false;
    mIsArrangementDone = false;
    mHasHandledScrollTempItems = false;
    mPrevPointerVisible = true;
    mDidDropCancelItem = false;
    mPreventCancellingSelection = true;

    mCurrentPouchNavTarget = -1;
    mCurrentlyHoldingItemSlot = -1;
    mStateStep = 0;
    mCursorLocation = dLytDepositBoxCursor_c::LOC_POUCH;
    mCurrentlyHoldingStockOrPouchSlot = 0;
    mItemSellValue = 0;
    field_0x19548 = -1;
    mNextLocation = dLytDepositBoxCursor_c::LOC_POUCH;
    mChangeCooldown = 0;
    mDroppedStockSlot = 0;

    return true;
}

bool dLytDepositMain_c::execute() {
    mDidDropCancelItem = false;
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
        if (mPouchItemIds[i] < LYT_CMN_PouchPotionHealthPlusPlusHalf) {
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

    mPrevPointerVisible = dPadNav::isPointerVisible();

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
    loadInitialState();

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
    showCursor();
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
        hideCursor();
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

void dLytDepositMain_c::setModeReverseChange() {
    mAnm[DEPOSIT_ANIM_CHANGE].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_CHANGE].setFrame(mAnm[DEPOSIT_ANIM_CHANGE].getLastFrame());
    mAnm[DEPOSIT_ANIM_CHANGE].setBackwardsOnce();
    mAnm[DEPOSIT_ANIM_SELECT_ON_POUCH].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_SELECT_ON_POUCH].setFrame(0.0f);
    mAnm[DEPOSIT_ANIM_SELECT_ON_POUCH].setForwardOnce();

    mLyt.calc();

    mStock[STOCK_ACTIVE].navigateOffIcon();
    mStock[STOCK_ACTIVE].setPreventCancellingSelection();
    mStock[STOCK_ACTIVE].fn_80156530(false);
    mStock[STOCK_ACTIVE].navigateToItem();
    mStock[STOCK_ACTIVE].returnToNoneMode();
    mCurrentPouchNavTarget = -4;
    hideCursor();
    mArrow.requestOut();
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_CHANGE_POUCH);
    mStateStep = 0;
    mStateMgr.changeState(StateID_ModeReverseChange);
    mIsIdle = false;
}

void dLytDepositMain_c::setModeScroll(bool leftRight, bool viaButton) {
    if (leftRight) {
        mAnm[DEPOSIT_ANIM_SCROLL_R].setAnimEnable(true);
        mAnm[DEPOSIT_ANIM_SCROLL_R].setFrame(0.0f);
    } else {
        mAnm[DEPOSIT_ANIM_SCROLL_L].setAnimEnable(true);
        mAnm[DEPOSIT_ANIM_SCROLL_L].setFrame(0.0f);
    }
    mLyt.calc();
    mStock[STOCK_ACTIVE].navigateOffIcon();
    mStock[STOCK_ACTIVE].setPreventCancellingSelection();
    mStock[STOCK_ACTIVE].fn_80156530(false);
    if (viaButton) {
        mArrow.triggerArrowPress();
    }
    hideCursor();
    mHasHandledScrollTempItems = false;
    mStateMgr.changeState(StateID_ModeScroll);
    mIsIdle = false;
}

void dLytDepositMain_c::setModeRecycle() {
    mAnm[DEPOSIT_ANIM_BTN_DECIDE_02].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_BTN_DECIDE_02].setFrame(0.0f);
    mLyt.calc();
    mStateStep = 0;
    mIsRecycleActive = true;
    field_0x19527 = true;

    if (mNextLocation == dLytDepositBoxCursor_c::LOC_POUCH) {
        setPouchItemsVisible(false);
    } else {
        mStock[STOCK_ACTIVE].fn_80156530(false);
    }
    dPadNav::setNavEnabled(false, false);
    mCursor.setVisible(false);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_SELECT_SELL);
    mStateMgr.changeState(StateID_ModeRecycle);
    mIsIdle = false;
}

void dLytDepositMain_c::setModeArrangementOut() {
    mAnm[DEPOSIT_ANIM_BTN_DECIDE_01].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_BTN_DECIDE_01].setFrame(0.0f);
    mLyt.calc();
    mIsRecycleActive = false;
    mCursorLocation = dLytDepositBoxCursor_c::LOC_STOCK;
    field_0x19527 = true;
    mStateStep = 0;
    dPadNav::setNavEnabled(false, false);
    mStateMgr.changeState(StateID_ModeArrangementOut);
    mIsIdle = false;
}

void dLytDepositMain_c::setModeOut() {
    mAnm[DEPOSIT_ANIM_BTN_DECIDE_00].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_BTN_DECIDE_00].setFrame(0.0f);
    mLyt.calc();

    if (*mStateMgr.getStateID() == StateID_ModeCheck) {
        navigateOffIcon();
    } else {
        mStock[STOCK_ACTIVE].navigateOffIcon();
    }

    mIsRecycleActive = false;
    field_0x19527 = false;

    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_SELECT_DECIDE);
    dPadNav::setNavEnabled(false, false);
    mStateMgr.changeState(StateID_ModeOut);
    mIsIdle = false;
}

void dLytDepositMain_c::loadStockItems(s32 hiddenSlot) {
    for (int i = 0; i < NUM_STOCKS; i++) {
        mStock[i].loadItems(hiddenSlot);
    }
}

static const char *sGroupName = "G_ref_00";

void dLytDepositMain_c::buildSubpanes() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DoButton");
    d2d::ResAccIf_c resAcc1;
    resAcc1.attach(data, "");

    for (int i = 0; i < RING_NUM_ITEMS; i++) {
        mRingNodes[i].mpLytPane = &mPouchItems[i];
        mPouchItems[i].build(&resAcc1, dLytCommonIconItem_c::POUCH);
        mRingList.PushBack(&mRingNodes[i]);
    }

    nw4r::lyt::GroupContainer *container = mLyt.getLayout()->GetGroupContainer();
    if (container != nullptr) {
        nw4r::lyt::Group *g = container->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mRingList);
        }
    }

    for (int i = 0; i < RING_NUM_ITEMS; i++) {
        mRingNodes[i].mpPane->SetVisible(false);
    }

    void *data2 = LayoutArcManager::GetInstance()->getLoadedData("Shop");
    d2d::ResAccIf_c resAcc2;
    resAcc2.attach(data2, "");

    static const u8 sInitialPageIndices[] = {0, 1, 4};

    bool isMainStock = true;
    for (int i = 0; i < NUM_STOCKS; i++) {
        mStockNodes[i].mpLytPane = &mStock[i];
        mStock[i].setPage(sInitialPageIndices[i]);
        mStock[i].setMainStock(isMainStock);
        mStockNodes[i].mpLytPane->build(&resAcc2);
        mStockList.PushBack(&mStockNodes[i]);
        isMainStock = false;
    }

    if (container != nullptr) {
        nw4r::lyt::Group *g = container->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mStockList);
        }
    }

    for (int i = 0; i < NUM_STOCKS; i++) {
        mStockNodes[i].mpPane->SetVisible(true);
    }
}

void dLytDepositMain_c::loadInitialState() {
    mAnm[DEPOSIT_ANIM_CHANGE].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_CHANGE].setFrame(0.0f);
    mAnm[DEPOSIT_ANIM_SELECT_ON_BOX].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_SELECT_ON_POUCH].setAnimEnable(true);

    if (!field_0x19527 || mCursorLocation == dLytDepositBoxCursor_c::LOC_POUCH) {
        mAnm[DEPOSIT_ANIM_SELECT_ON_BOX].setFrame(0.0f);
        mAnm[DEPOSIT_ANIM_SELECT_ON_POUCH].setToEnd();
    } else {
        mAnm[DEPOSIT_ANIM_SELECT_ON_BOX].setToEnd();
        mAnm[DEPOSIT_ANIM_SELECT_ON_POUCH].setFrame(0.0f);
    }

    mAnm[DEPOSIT_ANIM_BTN_ON_OFF_00].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_BTN_ON_OFF_00].setFrame(0.0f);
    mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setFrame(0.0f);
    mAnm[DEPOSIT_ANIM_RECYCLE].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_RECYCLE].setFrame(0.0f);

    mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_INPUT_00].setForwardOnce();
    mAnm[DEPOSIT_ANIM_INPUT_00].setFrame(mAnm[DEPOSIT_ANIM_INPUT_00].getLastFrame());

    mAnm[DEPOSIT_ANIM_INPUT_01].setAnimEnable(true);
    mAnm[DEPOSIT_ANIM_INPUT_01].setForwardOnce();
    mAnm[DEPOSIT_ANIM_INPUT_01].setFrame(0.0f);

    mLyt.calc();

    mAnm[DEPOSIT_ANIM_CHANGE].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_SELECT_ON_BOX].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_SELECT_ON_POUCH].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_BTN_ON_OFF_00].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_RECYCLE].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_INPUT_00].setAnimEnable(false);
    mAnm[DEPOSIT_ANIM_INPUT_01].setAnimEnable(false);

    for (int i = 0; i < RING_NUM_ITEMS; i++) {
        mPouchItemIds[i] = getPouchItemForSlot(i, false);
        f32 frame;
        if (!hasPouchSlot(i, false)) {
            frame = 0.0f;
            mRingNodes[i].mpPane->SetVisible(false);
        } else {
            frame = 1.0f;
            if (mPouchItemIds[i] != LYT_CMN_PouchPotionHealthPlusPlusHalf) {
                mRingNodes[i].mpPane->SetVisible(true);
                loadPouchItem(i);
            } else {
                mRingNodes[i].mpPane->SetVisible(false);
            }
        }
        mAnm[i + DEPOSIT_ANIM_HAVE_OFFSET].setAnimEnable(true);
        mAnm[i + DEPOSIT_ANIM_HAVE_OFFSET].setFrame(frame);
        mLyt.calc();
        mAnm[i + DEPOSIT_ANIM_HAVE_OFFSET].setAnimEnable(false);
    }
    // we'll actually be initializing mCurrentlyHoldingItemSlot to -1 right afterwards
    // so this is probably technically wrong
    loadStockItems(mCurrentlyHoldingItemSlot - 8);
    mIsPointingAtToStockBounding = false;
    mIsPointingAtToPouchBounding = false;
    mIsFinishActive = false;
    mIsSellActive = false;
    mIsSortActive = false;
    mIsRecycleActive = false;
    field_0x19527 = false;
    mIsArrangementDone = false;

    mCursorLocation = dLytDepositBoxCursor_c::LOC_POUCH;
    mCurrentlyHoldingStockOrPouchSlot = 0;
    mItemSellValue = 0;

    mCurrentPouchNavTarget = -1;
    mCurrentlyHoldingItemSlot = -1;
    field_0x19548 = -1;
}

void dLytDepositMain_c::loadPouchItem(s32 slot) {
    s32 amount = getPouchItemAmount(slot, false);
    mPouchItems[slot].setUnk(false);
    if (amount < 0) {
        mPouchItems[slot].setHasNumber(false);
    } else {
        mPouchItems[slot].setHasNumber(true);
        mPouchItems[slot].setNumber(amount);
    }
    mPouchItems[slot].setItem(mPouchItemIds[slot]);
    u8 color = getPouchItemNumberColor(slot, false);
    if (color != 3) {
        mPouchItems[slot].setNumberColor(color);
    }
    f32 durability = getPouchShieldDurability(slot, false);
    if (durability >= 0.0f) {
        mPouchItems[slot].setShieldDurability(durability);
    }
}

void dLytDepositMain_c::checkPointToStock() {
    dCursorHitCheck_c *d = dCsBase_c::GetInstance()->getHitCheck();
    mIsPointingAtToStockBounding = false;
    if (dPadNav::isPointerVisible() && d != nullptr && d->getType() == 'lyt ' &&
        static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mpBoundingBox) {
        // TODO: mIsPointingAtToStockBounding is always false at this point
        if (!mIsPointingAtToStockBounding) {
            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
        }
        mIsPointingAtToStockBounding = true;
    }
}

void dLytDepositMain_c::checkPointToPouch() {
    mIsPointingAtToPouchBounding = false;
    if (dPadNav::isPointerVisible()) {
        mCsPosition = dCsBase_c::GetInstance()->getCursorIf()->getCursorPos();
        nw4r::lyt::Pane *p = mLyt.findPane("N_ringAtari_00");
        nw4r::lyt::Size sz = p->GetSize();
        f32 radius = sz.width * 0.5f;
        radius *= radius;
        nw4r::math::MTX34 mtx = p->GetGlobalMtx();
        mVec3_c v(0.0f, 0.0f, 0.0f);
        MTXMultVec(mtx, v, v);
        // TODO maybe fake
        mVec2_c pos2 = mVec2_c(v.x, v.y);
        mVec2_c dist = pos2;
        dist -= mCsPosition;
        if (dist.squaredLength() < radius) {
            // TODO: field_0x19522 is always false at this point
            if (!mIsPointingAtToPouchBounding) {
                dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
            }
            mIsPointingAtToPouchBounding = true;
        }
    }
}

void dLytDepositMain_c::checkPouchItems() {
    bool selectIcon = false;
    s32 nextTarget = 0;
    if (dPadNav::isPointerVisible()) {
        for (nextTarget = 0; nextTarget < RING_NUM_ITEMS; nextTarget++) {
            if (mPouchItemIds[nextTarget] < LYT_CMN_PouchPotionHealthPlusPlusHalf &&
                mPouchItems[nextTarget].isCursorOver()) {
                mStock[STOCK_ACTIVE].disableSpecialModes();
                selectIcon = true;
                break;
            }
        }
    } else {
        if (!mStock[STOCK_ACTIVE].isNavModeItem()) {
            mStock[STOCK_ACTIVE].handleSpecialNavMode();
        } else if (!mStock[STOCK_ACTIVE].isModeSell() && !mStock[STOCK_ACTIVE].isModeSort() &&
                   !mStock[STOCK_ACTIVE].isModeFinish()) {
            s32 target = mCurrentPouchNavTarget;
            s32 direction = dPadNav::getFSStickNavDirection();
            if (target < 0 || mPrevPointerVisible) {
                if (direction != dPadNav::FS_STICK_NONE || dCsBase_c::GetInstance()->isCursorStickVisible()) {
                    if (target < 0) {
                        target += SLOT_STOCK_OFFSET;
                    }
                    nextTarget = target;
                    dCsBase_c::GetInstance()->setCursorStickTargetPane(mpRingBoundings[target]);
                    if (!mCursor.isVisible() && mCurrentlyHoldingItemSlot >= 0) {
                        mCursor.setVisible(true);
                    }
                    selectIcon = true;
                }
            } else {
                switch (checkNav()) {
                    case dPadNav::FS_STICK_UP:
                        nextTarget = mCurrentPouchNavTarget - 4;
                        selectIcon = true;
                        break;
                    case dPadNav::FS_STICK_RIGHT:
                        nextTarget = mCurrentPouchNavTarget + 1;
                        if (nextTarget >= 8) {
                            nextTarget = 0;
                        }
                        selectIcon = true;
                        break;
                    case dPadNav::FS_STICK_DOWN_RIGHT:
                        nextTarget = -1;
                        mStock[STOCK_ACTIVE].navigateToFinish();
                        break;
                    case dPadNav::FS_STICK_DOWN:
                        nextTarget = mCurrentPouchNavTarget + 4;
                        if (nextTarget >= 8) {
                            mIsPointingAtToStockBounding = true;
                            nextTarget = -1;
                        } else {
                            selectIcon = true;
                        }
                        break;
                    case dPadNav::FS_STICK_DOWN_LEFT:
                        nextTarget = -1;
                        mStock[STOCK_ACTIVE].navigateToSell();
                        loadCursor();
                        break;
                    case dPadNav::FS_STICK_LEFT:
                        nextTarget = mCurrentPouchNavTarget - 1;
                        if (nextTarget < 0) {
                            nextTarget = 7;
                        }
                        selectIcon = true;
                        break;
                    default:
                        nextTarget = mCurrentPouchNavTarget;
                        selectIcon = true;
                        break;
                }
                if (nextTarget >= 0) {
                    dCsBase_c::GetInstance()->setCursorStickTargetPane(mpRingBoundings[nextTarget]);
                }
            }
        }
    }

    if (selectIcon) {
        if (mCurrentPouchNavTarget != nextTarget) {
            if (mCurrentPouchNavTarget >= 0) {
                mPouchItems[mCurrentPouchNavTarget].setOff();
                mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(true);
            }
            if (mPouchItemIds[nextTarget] != LYT_CMN_PouchBlank4 &&
                mPouchItemIds[nextTarget] != LYT_CMN_PouchPotionHealthPlusPlusHalf &&
                (mCurrentlyHoldingItemSlot < 0 || mCurrentlyHoldingItemSlot != nextTarget)) {
                mPouchItems[nextTarget].setOn();
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POUCH_POINT_ITEM);
            } else {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POUCH_POINT_EMPTY);
            }

            s32 anmIdx = nextTarget + DEPOSIT_ANIM_FLASH_OFFSET;
            mAnm[anmIdx].setAnimEnable(true);
            mAnm[anmIdx].setFrame(0.0f);
            mLyt.calc();
            mAnm[anmIdx].setAnimEnable(false);
            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
            mCurrentPouchNavTarget = nextTarget;
        }
        if (mPouchItemIds[nextTarget] != LYT_CMN_PouchBlank4 &&
            mPouchItemIds[nextTarget] != LYT_CMN_PouchPotionHealthPlusPlusHalf &&
            (mCurrentlyHoldingItemSlot < 0 || mCurrentlyHoldingItemSlot != nextTarget)) {
            loadItemText(getPouchItemIdForIndex(nextTarget, false), dLytDepositBoxCursor_c::LOC_POUCH);
        } else {
            loadItemText(-1, dLytDepositBoxCursor_c::LOC_POUCH);
        }
        mStock[STOCK_ACTIVE].navigateToItem();
    } else {
        navigateOffIcon();
    }

    if (mCurrentPouchNavTarget >= 0) {
        s32 id;
        if (mCurrentlyHoldingItemSlot >= 0 && mCurrentlyHoldingItemSlot == mCurrentPouchNavTarget) {
            id = LYT_CMN_PouchBlank4;
        } else {
            id = mPouchItemIds[mCurrentPouchNavTarget];
        }
        mCursor.updateSlot(dLytDepositBoxCursor_c::LOC_POUCH, mCurrentPouchNavTarget, id, false);
    }
}

void dLytDepositMain_c::navigateOffIcon() {
    if (mCurrentPouchNavTarget >= 0) {
        // TODO why LYT_CMN_PouchPotionHealthPlusPlusHalf
        if (mPouchItemIds[mCurrentPouchNavTarget] != LYT_CMN_PouchBlank4 &&
            mPouchItemIds[mCurrentPouchNavTarget] != LYT_CMN_PouchPotionHealthPlusPlusHalf &&
            (mCurrentlyHoldingItemSlot < 0 || mCurrentlyHoldingItemSlot != mCurrentPouchNavTarget)) {
            mPouchItems[mCurrentPouchNavTarget].setOff();
            loadItemText(-1, dLytDepositBoxCursor_c::LOC_POUCH);
        }
        mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(true);
        mCurrentPouchNavTarget -= SLOT_STOCK_OFFSET;
    }
}

s32 dLytDepositMain_c::checkNav() {
    s32 direction = dPadNav::getFSStickNavDirection();
    switch (direction) {
        case dPadNav::FS_STICK_UP: {
            s32 target = mCurrentPouchNavTarget;
            if (target == 0) {
                direction = dPadNav::FS_STICK_NONE;
            } else if (target == 1 || target == 2 || target == 3) {
                direction = dPadNav::FS_STICK_LEFT;
            } else if (target == 5 || target == 6 || target == 7) {
                direction = dPadNav::FS_STICK_RIGHT;
            }
            break;
        }
        case dPadNav::FS_STICK_UP_RIGHT: {
            s32 target = mCurrentPouchNavTarget;
            if (target == 0 || target == 6 || target == 7) {
                direction = dPadNav::FS_STICK_RIGHT;
            } else if (target == 2 || target == 3 || target == 4) {
                direction = dPadNav::FS_STICK_LEFT;
            } else if (target == 5) {
                direction = dPadNav::FS_STICK_UP;
            } else {
                direction = dPadNav::FS_STICK_NONE;
            }
            break;
        }
        case dPadNav::FS_STICK_RIGHT: {
            s32 target = mCurrentPouchNavTarget;
            // wish I knew why this requires a range comparison...
            if (target >= 0 && target <= 1 || target == 7) {
                direction = dPadNav::FS_STICK_RIGHT;
            } else if (target == 2 || target == 3) {
                if (mCurrentlyHoldingItemSlot >= 0) {
                    direction = dPadNav::FS_STICK_NONE;
                } else {
                    direction = dPadNav::FS_STICK_DOWN_RIGHT;
                }
            } else if (target == 4 || target == 5) {
                direction = dPadNav::FS_STICK_LEFT;
            } else {
                direction = dPadNav::FS_STICK_UP;
            }
            break;
        }
        case dPadNav::FS_STICK_DOWN_RIGHT: {
            s32 target = mCurrentPouchNavTarget;
            if (target == 0 || target == 1) {
                direction = dPadNav::FS_STICK_RIGHT;
            } else if ((target == 2 || target == 3) && mCurrentlyHoldingItemSlot >= 0) {
                direction = dPadNav::FS_STICK_NONE;
            } else if (target == 4) {
                direction = dPadNav::FS_STICK_DOWN;
            } else if (target == 5 || target == 6) {
                direction = dPadNav::FS_STICK_LEFT;
            } else if (target == 7) {
                direction = dPadNav::FS_STICK_UP;
            }
            break;
        }
        case dPadNav::FS_STICK_DOWN: {
            s32 target = mCurrentPouchNavTarget;
            if (target == 0) {
                direction = dPadNav::FS_STICK_DOWN;
            } else if (target == 1 || target == 2 || target == 3) {
                direction = dPadNav::FS_STICK_RIGHT;
            } else if (target == 5 || target == 6 || target == 7) {
                direction = dPadNav::FS_STICK_LEFT;
            }
            break;
        }
        case dPadNav::FS_STICK_DOWN_LEFT: {
            // wish I knew why this requires a switch...
            switch (mCurrentPouchNavTarget) {
                case 0:
                case 7: direction = dPadNav::FS_STICK_LEFT; break;
                case 1:
                case 4: direction = dPadNav::FS_STICK_DOWN; break;
                case 2:
                case 3: direction = dPadNav::FS_STICK_RIGHT; break;
                case 5:
                case 6:
                    if (mItemSellValue > 0) {
                        direction = dPadNav::FS_STICK_DOWN_LEFT;
                    } else {
                        direction = dPadNav::FS_STICK_NONE;
                    }
                    break;
            }
            break;
        }
        case dPadNav::FS_STICK_LEFT: {
            // wish I knew why this requires a switch...
            switch (mCurrentPouchNavTarget) {
                case 0:
                case 1:
                case 7: direction = dPadNav::FS_STICK_LEFT; break;
                case 2: direction = dPadNav::FS_STICK_DOWN; break;
                case 3:
                case 4: direction = dPadNav::FS_STICK_RIGHT; break;
                case 5:
                case 6:
                    if (mItemSellValue > 0) {
                        direction = dPadNav::FS_STICK_DOWN_LEFT;
                    } else {
                        direction = dPadNav::FS_STICK_NONE;
                    }
                    break;
            }
            break;
        }
        case dPadNav::FS_STICK_UP_LEFT: {
            s32 target = mCurrentPouchNavTarget;
            if (target == 0 || target == 1 || target == 2) {
                direction = dPadNav::FS_STICK_LEFT;
            } else if (target == 3) {
                direction = dPadNav::FS_STICK_DOWN;
            } else if (target == 4 || target == 5 || target == 6) {
                direction = dPadNav::FS_STICK_RIGHT;
            } else {
                direction = dPadNav::FS_STICK_NONE;
            }
            break;
        }
    }

    return direction;
}

void dLytDepositMain_c::loadItemText(s32 itemId, s32 where) {
    dTextBox_c *box1, *box2;

    if (where == dLytDepositBoxCursor_c::LOC_POUCH) {
        box1 = mLyt.getTextBox("T_message_08");
        box2 = mLyt.getTextBox("T_messageS_08");
    } else {
        box1 = mLyt.getTextBox("T_nameSpace_00");
        // @bug shouldn't this be T_nameSpaceS_00 ?
        // causes a missing text shadow
        box2 = mLyt.getTextBox("T_nameSpace_00");
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

void dLytDepositMain_c::checkSellOrFinish() {
    s32 updateFlags = 0;
    dCursorHitCheck_c *d = dCsBase_c::GetInstance()->getHitCheck();
    if (dPadNav::isPointerVisible()) {
        if (d != nullptr && d->getType() == 'lyt ') {
            if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mpBoundingChoices &&
                mCurrentlyHoldingItemSlot < 0) {
                mStock[STOCK_ACTIVE].navigateToFinish();
                mStock[STOCK_ACTIVE].disableModeSort();
                mStock[STOCK_ACTIVE].disableModeSell();
                updateFlags = 1;
            } else if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mpBoundingItem12 &&
                       mItemSellValue != 0) {
                mStock[STOCK_ACTIVE].navigateToSell();
                mStock[STOCK_ACTIVE].disableModeFinish();
                mStock[STOCK_ACTIVE].disableModeSort();
                updateFlags = 2;
            }
        }
    } else {
        s32 direction = dPadNav::getFSStickNavDirection();
        if (mStock[STOCK_ACTIVE].isModeFinish()) {
            if (!mIsFinishActive || mPrevPointerVisible) {
                dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingChoices);
                updateFlags = 1;
            } else if (mNextLocation == dLytDepositBoxCursor_c::LOC_POUCH) {
                switch (direction) {
                    case dPadNav::FS_STICK_UP:
                    case dPadNav::FS_STICK_UP_LEFT:
                        mStock[STOCK_ACTIVE].disableModeFinish();
                        selectPouchSlot(2);
                        break;
                    case dPadNav::FS_STICK_LEFT:
                        mStock[STOCK_ACTIVE].disableModeFinish();
                        selectPouchSlot(3);
                        break;
                    default: updateFlags = 1; break;
                }
            } else {
                switch (direction) {
                    case dPadNav::FS_STICK_UP:
                    case dPadNav::FS_STICK_UP_RIGHT: {
                        mStock[STOCK_ACTIVE].disableModeFinish();
                        mArrow.setActiveArrow(1);
                        mStock[STOCK_ACTIVE].navigateToArrow(dLytCommonArrow_c::ARROW_RIGHT);
                        dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
                        mStock[STOCK_ACTIVE].saveArrowDirection();
                        break;
                    }
                    case dPadNav::FS_STICK_LEFT:
                    case dPadNav::FS_STICK_UP_LEFT: {
                        mStock[STOCK_ACTIVE].disableModeFinish();
                        mStock[STOCK_ACTIVE].navigateToSort();
                        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingCleanUp);
                        break;
                    }
                    default: {
                        updateFlags = 1;
                        break;
                    }
                }
            }
        } else if (mStock[STOCK_ACTIVE].isModeSell()) {
            if (!mIsSellActive || mPrevPointerVisible) {
                dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingItem12);
                mCursor.activateSell();
                updateFlags = 2;
            } else if (mNextLocation == dLytDepositBoxCursor_c::LOC_POUCH) {
                switch (direction) {
                    case dPadNav::FS_STICK_UP:
                    case dPadNav::FS_STICK_UP_RIGHT:
                        mStock[STOCK_ACTIVE].disableModeSell();
                        selectPouchSlot(6);
                        break;
                    case dPadNav::FS_STICK_RIGHT:
                        mStock[STOCK_ACTIVE].disableModeSell();
                        selectPouchSlot(5);
                        break;
                    case dPadNav::FS_STICK_DOWN_RIGHT:
                        if (mCurrentlyHoldingItemSlot < 0) {
                            mStock[STOCK_ACTIVE].disableModeSell();
                            mStock[STOCK_ACTIVE].navigateToFinish();
                            dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingChoices);
                        }
                        break;
                    default: updateFlags = 2; break;
                }
            } else {
                switch (direction) {
                    case dPadNav::FS_STICK_UP: {
                        mStock[STOCK_ACTIVE].disableModeSell();
                        mArrow.setActiveArrow(0);
                        mStock[STOCK_ACTIVE].navigateToArrow(dLytCommonArrow_c::ARROW_LEFT);
                        dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
                        break;
                    }
                    case dPadNav::FS_STICK_UP_RIGHT:
                    case dPadNav::FS_STICK_RIGHT:    {
                        mStock[STOCK_ACTIVE].disableModeSell();
                        returnCursorAfterCancellingSell(6);
                        break;
                    }
                    default: {
                        updateFlags = 2;
                        break;
                    }
                }
            }
        }
    }

    if (updateFlags & 1) {
        if (!mIsFinishActive) {
            mAnm[DEPOSIT_ANIM_BTN_ON_OFF_00].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_BTN_ON_OFF_00].setFrame(0.0f);
            mAnm[DEPOSIT_ANIM_BTN_ON_OFF_00].setForwardOnce();
            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
        }
        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingChoices);
        mIsFinishActive = true;
    } else if (mIsFinishActive) {
        mAnm[DEPOSIT_ANIM_BTN_ON_OFF_00].setAnimEnable(true);
        mAnm[DEPOSIT_ANIM_BTN_ON_OFF_00].setBackwardsOnce();
        mStock[STOCK_ACTIVE].disableModeFinish();
        mIsFinishActive = false;
    }

    if (updateFlags & 2) {
        if (!mIsSellActive && mItemSellValue != 0) {
            mAnm[DEPOSIT_ANIM_RECYCLE].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_RECYCLE].setFrame(0.0f);
            mAnm[DEPOSIT_ANIM_RECYCLE].setForwardOnce();
            mpPaneABtn->SetVisible(true);
            mpPanePrice->SetVisible(true);

            dTextBox_c *box1, *box2;
            box1 = mLyt.getTextBox("T_recycle_01");
            box2 = mLyt.getTextBox("T_recycleS_01");
            (void)calcNumDigits(mItemSellValue);
            static wchar_t buf[5];
            swprintf(buf, ARRAY_LENGTH(buf), L"%d", mItemSellValue);
            box1->setTextWithGlobalTextProcessor(buf, nullptr);
            box2->setTextWithGlobalTextProcessor(buf, nullptr);
            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
            if (mNextLocation != dLytDepositBoxCursor_c::LOC_POUCH) {
                mArrow.setBackwards(false);
            }
        }
        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingItem12);
        mIsSellActive = true;
    } else {
        if (mIsSellActive) {
            mAnm[DEPOSIT_ANIM_RECYCLE].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_RECYCLE].setBackwardsOnce();
            mStock[STOCK_ACTIVE].disableModeSell();
            mIsSellActive = false;
        }
        if (mNextLocation != dLytDepositBoxCursor_c::LOC_POUCH) {
            mArrow.setBackwards(true);
        }
    }
}

void dLytDepositMain_c::selectPouchSlot(s32 slot) {
    dCsBase_c::GetInstance()->setCursorStickTargetPane(mpRingBoundings[slot]);
    mStock[STOCK_ACTIVE].navigateToItem();
    mCurrentPouchNavTarget = slot;
    if (!dPadNav::isPointerVisible()) {
        dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
    }
    s32 item = mPouchItemIds[slot];
    if (mCurrentPouchNavTarget >= 0) {
        if (mCurrentlyHoldingItemSlot >= 0 && mCurrentlyHoldingItemSlot == mCurrentPouchNavTarget) {
            item = LYT_CMN_PouchBlank4;
        }
        mCursor.updateSlot(dLytDepositBoxCursor_c::LOC_POUCH, mCurrentPouchNavTarget, item, false);
    }
    if (item != LYT_CMN_PouchBlank4 && item != LYT_CMN_PouchPotionHealthPlusPlusHalf && !dPadNav::isPointerVisible()) {
        mPouchItems[slot].setOn();
        loadItemText(getPouchItemIdForIndex(slot, false), dLytDepositBoxCursor_c::LOC_POUCH);
    } else {
        loadItemText(-1, dLytDepositBoxCursor_c::LOC_POUCH);
    }

    mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(true);
    mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setFrame(0.0f);
    mLyt.calc();
    mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(false);
}

void dLytDepositMain_c::returnCursorAfterCancellingSell(s32 idx) {
    mStock[STOCK_ACTIVE].disableModeSell();
    if (dPadNav::isPointerVisible()) {
        idx -= 12;
    }
    mStock[STOCK_ACTIVE].selectNavTarget(idx);
    mStock[STOCK_ACTIVE].navigateToItem();
    s32 navTarget = mStock[STOCK_ACTIVE].getCurrentNavTarget();
    if (navTarget >= 0) {
        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[navTarget]);
        s32 item;
        if (mCurrentlyHoldingItemSlot >= 0 && navTarget == mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET) {
            item = LYT_CMN_PouchBlank4;
        } else {
            item = mStock[STOCK_ACTIVE].getActiveItem();
        }
        mCursor.updateSlot(dLytDepositBoxCursor_c::LOC_STOCK, navTarget, item, false);
    }
}

s32 dLytDepositMain_c::calcNumDigits(s32 value) {
    int cmp = 1000;
    int i = 4;
    for (; i > 1; i--) {
        if (value >= cmp) {
            break;
        }
        cmp /= 10;
    }
    return i;
}

void dLytDepositMain_c::checkSort() {
    bool b = false;
    dCursorHitCheck_c *d = dCsBase_c::GetInstance()->getHitCheck();
    if (dPadNav::isPointerVisible()) {
        if (d != nullptr && d->getType() == 'lyt ') {
            if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mpBoundingCleanUp &&
                mCurrentlyHoldingItemSlot < 0) {
                mStock[STOCK_ACTIVE].navigateToSort();
                mStock[STOCK_ACTIVE].disableModeSell();
                mStock[STOCK_ACTIVE].disableModeFinish();
                b = true;
            }
        }
    } else {
        if (mStock[STOCK_ACTIVE].isModeSort()) {
            if (!mIsSortActive || mPrevPointerVisible) {
                dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingCleanUp);
                b = true;
            } else {
                switch (dPadNav::getFSStickNavDirection()) {
                    case dPadNav::FS_STICK_UP:
                    case dPadNav::FS_STICK_UP_RIGHT:
                        mStock[STOCK_ACTIVE].disableModeSort();
                        returnCursorAfterCancellingSort(11);
                        break;
                    case dPadNav::FS_STICK_RIGHT:
                    case dPadNav::FS_STICK_DOWN_RIGHT:
                    case dPadNav::FS_STICK_DOWN:
                        mStock[STOCK_ACTIVE].disableModeSort();
                        mStock[STOCK_ACTIVE].navigateToFinish();
                        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingChoices);
                        break;
                    case dPadNav::FS_STICK_LEFT:
                    case dPadNav::FS_STICK_UP_LEFT:
                        mStock[STOCK_ACTIVE].disableModeSort();
                        returnCursorAfterCancellingSort(10);
                        break;
                    case dPadNav::FS_STICK_NONE:
                    case dPadNav::FS_STICK_DOWN_LEFT:
                        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingCleanUp);
                        b = true;
                        break;
                }
            }
        }
    }

    if (b) {
        if (!mIsSortActive) {
            mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setAnimEnable(true);
            mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setFrame(0.0f);
            mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setForwardOnce();
            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
        }
        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingCleanUp);
        mIsSortActive = true;
    } else if (mIsSortActive) {
        mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setAnimEnable(true);
        mAnm[DEPOSIT_ANIM_BTN_ON_OFF_01].setBackwardsOnce();
        mStock[STOCK_ACTIVE].disableModeSort();
        mIsSortActive = false;
    }
}

void dLytDepositMain_c::returnCursorAfterCancellingSort(s32 idx) {
    mStock[STOCK_ACTIVE].selectNavTarget(idx);
    s32 target = mStock[STOCK_ACTIVE].getCurrentNavTarget();
    mStock[STOCK_ACTIVE].navigateToItem();
    if (target >= 0) {
        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[target]);
    }
}

void dLytDepositMain_c::checkArrows() {
    if (dPadNav::isPointerVisible()) {
        s32 pointArrow = mArrow.getPointedAtArrow();
        if (pointArrow < dLytCommonArrow_c::ARROW_NONE) {
            if (pointArrow != mStock[STOCK_ACTIVE].getArrowDirection()) {
                mArrow.setActiveArrow(pointArrow);
                dCsBase_c::GetInstance()->setCursorStickTargetPane(mArrow.getArrowBounding(pointArrow));
                dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
                mStock[STOCK_ACTIVE].navigateToArrow(pointArrow);
                mStock[STOCK_ACTIVE].saveArrowDirection();
            }
        } else if (mStock[STOCK_ACTIVE].getArrowDirection() < dLytCommonArrow_c::ARROW_NONE) {
            mStock[STOCK_ACTIVE].navigateToArrow(pointArrow);
            mArrow.setActiveArrow(mArrow.getPointedAtArrow());
        }

    } else {
        if (mPrevPointerVisible && mStock[STOCK_ACTIVE].getSavedArrowDirection() < dLytCommonArrow_c::ARROW_NONE) {
            s32 dir = mStock[STOCK_ACTIVE].restoreArrowDirection();
            if (mStock[STOCK_ACTIVE].getArrowDirection() >= dLytCommonArrow_c::ARROW_NONE) {
                mArrow.setActiveArrow(dir);
                dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
            }
            dCsBase_c::GetInstance()->setCursorStickTargetPane(mArrow.getArrowBounding(dir));
            if (mCurrentlyHoldingItemSlot >= 0) {
                s32 slot = mCurrentlyHoldingItemSlot;
                s32 loc;
                if (slot < SLOT_STOCK_OFFSET) {
                    loc = dLytDepositBoxCursor_c::LOC_POUCH;
                } else {
                    loc = dLytDepositBoxCursor_c::LOC_STOCK;
                    slot -= SLOT_STOCK_OFFSET;
                }
                mCursor.updateSlot(loc, slot, LYT_CMN_PouchPotionHealthPlusPlusHalf, false);
            }
        } else if (mStock[STOCK_ACTIVE].getSavedArrowDirection() < dLytCommonArrow_c::ARROW_NONE) {
            s32 dir = mStock[STOCK_ACTIVE].restoreArrowDirection();
            dCsBase_c::GetInstance()->setCursorStickTargetPane(mArrow.getArrowBounding(dir));
            if (mCurrentlyHoldingItemSlot >= 0) {
                s32 slot = mCurrentlyHoldingItemSlot;
                s32 loc;
                if (slot < SLOT_STOCK_OFFSET) {
                    loc = dLytDepositBoxCursor_c::LOC_POUCH;
                } else {
                    loc = dLytDepositBoxCursor_c::LOC_STOCK;
                    slot -= SLOT_STOCK_OFFSET;
                }
                mCursor.updateSlot(loc, slot, LYT_CMN_PouchPotionHealthPlusPlusHalf, false);
            }
            mArrow.setActiveArrow(dir);
            if (!mPrevPointerVisible) {
                switch (dPadNav::getFSStickNavDirection()) {
                    case dPadNav::FS_STICK_UP_RIGHT:
                    case dPadNav::FS_STICK_RIGHT:
                        if (dir == dLytCommonArrow_c::ARROW_LEFT) {
                            returnCursorAfterCancellingArrow(0);
                        }
                        break;
                    case dPadNav::FS_STICK_DOWN_RIGHT:
                        if (dir == dLytCommonArrow_c::ARROW_LEFT) {
                            returnCursorAfterCancellingArrow(6);
                        }
                        break;
                    case dPadNav::FS_STICK_DOWN:
                        if (dir == dLytCommonArrow_c::ARROW_LEFT) {
                            if (mItemSellValue != 0) {
                                mArrow.setActiveArrow(dLytCommonArrow_c::ARROW_NONE);
                                mStock[STOCK_ACTIVE].navigateToArrow(dLytCommonArrow_c::ARROW_NONE);
                                mStock[STOCK_ACTIVE].navigateToSell();
                                dCsBase_c::GetInstance()->setCursorStickTargetPane(mpBoundingItem12);
                                loadCursor();
                            }
                        } else if (mCurrentlyHoldingItemSlot < 0) {
                            mArrow.setActiveArrow(dLytCommonArrow_c::ARROW_NONE);
                            mStock[STOCK_ACTIVE].navigateToArrow(dLytCommonArrow_c::ARROW_NONE);
                            mStock[STOCK_ACTIVE].navigateToFinish();
                            loadCursor();
                        }
                        break;
                    case dPadNav::FS_STICK_LEFT:
                    case dPadNav::FS_STICK_UP_LEFT:
                        if (dir == dLytCommonArrow_c::ARROW_RIGHT) {
                            returnCursorAfterCancellingArrow(5);
                        }
                        break;
                    case dPadNav::FS_STICK_DOWN_LEFT:
                        if (dir == dLytCommonArrow_c::ARROW_RIGHT) {
                            returnCursorAfterCancellingArrow(11);
                        }
                        break;
                }
            }
        }
    }
}

void dLytDepositMain_c::loadCursor() {
    if (mCurrentlyHoldingItemSlot >= 0) {
        s32 slot;
        s32 loc;
        s32 item;
        if (mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) {
            slot = mCurrentlyHoldingItemSlot;
            item = mPouchItemIds[mCurrentlyHoldingItemSlot];
            loc = dLytDepositBoxCursor_c::LOC_POUCH;
        } else {
            slot = mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET;
            item = mStock[STOCK_ACTIVE].getActiveItem();
            loc = dLytDepositBoxCursor_c::LOC_STOCK;
        }
        mCursor.updateSlot(loc, slot, item, mStock[STOCK_ACTIVE].isModeSell());
    }
}

void dLytDepositMain_c::returnCursorAfterCancellingArrow(s32 idx) {
    mArrow.setActiveArrow(dLytCommonArrow_c::ARROW_NONE);
    mStock[STOCK_ACTIVE].navigateToArrow(dLytCommonArrow_c::ARROW_NONE);
    mStock[STOCK_ACTIVE].selectNavTarget(idx);
    mStock[STOCK_ACTIVE].setSavedArrowDirection(dLytCommonArrow_c::ARROW_NONE);
    mStock[STOCK_ACTIVE].navigateToItem();
    if (mStock[STOCK_ACTIVE].getCurrentNavTarget() >= 0) {
        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[mStock[STOCK_ACTIVE].getCurrentNavTarget()]);
    }
}

void dLytDepositMain_c::setPouchItemsVisible(bool visible) {
    for (int i = 0; i < RING_NUM_ITEMS; i++) {
        if (mPouchItemIds[i] < LYT_CMN_PouchPotionHealthPlusPlusHalf) {
            mPouchItems[i].setVisible(visible);
        }
    }
}

void dLytDepositMain_c::checkForItemPickupOrDrop() {
    bool anyPickup = false;

    if (dPad::getDownTrigA()) {
        s32 item;
        if (mCurrentlyHoldingItemSlot < 0) {
            // Not holding an item...
            if (mCurrentPouchNavTarget >= 0 && mPouchItemIds[mCurrentPouchNavTarget] != LYT_CMN_PouchBlank4 &&
                mPouchItemIds[mCurrentPouchNavTarget] != LYT_CMN_PouchPotionHealthPlusPlusHalf) {
                // ... and picking up an item from the pouch
                mCursorLocation = dLytDepositBoxCursor_c::LOC_POUCH;
                mCurrentlyHoldingStockOrPouchSlot = convertLytPouchSlot2(mCurrentPouchNavTarget);
                item = getPouchItemIdForIndex(mCurrentPouchNavTarget, false);
                mCurrentlyHoldingItemSlot = mCurrentPouchNavTarget;
                mCursor.init(mCursorLocation, mCurrentPouchNavTarget, mPouchItemIds[mCurrentPouchNavTarget], false);
                mRingNodes[mCurrentPouchNavTarget].mpPane->SetVisible(false);
                mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(true);
                mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setFrame(0.0f);
                mLyt.calc();
                mAnm[mCurrentPouchNavTarget + DEPOSIT_ANIM_FLASH_OFFSET].setAnimEnable(false);

                mPouchItems[mCurrentPouchNavTarget].setOff();
                loadItemText(-1, dLytDepositBoxCursor_c::LOC_POUCH);
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POUCH_CHACH_ITEM);
                anyPickup = true;
            } else if (mStock[STOCK_ACTIVE].getCurrentNavTarget() >= 0) {
                s32 activeItem = mStock[STOCK_ACTIVE].getItem(mStock[STOCK_ACTIVE].getCurrentNavTarget());
                if (activeItem != LYT_CMN_PouchBlank4) {
                    // ... and picking up an item from the stock
                    s32 stockSlot = mStock[STOCK_ACTIVE].getCurrentSlot();
                    mCursorLocation = dLytDepositBoxCursor_c::LOC_STOCK;
                    mCurrentlyHoldingStockOrPouchSlot = stockSlot;
                    item = FileManager::GetInstance()->getItemCheckItem(stockSlot);
                    mCurrentlyHoldingItemSlot = stockSlot + SLOT_STOCK_OFFSET;
                    mCursor.init(
                        mCursorLocation, stockSlot,
                        mStock[STOCK_ACTIVE].getItem(mStock[STOCK_ACTIVE].getCurrentNavTarget()), false
                    );
                    mStock[STOCK_ACTIVE].pickUpOrPlaceItem(stockSlot, false);
                    loadItemText(-1, dLytDepositBoxCursor_c::LOC_STOCK);
                    mDroppedStockSlot = 0;
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
            if (mCurrentPouchNavTarget >= 0 &&
                mPouchItemIds[mCurrentPouchNavTarget] != LYT_CMN_PouchPotionHealthPlusPlusHalf) {
                // ... and dropping the item onto the pouch
                s32 oldItem = mPouchItemIds[mCurrentPouchNavTarget];
                if (mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) {
                    // Source item is from pouch, swap pouch items
                    mPouchItemIds[mCurrentPouchNavTarget] = mPouchItemIds[mCurrentlyHoldingItemSlot];
                    mPouchItemIds[mCurrentlyHoldingItemSlot] = oldItem;
                    swapAdventurePouchItems(
                        convertLytPouchSlot(mCurrentPouchNavTarget), convertLytPouchSlot(mCurrentlyHoldingItemSlot)
                    );
                    loadPouchItem(mCurrentPouchNavTarget);
                    mPouchItems[mCurrentlyHoldingItemSlot].reset();
                    loadPouchItem(mCurrentlyHoldingItemSlot);
                    if (mCurrentlyHoldingItemSlot == mCurrentPouchNavTarget) {
                        b2 = true;
                    }
                } else {
                    // Source item is from stock, move item from stock to pouch
                    s32 actualSlot = mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET;
                    mPouchItemIds[mCurrentPouchNavTarget] = getDepositItemForSlot(actualSlot);
                    swapStockAndPouchItems(convertLytPouchSlot(mCurrentPouchNavTarget), actualSlot);
                    loadStockItems(-1);
                    loadPouchItem(mCurrentPouchNavTarget);

                    for (int i = 0; i < NUM_STOCKS; i++) {
                        mStock[i].onDropItem();
                    }
                }

                if (oldItem != LYT_CMN_PouchBlank4 && !b2) {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POUCH_RELEASE_ITEM_CHANGE);
                } else {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POUCH_RELEASE_ITEM);
                }
                mItemSellValue = 0;
                anyPickup = true;
            } else if (mStock[STOCK_ACTIVE].getCurrentNavTarget() >= 0 &&
                       mStock[STOCK_ACTIVE].getActiveItem() != LYT_CMN_PouchPotionHealthPlusPlusHalf) {
                // ... and dropping the item into the stock
                mDroppedStockSlot = mStock[STOCK_ACTIVE].getCurrentSlot();
                s32 item = mStock[STOCK_ACTIVE].getActiveItem();
                if (mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) {
                    // Source item is from pouch, so move item from pouch to stock
                    swapStockAndPouchItems(convertLytPouchSlot(mCurrentlyHoldingItemSlot), mDroppedStockSlot);
                    mStock[STOCK_ACTIVE].setItem(
                        mStock[STOCK_ACTIVE].getCurrentNavTarget(), mPouchItemIds[mCurrentlyHoldingItemSlot]
                    );
                    mPouchItemIds[mCurrentlyHoldingItemSlot] = item;
                    mPouchItems[mCurrentlyHoldingItemSlot].reset();
                    loadPouchItem(mCurrentlyHoldingItemSlot);
                } else {
                    // Source item is from stock, so swap stock items
                    swapStockItems(mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET, mDroppedStockSlot);
                    loadStockItems(-1);
                    if (!dPadNav::isPointerVisible()) {
                        mStock[STOCK_ACTIVE].setPreventCancellingSelection();
                    }
                    mStock[STOCK_ACTIVE].fn_80156530(true);
                    mNextLocation = dLytDepositBoxCursor_c::LOC_STOCK;
                    if (mDroppedStockSlot == mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET) {
                        b2 = true;
                    }
                }

                for (int i = 0; i < NUM_STOCKS; i++) {
                    mStock[i].onDropItem();
                }

                if (item != LYT_CMN_PouchBlank4 && !b2) {
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
                mIsSellActive = false;
                if (mNextLocation == dLytDepositBoxCursor_c::LOC_POUCH) {
                    // TODO: Why is this not >= ...
                    if (mCurrentlyHoldingItemSlot > SLOT_STOCK_OFFSET) {
                        selectPouchSlot(0);
                    } else {
                        selectPouchSlot(mCurrentlyHoldingItemSlot);
                    }
                } else {
                    if ((mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) ||
                        ((mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET) / NUM_ICONS_PER_PAGE) !=
                            mStock[STOCK_ACTIVE].getPage()) {
                        returnCursorAfterCancellingSell(0);
                    } else {
                        returnCursorAfterCancellingSell(
                            (mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET) % NUM_ICONS_PER_PAGE
                        );
                    }
                }
            }
            if (mCurrentlyHoldingItemSlot < SLOT_STOCK_OFFSET) {
                // dropped pouch item, show pouch pane
                mRingNodes[mCurrentlyHoldingItemSlot].mpPane->SetVisible(true);
            } else {
                // dropped stock item
                mStock[STOCK_ACTIVE].pickUpOrPlaceItem(mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET, true);
                loadStockItems(-1);
                mStock[STOCK_ACTIVE].setPreventCancellingSelection();
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
            mDidDropCancelItem = true;
        }
    }
}

s32 dLytDepositMain_c::getItemSellValueFrame(s32 value) {
    if (value >= 100) {
        return 1;
    } else {
        return 0;
    }
}

void dLytDepositMain_c::loadPageText() {
    static wchar_t buf[4];
    swprintf(buf, ARRAY_LENGTH(buf), L"%1d/5", mStock[STOCK_ACTIVE].getPage() + 1);
    mLyt.getTextBox("T_page_00")->setTextWithGlobalTextProcessor(buf, nullptr);
    mLyt.getTextBox("T_pageS_00")->setTextWithGlobalTextProcessor(buf, nullptr);
}

void dLytDepositMain_c::hideCursor() {
    if (!dPadNav::isPointerVisible()) {
        dPadNav::setCursorStickInvisible();
        dCsBase_c::GetInstance()->setCursorStickVisible(false);
        if (mCurrentlyHoldingItemSlot >= 0) {
            mCursor.setVisible(false);
        }
    }
}

void dLytDepositMain_c::showCursor() {
    if (!dPadNav::isPointerVisible()) {
        dCsBase_c::GetInstance()->setCursorStickVisible(true);
    }
    dPadNav::setCursorStickVisible();
    if (mCurrentlyHoldingItemSlot >= 0) {
        mCursor.setVisible(true);
    }
}

void dLytDepositMain_c::updateStockCursor() {
    s32 navTarget = mStock[STOCK_ACTIVE].getCurrentNavTarget();
    if (navTarget < 0) {
        navTarget += 12;
    }
    s32 slot = navTarget + mStock[STOCK_ACTIVE].getPage() * NUM_ICONS_PER_PAGE;
    s32 item;
    item = mStock[STOCK_ACTIVE].getActiveItem();
    if (mCurrentlyHoldingItemSlot >= 0 && slot == mCurrentlyHoldingItemSlot - SLOT_STOCK_OFFSET) {
        item = LYT_CMN_PouchBlank4;
    } else {
        item = mStock[STOCK_ACTIVE].getItem(navTarget);
    }
    mCursor.updateSlot(dLytDepositBoxCursor_c::LOC_STOCK, slot, item, false);
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
    if (mInRequest) {
        mMain.setModeIn();
        mStateMgr.changeState(StateID_In);
        mInRequest = false;
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
void dLytDeposit_c::executeState_Check() {
    if (mMain.isIdle()) {
        if (dPad::getDownTrigA() && mMain.isSellableItem()) {
            mMain.setModeRecycle();
            mStateMgr.changeState(StateID_Recycle);
        } else if ((dPad::getDownTrigA() && mMain.getField_0x19523()) ||
                   (dPad::getDownTrigB() && !mMain.getField_0x1952B() && !mMain.isHoldingItem())) {
            mMain.setModeOut();
            mStateMgr.changeState(StateID_Out);
        } else if (mMain.isPointingAtToStockBounding()) {
            mMain.setModeChange(true);
            mStateMgr.changeState(StateID_Change);
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_CHANGE_STOCK);
        }
    }
}
void dLytDeposit_c::finalizeState_Check() {}

void dLytDeposit_c::initializeState_Change() {}
void dLytDeposit_c::executeState_Change() {
    if (mMain.isIdle()) {
        if ((dPad::getDownTrigA() && mMain.getField_0x19523()) ||
            (dPad::getDownTrigB() && !mMain.getField_0x1952B() && !mMain.isHoldingItem())) {
            mMain.setModeOut();
            mStateMgr.changeState(StateID_Out);
        } else if (checkForNavRight()) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_PAGE_RIGHT);
            mMain.setModeScroll(true, mScrollViaArrowButton);
            mStateMgr.changeState(StateID_Scroll);
            mScrollViaArrowButton = false;
        } else if (checkForNavLeft()) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_PAGE_LEFT);
            mMain.setModeScroll(false, mScrollViaArrowButton);
            mStateMgr.changeState(StateID_Scroll);
            mScrollViaArrowButton = false;
        } else if (mMain.isPointingAtToPouchBounding()) {
            mMain.setModeReverseChange();
            mStateMgr.changeState(StateID_ReverseChange);
        } else if (dPad::getDownTrigA() && mMain.isSellableItem()) {
            mMain.setModeRecycle();
            mStateMgr.changeState(StateID_Recycle);
        } else if (dPad::getDownTrigA() && mMain.getField_0x19525()) {
            // TODO: what does this do?
            if (!StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_TALKED_TO_STRICH)) {
                mMain.setModeArrangementOut();
                mStateMgr.changeState(StateID_ArrangementOut);
            } else {
                mMain.setModeArrangement();
                mStateMgr.changeState(StateID_Arrangement);
            }
            sortStock();
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_SELECT_TRIM);
        }
    }
}
void dLytDeposit_c::finalizeState_Change() {}

void dLytDeposit_c::initializeState_Arrangement() {}
void dLytDeposit_c::executeState_Arrangement() {
    if (mMain.isIdle()) {
        mMain.setModeChange(false);
        mStateMgr.changeState(StateID_Change);
    }
}
void dLytDeposit_c::finalizeState_Arrangement() {}

void dLytDeposit_c::initializeState_ReverseChange() {}
void dLytDeposit_c::executeState_ReverseChange() {
    if (mMain.isIdle()) {
        mMain.setModeCheck();
        mStateMgr.changeState(StateID_Check);
    }
}
void dLytDeposit_c::finalizeState_ReverseChange() {}

void dLytDeposit_c::initializeState_Scroll() {}
void dLytDeposit_c::executeState_Scroll() {
    if (mMain.isIdle()) {
        mMain.setModeChange(false);
        mStateMgr.changeState(StateID_Change);
    }
}
void dLytDeposit_c::finalizeState_Scroll() {}

void dLytDeposit_c::initializeState_Recycle() {}
void dLytDeposit_c::executeState_Recycle() {
    if (mMain.isIdle()) {
        mMain.setModeNone();
        mStateMgr.changeState(StateID_None);
    }
}
void dLytDeposit_c::finalizeState_Recycle() {}

void dLytDeposit_c::initializeState_ArrangementOut() {}
void dLytDeposit_c::executeState_ArrangementOut() {
    if (mMain.isIdle()) {
        mMain.setModeNone();
        mStateMgr.changeState(StateID_None);
    }
}
void dLytDeposit_c::finalizeState_ArrangementOut() {}

void dLytDeposit_c::initializeState_Out() {}
void dLytDeposit_c::executeState_Out() {
    if (mMain.isIdle()) {
        dCsBase_c::GetInstance()->setVisible(false);
        dPadNav::setNavEnabled(false, false);
        mIsMovingOut = true;
        mMain.setModeNone();
        mStateMgr.changeState(StateID_None);
    }
}
void dLytDeposit_c::finalizeState_Out() {}

SPECIAL_BASE_PROFILE(LYT_DEPOSIT, dLytDeposit_c, fProfile::LYT_DEPOSIT, 0x2B9, 0);

dLytDeposit_c *dLytDeposit_c::sInstance;

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
    mInRequest = false;
    mIsMovingOut = false;
    mScrollViaArrowButton = false;
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
    if (*mStateMgr.getStateID() != StateID_None || mIsMovingOut) {
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
            mScrollViaArrowButton = false;
            return true;
        }
    }

    if (dPad::getDownTrigA() && mMain.getArrowDirection() == dLytCommonArrow_c::ARROW_RIGHT) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_SELECT_TURN_PAGE_RIGHT);
        mScrollViaArrowButton = true;
        return true;
    } else if (dPadNav::isPointerVisible() && dPadNav::isMplsNavRightGesture()) {
        mMain.setField_0x1952C(false);
        mScrollViaArrowButton = false;
        return true;
    }

    return false;
}

bool dLytDeposit_c::checkForNavLeft() {
    if (dPad::getDownTrigLeft()) {
        if (!mMain.isSellableItem()) {
            mScrollViaArrowButton = false;
            return true;
        }
    }

    if (dPad::getDownTrigA() && mMain.getArrowDirection() == dLytCommonArrow_c::ARROW_LEFT) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_SELECT_TURN_PAGE_LEFT);
        mScrollViaArrowButton = true;
        return true;
    } else if (dPadNav::isPointerVisible() && !mMain.isSellableItem() && dPadNav::isMplsNavLeftGesture()) {
        mMain.setField_0x1952C(false);
        mScrollViaArrowButton = false;
        return true;
    }

    return false;
}
