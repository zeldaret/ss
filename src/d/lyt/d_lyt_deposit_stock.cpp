#include "d/lyt/d_lyt_deposit_stock.h"

#include "d/d_cs_base.h"
#include "d/d_pad_nav.h"
#include "d/d_rumble.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_arrow.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "d/lyt/d_lyt_util_items.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/lyt/lyt_group.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/file_manager.h"

STATE_DEFINE(dLytDepositStock_c, None);

void dLytDepositStock_c::initializeState_None() {}
void dLytDepositStock_c::executeState_None() {
    mIsIdle = true;
}
void dLytDepositStock_c::finalizeState_None() {}

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "depositBoxBg_00_flash.brlan", "G_flash_00"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_01"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_02"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_03"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_04"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_05"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_06"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_07"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_08"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_09"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_10"},
    {  "depositBoxBg_00_flash.brlan", "G_flash_11"},
    {   "depositBoxBg_00_loop.brlan",  "G_loop_00"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_00"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_01"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_02"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_03"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_04"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_05"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_06"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_07"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_08"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_09"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_10"},
    {"depositBoxBg_00_haveBox.brlan", "G_poach_11"},
};

#define DEPOSIT_STOCK_ANIM_FLASH_OFFSET 0
#define DEPOSIT_STOCK_ANIM_LOOP 12
#define DEPOSIT_STOCK_ANIM_HAVE_BOX_OFFSET 13

#define DEPOSIT_STOCK_NUM_ANIMS 25

bool dLytDepositStock_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("depositBoxBg_00.brlyt", nullptr);

    for (int i = 0; i < DEPOSIT_STOCK_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    int i;

    for (i = 0; i < DEPOSIT_STOCK_NUM_ANIMS; i++) {
        if (i < DEPOSIT_STOCK_ANIM_FLASH_OFFSET + NUM_ICONS_PER_PAGE) {
            mAnm[i].setAnimEnable(true);
            mAnm[i].setToEnd();
        } else {
            mAnm[i].setAnimEnable(true);
            mAnm[i].setFrame(0.0f);
        }
    }

    mLyt.calc();

    for (i = 0; i < DEPOSIT_STOCK_NUM_ANIMS; i++) {
        if (i != DEPOSIT_STOCK_ANIM_LOOP) {
            mAnm[i].setAnimEnable(false);
        }
    }

    initIcons();

    mStateMgr.changeState(StateID_None);

    mCurrentNavTarget = -12;
    mSelectedItemId = -1;
    mPickedUpIdx = -1;

    mIsSellBlocked = false;
    mIsSortBlocked = false;
    mPreventCancellingSelection = false;
    mPrevPointerVisible = true;
    mPickedUpItemOnThisPage = false;

    mArrowDirection = dLytCommonArrow_c::ARROW_NONE;

    mIsModePouch = false;
    mIsModeSell = false;
    mIsModeSort = false;
    mIsModeFinish = false;

    mSavedArrowDirection = dLytCommonArrow_c::ARROW_NONE;

    navigateToItem();

    return true;
}

bool dLytDepositStock_c::execute() {
    for (int i = 0; i < mNumSlots; i++) {
        mNodes[i].mpLytPane->execute();
    }

    if (mIsMainStock) {
        mStateMgr.executeState();
    }

    for (int i = 0; i < DEPOSIT_STOCK_ANIM_LOOP + 1; i++) {
        if (mAnm[i].isEnabled()) {
            if (i < NUM_ICONS_PER_PAGE && mAnm[i].isStop()) {
                mAnm[i].setAnimEnable(false);
            }

            mAnm[i].play();
        }
    }

    mLyt.calc();

    mPrevPointerVisible = dPadNav::isPointerVisible();

    return true;
}

bool dLytDepositStock_c::remove() {
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < NUM_ICONS_PER_PAGE; i++) {
        mNodes[i].mpLytPane->remove();
    }

    mLyt.unbindAnims();

    for (int i = 0; i < DEPOSIT_STOCK_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

static const char *sGroupName = "G_ref_00";

void dLytDepositStock_c::initIcons() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DoButton");
    d2d::ResAccIf_c resAcc;
    resAcc.attach(data, "");

    for (int i = 0; i < NUM_ICONS_PER_PAGE; i++) {
        mNodes[i].mpLytPane = &mIcons[i];
        mIcons[i].build(&resAcc, dLytCommonIconItem_c::POUCH);
        mSubpaneList.PushBack(&mNodes[i]);
    }

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mSubpaneList);
        }
    }

    for (int i = 0; i < mNumSlots; i++) {
        mNodes[i].mpPane->SetVisible(false);
    }
}

void dLytDepositStock_c::loadItems(s32 hiddenSlot) {
    s32 slot = mItemPage * NUM_ICONS_PER_PAGE;

    mNumSlots = 0;
    mPickedUpItemOnThisPage = false;

    for (int i = 0; i < NUM_ICONS_PER_PAGE; i++) {
        mIcons[i].reset();
        mItemIds[i] = getDepositItemForSlot(slot);

        f32 f = 0.0f;
        if (mItemIds[i] != LYT_CMN_PouchBlank4 && slot != hiddenSlot) {
            f = 1.0f;
        }
        loadIcon(i);
        mIcons[i].setVisible(false);
        if (slot == hiddenSlot) {
            mNodes[i].mpPane->SetVisible(false);
        } else {
            mNodes[i].mpPane->SetVisible(true);
        }

        mAnm[i + DEPOSIT_STOCK_ANIM_HAVE_BOX_OFFSET].setAnimEnable(true);
        mAnm[i + DEPOSIT_STOCK_ANIM_HAVE_BOX_OFFSET].setFrame(f);
        mLyt.calc();
        mAnm[i + DEPOSIT_STOCK_ANIM_HAVE_BOX_OFFSET].setAnimEnable(false);

        slot++;
        mNumSlots++;
    }
}

void dLytDepositStock_c::loadIcon(s32 idx) {
    s32 slot = idx + mItemPage * NUM_ICONS_PER_PAGE;
    s32 number = getDepositItemAmount(slot);

    mIcons[idx].setUnk(false);

    if (number < 0) {
        mIcons[idx].setHasNumber(false);
    } else {
        mIcons[idx].setHasNumber(true);
        mIcons[idx].setNumber(number);
    }
    mIcons[idx].setItem(mItemIds[idx]);
    u8 color = getDepositItemNumberColor(slot);
    if (color != 3) {
        mIcons[idx].setNumberColor(color);
    }
    f32 durability = getDepositShieldDurability(slot);
    if (durability >= 0.0f) {
        mIcons[idx].setShieldDurability(durability);
    }
}

void dLytDepositStock_c::nextPage(bool preventCancellingSelection) {
    mItemPage++;
    if (mItemPage >= NUM_PAGES) {
        mItemPage = 0;
    }
    mPreventCancellingSelection = preventCancellingSelection;
}

void dLytDepositStock_c::prevPage(bool preventCancellingSelection) {
    if (mItemPage != 0) {
        mItemPage--;
    } else {
        mItemPage = NUM_PAGES - 1;
    }
    mPreventCancellingSelection = preventCancellingSelection;
}

void dLytDepositStock_c::fn_80156530(bool unk) {
    for (int i = 0; i < NUM_ICONS_PER_PAGE; i++) {
        mIcons[i].setVisible(unk);
    }

    if (!mPreventCancellingSelection) {
        if (mCurrentNavTarget >= 0) {
            mAnm[mCurrentNavTarget + DEPOSIT_STOCK_ANIM_FLASH_OFFSET].setAnimEnable(true);
            mSelectedItemId = -1;
        }
        mCurrentNavTarget = -10;
    }

    mPreventCancellingSelection = false;
    mIsModePouch = false;
    mIsModeSell = false;
    if (!mIsModeSort) {
        // okay
        mIsModeSort = false;
    }
    mIsModeFinish = false;
    if (mArrowDirection >= dLytCommonArrow_c::ARROW_NONE) {
        handleSpecialNavMode();
    }
}

void dLytDepositStock_c::handleNavOrPoint() {
    bool selectIcon = false;
    s32 nextTarget = 0;
    if (dPadNav::isPointerVisible()) {
        for (nextTarget = 0; nextTarget < NUM_ICONS_PER_PAGE; nextTarget++) {
            if (mIcons[nextTarget].isCursorOver()) {
                selectIcon = true;
                saveArrowDirection();
                break;
            }
        }
    } else {
        s32 dir = checkNav();
        if (!isNavModeItem()) {
            handleSpecialNavMode();
        } else if (!mIsModePouch && !mIsModeSell && !mIsModeSort && !mIsModeFinish && mSavedArrowDirection >= 2) {
            s32 target = mCurrentNavTarget;
            if (target < 0 || mPrevPointerVisible) {
                if (dir != dPadNav::FS_STICK_NONE || dCsBase_c::GetInstance()->isCursorStickVisible()) {
                    if (target < 0) {
                        target += NUM_ICONS_PER_PAGE;
                    }
                    selectIcon = true;
                    nextTarget = target;
                }
            } else {
                switch (dir) {
                    case dPadNav::FS_STICK_UP:
                        if (target < 6) {
                            navigateToPouch();
                        } else {
                            nextTarget = target - 6;
                            selectIcon = true;
                        }
                        break;
                    case dPadNav::FS_STICK_UP_RIGHT:
                        nextTarget = target - 5;
                        selectIcon = true;
                        break;
                    case dPadNav::FS_STICK_RIGHT:
                        if (target % 6 == 5) {
                            navigateToArrow(dLytCommonArrow_c::ARROW_RIGHT);
                            saveArrowDirection();
                            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
                        } else {
                            nextTarget = target + 1;
                            selectIcon = true;
                        }
                        break;
                    case dPadNav::FS_STICK_DOWN_RIGHT:
                        nextTarget = target + 7;
                        selectIcon = true;
                        break;
                    case dPadNav::FS_STICK_DOWN:
                        if (target >= 10) {
                            navigateToSort();
                        } else if (target >= 6 && target <= 7) {
                            navigateToSell();
                        } else {
                            nextTarget = target + 6;
                            selectIcon = true;
                        }
                        break;
                    case dPadNav::FS_STICK_DOWN_LEFT:
                        nextTarget = target + 5;
                        selectIcon = true;
                        break;
                    case dPadNav::FS_STICK_LEFT:
                        if (target % 6 == 0) {
                            navigateToArrow(dLytCommonArrow_c::ARROW_LEFT);
                            saveArrowDirection();
                            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
                        } else {
                            nextTarget = target - 1;
                            selectIcon = true;
                        }
                        break;
                    case dPadNav::FS_STICK_UP_LEFT:
                        nextTarget = target - 7;
                        selectIcon = true;
                        break;
                    default:
                        nextTarget = target;
                        selectIcon = true;
                        break;
                }
            }
        }
    }

    if (selectIcon) {
        mSavedArrowDirection = dLytCommonArrow_c::ARROW_NONE;
        if (mCurrentNavTarget != nextTarget) {
            if (mCurrentNavTarget >= 0) {
                mIcons[mCurrentNavTarget].setOff();
                mAnm[mCurrentNavTarget + DEPOSIT_STOCK_ANIM_FLASH_OFFSET].setAnimEnable(true);
            }
            if (mItemIds[nextTarget] != LYT_CMN_PouchBlank4 &&
                mItemIds[nextTarget] != LYT_CMN_PouchPotionHealthPlusPlusHalf) {
                mIcons[nextTarget].setOn();
                mSelectedItemId = FileManager::sInstance->getItemCheckItem(nextTarget + mItemPage * NUM_ICONS_PER_PAGE);
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_POINT_ITEM);
            } else {
                mSelectedItemId = -1;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_POINT_EMPTY);
            }

            s32 anmIdx = nextTarget + DEPOSIT_STOCK_ANIM_FLASH_OFFSET;
            mAnm[anmIdx].setAnimEnable(true);
            mAnm[anmIdx].setFrame(0.0f);
            mLyt.calc();
            mAnm[anmIdx].setAnimEnable(false);
            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
            mCurrentNavTarget = nextTarget;
            mIsModeSell = false;
            mIsModeSort = false;
            mIsModeFinish = false;
            navigateToItem();
        }
        mNavMode = 0;
    } else {
        navigateOffIcon();
    }
}

void dLytDepositStock_c::navigateOffIcon() {
    if (mCurrentNavTarget >= 0) {
        // TODO why LYT_CMN_PouchPotionHealthPlusPlusHalf
        if (mItemIds[mCurrentNavTarget] != LYT_CMN_PouchBlank4 &&
            mItemIds[mCurrentNavTarget] != LYT_CMN_PouchPotionHealthPlusPlusHalf) {
            mIcons[mCurrentNavTarget].setOff();
            mSelectedItemId = -1;
        }
        mAnm[mCurrentNavTarget + DEPOSIT_STOCK_ANIM_FLASH_OFFSET].setAnimEnable(true);
        mCurrentNavTarget -= NUM_ICONS_PER_PAGE;
    }
}

s32 dLytDepositStock_c::checkNav() {
    // Note: Navigating UP when in the first row means switching from stock to pouch

    s32 target = mCurrentNavTarget;
    s32 direction = dPadNav::getFSStickNavDirection();
    if (target >= 0) {
        switch (direction) {
            case dPadNav::FS_STICK_UP_RIGHT:
                if (target == 11) {
                    direction = dPadNav::FS_STICK_UP;
                } else if (target < 6) {
                    if (target < 4) {
                        direction = dPadNav::FS_STICK_UP;
                    } else if (target == 5) {
                        direction = dPadNav::FS_STICK_NONE;
                    } else {
                        direction = dPadNav::FS_STICK_RIGHT;
                    }
                }
                break;
            case dPadNav::FS_STICK_DOWN_RIGHT:
                if (target == 5) {
                    direction = dPadNav::FS_STICK_DOWN;
                } else if (target >= 6) {
                    if (target >= 10) {
                        if (mIsSortBlocked) {
                            if (target == 11) {
                                direction = dPadNav::FS_STICK_NONE;
                            } else {
                                direction = dPadNav::FS_STICK_RIGHT;
                            }

                        } else {
                            direction = dPadNav::FS_STICK_DOWN;
                        }
                    } else {
                        direction = dPadNav::FS_STICK_RIGHT;
                    }
                }
                break;
            case dPadNav::FS_STICK_DOWN:
                if (target >= 6) {
                    if (target < 8) {
                        if (mIsSellBlocked) {
                            direction = dPadNav::FS_STICK_NONE;
                        }
                    } else if (target >= 10) {
                        if (mIsSortBlocked) {
                            direction = dPadNav::FS_STICK_NONE;
                        }
                    } else {
                        direction = dPadNav::FS_STICK_NONE;
                    }
                }
                break;
            case dPadNav::FS_STICK_DOWN_LEFT:
                if (target == 0 || target == 11) {
                    if (target == 11 && mIsSortBlocked) {
                        direction = dPadNav::FS_STICK_LEFT;
                    } else {
                        direction = dPadNav::FS_STICK_DOWN;
                    }
                } else if (target >= 6) {
                    if (target <= 7) {
                        if (mIsSellBlocked) {
                            if (target == 6) {
                                direction = dPadNav::FS_STICK_NONE;
                            } else {
                                direction = dPadNav::FS_STICK_LEFT;
                            }
                        } else {
                            direction = dPadNav::FS_STICK_DOWN;
                        }
                    } else if (target == 11) {
                        if (mIsSortBlocked) {
                            direction = dPadNav::FS_STICK_NONE;
                        } else {
                            direction = dPadNav::FS_STICK_DOWN;
                        }
                    } else {
                        direction = dPadNav::FS_STICK_LEFT;
                    }
                }
                break;
            case dPadNav::FS_STICK_UP_LEFT:
                if (target == 6) {
                    direction = dPadNav::FS_STICK_UP;
                } else if (target < 6) {
                    if (target == 0) {
                        direction = dPadNav::FS_STICK_NONE;
                    } else if (target == 1) {
                        direction = dPadNav::FS_STICK_LEFT;
                    } else {
                        direction = dPadNav::FS_STICK_UP;
                    }
                }
                break;
        }
    }

    return direction;
}

s32 dLytDepositStock_c::getCurrentSlot() const {
    return mCurrentNavTarget + mItemPage * NUM_ICONS_PER_PAGE;
}

void dLytDepositStock_c::setItem(s32 idx, s32 item) {
    mItemIds[idx] = item;
    realizeItem(idx);
    mIcons[idx].setOn();
    mPickedUpItemOnThisPage = false;
}

void dLytDepositStock_c::realizeItem(s32 idx) {
    f32 frame = 0.0f;
    if (mItemIds[idx] != LYT_CMN_PouchBlank4) {
        frame = 1.0f;
    }
    loadIcon(idx);
    mIcons[idx].setVisible(true);
    s32 anmIdx = idx + DEPOSIT_STOCK_ANIM_HAVE_BOX_OFFSET;
    mAnm[anmIdx].setAnimEnable(true);
    mAnm[anmIdx].setFrame(frame);
    mLyt.calc();
    mAnm[anmIdx].setAnimEnable(false);
}

void dLytDepositStock_c::selectNavTarget(s32 idx) {
    if (idx < NUM_ICONS_PER_PAGE) {
        if (mCurrentNavTarget >= 0) {
            mAnm[mCurrentNavTarget + DEPOSIT_STOCK_ANIM_FLASH_OFFSET].setAnimEnable(true);
        }
        mCurrentNavTarget = idx;

        if (mCurrentNavTarget >= 0) {
            mAnm[mCurrentNavTarget + DEPOSIT_STOCK_ANIM_FLASH_OFFSET].setAnimEnable(true);
            mAnm[mCurrentNavTarget + DEPOSIT_STOCK_ANIM_FLASH_OFFSET].setFrame(0.0f);
            mLyt.calc();
            mAnm[mCurrentNavTarget + DEPOSIT_STOCK_ANIM_FLASH_OFFSET].setAnimEnable(false);
            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
        }
    }
}

void dLytDepositStock_c::pickUpOrPlaceItem(s32 slot, bool place) {
    s32 page = slot / NUM_ICONS_PER_PAGE;
    s32 idx = slot % NUM_ICONS_PER_PAGE;
    if (place) {
        mNodes[idx].mpPane->SetVisible(place);
    }

    if (mItemPage != page) {
        if (place) {
            mPickedUpItemOnThisPage = false;
        }
    } else {
        s32 item = mItemIds[idx];
        f32 frame;
        if (place) {
            // TODO why LYT_CMN_PouchPotionHealthPlusPlusHalf
            if (item != LYT_CMN_PouchPotionHealthPlusPlusHalf && item != LYT_CMN_PouchBlank4) {
                frame = 1.0f;
                if (mPickedUpItemOnThisPage) {
                    s32 otherIdx = mPickedUpIdx % NUM_ICONS_PER_PAGE;
                    mIcons[idx].setItem(mItemIds[otherIdx]);
                    mPickedUpItemOnThisPage = false;
                }
            } else {
                frame = 0.0f;
            }
            mPickedUpIdx = -1;
        } else {
            mPickedUpIdx = slot;
            frame = 0.0f;
            mPickedUpItemOnThisPage = true;
            mIcons[idx].setOff();
            mIcons[idx].setHasNumber(false);
            mIcons[idx].setItem(LYT_CMN_PouchBlank4);
        }

        s32 anmIdx = idx + DEPOSIT_STOCK_ANIM_HAVE_BOX_OFFSET;
        mAnm[anmIdx].setAnimEnable(true);
        mAnm[anmIdx].setFrame(frame);
        mLyt.calc();
        mAnm[anmIdx].setAnimEnable(false);
    }
}

void dLytDepositStock_c::navigateToPouch() {
    mIsModePouch = true;
    mSavedArrowDirection = dLytCommonArrow_c::ARROW_NONE;
}

void dLytDepositStock_c::navigateToSell() {
    if (!mIsModeSell) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POINT_SELL);
    }
    mIsModeSell = true;
    mNavMode = NAV_SELL;
    mSavedArrowDirection = dLytCommonArrow_c::ARROW_NONE;
}

void dLytDepositStock_c::navigateToSort() {
    if (!mIsModeSort) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_STOCK_POINT_TRIM);
    }
    mIsModeSort = true;
    mNavMode = NAV_SORT;
    mSavedArrowDirection = dLytCommonArrow_c::ARROW_NONE;
}

void dLytDepositStock_c::navigateToFinish() {
    if (!mIsModeFinish) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_STK_POINT_DECIDE);
    }
    mIsModeFinish = true;
    mNavMode = NAV_FINISH;
    mSavedArrowDirection = dLytCommonArrow_c::ARROW_NONE;
}

void dLytDepositStock_c::saveArrowDirection() {
    mSavedArrowDirection = mArrowDirection;
}

void dLytDepositStock_c::navigateToArrow(s32 arrowDirection) {
    mArrowDirection = arrowDirection;
    if (mArrowDirection < dLytCommonArrow_c::ARROW_NONE) {
        mNavMode = NAV_ARROW;
    }
}

s32 dLytDepositStock_c::restoreArrowDirection() {
    if (mSavedArrowDirection < dLytCommonArrow_c::ARROW_NONE && mArrowDirection >= dLytCommonArrow_c::ARROW_NONE) {
        navigateToArrow(mSavedArrowDirection);
    }
    return mArrowDirection;
}

void dLytDepositStock_c::navigateToItem() {
    mNavMode = NAV_ITEM;
}

void dLytDepositStock_c::handleSpecialNavMode() {
    switch (mNavMode) {
        case NAV_SELL:   navigateToSell(); break;
        case NAV_SORT:   navigateToSort(); break;
        case NAV_FINISH: navigateToFinish(); break;
        case NAV_ARROW:
            if (mArrowDirection < dLytCommonArrow_c::ARROW_NONE) {
                saveArrowDirection();
            }
            break;
    }
}

bool dLytDepositStock_c::isNavModeItem() const {
    return mNavMode == NAV_ITEM;
}
