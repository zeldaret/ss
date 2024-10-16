#ifndef D_A_OBJ_TIME_DOOR_BEFORE_H
#define D_A_OBJ_TIME_DOOR_BEFORE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOTimeDoorBefore_c : public dAcObjBase_c {
public:
    dAcOTimeDoorBefore_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOTimeDoorBefore_c() {}

    STATE_FUNC_DECLARE(dAcOTimeDoorBefore_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTimeDoorBefore_c);
};

#endif
