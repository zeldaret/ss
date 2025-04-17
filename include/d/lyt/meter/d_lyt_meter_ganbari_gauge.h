#ifndef D_LYT_METER_GANBARI_GAUGE_H
#define D_LYT_METER_GANBARI_GAUGE_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytMeterGanbariGauge_c : public d2d::dSubPane {
public:
    dLytMeterGanbariGauge_c() : mStateMgrMain(*this, sStateID::null), mStateMgrWheel(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    virtual ~dLytMeterGanbariGauge_c() {}

    bool fn_80104760() const;

private:
    void realizeAnimState();
    static bool isStaminaLow(f32);
    static bool isStaminaCritical(f32);
    bool fn_80104710(bool) const;
    bool isCrawling() const;
    static bool swimmingRelated();
    bool fn_801047B0() const;
    bool displayAirInsteadOfStamina();

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

    STATE_MGR(dLytMeterGanbariGauge_c) mStateMgrMain;
    STATE_MGR(dLytMeterGanbariGauge_c) mStateMgrWheel;

    /* 0x080 */ d2d::dLytSub mLyt;
    /* 0x114 */ d2d::AnmGroup_c mAnm[16];
    /* 0x514 */ nw4r::lyt::Pane *mpPane;
    /* 0x518 */ f32 field_0x518;
    /* 0x51C */ f32 field_0x51C;
    /* 0x520 */ u8 _0x520[0x524 - 0x520];
    /* 0x524 */ mVec3_c field_0x524;
    /* 0x530 */ u8 field_0x530;
    /* 0x534 */ s32 field_0x534;
    /* 0x538 */ s32 field_0x538;
    /* 0x53C */ s32 mOutWaitTimer;
    /* 0x540 */ s32 field_0x540;
    /* 0x544 */ s32 field_0x544;
    /* 0x548 */ s32 field_0x548;
    /* 0x54C */ f32 field_0x54C;
    /* 0x550 */ f32 field_0x550;

    /* 0x554 */ f32 field_0x554;
    u8 field_0x558;
    u8 field_0x559;
    bool field_0x55A;
    u8 field_0x55B;
};

STATIC_ASSERT(sizeof(dLytMeterGanbariGauge_c) == 0x55C);

#endif
