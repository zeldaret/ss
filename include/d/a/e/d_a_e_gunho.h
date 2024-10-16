#ifndef D_A_E_GUNHO_H
#define D_A_E_GUNHO_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEgunho_c : public dAcEnBase_c {
public:
    dAcEgunho_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEgunho_c() {}

    STATE_FUNC_DECLARE(dAcEgunho_c, OpenClose);
    STATE_FUNC_DECLARE(dAcEgunho_c, Wait);
    STATE_FUNC_DECLARE(dAcEgunho_c, NormalMove);
    STATE_FUNC_DECLARE(dAcEgunho_c, Search);
    STATE_FUNC_DECLARE(dAcEgunho_c, AttackBullet);
    STATE_FUNC_DECLARE(dAcEgunho_c, AttackBomb);
    STATE_FUNC_DECLARE(dAcEgunho_c, Damage);
    STATE_FUNC_DECLARE(dAcEgunho_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEgunho_c);
};

#endif
