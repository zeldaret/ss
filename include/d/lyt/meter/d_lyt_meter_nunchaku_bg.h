#ifndef D_LYT_METER_NUNCHAKU_BG_H
#define D_LYT_METER_NUNCHAKU_BG_H

#include <d/lyt/d2d.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytMeterNunchakuBg_c : public dLytMeterBase {
public:
    dLytMeterNunchakuBg_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterNunchakuBg_c() {}
private:
    STATE_FUNC_DECLARE(dLytMeterNunchakuBg_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterNunchakuBg_c, On);
    STATE_FUNC_DECLARE(dLytMeterNunchakuBg_c, Active);
    STATE_FUNC_DECLARE(dLytMeterNunchakuBg_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterNunchakuBg_c);
    d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x??? */ u8 padding[0x12608 - 0x125D8];
};

#endif
