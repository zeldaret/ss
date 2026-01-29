#ifndef D_A_OBJ_TRUCK_STOPPER_H
#define D_A_OBJ_TRUCK_STOPPER_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOtruckStopper_c : public dAcObjBase_c {
public:
    dAcOtruckStopper_c() : mStateMgr(*this) {}
    virtual ~dAcOtruckStopper_c() {}

    STATE_FUNC_DECLARE(dAcOtruckStopper_c, Wait);
    STATE_FUNC_DECLARE(dAcOtruckStopper_c, Break);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOtruckStopper_c);
};

#endif
