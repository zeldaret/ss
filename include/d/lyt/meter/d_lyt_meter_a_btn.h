#ifndef D_LYT_METER_A_BTN_H
#define D_LYT_METER_A_BTN_H

#include <d/lyt/d2d.h>
#include <d/lyt/d_lyt_sub.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytMeterABtn_c : public dLytMeterBase {
public:
    dLytMeterABtn_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterABtn_c() {}
private:
    STATE_FUNC_DECLARE(dLytMeterABtn_c, InvisibleWait);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, In);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Out);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, On);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Active);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterABtn_c);
    dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnmGroups[3];
};

#endif
