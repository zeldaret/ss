#ifndef D_LYT_BIRD_GAUGE_H
#define D_LYT_BIRD_GAUGE_H

#include <d/lyt/d2d.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <m/m_color.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytBirdGaugeMain_c {
public:
    dLytBirdGaugeMain_c();
    virtual ~dLytBirdGaugeMain_c() {}
private:
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeNone);
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeIn);
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeDash);
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeRecovery);
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeOut);

    /* 0x04 */ UI_STATE_MGR_DECLARE(dLytBirdGaugeMain_c);
    /* 0x40 */ d2d::dLytSub mLyt;
    /* 0xD4 */ d2d::AnmGroup_c mAnmGroups[8];
};

class dLytBirdGauge_c : public dLytMeterBase {
public:
    dLytBirdGauge_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytBirdGauge_c() {}

private:
    STATE_FUNC_DECLARE(dLytBirdGauge_c, In);
    STATE_FUNC_DECLARE(dLytBirdGauge_c, None);
    STATE_FUNC_DECLARE(dLytBirdGauge_c, Dash);
    STATE_FUNC_DECLARE(dLytBirdGauge_c, Recovery);
    STATE_FUNC_DECLARE(dLytBirdGauge_c, Out);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytBirdGauge_c);
    /* 0x008 */ d2d::ResAccIf_c mResAcc;
    /* 0x3B4 */ dLytBirdGaugeMain_c mMain;
};

#endif
