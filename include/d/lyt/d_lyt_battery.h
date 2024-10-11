#ifndef D_LYT_BATTERY_H
#define D_LYT_BATTERY_H

#include <d/lyt/d2d.h>
#include <d/lyt/d_lyt_sub.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytBattery_c {
public:
    static bool create();
    static bool draw();
    bool dismissManually();

private:
    dLytBattery_c() : mStateMgr(*this, sStateID::null) {
        sInstance = this;
    }

    bool doDraw();
    bool init();

    bool hasEnoughBatteryCharge();

    STATE_FUNC_DECLARE(dLytBattery_c, Off);
    STATE_FUNC_DECLARE(dLytBattery_c, In);
    STATE_FUNC_DECLARE(dLytBattery_c, On);
    STATE_FUNC_DECLARE(dLytBattery_c, None);
    STATE_FUNC_DECLARE(dLytBattery_c, Out);

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytBattery_c);
    /* 0x040 */ d2d::ResAccIf_c mResAcc;
    /* 0x0EB */ dLytSub mLyt;
    /* 0x110 */ d2d::AnmGroup_c mAnmGroups[4];
    /* 0x540 */ bool mDismissManually;

    static dLytBattery_c *sInstance;
};

#endif
