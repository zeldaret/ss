#ifndef D_A_E_KS_H
#define D_A_E_KS_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEKs_c : public dAcEnBase_c {
public:
    dAcEKs_c() : mStateMgr(*this) {}
    virtual ~dAcEKs_c() {}

    STATE_FUNC_DECLARE(dAcEKs_c, Wait);
    STATE_FUNC_DECLARE(dAcEKs_c, WakeUp);
    STATE_FUNC_DECLARE(dAcEKs_c, ReturnToWait);
    STATE_FUNC_DECLARE(dAcEKs_c, WaitReady);
    STATE_FUNC_DECLARE(dAcEKs_c, Move);
    STATE_FUNC_DECLARE(dAcEKs_c, Chase);
    STATE_FUNC_DECLARE(dAcEKs_c, ChaseAttack);
    STATE_FUNC_DECLARE(dAcEKs_c, Fighting);
    STATE_FUNC_DECLARE(dAcEKs_c, AttackReady);
    STATE_FUNC_DECLARE(dAcEKs_c, Attack);
    STATE_FUNC_DECLARE(dAcEKs_c, Damage);
    STATE_FUNC_DECLARE(dAcEKs_c, Stun);
    STATE_FUNC_DECLARE(dAcEKs_c, WindBlow);
    STATE_FUNC_DECLARE(dAcEKs_c, PathMove);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEKs_c);
};

#endif
