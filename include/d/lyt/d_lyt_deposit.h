#ifndef D_LYT_DEPOSIT_H
#define D_LYT_DEPOSIT_H

#include "d/d_cursor_hit_check.h"
#include "d/d_pad_nav.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_base.h"
#include "d/lyt/d_lyt_common_arrow.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_lyt_deposit_box_cursor.h"
#include "d/lyt/d_lyt_deposit_stock.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytDepositMain_c {
public:
    dLytDepositMain_c();
    virtual ~dLytDepositMain_c() {
        dPadNav::setNavEnabled(false, false);
    }

    bool build(d2d::ResAccIf_c *resAcc);
    bool execute();
    bool draw();
    bool remove();

    void setModeNone();
    void setModeIn();
    void setModeCheck();
    void setModeChange(bool unk);
    void setModeArrangement();

    bool isSellableItem() const {
        return field_0x19524 && mItemSellValue > 0;
    }

    s32 getArrowDirection() const {
        return mStock[STOCK_ACTIVE].getArrowDirection();
    }

    void setField_0x1952C(bool b) {
        field_0x1952C = b;
    }

    s32 getStateStep() const {
        return mStateStep;
    }

    bool isIdle() const {
        return mIsIdle;
    }

private:
    static const s32 NUM_STOCKS = 3;
    static const s32 STOCK_ACTIVE = 0;

    static const s32 RING_NUM_ITEMS = 8;

    static const s32 SLOT_STOCK_OFFSET = 8;

    static const s32 NUM_ICONS_PER_PAGE = 12;
    static const s32 NUM_PAGES = 5;

    void setPouchItemsVisible(bool visible);
    void fn_802AA7E0();
    void fn_802AD370();

    void fn_802AAFC0();
    void fn_802AB210();
    void checkForItemPickupOrDrop();
    void fn_802ABB60();

    void fn_802AB080();
    void fn_802AC3C0();
    void fn_802AD4C0();
    void loadItemText(s32 itemId, s32 where);
    void fn_802AC670();
    void fn_802AC290(s32);
    void fn_802AD460();
    void loadItems(s32 hiddenSlot);
    s32 getItemSellValueFrame(s32 value);
    void fn_802AC0E0(s32);
    void fn_802AAEC0(s32);
    void fn_8018ADA0(s32);
    void fn_802AAB30();
    void fn_802AD400();

    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeNone);
    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeIn);
    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeCheck);
    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeChange);
    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeArrangement);
    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeReverseChange);
    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeScroll);
    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeRecycle);
    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeArrangementOut);
    STATE_FUNC_DECLARE(dLytDepositMain_c, ModeOut);

    /* 0x00004 */ UI_STATE_MGR_DECLARE(dLytDepositMain_c);
    /* 0x00040 */ d2d::LytBase_c mLyt;
    /* 0x000D0 */ d2d::AnmGroup_c mAnm[36];

    /* 0x009D0 */ nw4r::lyt::Bounding *mpBoundingBox;
    /* 0x009D4 */ nw4r::lyt::Bounding *mpBoundingChoices;
    /* 0x009D8 */ nw4r::lyt::Bounding *mpBoundingCleanUp;
    /* 0x009DC */ nw4r::lyt::Bounding *mpBoundingItem12;
    /* 0x009E0 */ nw4r::lyt::Bounding *mpItemBoundings[12];
    /* 0x00A10 */ nw4r::lyt::Bounding *mpRingBoundings[8];
    /* 0x00A30 */ nw4r::lyt::Pane *mpPaneABtn;
    /* 0x00A34 */ nw4r::lyt::Pane *mpPanePrice;
    /* 0x00A38 */ nw4r::lyt::Pane *mpPaneCommonArrow;
    /* 0x00A3C */ dWindow_c *mpWindows[2];
    /* 0x00A44 */ dTextBox_c *mpSizeBoxes[2];
    /* 0x00A4C */ dCursorHitCheckLyt_c mCsHitCheck;

    u8 _0x00002[0x8];

    /* 0x00A7C */ dLytDepositStock_c mStock[NUM_STOCKS];
    /* 0x143F0 */ dLytDepositBoxCursor_c mCursor;
    /* 0x14D48 */ dLytCommonIconItem_c mPouchItems[RING_NUM_ITEMS];
    /* 0x18D88 */ dLytCommonArrow_c mArrow;
    /* 0x19458 */ d2d::SubPaneList mStockList;
    /* 0x19464 */ d2d::SubPaneListNode mStockNodes[NUM_STOCKS];
    /* 0x19494 */ d2d::SubPaneList mRingList;
    /* 0x194A0 */ d2d::SubPaneListNode mRingNodes[RING_NUM_ITEMS];
    /* 0x19520 */ bool mIsIdle;
    /* 0x19521 */ bool field_0x19521;
    /* 0x19522 */ bool field_0x19522;
    /* 0x19523 */ bool field_0x19523;
    /* 0x19524 */ bool field_0x19524;
    /* 0x19525 */ bool field_0x19525;
    /* 0x19526 */ bool field_0x19526;
    /* 0x19527 */ bool field_0x19527;
    /* 0x19528 */ bool field_0x19528;
    /* 0x19529 */ bool mHasScrolledOtherStocks;
    /* 0x1952A */ bool field_0x1952A;
    /* 0x1952B */ bool field_0x1952B;
    /* 0x1952C */ bool field_0x1952C;
    /* 0x19530 */ s32 mCurrentPouchNavTarget;
    /* 0x19534 */ s32 mCurrentlyHoldingItemSlot;
    /* 0x19538 */ s32 mStateStep;
    /* 0x1953C */ s32 mCursorLocation;
    /* 0x19540 */ UNKWORD field_0x19540;
    /* 0x19544 */ s32 mItemSellValue;
    /* 0x19548 */ UNKWORD field_0x19548;
    /* 0x1954C */ UNKWORD field_0x1954C;
    /* 0x19550 */ UNKWORD field_0x19550;
    /* 0x19554 */ UNKWORD field_0x19554;
    /* 0x19558 */ u8 mPouchItemIds[RING_NUM_ITEMS];
};

class dLytDeposit_c : public dLytBase_c {
public:
    dLytDeposit_c() : mStateMgr(*this) {}
    virtual ~dLytDeposit_c() {
        removeLyt();
    }

    static dLytDeposit_c *GetInstance() {
        return sInstance;
    }

    virtual bool build() override;
    virtual int create() override;
    virtual int execute() override;
    virtual int draw() override;
    virtual int doDelete() override;

private:
    bool checkForNavRight();
    bool checkForNavLeft();

    static dLytDeposit_c *sInstance;

    STATE_FUNC_DECLARE(dLytDeposit_c, None);
    STATE_FUNC_DECLARE(dLytDeposit_c, In);
    STATE_FUNC_DECLARE(dLytDeposit_c, Check);
    STATE_FUNC_DECLARE(dLytDeposit_c, Change);
    STATE_FUNC_DECLARE(dLytDeposit_c, Arrangement);
    STATE_FUNC_DECLARE(dLytDeposit_c, ReverseChange);
    STATE_FUNC_DECLARE(dLytDeposit_c, Scroll);
    STATE_FUNC_DECLARE(dLytDeposit_c, Recycle);
    STATE_FUNC_DECLARE(dLytDeposit_c, ArrangementOut);
    STATE_FUNC_DECLARE(dLytDeposit_c, Out);

    /* 0x08C */ UI_STATE_MGR_DECLARE(dLytDeposit_c);
    /* 0x0C8 */ d2d::ResAccIf_c mResAcc;
    /* 0x438 */ dLytDepositMain_c mMain;
    /* 0x19998 */ bool field_0x19998;
    /* 0x19999 */ bool field_0x19999;
    /* 0x1999A */ bool field_0x1999A;
};

#endif
