#ifndef D_A_OBJ_STEP_GUMARM_H
#define D_A_OBJ_STEP_GUMARM_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOStepGumarm_c : public dAcObjBase_c {
public:
    dAcOStepGumarm_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOStepGumarm_c() {}

    STATE_FUNC_DECLARE(dAcOStepGumarm_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOStepGumarm_c);
};

#endif
