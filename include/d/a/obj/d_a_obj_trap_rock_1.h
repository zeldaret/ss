#ifndef D_A_OBJ_TRAP_ROCK_1_H
#define D_A_OBJ_TRAP_ROCK_1_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOtrapRock1_c : public dAcObjBase_c {
public:
    dAcOtrapRock1_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOtrapRock1_c() {}

    STATE_FUNC_DECLARE(dAcOtrapRock1_c, TrapWait);
    STATE_FUNC_DECLARE(dAcOtrapRock1_c, TrapAction);
    STATE_FUNC_DECLARE(dAcOtrapRock1_c, TrapReturn);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOtrapRock1_c);
};

#endif
