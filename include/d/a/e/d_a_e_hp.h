#ifndef D_A_E_HP_H
#define D_A_E_HP_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEHp_c : public dAcEnBase_c {
public:
    dAcEHp_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEHp_c() {}

    STATE_FUNC_DECLARE(dAcEHp_c, BobUpAndDown);
    STATE_FUNC_DECLARE(dAcEHp_c, Chase);
    STATE_FUNC_DECLARE(dAcEHp_c, Attack);
    STATE_FUNC_DECLARE(dAcEHp_c, AtkWait);
    STATE_FUNC_DECLARE(dAcEHp_c, Extract);
    STATE_FUNC_DECLARE(dAcEHp_c, Petrify);
    STATE_FUNC_DECLARE(dAcEHp_c, Fluster);
    STATE_FUNC_DECLARE(dAcEHp_c, Escape);
    STATE_FUNC_DECLARE(dAcEHp_c, Damage);
    STATE_FUNC_DECLARE(dAcEHp_c, VcmDmg);
    STATE_FUNC_DECLARE(dAcEHp_c, Stun);
    STATE_FUNC_DECLARE(dAcEHp_c, Die);
    STATE_FUNC_DECLARE(dAcEHp_c, Dummy);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEHp_c);
};

#endif
