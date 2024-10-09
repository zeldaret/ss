#ifndef D_T_PUMPKIN_CLAY_H
#define D_T_PUMPKIN_CLAY_H

#include <d/a/d_a_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dTgPumpkinClay_c : public dAcBase_c {
public:
    dTgPumpkinClay_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgPumpkinClay_c() {}

    STATE_FUNC_DECLARE(dTgPumpkinClay_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgPumpkinClay_c);
};

#endif
