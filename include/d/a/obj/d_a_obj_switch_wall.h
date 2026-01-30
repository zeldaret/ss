#ifndef D_A_OBJ_SWITCH_WALL_H
#define D_A_OBJ_SWITCH_WALL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOswitchWall_c : public dAcObjBase_c {
public:
    dAcOswitchWall_c() : mStateMgr(*this) {}
    virtual ~dAcOswitchWall_c() {}

    STATE_FUNC_DECLARE(dAcOswitchWall_c, Up);
    STATE_FUNC_DECLARE(dAcOswitchWall_c, Down);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOswitchWall_c);
};

#endif
