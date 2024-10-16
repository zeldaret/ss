#ifndef D_LYT_METER_CROSS_BTN_H
#define D_LYT_METER_CROSS_BTN_H

#include <d/lyt/d2d.h>
#include <d/lyt/d_lyt_sub.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

// Size 0x80
class dLytMeterCrossBtnParts_c {
public:
    dLytMeterCrossBtnParts_c(): mStateMgr(*this, sStateID::null) {}
    ~dLytMeterCrossBtnParts_c() {}
private:
    UI_STATE_MGR_DECLARE(dLytMeterCrossBtnParts_c);

    u32 padding[17];
};

class dLytMeterCrossBtn_c : public dLytMeterBase {
public:
    dLytMeterCrossBtn_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterCrossBtn_c() {}
private:
    STATE_FUNC_DECLARE(dLytMeterCrossBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterCrossBtn_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeterCrossBtn_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeterCrossBtn_c, Unuse);

    UI_STATE_MGR_DECLARE(dLytMeterCrossBtn_c);
    dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnmGroups[12];
    /* 0x3D8 */ u8 field_0x3D8[0x420 - 0x3D8];
    /* 0x420 */ dLytMeterCrossBtnParts_c mParts[4];
};

#endif
