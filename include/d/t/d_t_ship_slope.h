#ifndef D_T_SHIP_SLOPE_H
#define D_T_SHIP_SLOPE_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgShipSlope_c : public dTg_c {
public:
    dTgShipSlope_c() : mStateMgr(*this) {}
    virtual ~dTgShipSlope_c() {}

    STATE_FUNC_DECLARE(dTgShipSlope_c, Wait);
    STATE_FUNC_DECLARE(dTgShipSlope_c, Stop);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgShipSlope_c);
};

#endif
