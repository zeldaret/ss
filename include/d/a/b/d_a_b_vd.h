#ifndef D_A_B_VD_H
#define D_A_B_VD_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcBvd_c : public dAcEnBase_c {
public:
    dAcBvd_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcBvd_c() {}

    STATE_FUNC_DECLARE(dAcBvd_c, Opening);
    STATE_FUNC_DECLARE(dAcBvd_c, Wait);
    STATE_FUNC_DECLARE(dAcBvd_c, WaitPlayer);
    STATE_FUNC_DECLARE(dAcBvd_c, Run);
    STATE_FUNC_DECLARE(dAcBvd_c, FireAttack);
    STATE_FUNC_DECLARE(dAcBvd_c, Eat);
    STATE_FUNC_DECLARE(dAcBvd_c, Damage);
    STATE_FUNC_DECLARE(dAcBvd_c, BombDamage);
    STATE_FUNC_DECLARE(dAcBvd_c, BodyReverse);
    STATE_FUNC_DECLARE(dAcBvd_c, InitMove);
    STATE_FUNC_DECLARE(dAcBvd_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcBvd_c);
};

#endif
