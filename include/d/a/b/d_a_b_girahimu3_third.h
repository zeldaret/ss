#ifndef D_A_B_GIRAHIMU3_THIRD_H
#define D_A_B_GIRAHIMU3_THIRD_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcGirahimu3Third_c : public dAcEnBase_c {
public:
    dAcGirahimu3Third_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcGirahimu3Third_c() {}

    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, Demo);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, SceneChange);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, Wait);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, Walk);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, GuardSword);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, Chance);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, SwordBeamDamage);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, RepairClaymore);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, Attack);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, CounterAttack);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, EscapeWarp);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, EscapeWarp2);
    STATE_FUNC_DECLARE(dAcGirahimu3Third_c, GuardJust);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcGirahimu3Third_c);
};

#endif
