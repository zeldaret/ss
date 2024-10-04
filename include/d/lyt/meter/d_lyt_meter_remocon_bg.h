#include <d/lyt/d2d.h>
#include <d/lyt/meter/d_lyt_meter.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>


class dLytSub : public d2d::LytBase_c {
public:
    dLytSub() {}
    virtual bool build(const char *name, m2d::ResAccIf_c *acc) override;

    const char *getName() const {
        return mpName;
    }
private:
    const char *mpName;
};

class dLytMeterRemoconBg_c : public dLytMeterBase {
public:
    dLytMeterRemoconBg_c();
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *LytMeter0x18() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *LytMeter0x20() const override;

private:
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, On);
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, Active);
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, Off);

    STATE_MGR_DECLARE(dLytMeterRemoconBg_c);
    dLytSub mLyt;
    /* 0xD8 */ nw4r::lyt::Pane *mpPane;
    /* 0xDC */ u32 field_0xDC;
    /* 0xE0 */ u32 field_0xE0;
    /* 0xE4 */ u32 field_0xE4;
    /* 0xE8 */ u32 field_0xE8;
    /* 0xEC */ bool field_0xEC;
};
