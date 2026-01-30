#ifndef D_T_SHIP_FLOOD_H
#define D_T_SHIP_FLOOD_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgShipFlood_c : public dTg_c {
public:
    dTgShipFlood_c() : mStateMgr(*this) {}
    virtual ~dTgShipFlood_c() {}

    STATE_FUNC_DECLARE(dTgShipFlood_c, Wait);
    STATE_FUNC_DECLARE(dTgShipFlood_c, Stop);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgShipFlood_c);
};

#endif
