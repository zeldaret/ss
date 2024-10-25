#ifndef D_LYT_METER_MINUS_BTN_H
#define D_LYT_METER_MINUS_BTN_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"
#include "toBeSorted/effects_struct.h"

// At 0x8, [8], size 0x48
class dLytMeterMinusBtnMenuIcon_c {
public:
    dLytMeterMinusBtnMenuIcon_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeterMinusBtnMenuIcon_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterMinusBtnMenuIcon_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnMenuIcon_c, On);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnMenuIcon_c, Select);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnMenuIcon_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterMinusBtnMenuIcon_c);

    u32 padding2;
    u32 padding3;
};

// At 0x248, standalone
class dLytMeterMinusBtnBlink_c {
public:
    dLytMeterMinusBtnBlink_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterMinusBtnBlink_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterMinusBtnBlink_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnBlink_c, On);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnBlink_c, Select);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnBlink_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterMinusBtnBlink_c);

    u32 padding;
    u32 padding2;
};

// At 0x28C, [2], size 0x44
class dLytMeterMinusBtnText_c {
public:
    dLytMeterMinusBtnText_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterMinusBtnText_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterMinusBtnText_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnText_c, On);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnText_c, Select);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnText_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterMinusBtnText_c);

    u32 padding;
    u32 padding2;
};

class dLytMeterMinusBtn_c : public d2d::dSubPane {
public:
    dLytMeterMinusBtn_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterMinusBtn_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, MenuSelectingIn);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, MenuSelecting);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, MenuSelectingOut);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, Unuse);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, DemoMove);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, DemoOut);

    dLytMeterMinusBtnMenuIcon_c mIcon[8];
    dLytMeterMinusBtnBlink_c mBlink;
    dLytMeterMinusBtnText_c mText[2];
    UI_STATE_MGR_DECLARE(dLytMeterMinusBtn_c);

    /* 0x490 */ d2d::dLytSub mLyt;

    /* 0x524 */ d2d::AnmGroup_c mAnmGroups[33];

    // What is here?
    u8 padding2[0xE30 - 0xDA4];

    /* 0xE34 */ dLytCommonIconItem_c mItemIcons[8];

    d2d::SubPaneList mNodeList;
    d2d::SubPaneListNode mNodes[8];
    EffectsStruct mEffects[4];

    s32 field_0x574C;
    s32 field_0x5750;
    s32 field_0x5754;
    s32 field_0x5758;
    s32 field_0x575C;
    s32 field_0x5760;
    s32 field_0x5764;
    s32 field_0x5768;
    /* 0x??? */ u8 padding[0x13730 - 0x136D8];
};

#endif
