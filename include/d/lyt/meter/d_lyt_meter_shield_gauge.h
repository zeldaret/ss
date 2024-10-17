#ifndef D_LYT_METER_SHIELD_GAUGE_H
#define D_LYT_METER_SHIELD_GAUGE_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytMeterShieldGauge_c : public d2d::dSubPane {
public:
    dLytMeterShieldGauge_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterShieldGauge_c() {}

    void setContainerGroup(d2d::AnmGroup_c *g) {
        mpContainerAnmGroup = g;
    }
private:
    STATE_FUNC_DECLARE(dLytMeterShieldGauge_c, Normal);
    STATE_FUNC_DECLARE(dLytMeterShieldGauge_c, Use);
    STATE_FUNC_DECLARE(dLytMeterShieldGauge_c, ToMax);

    UI_STATE_MGR_DECLARE(dLytMeterShieldGauge_c);
    d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnmGroups[8];
    /* 0x??? */ d2d::AnmGroup_c *mpContainerAnmGroup;
    /* 0x??? */ u8 padding[0x130C4 - 0x13080];
};

#endif
