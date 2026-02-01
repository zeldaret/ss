#ifndef D_T_BCZ_H
#define D_T_BCZ_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgBcZ_c : public dTg_c {
public:
    dTgBcZ_c() : mStateMgr(*this) {}
    virtual ~dTgBcZ_c() {}

    STATE_FUNC_DECLARE(dTgBcZ_c, Wait);
    STATE_FUNC_DECLARE(dTgBcZ_c, RaiseBcZ);
    STATE_FUNC_DECLARE(dTgBcZ_c, RaiseLeft);
    STATE_FUNC_DECLARE(dTgBcZ_c, ControlBcZ);
    STATE_FUNC_DECLARE(dTgBcZ_c, DoNothing);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgBcZ_c);
};

#endif
