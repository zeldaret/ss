#ifndef D_LYT_METER_GANBARI_GAUGE_H
#define D_LYT_METER_GANBARI_GAUGE_H

#include <d/lyt/d2d.h>
#include <d/lyt/d_lyt_sub.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytMeterGanbariGauge_c : public dLytMeterBase {
public:
    dLytMeterGanbariGauge_c() : mStateMgr1(*this, sStateID::null), mStateMgr2(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterGanbariGauge_c() {}
private:
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, InvisibleWait);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, In);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, OutWait);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, Out);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, CameraOut);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, Full);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, FullGutsUse);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, Normal);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, Caution);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, ToMin);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, Recovery);
    STATE_FUNC_DECLARE(dLytMeterGanbariGauge_c, ToMax);

    STATE_MGR(dLytMeterGanbariGauge_c) mStateMgr1;
    STATE_MGR(dLytMeterGanbariGauge_c) mStateMgr2;

    /* 0x080 */ dLytSub mLyt;
    /* 0x114 */ d2d::AnmGroup_c mAnmGroups[16];
};


#endif
