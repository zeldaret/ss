#ifndef D_LYT_METER_Z_BTN_H
#define D_LYT_METER_Z_BTN_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytMeterZBtn_c : public d2d::dSubPane {
public:
    dLytMeterZBtn_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterZBtn_c() {}
private:
    STATE_FUNC_DECLARE(dLytMeterZBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterZBtn_c, On);
    STATE_FUNC_DECLARE(dLytMeterZBtn_c, Active);
    STATE_FUNC_DECLARE(dLytMeterZBtn_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterZBtn_c);
    d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x??? */ u8 padding[0x121C8 - 0x12198];
};

#endif
