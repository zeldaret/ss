#ifndef D_LYT_METER_PLUS_BTN_H
#define D_LYT_METER_PLUS_BTN_H

#include <d/lyt/d2d.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytMeterPlusBtn_c : public dLytMeterBase {
public:
    dLytMeterPlusBtn_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterPlusBtn_c() {}
private:
    STATE_FUNC_DECLARE(dLytMeterPlusBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterPlusBtn_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeterPlusBtn_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeterPlusBtn_c, Unuse);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterPlusBtn_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x??? */ u8 padding[0x7A64 - 0x7A38];
};

#endif
