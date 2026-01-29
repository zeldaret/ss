#ifndef D_A_E_BC_H
#define D_A_E_BC_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"

class dAcEbc_c : public dAcEnBase_c {
public:
    dAcEbc_c() : mStateMgr(*this) {}
    virtual ~dAcEbc_c() {}

    STATE_FUNC_DECLARE(dAcEbc_c, Standby);
    STATE_FUNC_DECLARE(dAcEbc_c, Home);
    STATE_FUNC_DECLARE(dAcEbc_c, Normal);
    STATE_FUNC_DECLARE(dAcEbc_c, Lantern);
    STATE_FUNC_DECLARE(dAcEbc_c, Yagura);
    STATE_FUNC_DECLARE(dAcEbc_c, Path);
    STATE_FUNC_DECLARE(dAcEbc_c, LanternPath);
    STATE_FUNC_DECLARE(dAcEbc_c, RopeWalk);
    STATE_FUNC_DECLARE(dAcEbc_c, FightStart);
    STATE_FUNC_DECLARE(dAcEbc_c, FindCall);
    STATE_FUNC_DECLARE(dAcEbc_c, Fight);
    STATE_FUNC_DECLARE(dAcEbc_c, Attack);
    STATE_FUNC_DECLARE(dAcEbc_c, BigAttack);
    STATE_FUNC_DECLARE(dAcEbc_c, StepAttack);
    STATE_FUNC_DECLARE(dAcEbc_c, RockAttack);
    STATE_FUNC_DECLARE(dAcEbc_c, CarryDamage);
    STATE_FUNC_DECLARE(dAcEbc_c, Damage);
    STATE_FUNC_DECLARE(dAcEbc_c, AirDamage);
    STATE_FUNC_DECLARE(dAcEbc_c, Fire);
    STATE_FUNC_DECLARE(dAcEbc_c, Fail);
    STATE_FUNC_DECLARE(dAcEbc_c, Fall);
    STATE_FUNC_DECLARE(dAcEbc_c, BombAction);
    STATE_FUNC_DECLARE(dAcEbc_c, StabGuard);
    STATE_FUNC_DECLARE(dAcEbc_c, Stun);
    STATE_FUNC_DECLARE(dAcEbc_c, BeeDamage);
    STATE_FUNC_DECLARE(dAcEbc_c, WhipCapture);
    STATE_FUNC_DECLARE(dAcEbc_c, WakidasiWait);
    STATE_FUNC_DECLARE(dAcEbc_c, SlopeSlide);
    STATE_FUNC_DECLARE(dAcEbc_c, Drop);
    STATE_FUNC_DECLARE(dAcEbc_c, MastDrop);
    STATE_FUNC_DECLARE(dAcEbc_c, AStandby);
    STATE_FUNC_DECLARE(dAcEbc_c, AFindCall);
    STATE_FUNC_DECLARE(dAcEbc_c, APath);
    STATE_FUNC_DECLARE(dAcEbc_c, AAttack);
    STATE_FUNC_DECLARE(dAcEbc_c, ADamage);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEbc_c);
};

#endif
