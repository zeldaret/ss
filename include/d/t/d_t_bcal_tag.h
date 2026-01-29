#ifndef D_T_BCAL_TAG_H
#define D_T_BCAL_TAG_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgBcArL_c : public dTg_c {
public:
    dTgBcArL_c() : mStateMgr(*this) {}
    virtual ~dTgBcArL_c() {}

    STATE_FUNC_DECLARE(dTgBcArL_c, Wait);
    STATE_FUNC_DECLARE(dTgBcArL_c, Create);
    STATE_FUNC_DECLARE(dTgBcArL_c, AttackWait);
    STATE_FUNC_DECLARE(dTgBcArL_c, Attack);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgBcArL_c);
};

#endif
