#ifndef D_A_E_LIZARUFOS_H
#define D_A_E_LIZARUFOS_H

#include <d/a/e/d_a_en_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcELizarufos_c : public dAcEnBase_c {
public:
    dAcELizarufos_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcELizarufos_c() {}

    STATE_FUNC_DECLARE(dAcELizarufos_c, Demo);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Wait);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Kyoro);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Find);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Find2);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Move);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Search);
    STATE_FUNC_DECLARE(dAcELizarufos_c, PathMove);
    STATE_FUNC_DECLARE(dAcELizarufos_c, FightWait);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Provoke);
    STATE_FUNC_DECLARE(dAcELizarufos_c, ArmAttack);
    STATE_FUNC_DECLARE(dAcELizarufos_c, TailAttack);
    STATE_FUNC_DECLARE(dAcELizarufos_c, FireAttack);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Step);
    STATE_FUNC_DECLARE(dAcELizarufos_c, BackJump);
    STATE_FUNC_DECLARE(dAcELizarufos_c, CliffJump);
    STATE_FUNC_DECLARE(dAcELizarufos_c, GuardWait);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Guard);
    STATE_FUNC_DECLARE(dAcELizarufos_c, JustGuard);
    STATE_FUNC_DECLARE(dAcELizarufos_c, RetMove);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Damage);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Falter);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Stun);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Fall);
    STATE_FUNC_DECLARE(dAcELizarufos_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcELizarufos_c);
};

#endif
