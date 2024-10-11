#ifndef D_LYT_METER_RUPY_H
#define D_LYT_METER_RUPY_H

#include <d/lyt/d2d.h>
#include <d/lyt/d_lyt_sub.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytMeterRupyStart_c {
public:
    dLytMeterRupyStart_c() : mStateMgr(*this, sStateID::null) {}

private:
    STATE_FUNC_DECLARE(dLytMeterRupyStart_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterRupyStart_c, Start);

    UI_STATE_MGR_DECLARE(dLytMeterRupyStart_c);

    u8 field_0x3C[0x8];
};

class dLytMeterRupyBlink_c {
public:
    dLytMeterRupyBlink_c() : mStateMgr(*this, sStateID::null) {}

private:
    STATE_FUNC_DECLARE(dLytMeterRupyBlink_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterRupyBlink_c, Flash);

    UI_STATE_MGR_DECLARE(dLytMeterRupyBlink_c);

    u8 field_0x3C[0x8];
};

class dLytMeterRupy_c : public dLytMeterBase {
public:
    dLytMeterRupy_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool LytMeter0x10() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual void *LytMeter0x1C() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterRupy_c() {}
private:
    STATE_FUNC_DECLARE(dLytMeterRupy_c, In);
    STATE_FUNC_DECLARE(dLytMeterRupy_c, Active);
    STATE_FUNC_DECLARE(dLytMeterRupy_c, Start);
    STATE_FUNC_DECLARE(dLytMeterRupy_c, Out);

    UI_STATE_MGR_DECLARE(dLytMeterRupy_c);

    /* 0x044 */ dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnmGroups[22];
    /* 0x658 */ dLytMeterRupyStart_c mStartParts[4];
    /* 0x768 */ dLytMeterRupyBlink_c mBlinkParts[4];
};


#endif
