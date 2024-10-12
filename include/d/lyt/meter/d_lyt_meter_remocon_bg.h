#ifndef D_LYT_METER_REMOCON_BG_H
#define D_LYT_METER_REMOCON_BG_H

#include "d/lyt/d2d.h"
#include "d/lyt/meter/d_lyt_meter_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytMeterRemoconBg_c : public dLytMeterBase {
public:
    dLytMeterRemoconBg_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterRemoconBg_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, On);
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, Active);
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterRemoconBg_c);
    d2d::dLytSub mLyt;
    /* 0xD8 */ nw4r::lyt::Pane *mpPane;
    /* 0xDC */ u32 field_0xDC;
    /* 0xE0 */ u32 field_0xE0;
    /* 0xE4 */ u32 field_0xE4;
    /* 0xE8 */ u32 field_0xE8;
    /* 0xEC */ bool field_0xEC;
};

#endif
