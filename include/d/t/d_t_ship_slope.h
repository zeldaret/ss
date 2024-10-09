#ifndef D_T_SHIP_SLOPE_H
#define D_T_SHIP_SLOPE_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgShipSlope_c : public dAcBase_c {
public:
    dTgShipSlope_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgShipSlope_c() {}

    STATE_FUNC_DECLARE(dTgShipSlope_c, Wait);
    STATE_FUNC_DECLARE(dTgShipSlope_c, Stop);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgShipSlope_c);
};

#endif
