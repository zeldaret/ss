#ifndef D_T_BCAL_TAG_H
#define D_T_BCAL_TAG_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dTgBcArL_c : public dAcBase_c {
public:
    dTgBcArL_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgBcArL_c() {}

    STATE_FUNC_DECLARE(dTgBcArL_c, Wait);
    STATE_FUNC_DECLARE(dTgBcArL_c, Create);
    STATE_FUNC_DECLARE(dTgBcArL_c, AttackWait);
    STATE_FUNC_DECLARE(dTgBcArL_c, Attack);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgBcArL_c);
};

#endif
