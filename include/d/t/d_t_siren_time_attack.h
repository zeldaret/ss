#ifndef D_T_SIREN_TIME_ATTACK_H
#define D_T_SIREN_TIME_ATTACK_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgSirenTimeAttack_c : public dTg_c {
public:
    dTgSirenTimeAttack_c() : mStateMgr(*this) {}
    virtual ~dTgSirenTimeAttack_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgSirenTimeAttack_c);
};

#endif
