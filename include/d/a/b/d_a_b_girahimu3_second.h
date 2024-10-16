#ifndef D_A_B_GIRAHIMU3_SECOND_H
#define D_A_B_GIRAHIMU3_SECOND_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcGirahimu3Second_c : public dAcEnBase_c {
public:
    dAcGirahimu3Second_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcGirahimu3Second_c() {}

    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, Demo);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, Demo2);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, Wait);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, Walk);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, Attack);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, CounterAttack);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, Disturb);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, SwordBeamDamage);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, GuardSword);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, Provoke);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, KnifeReady);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, KnifeAttack);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, SwordBeamReady);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, SwordBeamAttack);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, SwordBeamRally);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, GuardJust);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, Damage);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, SummonSword);
    STATE_FUNC_DECLARE(dAcGirahimu3Second_c, EscapeWarp);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcGirahimu3Second_c);
};

#endif
