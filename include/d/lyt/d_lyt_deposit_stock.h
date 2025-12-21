#ifndef D_LYT_DEPOSIT_STOCK_H
#define D_LYT_DEPOSIT_STOCK_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytDepositStock_c : public d2d::dSubPane {
public:
    enum ArrowDirection_e {
        ARROW_LEFT,
        ARROW_RIGHT,

        ARROW_NONE,
    };

    enum NavMode_e {
        NAV_ITEM,
        NAV_SELL,
        NAV_SORT,
        NAV_FINISH,
        NAV_ARROW,
    };

public:
    dLytDepositStock_c() : mStateMgr(*this) {}
    virtual ~dLytDepositStock_c() {}

    /* vt 0x0C */ virtual bool build(d2d::ResAccIf_c *resAcc) override;
    /* vt 0x10 */ virtual bool remove() override;
    /* vt 0x14 */ virtual bool execute() override;
    /* vt 0x18 */ virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    /* vt 0x1C */ virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    /* vt 0x20 */ virtual const char *getName() const override {
        return mLyt.getName();
    }

    void nextPage(bool unk);
    void prevPage(bool unk);
    void fn_80156530(bool unk);

    void navigateToSell();
    void navigateToSort();
    void navigateToFinish();
    void saveArrowDirection();
    void navigateToArrow(s32 arrowDirection);
    s32 restoreArrowDirection();
    void handleSpecialNavMode();
    bool isNavModeItem() const;
    void navigateOffIcon();
    void handleNavOrPoint();

    void pickUpItem(s32 slot, bool unk);
    void selectNavTarget(s32 idx);
    void setItem(s32 idx, s32 item);
    s32 getCurrentSlot() const;

    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dLytDepositStock_c);

private:
    static const s32 NUM_ICONS_PER_PAGE = 12;
    static const s32 NUM_PAGES = 5;

    void initIcons();
    void navigateToItem();
    void loadItems(s32 hiddenSlot);
    void loadIcon(s32 idx);
    void navigateToPouch();
    void realizeItem(s32 idx);

    /**
     * Check if the FS stick is pushed in a direction. Diagonal movements
     * are adjusted to horizontal and vertical navigations if necessary.
     * @returns FS stick direction
     */
    s32 checkNav();

    STATE_FUNC_DECLARE(dLytDepositStock_c, None);

    /* 0x0008 */ UI_STATE_MGR_DECLARE(dLytDepositStock_c);
    /* 0x0044 */ d2d::dLytSub mLyt;
    /* 0x00D8 */ d2d::AnmGroup_c mAnm[25];
    /* 0x0718 */ dLytCommonIconItem_c mIcons[NUM_ICONS_PER_PAGE];
    /* 0x6778 */ d2d::SubPaneList mSubpaneList;
    /* 0x6784 */ d2d::SubPaneListNode mNodes[NUM_ICONS_PER_PAGE];
    /* 0x6844 */ bool field_0x6844;
    /* 0x6845 */ bool field_0x6845;
    /* 0x6846 */ bool field_0x6846;
    /* 0x6847 */ bool field_0x6847;
    /* 0x6848 */ bool field_0x6848;
    /* 0x6849 */ bool field_0x6849;
    /* 0x684A */ bool field_0x684A;
    /* 0x684B */ bool field_0x684B;
    /* 0x684C */ bool field_0x684C;
    /* 0x684D */ bool field_0x684D;
    /* 0x684E */ bool field_0x684E;
    /* 0x6850 */ s32 mCurrentNavTarget;
    /* 0x6854 */ UNKWORD field_0x6854;
    /* 0x6858 */ s32 mSelectedItemId;
    /* 0x685C */ UNKWORD field_0x685C;
    /* 0x6860 */ s32 mArrowDirection;
    /* 0x6864 */ s32 mSavedArrowDirection;
    /* 0x6868 */ s32 mNavMode;
    /* 0x686C */ u8 mItemIds[NUM_ICONS_PER_PAGE];
    /* 0x6878 */ u8 mItemPage;
};

#endif
