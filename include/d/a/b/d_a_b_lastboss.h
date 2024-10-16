#ifndef D_A_B_LASTBOSS_H
#define D_A_B_LASTBOSS_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcBlastboss_c : public dAcEnBase_c {
public:
    dAcBlastboss_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcBlastboss_c() {}

    STATE_FUNC_DECLARE(dAcBlastboss_c, Fight);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Guard);
    STATE_FUNC_DECLARE(dAcBlastboss_c, GuardBreak);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Attack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, CounterAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, PunchAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, DashAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, SmallAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, ThunderAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Damage);
    STATE_FUNC_DECLARE(dAcBlastboss_c, SitDamage);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Down);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Stun);
    STATE_FUNC_DECLARE(dAcBlastboss_c, ThunderWait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcBlastboss_c);
};

#endif
