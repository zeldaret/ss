#ifndef D_T_BARREL_H
#define D_T_BARREL_H

#include "d/a/d_a_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dTgBarrel_c : public dAcBase_c {
public:
    dTgBarrel_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgBarrel_c() {}

    STATE_FUNC_DECLARE(dTgBarrel_c, Wait);
    STATE_FUNC_DECLARE(dTgBarrel_c, Stop);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgBarrel_c);
};

#endif
