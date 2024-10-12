#ifndef D_LYT_METER_DRINK_H
#define D_LYT_METER_DRINK_H

#include <d/lyt/d2d.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytMeterDrinkParts_c {
public:
    dLytMeterDrinkParts_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDrinkParts_c() {}
private:
    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterDrinkParts_c);
};

class dLytMeterDrink_c {
public:
    dLytMeterDrink_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDrink_c() {}

    virtual void BossKey0x8() {}
    virtual void BossKey0xC() {}

    bool build(d2d::ResAccIf_c *);
private:


    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterDrink_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnmGroups[20];
    /* 0x5E4 */ dLytMeterDrinkParts_c mParts[3];
};


#endif
