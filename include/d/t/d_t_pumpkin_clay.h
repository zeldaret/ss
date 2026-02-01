#ifndef D_T_PUMPKIN_CLAY_H
#define D_T_PUMPKIN_CLAY_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgPumpkinClay_c : public dTg_c {
public:
    dTgPumpkinClay_c() : mStateMgr(*this) {}
    virtual ~dTgPumpkinClay_c() {}

    STATE_FUNC_DECLARE(dTgPumpkinClay_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgPumpkinClay_c);
};

#endif
