#ifndef D_LYT_METER_ITEM_SELECT_H
#define D_LYT_METER_ITEM_SELECT_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "toBeSorted/effects_struct.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytMeterItemSelectIcon_c {
public:
    dLytMeterItemSelectIcon_c(): mStateMgr(*this, sStateID::null) {}
    ~dLytMeterItemSelectIcon_c() {}
private:
    UI_STATE_MGR_DECLARE(dLytMeterItemSelectIcon_c);

    u32 padding;
    u32 padding2;
};

class dLytMeterItemSelectBlink_c {
public:
    dLytMeterItemSelectBlink_c(): mStateMgr(*this, sStateID::null) {}
    ~dLytMeterItemSelectBlink_c() {}
private:
    UI_STATE_MGR_DECLARE(dLytMeterItemSelectBlink_c);

    u32 padding;
    u32 padding2;
};

class dLytMeterItemSelectText_c {
public:
    dLytMeterItemSelectText_c(): mStateMgr(*this, sStateID::null) {}
    ~dLytMeterItemSelectText_c() {}
private:
    UI_STATE_MGR_DECLARE(dLytMeterItemSelectText_c);

    u32 padding;
    u32 padding2;
};

class dLytMeterItemSelectBtn_c {
public:
    dLytMeterItemSelectBtn_c(): mStateMgr(*this, sStateID::null) {}
    ~dLytMeterItemSelectBtn_c() {}
private:
    UI_STATE_MGR_DECLARE(dLytMeterItemSelectBtn_c);

    u8 padding[0x58 - 0x3C];
};

class dLytMeterItemSelect_c : public d2d::dSubPane {
public:
    dLytMeterItemSelect_c();
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterItemSelect_c() {}
private:
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

    dLytMeterItemSelectIcon_c mIcon[8];
    dLytMeterItemSelectBlink_c mBlink;
    dLytMeterItemSelectText_c mText[2];
    dLytMeterItemSelectBtn_c mBtn[4];
    UI_STATE_MGR_DECLARE(dLytMeterItemSelect_c);

    /* 0x490 */ d2d::dLytSub mLyt;

    /* 0x524 */ d2d::AnmGroup_c mAnmGroups[34];

    // What is here?
    u8 padding2[0xE34 - 0xDA4]; 

    /* 0xE34 */ dLytCommonIconItem_c mItemIcons[9];

    d2d::SubPaneList mNodeList;
    d2d::SubPaneListNode mNodes[9];
    EffectsStruct mEffects;
    
    s32 field_0x574C;
    s32 field_0x5750;
    s32 field_0x5754;
    s32 field_0x5758;
    s32 field_0x575C;
    s32 field_0x5760;
    s32 field_0x5764;
    s32 field_0x5768;

    /* 0x??? */ u8 padding[0x7260 - 0x7204];
};

#endif
