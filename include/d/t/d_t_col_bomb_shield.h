#ifndef D_T_COL_BOMB_SHIELD_H
#define D_T_COL_BOMB_SHIELD_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dTgColBombShield : public dAcBase_c {
public:
    dTgColBombShield() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgColBombShield() {}

    STATE_FUNC_DECLARE(dTgColBombShield, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgColBombShield);
};

#endif
