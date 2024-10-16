#ifndef D_A_E_OC_H
#define D_A_E_OC_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEoc_c : public dAcEnBase_c {
public:
    dAcEoc_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEoc_c() {}

    STATE_FUNC_DECLARE(dAcEoc_c, Wait);
    STATE_FUNC_DECLARE(dAcEoc_c, Standby);
    STATE_FUNC_DECLARE(dAcEoc_c, FindCall);
    STATE_FUNC_DECLARE(dAcEoc_c, Fight);
    STATE_FUNC_DECLARE(dAcEoc_c, Attack);
    STATE_FUNC_DECLARE(dAcEoc_c, BigAttack);
    STATE_FUNC_DECLARE(dAcEoc_c, Damage);
    STATE_FUNC_DECLARE(dAcEoc_c, Fire);
    STATE_FUNC_DECLARE(dAcEoc_c, Fail);
    STATE_FUNC_DECLARE(dAcEoc_c, StabGuard);
    STATE_FUNC_DECLARE(dAcEoc_c, Stun);
    STATE_FUNC_DECLARE(dAcEoc_c, PipeRun);
    STATE_FUNC_DECLARE(dAcEoc_c, PipeWait);
    STATE_FUNC_DECLARE(dAcEoc_c, Whistle);
    STATE_FUNC_DECLARE(dAcEoc_c, Flicked);
    STATE_FUNC_DECLARE(dAcEoc_c, AirDamage);
    STATE_FUNC_DECLARE(dAcEoc_c, Demo);
    STATE_FUNC_DECLARE(dAcEoc_c, PipeChase);
    STATE_FUNC_DECLARE(dAcEoc_c, SearchPipe);
    STATE_FUNC_DECLARE(dAcEoc_c, GetPipe);
    STATE_FUNC_DECLARE(dAcEoc_c, Summon);
    STATE_FUNC_DECLARE(dAcEoc_c, BombCarry);
    STATE_FUNC_DECLARE(dAcEoc_c, BombThrow);
    STATE_FUNC_DECLARE(dAcEoc_c, Escape);
    STATE_FUNC_DECLARE(dAcEoc_c, Equip);
    STATE_FUNC_DECLARE(dAcEoc_c, WhipCapture);
    STATE_FUNC_DECLARE(dAcEoc_c, DemoMove);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEoc_c);
};

#endif
