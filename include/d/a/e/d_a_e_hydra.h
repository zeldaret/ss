#ifndef D_A_E_HYDRA_H
#define D_A_E_HYDRA_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEhydra_c : public dAcEnBase_c {
public:
    dAcEhydra_c() : mStateMgr(*this) {}
    virtual ~dAcEhydra_c() {}

    STATE_FUNC_DECLARE(dAcEhydra_c, Wait);
    STATE_FUNC_DECLARE(dAcEhydra_c, Walk);
    STATE_FUNC_DECLARE(dAcEhydra_c, Run);
    STATE_FUNC_DECLARE(dAcEhydra_c, Ret);
    STATE_FUNC_DECLARE(dAcEhydra_c, Attack);
    STATE_FUNC_DECLARE(dAcEhydra_c, AttackWait);
    STATE_FUNC_DECLARE(dAcEhydra_c, Recover);
    STATE_FUNC_DECLARE(dAcEhydra_c, Stun);
    STATE_FUNC_DECLARE(dAcEhydra_c, Damage);
    STATE_FUNC_DECLARE(dAcEhydra_c, EtcDamage);
    STATE_FUNC_DECLARE(dAcEhydra_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEhydra_c);
};

#endif
