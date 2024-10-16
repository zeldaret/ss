#ifndef D_T_BCZ_H
#define D_T_BCZ_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dTgBcZ_c : public dAcBase_c {
public:
    dTgBcZ_c() : mStateMgr(*this, sStateID::null) {}
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
