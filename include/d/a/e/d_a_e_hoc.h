#ifndef D_A_E_HOC_H
#define D_A_E_HOC_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEhoc_c : public dAcEnBase_c {
public:
    dAcEhoc_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEhoc_c() {}

    STATE_FUNC_DECLARE(dAcEhoc_c, Fight);
    STATE_FUNC_DECLARE(dAcEhoc_c, Attack);
    STATE_FUNC_DECLARE(dAcEhoc_c, BigAttack);
    STATE_FUNC_DECLARE(dAcEhoc_c, AttackAfter);
    STATE_FUNC_DECLARE(dAcEhoc_c, Escape);
    STATE_FUNC_DECLARE(dAcEhoc_c, Damage);
    STATE_FUNC_DECLARE(dAcEhoc_c, Fire);
    STATE_FUNC_DECLARE(dAcEhoc_c, Fail);
    STATE_FUNC_DECLARE(dAcEhoc_c, StabGuard);
    STATE_FUNC_DECLARE(dAcEhoc_c, Stun);
    STATE_FUNC_DECLARE(dAcEhoc_c, DropOut);
    STATE_FUNC_DECLARE(dAcEhoc_c, Return);
    STATE_FUNC_DECLARE(dAcEhoc_c, DashReturn);
    STATE_FUNC_DECLARE(dAcEhoc_c, Wait);
    STATE_FUNC_DECLARE(dAcEhoc_c, Summon);
    STATE_FUNC_DECLARE(dAcEhoc_c, AirDamage);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEhoc_c);
};

#endif
