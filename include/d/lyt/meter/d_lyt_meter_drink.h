#ifndef D_LYT_METER_DRINK_H
#define D_LYT_METER_DRINK_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytMeterDrinkParts_c {
public:
    dLytMeterDrinkParts_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDrinkParts_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterDrinkParts_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterDrinkParts_c, In);
    STATE_FUNC_DECLARE(dLytMeterDrinkParts_c, Visible);
    STATE_FUNC_DECLARE(dLytMeterDrinkParts_c, Out);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterDrinkParts_c);
    /* 0x03C */ u8 field_0x03C[0x18];
};

class dLytMeterDrink_c {
public:
    dLytMeterDrink_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDrink_c() {}

    virtual void BossKey0x8() {}
    virtual void BossKey0xC() {}

    bool build(d2d::ResAccIf_c *);
    bool remove();

private:
    STATE_FUNC_DECLARE(dLytMeterDrink_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterDrink_c, In);
    STATE_FUNC_DECLARE(dLytMeterDrink_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDrink_c, Move);
    STATE_FUNC_DECLARE(dLytMeterDrink_c, Out);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterDrink_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnmGroups[20];
    /* 0x5D4 */ u8 field_0x5D4[0x5E4 - 0x5D4];
    /* 0x5E4 */ dLytMeterDrinkParts_c mParts[3];
};

#endif
