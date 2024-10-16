#ifndef D_A_OBJ_WINDMILL_DESERT_H
#define D_A_OBJ_WINDMILL_DESERT_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOwindmillDesert_c : public dAcObjBase_c {
public:
    dAcOwindmillDesert_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOwindmillDesert_c() {}

    STATE_FUNC_DECLARE(dAcOwindmillDesert_c, Wait);
    STATE_FUNC_DECLARE(dAcOwindmillDesert_c, Acceleration);
    STATE_FUNC_DECLARE(dAcOwindmillDesert_c, RollMaxSpeed);
    STATE_FUNC_DECLARE(dAcOwindmillDesert_c, Deceleration);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOwindmillDesert_c);
};

#endif
