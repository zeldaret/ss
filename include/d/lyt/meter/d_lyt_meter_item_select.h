#ifndef D_LYT_METER_ITEM_SELECT_H
#define D_LYT_METER_ITEM_SELECT_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"
#include "toBeSorted/effects_struct.h"

class dLytMeterItemSelectIcon_c {
public:
    dLytMeterItemSelectIcon_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterItemSelectIcon_c() {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

    void setShouldBeSelect(u32 v) {
        mShouldBeSelect = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterItemSelectIcon_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterItemSelectIcon_c, On);
    STATE_FUNC_DECLARE(dLytMeterItemSelectIcon_c, Select);
    STATE_FUNC_DECLARE(dLytMeterItemSelectIcon_c, Off);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterItemSelectIcon_c);

    /* 0x3C */ d2d::AnmGroup_c *mpAnm;
    /* 0x40 */ u32 mShouldBeSelect;
};

class dLytMeterItemSelectBlink_c {
public:
    dLytMeterItemSelectBlink_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterItemSelectBlink_c() {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterItemSelectBlink_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterItemSelectBlink_c, On);
    STATE_FUNC_DECLARE(dLytMeterItemSelectBlink_c, Select);
    STATE_FUNC_DECLARE(dLytMeterItemSelectBlink_c, Off);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterItemSelectBlink_c);

    /* 0x3C */ d2d::AnmGroup_c *mpAnm;
    /* 0x40 */ u32 mShouldBeSelect;
};

class dLytMeterItemSelectText_c {
public:
    dLytMeterItemSelectText_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterItemSelectText_c() {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterItemSelectText_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterItemSelectText_c, On);
    STATE_FUNC_DECLARE(dLytMeterItemSelectText_c, Select);
    STATE_FUNC_DECLARE(dLytMeterItemSelectText_c, Off);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterItemSelectText_c);

    /* 0x3C */ d2d::AnmGroup_c *mpAnm;
    /* 0x40 */ u32 mShouldBeSelect;
};

class dLytMeterItemSelectBtn_c {
public:
    dLytMeterItemSelectBtn_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterItemSelectBtn_c() {}

    void init(s32 i, d2d::dLytSub *lyt, nw4r::lyt::Pane *pane);
    void setVisible(bool bVisible, bool bInstant);
    void setVisible(bool bVisible);
    void setVisible2(bool bVisible);
    void execute();
    
    bool isSettled() const;

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

    u8 getField_0x57() const {
        return field_0x57;
    }

    void setField_0x57(u8 val) {
        field_0x57 = val;
    }

    bool getShouldBeVisible() const {
        return mShouldBeVisible;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterItemSelectBtn_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterItemSelectBtn_c, On);
    STATE_FUNC_DECLARE(dLytMeterItemSelectBtn_c, Visible);
    STATE_FUNC_DECLARE(dLytMeterItemSelectBtn_c, Off);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterItemSelectBtn_c);

    /* 0x3C */ d2d::dLytSub *mpLyt;
    /* 0x40 */ d2d::AnmGroup_c *mpAnm;
    /* 0x44 */ nw4r::lyt::Pane *mpPane1;
    /* 0x48 */ nw4r::lyt::Pane *mpPane2;
    /* 0x4C */ s32 mDelay;
    /* 0x50 */ s32 mIdx;
    /* 0x54 */ bool mShouldBeVisible;
    /* 0x55 */ bool mInstant;
    /* 0x56 */ u8 field_0x56;
    /* 0x57 */ u8 field_0x57;
};

class dLytMeterItemSelect_c : public d2d::dSubPane {
public:
    dLytMeterItemSelect_c();
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterItemSelect_c() {}

private:
    void fn_800F0680(u8);
    s32 fn_800F0990(u8);
    s32 fn_800F07D0(s32);
    void fn_800EFAD0(s32, bool);
    void fn_800EF7C0();
    void fn_800EF8C0(u8);
    bool fn_800F09E0();
    void fn_800F0440(s32);
    void fn_800F0700();
    bool fn_800F01B0(s32);
    bool fn_800F01E0(s32);

    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, InitWait);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, SelectIn);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, Select);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, SetIn);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, SetSpecialItemIn);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, Set);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, SetOut);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, SetNone);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, SelectOut);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, ResetIn);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, Reset);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, ResetOut);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, Unuse);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, DemoMove);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, DemoOut0);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, DemoOut1);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, FrameOffIn);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, FrameOffMove);
    STATE_FUNC_DECLARE(dLytMeterItemSelect_c, FrameOffOut);

    /* 0x0008 */ dLytMeterItemSelectIcon_c mIcon[8];
    /* 0x0228 */ dLytMeterItemSelectBlink_c mBlink;
    /* 0x026C */ dLytMeterItemSelectText_c mText[2];
    /* 0x02F4 */ dLytMeterItemSelectBtn_c mBtn[4];
    /* 0x0454 */ UI_STATE_MGR_DECLARE(dLytMeterItemSelect_c);
    /* 0x0490 */ d2d::dLytSub mLyt;
    /* 0x0524 */ d2d::AnmGroup_c mAnm[34];

    /* 0x0DA4 */ nw4r::lyt::Pane *mpPanes[17];

    /* 0x0DE8 */ nw4r::lyt::Pane *mpSomePane;
    /* 0x0DEC */ dTextBox_c *mpTextBoxes[10];
    /* 0x0E14 */ dWindow_c *mpWindows[4];
    /* 0x0E24 */ dTextBox_c *mpSizeBoxes[4];
    /* 0x0E34 */ dLytCommonIconItem_c mItemIcons[9];
    /* 0x567C */ d2d::SubPaneList mSubpaneList;
    /* 0x5688 */ d2d::SubPaneListNode mSubpanes[9];
    /* 0x5718 */ EffectsStruct mEffects;

    /* 0x574C */ s32 field_0x574C;
    /* 0x5750 */ s32 field_0x5750;
    /* 0x5754 */ s32 field_0x5754;
    /* 0x5758 */ s32 field_0x5758;
    /* 0x575C */ s32 field_0x575C;
    /* 0x5760 */ s32 field_0x5760;
    /* 0x5764 */ s32 field_0x5764;
    /* 0x5768 */ s32 field_0x5768;
    /* 0x576C */ s32 field_0x576C;
    /* 0x5770 */ s32 field_0x5770;
    /* 0x5774 */ s32 field_0x5774;
    /* 0x5778 */ f32 field_0x5778;
    /* 0x577C */ f32 field_0x577C;
    /* 0x5780 */ f32 field_0x5780;
    /* 0x5784 */ f32 field_0x5784;
    /* 0x5788 */ s32 field_0x5788;
    /* 0x578C */ s32 field_0x578C;
    /* 0x5790 */ s32 field_0x5790;
    /* 0x5794 */ u8 field_0x5794;
    /* 0x5795 */ u8 field_0x5795;
    /* 0x5796 */ u8 field_0x5796;
    /* 0x5797 */ u8 field_0x5797;
    /* 0x5798 */ u8 field_0x5798;
    /* 0x5799 */ u8 field_0x5799;
    /* 0x579A */ u8 field_0x579A;
    /* 0x579B */ u8 field_0x579B;
    /* 0x579C */ u8 field_0x579C;
    /* 0x579C */ u8 field_0x579D;
    /* 0x579C */ u8 field_0x579E;
    /* 0x579C */ u8 field_0x579F;
    /* 0x57A0 */ u8 field_0x57A0;
    /* 0x57A1 */ u8 field_0x57A1;
    /* 0x57A2 */ u8 field_0x57A2;
    /* 0x57A3 */ u8 field_0x57A3[8];
    /* 0x57AB */ u8 field_0x57AB[8];
    /* 0x57B3 */ u8 field_0x57B3;
    /* 0x57B4 */ u8 field_0x57B4;
    /* 0x57B5 */ u8 field_0x57B5;
    /* 0x57B6 */ bool field_0x57B6;
    /* 0x57B7 */ bool field_0x57B7;
    /* 0x57B8 */ u8 field_0x57B8;
    /* 0x57B9 */ u8 field_0x57B9;
    /* 0x57BA */ u8 field_0x57BA;
    /* 0x57BB */ u8 field_0x57BB;
    /* 0x57BC */ u8 field_0x57BC;
    /* 0x57BD */ u8 field_0x57BD;
    /* 0x57BE */ u8 field_0x57BE;
    /* 0x57BF */ u8 field_0x57BF;
    /* 0x57C0 */ u8 field_0x57C0;
    /* 0x57C1 */ u8 field_0x57C1;
    /* 0x57C2 */ u8 field_0x57C2;
    /* 0x57C3 */ u8 field_0x57C3;
    /* 0x57C4 */ u8 field_0x57C4;
    /* 0x57C5 */ u8 field_0x57C5;
};

#endif
