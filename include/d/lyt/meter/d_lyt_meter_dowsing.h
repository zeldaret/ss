#ifndef D_LYT_METER_DOWSING_H
#define D_LYT_METER_DOWSING_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytMeterDowsingMenuIcon_c {
public:
    dLytMeterDowsingMenuIcon_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeterDowsingMenuIcon_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterDowsingMenuIcon_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsingMenuIcon_c, On);
    STATE_FUNC_DECLARE(dLytMeterDowsingMenuIcon_c, Select);
    STATE_FUNC_DECLARE(dLytMeterDowsingMenuIcon_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterDowsingMenuIcon_c);
    u32 field_0x40;
    u32 field_0x44;
};

class dLytMeterDowsingBlink_c {
public:
    dLytMeterDowsingBlink_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDowsingBlink_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterDowsingBlink_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsingBlink_c, On);
    STATE_FUNC_DECLARE(dLytMeterDowsingBlink_c, Select);
    STATE_FUNC_DECLARE(dLytMeterDowsingBlink_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterDowsingBlink_c);
    u32 field_0x40;
    u32 field_0x44;
};

class dLytMeterDowsingInput_c {
public:
    dLytMeterDowsingInput_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDowsingInput_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterDowsingInput_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsingInput_c, On);
    STATE_FUNC_DECLARE(dLytMeterDowsingInput_c, Select);
    STATE_FUNC_DECLARE(dLytMeterDowsingInput_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterDowsingInput_c);
    u32 field_0x40;
    u32 field_0x44;
};

class dLytMeterDowsingText_c {
public:
    dLytMeterDowsingText_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDowsingText_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterDowsingText_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsingText_c, On);
    STATE_FUNC_DECLARE(dLytMeterDowsingText_c, Select);
    STATE_FUNC_DECLARE(dLytMeterDowsingText_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterDowsingText_c);
    u32 field_0x40;
    u32 field_0x44;
};

class dLytMeterDowsing_c : public d2d::dSubPane {
public:
    dLytMeterDowsing_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterDowsing_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, MenuSelectingIn);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, MenuSelecting);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, MenuSelectingOut);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, Reset);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, Unuse);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, DemoMove);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, DemoOut);

    /* 0x008 */ dLytMeterDowsingMenuIcon_c mIcon[8];
    /* 0x248 */ dLytMeterDowsingBlink_c mBlink;
    /* 0x28C */ dLytMeterDowsingInput_c mInput;
    /* 0x2D0 */ dLytMeterDowsingText_c mText[2];
    /* 0x358 */ UI_STATE_MGR_DECLARE(dLytMeterDowsing_c);

    /* 0x490 */ d2d::dLytSub mLyt;

    /* 0x428 */ d2d::AnmGroup_c mAnmGroups[29];
    /* 0xB68 */ u8 field_0xB68[0xBEC - 0xB68];
    /* 0xBEC */ dLytCommonIconItem_c mItemIcons[9];

    /* 0x5434 */ d2d::SubPaneList mNodeList;
    /* 0x5440 */ d2d::SubPaneListNode mNodes[9];
    /* 0x??? */ u8 padding[0x11E34 - 0x11DF8];
};

#endif
