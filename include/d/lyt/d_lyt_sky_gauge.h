#ifndef D_LYT_SKY_GAUGE_H
#define D_LYT_SKY_GAUGE_H

#include <d/lyt/d2d.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <m/m_color.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytSkyGaugeMain_c {
public:
    dLytSkyGaugeMain_c();
    virtual ~dLytSkyGaugeMain_c() {}
private:
    STATE_FUNC_DECLARE(dLytSkyGaugeMain_c, ModeNone);
    STATE_FUNC_DECLARE(dLytSkyGaugeMain_c, ModeIn);
    STATE_FUNC_DECLARE(dLytSkyGaugeMain_c, ModeMove);
    STATE_FUNC_DECLARE(dLytSkyGaugeMain_c, ModeOut);

    /* 0x04 */ UI_STATE_MGR_DECLARE(dLytSkyGaugeMain_c);
    /* 0x40 */ d2d::dLytSub mLyt;
    /* 0xD4 */ d2d::AnmGroup_c mAnmGroups[3];
};

class dLytSkyGauge_c : public dLytMeterBase {
public:
    dLytSkyGauge_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;

    virtual ~dLytSkyGauge_c() {}

private:
    STATE_FUNC_DECLARE(dLytSkyGauge_c, ModeNone);
    STATE_FUNC_DECLARE(dLytSkyGauge_c, ModeIn);
    STATE_FUNC_DECLARE(dLytSkyGauge_c, ModeMove);
    STATE_FUNC_DECLARE(dLytSkyGauge_c, ModeOut);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytSkyGauge_c);
    /* 0x008 */ d2d::ResAccIf_c mResAcc;
    /* 0x3B4 */ dLytSkyGaugeMain_c mMain;
};

#endif
