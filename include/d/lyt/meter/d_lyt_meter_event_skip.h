#ifndef D_LYT_METER_EVENT_SKIP_H
#define D_LYT_METER_EVENT_SKIP_H

#include <d/lyt/d2d.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

// size 0x164
class dLytMeterEventSkip_c {
public:
    dLytMeterEventSkip_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeterEventSkip_c() {}
    bool build(d2d::ResAccIf_c *resAcc);
private:
    STATE_FUNC_DECLARE(dLytMeterEventSkip_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterEventSkip_c, In);
    STATE_FUNC_DECLARE(dLytMeterEventSkip_c, Visible);
    STATE_FUNC_DECLARE(dLytMeterEventSkip_c, Out);

    UI_STATE_MGR_DECLARE(dLytMeterEventSkip_c);
    d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnmGroups[2];
    /* 0x150 */ u8 field_0x150[0x164 - 0x150];
};

#endif
