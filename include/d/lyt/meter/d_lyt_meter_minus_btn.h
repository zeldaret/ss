#ifndef D_LYT_METER_MINUS_BTN_H
#define D_LYT_METER_MINUS_BTN_H

#include <d/lyt/d2d.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <d/lyt/d_lyt_common_icon_item.h>
#include <toBeSorted/effects_struct.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

// At 0x8, [8], size 0x48
class dLytMeterMinusBtnMenuIcon_c {
public:
    dLytMeterMinusBtnMenuIcon_c(): mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeterMinusBtnMenuIcon_c() {}
private:
    UI_STATE_MGR_DECLARE(dLytMeterMinusBtnMenuIcon_c);

    u32 padding2;
    u32 padding3;
};

// At 0x248, standalone
class dLytMeterMinusBtnBlink_c {
public:
    dLytMeterMinusBtnBlink_c(): mStateMgr(*this, sStateID::null) {}
    ~dLytMeterMinusBtnBlink_c() {}
private:
    UI_STATE_MGR_DECLARE(dLytMeterMinusBtnBlink_c);

    u32 padding;
    u32 padding2;
};

// At 0x28C, [2], size 0x44
class dLytMeterMinusBtnText_c {
public:
    dLytMeterMinusBtnText_c(): mStateMgr(*this, sStateID::null) {}
    ~dLytMeterMinusBtnText_c() {}
private:
    UI_STATE_MGR_DECLARE(dLytMeterMinusBtnText_c);

    u32 padding;
    u32 padding2;
};

class dLytMeterMinusBtn_c : public dLytMeterBase {
public:
    dLytMeterMinusBtn_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterMinusBtn_c() {}
private:

    dLytMeterMinusBtnMenuIcon_c mIcon[8];
    dLytMeterMinusBtnBlink_c mBlink;
    dLytMeterMinusBtnText_c mText[2];
    UI_STATE_MGR_DECLARE(dLytMeterMinusBtn_c);

    /* 0x490 */ d2d::dLytSub mLyt;

    /* 0x524 */ d2d::AnmGroup_c mAnmGroups[33];

    // What is here?
    u8 padding2[0xE30 - 0xDA4];

    /* 0xE34 */ dLytCommonIconItem_c mItemIcons[8];

    LytMeterGroup mNodeList;
    LytMeterListNode mNodes[8];
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
