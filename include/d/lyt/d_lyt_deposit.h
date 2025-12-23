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

private:
    void setPouchItemsVisible(bool visible);
    void fn_802AA7E0();
    void fn_802AD370();
    void fn_802A9E80();

    void fn_802AAFC0();
    void fn_802AB210();
    void fn_802ACAF0();
    void fn_802ABB60();

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

    /* 0x00A7C */ dLytDepositStock_c mStock[3];
    /* 0x143F0 */ dLytDepositBoxCursor_c mCursor;
    /* 0x14D48 */ dLytCommonIconItem_c mPouchItems[8];
    /* 0x18D88 */ dLytCommonArrow_c mArrow;
    /* 0x19458 */ d2d::SubPaneList mSubpaneList1;
    /* 0x19464 */ d2d::SubPaneListNode mNodes1[3];
    /* 0x19494 */ d2d::SubPaneList mSubpaneList2;
    /* 0x194A0 */ d2d::SubPaneListNode mNodes2[8];
    /* 0x19520 */ bool field_0x19520;
    /* 0x19521 */ bool field_0x19521;
    /* 0x19522 */ bool field_0x19522;
    /* 0x19523 */ bool field_0x19523;
    /* 0x19524 */ bool field_0x19524;
    /* 0x19525 */ bool field_0x19525;
    /* 0x19526 */ bool field_0x19526;
    /* 0x19527 */ bool field_0x19527;
    /* 0x19528 */ bool field_0x19528;
    /* 0x19529 */ bool field_0x19529;
    /* 0x1952A */ bool field_0x1952A;
    /* 0x1952B */ bool field_0x1952B;
    /* 0x1952C */ bool field_0x1952C;
    /* 0x19530 */ UNKWORD field_0x19530;
    /* 0x19534 */ UNKWORD field_0x19534;
    /* 0x19538 */ UNKWORD field_0x19538;
    /* 0x1953C */ UNKWORD field_0x1953C;
    /* 0x19540 */ UNKWORD field_0x19540;
    /* 0x19544 */ UNKWORD field_0x19544;
    /* 0x19548 */ UNKWORD field_0x19548;
    /* 0x1954C */ UNKWORD field_0x1954C;
    /* 0x19550 */ UNKWORD field_0x19550;
    /* 0x19554 */ UNKWORD field_0x19554;
    /* 0x19558 */ u8 mPouchItemIds[8];
};

class dLytDeposit_c : public dLytBase_c {
public:
    dLytDeposit_c() : mStateMgr(*this) {}

    static dLytDeposit_c *GetInstance() {
        return sInstance;
    }

private:
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
};

#endif
